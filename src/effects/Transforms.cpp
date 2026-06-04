#include "effects/Transforms.h"
#include <QColor>
#include <QTransform>

namespace effects {

QImage crop(const QImage &img, const QRect &r) {
    // Giao vùng yêu cầu với biên ảnh; rỗng -> trả ảnh rỗng.
    QRect c = r.intersected(img.rect());
    if (c.isEmpty()) return QImage();
    return img.copy(c);
}

QImage rotate90(const QImage &img, bool clockwise) {
    if (img.isNull()) return QImage();
    QTransform t;
    t.rotate(clockwise ? 90.0 : -90.0);
    return img.transformed(t, Qt::SmoothTransformation);
}

QImage flipHorizontal(const QImage &img) {
    // mirrored(horizontal, vertical)
    return img.mirrored(true, false);
}

QImage flipVertical(const QImage &img) {
    return img.mirrored(false, true);
}

QImage scaledTo(const QImage &img, const QSize &size) {
    if (img.isNull() || size.isEmpty()) return QImage();
    return img.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QImage autoTrim(const QImage &img) {
    if (img.isNull() || img.width() == 0 || img.height() == 0) return img;

    // Màu nền lấy từ pixel góc trên-trái.
    QImage src = img.convertToFormat(QImage::Format_ARGB32);
    const QRgb bg = src.pixel(0, 0);

    const int w = src.width();
    const int h = src.height();
    int left = w, top = h, right = -1, bottom = -1;

    // Quét tìm bounding box các pixel KHÁC màu nền.
    for (int y = 0; y < h; ++y) {
        const QRgb *line = reinterpret_cast<const QRgb *>(src.constScanLine(y));
        for (int x = 0; x < w; ++x) {
            if (line[x] != bg) {
                if (x < left) left = x;
                if (x > right) right = x;
                if (y < top) top = y;
                if (y > bottom) bottom = y;
            }
        }
    }

    // Cả ảnh đồng màu -> không có pixel khác nền -> trả nguyên ảnh.
    if (right < 0 || bottom < 0) return img;

    QRect box(left, top, right - left + 1, bottom - top + 1);
    return img.copy(box);
}

} // namespace effects
