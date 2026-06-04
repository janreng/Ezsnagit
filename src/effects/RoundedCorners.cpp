#include "effects/RoundedCorners.h"
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <algorithm>

namespace effects {

QImage roundCorners(const QImage &img, int radius) {
    // Ảnh rỗng -> trả về như cũ (đã chuẩn ARGB32 cho nhất quán).
    if (img.isNull())
        return img.convertToFormat(QImage::Format_ARGB32);

    // radius <= 0 -> không bo, trả copy ARGB32 nguyên ảnh.
    if (radius <= 0)
        return img.convertToFormat(QImage::Format_ARGB32);

    const int w = img.width();
    const int h = img.height();

    // Kẹp bán kính <= nửa cạnh ngắn để góc bo không vượt quá ảnh.
    const int maxR = std::min(w, h) / 2;
    int r = std::min(radius, maxR);
    if (r <= 0)
        return img.convertToFormat(QImage::Format_ARGB32);

    // Ảnh đích trong suốt hoàn toàn (alpha=0 khắp nơi).
    QImage out(w, h, QImage::Format_ARGB32);
    out.fill(Qt::transparent);

    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Clip theo path bo góc rồi vẽ ảnh gốc -> ngoài vùng bo vẫn trong suốt.
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, w, h), r, r);
    p.setClipPath(path);
    p.drawImage(0, 0, img);
    p.end();

    return out;
}

} // namespace effects
