#include "ui/RegionOverlay.h"
#include "ui/Magnifier.h"
#include "capture/ScreenGeometry.h"

#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QGuiApplication>

RegionOverlay::RegionOverlay(const QImage &frozen, const QRect &overlayGeometry, QWidget *parent)
    : QWidget(parent), m_frozen(frozen)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                   | Qt::BypassWindowManagerHint);
    setGeometry(overlayGeometry);
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
    m_mag = new Magnifier(this);   // kính lúp con, hiện khi di chuột
}

// Đặt kính lúp gần con trỏ (lệch để không che vùng đang ngắm), trong biên overlay.
void RegionOverlay::updateMagnifier(const QPoint &cursor)
{
    if (!m_mag) return;
    const QPoint imgPt = capture::mapWidgetPointToImage(cursor, size(), m_frozen.size());
    m_mag->showAt(m_frozen, imgPt, 8);
    QPoint pos = cursor + QPoint(20, 20);
    if (pos.x() + m_mag->width() > width())  pos.setX(cursor.x() - 20 - m_mag->width());
    if (pos.y() + m_mag->height() > height()) pos.setY(cursor.y() - 20 - m_mag->height());
    m_mag->move(pos);
    m_mag->show();
    m_mag->raise();
}

QRect RegionOverlay::selectionWidgetRect() const
{
    return QRect(m_start, m_cur).normalized();
}

void RegionOverlay::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    // Ảnh nền (đã đóng băng) scale vừa khít widget.
    p.drawImage(rect(), m_frozen);
    // Lớp tối phủ toàn bộ.
    p.fillRect(rect(), QColor(0, 0, 0, 120));

    if (m_selecting) {
        const QRect sel = selectionWidgetRect();
        // Vùng chọn: vẽ lại ảnh gốc (sáng) + viền + kích thước.
        p.drawImage(sel, m_frozen, sel);
        p.setPen(QPen(QColor(33, 115, 70), 2)); // xanh Ezcel/Excel
        p.drawRect(sel.adjusted(0, 0, -1, -1));

        const QImage &img = m_frozen;
        const QRect inImg = capture::mapWidgetRectToImage(sel, size(), img.size());
        const QString label = QStringLiteral("%1 × %2").arg(inImg.width()).arg(inImg.height());
        p.setPen(Qt::white);
        QRect tr(sel.topLeft() - QPoint(0, 20), QSize(120, 18));
        p.fillRect(QRect(sel.topLeft() - QPoint(0, 20), QSize(80, 18)), QColor(33, 115, 70));
        p.drawText(tr.adjusted(4, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, label);
    }
}

void RegionOverlay::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton) { close(); return; }   // hủy (closeEvent)
    if (e->button() == Qt::LeftButton) {
        m_selecting = true;
        m_start = m_cur = e->pos();
        update();
    }
}

void RegionOverlay::mouseMoveEvent(QMouseEvent *e)
{
    if (m_selecting) { m_cur = e->pos(); update(); }
    updateMagnifier(e->pos());   // kính lúp theo con trỏ (cả khi chưa kéo)
}

void RegionOverlay::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton || !m_selecting) return;
    if (m_mag) m_mag->hide();
    m_selecting = false;
    const QRect sel = selectionWidgetRect();
    if (sel.width() < 3 || sel.height() < 3) { close(); return; }   // quá nhỏ -> hủy

    const QRect inImg = capture::mapWidgetRectToImage(sel, size(), m_frozen.size());
    if (inImg.isEmpty()) { close(); return; }
    m_done = true;
    emit regionSelected(m_frozen.copy(inImg));
    close();
}

void RegionOverlay::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) close();
}

void RegionOverlay::closeEvent(QCloseEvent *)
{
    if (!m_done) { m_done = true; emit cancelled(); }   // mọi đường đóng -> luôn có 1 tín hiệu
}
