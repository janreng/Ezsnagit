#include "ui/MainWindow.h"
#include "ui/RegionOverlay.h"
#include "ui/WindowPickerOverlay.h"
#include "ui/CanvasWidget.h"
#include "ui/StyleBar.h"
#include "ui/PanoramicDialog.h"
#include "ui/RecentTray.h"
#include "capture/StitchMany.h"
#include "canvas/QuickStyle.h"
#include "core/Settings.h"
#include "core/Version.h"
#include "capture/ScreenCapture.h"
#include "hotkey/GlobalHotkey.h"
#include "share/ImageClipboard.h"
#include "effects/Transforms.h"
#include "effects/Filters.h"
#include "effects/Frame.h"
#include "effects/RoundedCorners.h"
#include "ui/PenWidthPicker.h"
#include "ui/PreferencesDialog.h"
#include "canvas/EzsnagxFile.h"

#include <QScrollArea>
#include <QStatusBar>
#include <QLabel>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QImage>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QColorDialog>
#include <QDockWidget>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>

using canvas::ObjType;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Ezsnagit"));
    resize(1000, 680);

    // Canvas editor trong scroll area (1:1 pixel ảnh).
    m_canvas = new CanvasWidget(this);
    m_scroll = new QScrollArea(this);
    m_scroll->setWidget(m_canvas);
    m_scroll->setWidgetResizable(false);
    m_scroll->setAlignment(Qt::AlignCenter);
    setCentralWidget(m_scroll);

    // Khay ảnh chụp gần đây (dock dưới).
    auto *recentDock = new QDockWidget(QStringLiteral("Ảnh gần đây"), this);
    recentDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    m_recent = new RecentTray(recentDock);
    recentDock->setWidget(m_recent);
    addDockWidget(Qt::BottomDockWidgetArea, recentDock);
    connect(m_recent, &RecentTray::captureChosen, this, [this](const QImage &img) {
        m_canvas->setImage(img);   // mở lại ảnh gần đây (không thêm lại vào khay)
    });

    // Thanh trạng thái: vị trí con trỏ (pixel ảnh), cập nhật khi rê chuột trên canvas.
    auto *posLabel = new QLabel(this);
    statusBar()->addPermanentWidget(posLabel);
    connect(m_canvas, &CanvasWidget::cursorMoved, this, [posLabel](const QPoint &p) {
        posLabel->setText(QStringLiteral("(%1, %2)").arg(p.x()).arg(p.y()));
    });

    // --- Menu Tệp ---
    QMenu *fileMenu = menuBar()->addMenu(QStringLiteral("&Tệp"));
    QAction *openAct = fileMenu->addAction(QStringLiteral("Mở…"), QKeySequence::Open, this, &MainWindow::openFile);
    QAction *saveAct = fileMenu->addAction(QStringLiteral("Lưu…"), QKeySequence::Save, this, &MainWindow::saveFile);
    QAction *expAct  = fileMenu->addAction(QStringLiteral("Xuất PNG…"), this, &MainWindow::exportPng);
    fileMenu->addAction(QStringLiteral("Xuất JPG…"), this, [this] {
        if (!m_canvas->hasImage()) { statusBar()->showMessage(QStringLiteral("Chưa có ảnh để xuất"), 2000); return; }
        QString path = QFileDialog::getSaveFileName(this, QStringLiteral("Xuất JPG"),
                                                    QStringLiteral("anh.jpg"), QStringLiteral("JPEG (*.jpg)"));
        if (path.isEmpty()) return;
        if (!path.endsWith(QStringLiteral(".jpg"), Qt::CaseInsensitive)) path += QStringLiteral(".jpg");
        if (m_canvas->document().renderFlattened().save(path, "JPG", 92))
            statusBar()->showMessage(QStringLiteral("Đã xuất %1").arg(path), 4000);
        else
            statusBar()->showMessage(QStringLiteral("Xuất JPG thất bại"), 3000);
    });
    fileMenu->addSeparator();
    fileMenu->addAction(QStringLiteral("Lưu nhanh PNG"), QKeySequence(QStringLiteral("Ctrl+Shift+S")),
                        this, &MainWindow::quickSave);
    fileMenu->addAction(QStringLiteral("Đặt thư mục lưu mặc định…"), this, [this] {
        const QString dir = QFileDialog::getExistingDirectory(
            this, QStringLiteral("Chọn thư mục lưu mặc định"), core::Settings::defaultSaveDir());
        if (!dir.isEmpty()) {
            core::Settings::setDefaultSaveDir(dir);
            statusBar()->showMessage(QStringLiteral("Thư mục lưu mặc định: %1").arg(dir), 3000);
        }
    });
    fileMenu->addSeparator();
    fileMenu->addAction(QStringLiteral("Thiết lập…"), this, [this] {
        PreferencesDialog(this).exec();
    });
    Q_UNUSED(openAct); Q_UNUSED(saveAct); Q_UNUSED(expAct);

    // --- Menu Hiệu ứng (áp lên ảnh đã gộp annotation) ---
    QMenu *fxMenu = menuBar()->addMenu(QStringLiteral("&Hiệu ứng"));
    auto applyFilter = [this](QImage (*fn)(const QImage &)) {
        if (!m_canvas->hasImage()) return;
        m_canvas->setImage(fn(m_canvas->document().renderFlattened()));
    };
    fxMenu->addAction(QStringLiteral("Trắng đen"), this, [applyFilter] { applyFilter(effects::grayscale); });
    fxMenu->addAction(QStringLiteral("Sepia"),     this, [applyFilter] { applyFilter(effects::sepia); });
    fxMenu->addAction(QStringLiteral("Đảo màu"),   this, [applyFilter] { applyFilter(effects::invert); });
    fxMenu->addSeparator();
    fxMenu->addAction(QStringLiteral("Cắt viền thừa"), this, [applyFilter] { applyFilter(effects::autoTrim); });
    fxMenu->addAction(QStringLiteral("Thêm viền"), this, [this] {
        if (m_canvas->hasImage())
            m_canvas->setImage(effects::addBorder(m_canvas->document().renderFlattened(), 6, QColor(40, 40, 40)));
    });
    fxMenu->addAction(QStringLiteral("Đổ bóng"), this, [this] {
        if (m_canvas->hasImage())
            m_canvas->setImage(effects::dropShadow(m_canvas->document().renderFlattened()));
    });
    fxMenu->addAction(QStringLiteral("Bo góc"), this, [this] {
        if (m_canvas->hasImage())
            m_canvas->setImage(effects::roundCorners(m_canvas->document().renderFlattened(), 16));
    });
    fxMenu->addSeparator();
    fxMenu->addAction(QStringLiteral("Đổi kích thước…"), this, [this] {
        if (!m_canvas->hasImage()) return;
        const QImage cur = m_canvas->document().renderFlattened();
        bool ok = false;
        const int w = QInputDialog::getInt(this, QStringLiteral("Đổi kích thước"),
                                           QStringLiteral("Chiều rộng (px):"), cur.width(), 1, 20000, 1, &ok);
        if (!ok) return;
        const int h = QInputDialog::getInt(this, QStringLiteral("Đổi kích thước"),
                                           QStringLiteral("Chiều cao (px):"), cur.height(), 1, 20000, 1, &ok);
        if (!ok) return;
        m_canvas->setImage(effects::scaledTo(cur, QSize(w, h)));
    });

    // --- Menu Trợ giúp ---
    QMenu *helpMenu = menuBar()->addMenu(QStringLiteral("Trợ &giúp"));
    helpMenu->addAction(QStringLiteral("Giới thiệu Ezsnagit…"), this, [this] {
        QMessageBox::about(this, QStringLiteral("Giới thiệu Ezsnagit"),
            QStringLiteral("<b>Ezsnagit %1</b><br>Phần mềm chụp & chú thích màn hình.<br>"
                           "Miễn phí · chạy offline · nhẹ.").arg(core::appVersion()));
    });

    // --- Toolbar chụp ---
    auto *tb = addToolBar(QStringLiteral("Chụp"));
    tb->setMovable(false);
    tb->addAction(QStringLiteral("Chụp vùng"), QKeySequence(QStringLiteral("Ctrl+Shift+A")),
                  this, &MainWindow::captureRegion);
    tb->addAction(QStringLiteral("Chụp cửa sổ"), QKeySequence(QStringLiteral("Ctrl+Shift+W")),
                  this, &MainWindow::captureWindow);
    tb->addAction(QStringLiteral("Chụp toàn màn hình"), QKeySequence(QStringLiteral("Ctrl+Shift+F")),
                  this, &MainWindow::captureFullScreen);
    tb->addAction(QStringLiteral("Chụp ghép dọc"), this, &MainWindow::captureScrolling);
    tb->addAction(QStringLiteral("Chụp sau 3 giây"), this, [this] {
        statusBar()->showMessage(QStringLiteral("Sẽ chụp toàn màn hình sau 3 giây…"), 3000);
        QTimer::singleShot(3000, this, &MainWindow::captureFullScreen);
    });

    // --- Toolbar annotation ---
    auto *atb = addToolBar(QStringLiteral("Chú thích"));
    atb->setMovable(false);
    auto *grp = new QActionGroup(this);
    grp->setExclusive(true);
    auto addTool = [&](const QString &name, ObjType t, bool checked, const QString &key) {
        QAction *a = atb->addAction(name);
        a->setCheckable(true);
        a->setChecked(checked);
        if (!key.isEmpty()) a->setShortcut(QKeySequence(key));
        a->setToolTip(QStringLiteral("%1 (%2)").arg(name, key));
        grp->addAction(a);
        connect(a, &QAction::triggered, this, [this, t] { m_canvas->setTool(t); });
    };
    addTool(QStringLiteral("Mũi tên"), ObjType::Arrow, true, QStringLiteral("A"));
    addTool(QStringLiteral("Đường"), ObjType::Line, false, QStringLiteral("L"));
    addTool(QStringLiteral("Khung"), ObjType::Box, false, QStringLiteral("B"));
    addTool(QStringLiteral("Tô sáng"), ObjType::Highlight, false, QStringLiteral("H"));
    addTool(QStringLiteral("Chữ"), ObjType::Text, false, QStringLiteral("T"));
    addTool(QStringLiteral("Số bước"), ObjType::Step, false, QStringLiteral("S"));
    addTool(QStringLiteral("Làm mờ"), ObjType::Blur, false, QStringLiteral("M"));
    addTool(QStringLiteral("Cắt"), ObjType::Crop, false, QStringLiteral("C"));
    m_canvas->setTool(ObjType::Arrow);

    atb->addAction(QStringLiteral("Màu…"), this, [this] {
        const QColor c = QColorDialog::getColor(QColor(220, 30, 30), this, QStringLiteral("Chọn màu"));
        if (c.isValid()) m_canvas->setColor(c);
    });
    auto *penPicker = new PenWidthPicker(atb);
    connect(penPicker, &PenWidthPicker::widthChanged, this, [this](int w) { m_canvas->setPenWidth(w); });
    atb->addWidget(penPicker);
    atb->addSeparator();
    atb->addAction(QStringLiteral("Hoàn tác"), QKeySequence::Undo, this, [this] { m_canvas->undo(); });
    atb->addAction(QStringLiteral("Làm lại"), QKeySequence::Redo, this, [this] { m_canvas->redo(); });
    atb->addSeparator();
    atb->addAction(QStringLiteral("Xoay phải"), this, &MainWindow::rotateRight);
    atb->addAction(QStringLiteral("Lật ngang"), this, &MainWindow::flipHorizontal);
    atb->addAction(QStringLiteral("Sao chép"), QKeySequence::Copy, this, &MainWindow::copyToClipboard);

    // --- Thanh Quick Styles (preset nhanh: 1 click đổi tool+màu+nét) ---
    addToolBarBreak();
    auto *stb = addToolBar(QStringLiteral("Style nhanh"));
    stb->setMovable(false);
    auto *styleBar = new StyleBar(stb);
    connect(styleBar, &StyleBar::styleChosen, this, [this](const canvas::QuickStyle &s) {
        m_canvas->setTool(s.type);
        m_canvas->setColor(s.color);
        m_canvas->setPenWidth(s.penWidth);
        statusBar()->showMessage(QStringLiteral("Style: %1").arg(s.name), 1500);
    });
    stb->addWidget(styleBar);

    // Phím tắt TOÀN CỤC: Print Screen -> chụp vùng (kể cả khi app ở nền).
    m_hotkey = new hotkey::GlobalHotkey(this);
    if (m_hotkey->registerHotkey(1, Qt::NoModifier, Qt::Key_Print))
        connect(m_hotkey, &hotkey::GlobalHotkey::activated, this, [this](int id) {
            if (id == 1) captureRegion();
        });

    statusBar()->showMessage(QStringLiteral("Ezsnagit %1 — sẵn sàng").arg(core::appVersion()));
}

