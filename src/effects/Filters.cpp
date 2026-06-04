#include "effects/Filters.h"
#include <QColor>
#include <QtGlobal>
#include <algorithm>
#include <cmath>

namespace effects {

// Đảm bảo ảnh ở Format_ARGB32 để quét scanLine theo từng pixel 0xAARRGGBB.
static QImage toArgb32(const QImage &img) {
    if (img.format() == QImage::Format_ARGB32) return img;
    return img.convertToFormat(QImage::Format_ARGB32);
}

// Clamp một giá trị int về [0,255].
static inline int clamp255(int v) {
    return qBound(0, v, 255);
}

// Khung quét chung: duyệt mọi pixel, gọi fn(r,g,b) -> trả (r,g,b) mới.
// Alpha của từng pixel được giữ nguyên. fn trả mảng 3 phần tử đã clamp.
template <typename Fn>
static QImage mapPixels(const QImage &img, Fn fn) {
    if (img.isNull()) return img;

    QImage out = toArgb32(img).copy(); // bản copy độc lập, đúng định dạng
    const int w = out.width();
    const int h = out.height();

    for (int y = 0; y < h; ++y) {
        // scanLine của ARGB32 là mảng QRgb (uint 0xAARRGGBB) -> nhanh.
        QRgb *line = reinterpret_cast<QRgb *>(out.scanLine(y));
        for (int x = 0; x < w; ++x) {
            const QRgb px = line[x];
            const int a = qAlpha(px);
            int rgb[3] = { qRed(px), qGreen(px), qBlue(px) };
            fn(rgb);
            line[x] = qRgba(clamp255(rgb[0]), clamp255(rgb[1]), clamp255(rgb[2]), a);
        }
    }
    return out;
}

QImage grayscale(const QImage &img) {
    return mapPixels(img, [](int *rgb) {
        // Luma chuẩn — làm tròn bằng +0.5.
        const int g = static_cast<int>(0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2] + 0.5);
        rgb[0] = rgb[1] = rgb[2] = g;
    });
}

QImage invert(const QImage &img) {
    return mapPixels(img, [](int *rgb) {
        rgb[0] = 255 - rgb[0];
        rgb[1] = 255 - rgb[1];
        rgb[2] = 255 - rgb[2];
    });
}

QImage sepia(const QImage &img) {
    return mapPixels(img, [](int *rgb) {
        const int r = rgb[0], g = rgb[1], b = rgb[2];
        // Công thức sepia chuẩn (clamp lo ở khung mapPixels).
        rgb[0] = static_cast<int>(0.393 * r + 0.769 * g + 0.189 * b + 0.5);
        rgb[1] = static_cast<int>(0.349 * r + 0.686 * g + 0.168 * b + 0.5);
        rgb[2] = static_cast<int>(0.272 * r + 0.534 * g + 0.131 * b + 0.5);
    });
}

QImage adjustBrightness(const QImage &img, int delta) {
    return mapPixels(img, [delta](int *rgb) {
        rgb[0] += delta;
        rgb[1] += delta;
        rgb[2] += delta;
    });
}

QImage adjustContrast(const QImage &img, double factor) {
    return mapPixels(img, [factor](int *rgb) {
        for (int i = 0; i < 3; ++i) {
            rgb[i] = static_cast<int>(std::lround(128.0 + (rgb[i] - 128) * factor));
        }
    });
}

} // namespace effects
