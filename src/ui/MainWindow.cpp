#include "ui/MainWindow.h"
#include "ui/RegionOverlay.h"
#include "ui/WindowPickerOverlay.h"
#include "core/Version.h"
#include "capture/ScreenCapture.h"
#include "hotkey/GlobalHotkey.h"
#include "share/ImageClipboard.h"

#include <QLabel>
#include <QScrollArea>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Ezsnagit"));
    resize(1000, 680);

    // Thanh công cụ tối giản (P1): chụp toàn màn hình.
    auto *tb = addToolBar(QStringLiteral("Capture"));
    tb->setMovable(false);
    QAction *regAct = tb->addAction(QStringLiteral("Chụp vùng"));
    regAct->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+A")));
    connect(regAct, &QAction::triggered, this, &MainWindow::captureRegion);

    QAction *winAct = tb->addAction(QStringLiteral("Chụp cửa sổ"));
    winAct->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+W")));
    connect(winAct, &QAction::triggered, this, &MainWindow::captureWindow);

    QAction *capAct = tb->addAction(QStringLiteral("Chụp toàn màn hình"));
    capAct->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+F")));
    connect(capAct, &QAction::triggered, this, &MainWindow::captureFullScreen);

    tb->addSeparator();
    QAction *copyAct = tb->addAction(QStringLiteral("Sao chép"));
    copyAct->setShortcut(QKeySequence::Copy);   // Ctrl+C
    connect(copyAct, &QAction::triggered, this, &MainWindow::copyToClipboard);

    // Phím tắt TOÀN CỤC: Print Screen -> chụp vùng (kể cả khi app ở nền).
    m_hotkey = new hotkey::GlobalHotkey(this);
    if (m_hotkey->registerHotkey(1, Qt::NoModifier, Qt::Key_Print))
        connect(m_hotkey, &hotkey::GlobalHotkey::activated, this, [this](int id) {
            if (id == 1) captureRegion();
        });

    // Canvas = QLabel trong QScrollArea (placeholder cho editor canvas sau này).
    m_canvas = new QLabel(this);
    m_canvas->setAlignment(Qt::AlignCenter);
    m_canvas->setText(QStringLiteral("Ezsnagit %1\nBấm \"Chụp toàn màn hình\" (Ctrl+Shift+F)")
                          .arg(core::appVersion()));
    m_scroll = new QScrollArea(this);
    m_scroll->setWidget(m_canvas);
    // resizable=false: canvas giữ kích thước thật của ảnh chụp -> có scrollbar khi ảnh lớn
    // hơn viewport (xem ảnh full-size). Nội dung nhỏ (text placeholder) được căn giữa.
    m_scroll->setWidgetResizable(false);
    m_scroll->setAlignment(Qt::AlignCenter);
    setCentralWidget(m_scroll);

    statusBar()->showMessage(QStringLiteral("Sẵn sàng"));
}

void MainWindow::captureFullScreen()
{
    // Ẩn cửa sổ app để không tự chụp chính nó, đợi một nhịp cho redraw rồi chụp.
    hide();
    QTimer::singleShot(200, this, [this] {
        const QImage img = capture::captureVirtualDesktop();
        show();
        raise();
        activateWindow();
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
        // Vùng logical phủ hết các màn hình (để overlay che toàn bộ).
        QRect vg;
        for (const QScreen *s : QGuiApplication::screens())
            vg = vg.united(s->geometry());

        auto *ov = new RegionOverlay(full, vg);
        ov->setAttribute(Qt::WA_DeleteOnClose);
        connect(ov, &RegionOverlay::regionSelected, this, [this](const QImage &img) {
            show(); raise(); activateWindow();
            showCaptured(img);
        });
        connect(ov, &RegionOverlay::cancelled, this, [this] {
            show(); raise(); activateWindow();
            statusBar()->showMessage(QStringLiteral("Đã hủy chụp vùng"), 2000);
        });
        ov->show();
        ov->raise();
        ov->activateWindow();
        ov->setFocus();
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
        // Rect cửa sổ ở toạ độ virtual desktop -> dời về toạ độ ẢNH (gốc ảnh = góc virtual desktop).
        const QPoint origin = capture::virtualDesktopRect().topLeft();
        QVector<QRect> winsImg;
        for (const QRect &r : capture::enumerateWindowRects())
            winsImg.append(r.translated(-origin));

        QRect vg;
        for (const QScreen *s : QGuiApplication::screens())
            vg = vg.united(s->geometry());

        auto *ov = new WindowPickerOverlay(full, winsImg, vg);
        ov->setAttribute(Qt::WA_DeleteOnClose);
        connect(ov, &WindowPickerOverlay::windowSelected, this, [this](const QImage &img) {
            show(); raise(); activateWindow();
            showCaptured(img);
        });
        connect(ov, &WindowPickerOverlay::cancelled, this, [this] {
            show(); raise(); activateWindow();
            statusBar()->showMessage(QStringLiteral("Đã hủy chụp cửa sổ"), 2000);
        });
        ov->show();
        ov->raise();
        ov->activateWindow();
        ov->setFocus();
    });
}

void MainWindow::showCaptured(const QImage &img)
{
    m_lastImage = img;
    m_canvas->setPixmap(QPixmap::fromImage(img));
    m_canvas->resize(img.size());
    // Tự copy vào clipboard ngay (enabler bug-report loop: chụp -> dán).
    share::copyImageToClipboard(img);
    statusBar()->showMessage(
        QStringLiteral("Đã chụp %1 × %2 px — đã copy vào clipboard").arg(img.width()).arg(img.height()), 5000);
}

void MainWindow::copyToClipboard()
{
    if (m_lastImage.isNull()) {
        statusBar()->showMessage(QStringLiteral("Chưa có ảnh để sao chép"), 2000);
        return;
    }
    if (share::copyImageToClipboard(m_lastImage))
        statusBar()->showMessage(QStringLiteral("Đã sao chép ảnh vào clipboard"), 2000);
}