// Vùng logical phủ hết các màn hình (để overlay che toàn bộ).
static QRect allScreensGeometry() {
    QRect vg;
    for (const QScreen *s : QGuiApplication::screens())
        vg = vg.united(s->geometry());
    return vg;
}

void MainWindow::captureFullScreen()
{
    hide();
    QTimer::singleShot(200, this, [this] {
        const QImage img = capture::captureVirtualDesktop();
        show(); raise(); activateWindow();
        if (img.isNull()) {
            statusBar()->showMessage(QStringLiteral("Chụp thất bại (nền tảng chưa hỗ trợ?)"), 4000);
            return;
        }
        showCaptured(img);
    });
}

void MainWindow::captureRegion()
{
    hide();
    QTimer::singleShot(200, this, [this] {
        const QImage full = capture::captureVirtualDesktop();
        if (full.isNull()) {
            show();
            statusBar()->showMessage(QStringLiteral("Chụp thất bại (nền tảng chưa hỗ trợ?)"), 4000);
            return;
        }
        auto *ov = new RegionOverlay(full, allScreensGeometry());
        ov->setAttribute(Qt::WA_DeleteOnClose);
        connect(ov, &RegionOverlay::regionSelected, this, [this](const QImage &img) {
            show(); raise(); activateWindow();
            showCaptured(img);
        });
        connect(ov, &RegionOverlay::cancelled, this, [this] {
            show(); raise(); activateWindow();
            statusBar()->showMessage(QStringLiteral("Đã hủy chụp vùng"), 2000);
        });
        ov->show(); ov->raise(); ov->activateWindow(); ov->setFocus();
    });
}

