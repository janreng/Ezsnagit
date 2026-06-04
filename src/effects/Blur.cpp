#include "effects/Blur.h"
#include <QPainter>
#include <QSize>
#include <algorithm>

namespace effects {

// Tính vùng làm việc: giao region với biên ảnh. Rỗng -> trả QRect rỗng.
static QRect clampRegion(const QImage &img, const QRect &region) {
    return region.intersected(img.rect());
}

// Áp một patch (đã xử lý) đè lên vùng r của một bản copy ảnh gốc.
static QImage paintPatch(const QImage &img, const QRect &r, const QImage &patch) {
    QImage out = img.copy(); // bản copy độc lập với ảnh gốc
    QPainter p(&out);
    p.drawImage(r.topLeft(), patch);
    p.end();
    return out;
}

QImage blurRegion(const QImage &img, const QRect &region, int strength) {
    if (img.isNull()) return img;

    QRect r = clampRegion(img, region);
    if (r.isEmpty()) return img.copy(); // không có gì để làm -> copy nguyên ảnh

    // strength tối thiểu 1 để tránh chia 0 / kích thước âm.
    const int s = std::max(1, strength);

    // Crop vùng cần mờ.
    QImage patch = img.copy(r);

    // Kích thước thu nhỏ — tối thiểu 1px mỗi chiều.
    const int smallW = std::max(1, patch.width() / s);
    const int smallH = std::max(1, patch.height() / s);

    // Scale xuống rồi scale lên lại bằng SmoothTransformation -> hiệu ứng mờ.
    QImage small = patch.scaled(smallW, smallH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage blurred = small.scaled(patch.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    return paintPatch(img, r, blurred);
}

QImage pixelateRegion(const QImage &img, const QRect &region, int blockSize) {
    if (img.isNull()) return img;

    QRect r = clampRegion(img, region);
    if (r.isEmpty()) return img.copy();

    // blockSize tối thiểu 1 để tránh chia 0 / kích thước âm.
    const int b = std::max(1, blockSize);

    QImage patch = img.copy(r);

    const int smallW = std::max(1, patch.width() / b);
    const int smallH = std::max(1, patch.height() / b);

    // Scale xuống (smooth để gom màu) rồi scale lên bằng FastTransformation (nearest)
    // -> các ô vuông pixel to, sắc cạnh kiểu mosaic.
    QImage small = patch.scaled(smallW, smallH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage mosaic = small.scaled(patch.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    return paintPatch(img, r, mosaic);
}

} // namespace effects
