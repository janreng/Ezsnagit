#include "ui/WindowPickerOverlay.h"
#include "capture/ScreenGeometry.h"

#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCloseEvent>

WindowPickerOverlay::WindowPickerOverlay(const QImage &frozen, const QVector<QRect> &windowRectsImg,
                                         const QRect &overlayGeometry, QWidget *parent)
    : QWidget(parent), m_frozen(frozen), m_winImg(windowRectsImg)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                   | Qt::BypassWindowManagerHint);
    setGeometry(overlayGeometry);
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
}

// Đổi rect (toạ độ ảnh) -> rect (toạ độ widget) để vẽ highlight.
static QRect imgRectToWidget(const QRect &r, const QSize &widget, const QSize &img) {
    if (img.isEmpty()) return QRect();
    const double sx = double(widget.width())  / img.width();
    const double sy = double(widget.height()) / img.height();
    return QRect(qRound(r.x() * sx), qRound(r.y() * sy),
                 qRound(r.width() * sx), qRound(r.height() * sy));
}

void WindowPickerOverlay::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(rect(), m_frozen);
    p.fillRect(rect(), QColor(0, 0, 0, 120));

    if (m_hover >= 0 && m_hover < m_winImg.size()) {
        const QRect wr = imgRectToWidget(m_winImg.at(m_hover), size(), m_frozen.size());
        p.drawImage(wr, m_frozen, m_winImg.at(m_hover)); // vùng cửa sổ: vẽ lại sáng
        p.setPen(QPen(QColor(33, 115, 70), 2));
        p.drawRect(wr.adjusted(0, 0, -1, -1));
    }
}

void WindowPickerOverlay::mouseMoveEvent(QMouseEvent *e)
{
    const QPoint imgPt = capture::mapWidgetPointToImage(e->pos(), size(), m_frozen.size());
    const int hit = capture::topWindowAt(m_winImg, imgPt);
    if (hit != m_hover) { m_hover = hit; update(); }
}

void WindowPickerOverlay::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton) { close(); return; }
    if (e->button() != Qt::LeftButton) return;
    QRect wr;
    if (m_hover >= 0 && m_hover < m_winImg.size())
        wr = m_winImg.at(m_hover).intersected(QRect(QPoint(0, 0), m_frozen.size()));
    if (wr.isEmpty()) { close(); return; }      // không trúng cửa sổ -> hủy (closeEvent)
    m_done = true;
    emit windowSelected(m_frozen.copy(wr));
    close();
}

void WindowPickerOverlay::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) close();
}

void WindowPickerOverlay::closeEvent(QCloseEvent *)
{
    if (!m_done) { m_done = true; emit cancelled(); }   // mọi đường đóng -> luôn có 1 tín hiệu
}