void MainWindow::captureWindow()
{
    hide();
    QTimer::singleShot(200, this, [this] {
        const QImage full = capture::captureVirtualDesktop();
        if (full.isNull()) {
            show();
            statusBar()->showMessage(QStringLiteral("Chụp thất bại (nền tảng chưa hỗ trợ?)"), 4000);
            return;
        }
        const QPoint origin = capture::virtualDesktopRect().topLeft();
        QVector<QRect> winsImg;
        for (const QRect &r : capture::enumerateWindowRects())
            winsImg.append(r.translated(-origin));

        auto *ov = new WindowPickerOverlay(full, winsImg, allScreensGeometry());
        ov->setAttribute(Qt::WA_DeleteOnClose);
        connect(ov, &WindowPickerOverlay::windowSelected, this, [this](const QImage &img) {
            show(); raise(); activateWindow();
            showCaptured(img);
        });
        connect(ov, &WindowPickerOverlay::cancelled, this, [this] {
            show(); raise(); activateWindow();
            statusBar()->showMessage(QStringLiteral("Đã hủy chụp cửa sổ"), 2000);
        });
        ov->show(); ov->raise(); ov->activateWindow(); ov->setFocus();
    });
}

void MainWindow::captureScrolling()
{
    auto *dlg = new PanoramicDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    // Mỗi lần bấm "Chụp thêm khung": ẩn dialog + app, chọn vùng, nạp khung vào dialog.
    connect(dlg, &PanoramicDialog::captureRequested, this, [this, dlg] {
        dlg->hide();
        hide();
        QTimer::singleShot(200, this, [this, dlg] {
            const QImage full = capture::captureVirtualDesktop();
            auto reshow = [this, dlg] { show(); dlg->show(); dlg->raise(); dlg->activateWindow(); };
            if (full.isNull()) { reshow(); return; }
            auto *ov = new RegionOverlay(full, allScreensGeometry());
            ov->setAttribute(Qt::WA_DeleteOnClose);
            connect(ov, &RegionOverlay::regionSelected, this, [this, dlg, reshow](const QImage &img) {
                show(); dlg->addFrame(img); Q_UNUSED(reshow); dlg->show(); dlg->raise(); dlg->activateWindow();
            });
            connect(ov, &RegionOverlay::cancelled, this, [reshow] { reshow(); });
            ov->show(); ov->raise(); ov->activateWindow(); ov->setFocus();
        });
    });

    // "Ghép xong": nối các khung thành ảnh dài rồi mở trong editor.
    connect(dlg, &QDialog::accepted, this, [this, dlg] {
        const QImage pano = capture::stitchMany(dlg->frames());
        if (!pano.isNull()) {
            showCaptured(pano);
            statusBar()->showMessage(
                QStringLiteral("Đã ghép %1 khung -> %2 × %3 px")
                    .arg(dlg->frames().size()).arg(pano.width()).arg(pano.height()), 5000);
        }
    });

    dlg->show();
}

