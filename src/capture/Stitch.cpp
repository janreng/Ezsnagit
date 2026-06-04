#include "capture/Stitch.h"
#include <QPainter>
#include <algorithm>

namespace capture {

// So khớp `overlap` hàng cuối của top với `overlap` hàng đầu của bottom.
// Trả true nếu tỉ lệ pixel giống >= 99%. So theo từng pixel (ARGB).
static bool rowsMatch(const QImage &top, const QImage &bottom, int overlap) {
    const int w = top.width();
    const int topStart = top.height() - overlap; // hàng đầu tiên của vùng đáy top
    qint64 total = static_cast<qint64>(overlap) * w;
    if (total <= 0) return false;
    qint64 same = 0;
    for (int i = 0; i < overlap; ++i) {
        const int ty = topStart + i;   // hàng trong top
        const int by = i;              // hàng tương ứng trong bottom
        for (int x = 0; x < w; ++x) {
            if (top.pixel(x, ty) == bottom.pixel(x, by)) ++same;
        }
    }
    // >= 99% pixel giống (cho phép sai khác nhỏ do nén/anti-alias khi cuộn).
    return same * 100 >= total * 99;
}

int detectVerticalOverlap(const QImage &top, const QImage &bottom, int maxOverlap) {
    if (top.isNull() || bottom.isNull()) return 0;
    if (top.width() != bottom.width()) return 0; // khác width -> không ghép được

    int hi = std::min(top.height(), bottom.height());
    if (maxOverlap > 0) hi = std::min(hi, maxOverlap);
    if (hi <= 0) return 0;

    // Thử từ lớn xuống nhỏ; trả overlap LỚN NHẤT khớp.
    for (int overlap = hi; overlap >= 1; --overlap) {
        if (rowsMatch(top, bottom, overlap)) return overlap;
    }
    return 0;
}

QImage stitchVertical(const QImage &top, const QImage &bottom, int overlap) {
    if (top.isNull()) return bottom;
    if (bottom.isNull()) return top;

    // Kẹp overlap về [0, bottom.height].
    overlap = std::clamp(overlap, 0, bottom.height());

    const int w = top.width();
    const int outH = top.height() + bottom.height() - overlap;

    QImage out(w, outH, QImage::Format_ARGB32);
    out.fill(Qt::transparent);

    QPainter p(&out);
    // Vẽ toàn bộ top ở (0,0).
    p.drawImage(0, 0, top);
    // Vẽ phần bottom từ hàng `overlap` trở xuống, đặt ngay dưới top.
    const int bottomKeep = bottom.height() - overlap;
    if (bottomKeep > 0) {
        p.drawImage(QPoint(0, top.height()),
                    bottom,
                    QRect(0, overlap, w, bottomKeep));
    }
    p.end();
    return out;
}

QImage stitchAuto(const QImage &top, const QImage &bottom) {
    return stitchVertical(top, bottom, detectVerticalOverlap(top, bottom));
}

} // namespace capture
