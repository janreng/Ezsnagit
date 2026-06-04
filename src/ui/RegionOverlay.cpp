#include "ui/RegionOverlay.h"
#include "capture/ScreenGeometry.h"

#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGuiApplication>

RegionOverlay::RegionOverlay(const QImage &frozen, const QRect &overlayGeometry, QWidget *parent)
    : QWidget(parent), m_frozen(frozen)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                   | Qt::BypassWindowManagerHint);
    setGeometry(overlayGeometry);
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
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
    if (e->button() == Qt::RightButton) { emit cancelled(); close(); return; }
    if (e->button() == Qt::LeftButton) {
        m_selecting = true;
        m_start = m_cur = e->pos();
        update();
    }
}

void RegionOverlay::mouseMoveEvent(QMouseEvent *e)
{
    if (m_selecting) { m_cur = e->pos(); update(); }
}

void RegionOverlay::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton || !m_selecting) return;
    m_selecting = false;
    const QRect sel = selectionWidgetRect();
    if (sel.width() < 3 || sel.height() < 3) { emit cancelled(); close(); return; }

    const QRect inImg = capture::mapWidgetRectToImage(sel, size(), m_frozen.size());
    if (inImg.isEmpty()) { emit cancelled(); close(); return; }
    emit regionSelected(m_frozen.copy(inImg));
    close();
}

void RegionOverlay::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) { emit cancelled(); close(); }
}