void MainWindow::showCaptured(const QImage &img)
{
    m_canvas->setImage(img);
    if (m_recent) m_recent->addCapture(img);   // lưu vào khay ảnh gần đây
    // Tự copy clipboard nếu bật (enabler bug-report loop: chụp -> dán).
    if (core::Settings::autoCopy())
        share::copyImageToClipboard(img);
    statusBar()->showMessage(
        QStringLiteral("Đã chụp %1 × %2 px").arg(img.width()).arg(img.height()), 5000);
}

void MainWindow::quickSave()
{
    if (!m_canvas->hasImage()) {
        statusBar()->showMessage(QStringLiteral("Chưa có ảnh để lưu"), 2000);
        return;
    }
    const QString dir = core::Settings::defaultSaveDir();
    QDir().mkpath(dir);
    const int seq = core::Settings::nextQuickSaveSeq();
    const QString path = QDir(dir).filePath(QStringLiteral("Ezsnagit-%1.png").arg(seq));
    if (share::saveImagePng(m_canvas->document().renderFlattened(), path))
        statusBar()->showMessage(QStringLiteral("Đã lưu nhanh: %1").arg(path), 4000);
    else
        statusBar()->showMessage(QStringLiteral("Lưu nhanh thất bại"), 3000);
}

void MainWindow::copyToClipboard()
{
    if (!m_canvas->hasImage()) {
        statusBar()->showMessage(QStringLiteral("Chưa có ảnh để sao chép"), 2000);
        return;
    }
    if (share::copyImageToClipboard(m_canvas->document().renderFlattened()))
        statusBar()->showMessage(QStringLiteral("Đã sao chép ảnh (kèm chú thích) vào clipboard"), 2000);
}

