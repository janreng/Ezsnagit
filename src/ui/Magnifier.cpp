#include "Magnifier.h"

#include <QPainter>
#include <QPaintEvent>
#include <QRect>

Magnifier::Magnifier(QWidget *parent)
    : QWidget(parent) {
    // Frameless child widget, kích thước nhỏ cố định.
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setFixedSize(132, 132);
}

void Magnifier::showAt(const QImage &source, const QPoint &focusImgPt, int zoom) {
    m_source = source;
    m_focusImgPt = focusImgPt;
    m_zoom = (zoom > 0) ? zoom : 1;   // tránh chia cho 0
    update();                          // trigger repaint, không tự move()
}

bool Magnifier::hasImage() const {
    return !m_source.isNull();
}

void Magnifier::paintEvent(QPaintEvent * /*event*/) {
    QPainter p(this);
    const QRect widgetRect(0, 0, width(), height());

    // Không có ảnh -> tô xám.
    if (m_source.isNull()) {
        p.fillRect(widgetRect, QColor(128, 128, 128));
        return;
    }

    // Kích thước vùng crop trong ảnh nguồn (tính theo pixel ảnh).
    const int cropW = width() / m_zoom;
    const int cropH = height() / m_zoom;

    // Vùng crop "lý tưởng" — căn giữa quanh focus point (có thể tràn ra ngoài ảnh).
    const int idealX = m_focusImgPt.x() - cropW / 2;
    const int idealY = m_focusImgPt.y() - cropH / 2;
    const QRect idealCrop(idealX, idealY, cropW, cropH);

    // Nền tối cho phần (nếu có) nằm ngoài ảnh.
    p.fillRect(widgetRect, QColor(40, 40, 40));

    // Clamp vùng crop vào trong ảnh nguồn.
    const QRect imageRect(0, 0, m_source.width(), m_source.height());
    const QRect crop = idealCrop.intersected(imageRect);

    if (!crop.isEmpty()) {
        // Crop phần hợp lệ, phóng to bằng nearest-neighbor (FastTransformation)
        // để thấy rõ từng pixel.
        const QImage piece = m_source.copy(crop);

        // Tính đúng vị trí vẽ trong widget: bù phần bị clamp ở mép trái/trên.
        const int offsetX = (crop.left() - idealCrop.left()) * m_zoom;
        const int offsetY = (crop.top() - idealCrop.top()) * m_zoom;
        const int destW = crop.width() * m_zoom;
        const int destH = crop.height() * m_zoom;
        const QRect destRect(offsetX, offsetY, destW, destH);

        const QImage scaled = piece.scaled(destW, destH,
                                           Qt::IgnoreAspectRatio,
                                           Qt::FastTransformation);
        p.drawImage(destRect.topLeft(), scaled);
    }

    // Crosshair 1px ở chính giữa widget.
    p.setPen(QPen(QColor(33, 115, 70), 1));
    const int cx = width() / 2;
    const int cy = height() / 2;
    p.drawLine(cx, 0, cx, height());
    p.drawLine(0, cy, width(), cy);

    // Khung viền (color 33,115,70).
    p.setPen(QPen(QColor(33, 115, 70), 1));
    p.setBrush(Qt::NoBrush);
    p.drawRect(0, 0, width() - 1, height() - 1);
}