void MainWindow::rotateRight()
{
    if (!m_canvas->hasImage()) return;
    m_canvas->setImage(effects::rotate90(m_canvas->document().renderFlattened(), true));
}

void MainWindow::flipHorizontal()
{
    if (!m_canvas->hasImage()) return;
    m_canvas->setImage(effects::flipHorizontal(m_canvas->document().renderFlattened()));
}

void MainWindow::openFile()
{
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Mở tài liệu"), QString(),
        QStringLiteral("Ezsnagit (*.ezsnagx);;Ảnh (*.png *.jpg *.bmp)"));
    if (path.isEmpty()) return;

    if (path.endsWith(QStringLiteral(".ezsnagx"), Qt::CaseInsensitive)) {
        canvas::CanvasDocument doc;
        if (canvas::loadEzsnagx(doc, path)) {
            m_canvas->loadDocument(doc);
            statusBar()->showMessage(QStringLiteral("Đã mở %1").arg(path), 4000);
        } else {
            statusBar()->showMessage(QStringLiteral("Mở thất bại"), 4000);
        }
    } else {
        QImage img(path);
        if (!img.isNull()) showCaptured(img);
        else statusBar()->showMessage(QStringLiteral("Không đọc được ảnh"), 4000);
    }
}

void MainWindow::saveFile()
{
    if (!m_canvas->hasImage()) {
        statusBar()->showMessage(QStringLiteral("Chưa có gì để lưu"), 2000);
        return;
    }
    QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("Lưu tài liệu"), QStringLiteral("anh.ezsnagx"),
        QStringLiteral("Ezsnagit (*.ezsnagx)"));
    if (path.isEmpty()) return;
    if (!path.endsWith(QStringLiteral(".ezsnagx"), Qt::CaseInsensitive))
        path += QStringLiteral(".ezsnagx");
    if (canvas::saveEzsnagx(m_canvas->document(), path))
        statusBar()->showMessage(QStringLiteral("Đã lưu %1").arg(path), 4000);
    else
        statusBar()->showMessage(QStringLiteral("Lưu thất bại"), 4000);
}

void MainWindow::exportPng()
{
    if (!m_canvas->hasImage()) {
        statusBar()->showMessage(QStringLiteral("Chưa có ảnh để xuất"), 2000);
        return;
    }
    QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("Xuất PNG"), QStringLiteral("anh.png"),
        QStringLiteral("PNG (*.png)"));
    if (path.isEmpty()) return;
    if (!path.endsWith(QStringLiteral(".png"), Qt::CaseInsensitive))
        path += QStringLiteral(".png");
    if (canvas::exportPng(m_canvas->document(), path))
        statusBar()->showMessage(QStringLiteral("Đã xuất %1").arg(path), 4000);
    else
        statusBar()->showMessage(QStringLiteral("Xuất thất bại"), 4000);
}
