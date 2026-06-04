#include "effects/Frame.h"
#include <QPainter>
#include <QRect>

namespace effects {

QImage addBorder(const QImage &img, int width, const QColor &color) {
    if (img.isNull()) return QImage();
    // width<=0 -> không vẽ gì, trả bản copy.
    if (width <= 0) return img.copy();

    // Đảm bảo format có alpha & tách khỏi ảnh gốc.
    QImage out = img.convertToFormat(QImage::Format_ARGB32);

    const int w = out.width();
    const int h = out.height();

    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, false);
    // Vẽ đè màu đặc, không hoà trộn alpha (giữ màu viền đúng nguyên).
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.setPen(Qt::NoPen);
    p.setBrush(color);

    // Kẹp bề dày để không vượt quá nửa ảnh (tránh các rect chồng âm).
    const int bw = qMin(width, (qMin(w, h) + 1) / 2);

    // 4 dải viền vẽ ĐÈ vào trong, không đổi kích thước ảnh.
    p.drawRect(QRect(0, 0, w, bw));               // trên
    p.drawRect(QRect(0, h - bw, w, bw));          // dưới
    p.drawRect(QRect(0, 0, bw, h));               // trái
    p.drawRect(QRect(w - bw, 0, bw, h));          // phải
    p.end();

    return out;
}

QImage dropShadow(const QImage &img, int margin, int blurRadius,
                  const QColor &shadow) {
    if (img.isNull()) return QImage();
    if (margin < 0) margin = 0;
    if (blurRadius < 0) blurRadius = 0;

    const int w = img.width();
    const int h = img.height();

    // Canvas mở rộng, nền trong suốt.
    QImage out(w + 2 * margin, h + 2 * margin, QImage::Format_ARGB32);
    out.fill(Qt::transparent);

    // Vị trí ảnh gốc nằm giữa canvas.
    const int imgX = margin;
    const int imgY = margin;

    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);

    // --- Bóng đổ ---
    // Vẽ nhiều lớp rect alpha giảm dần để giả blur (kỹ thuật xếp lớp).
    // Lớp ngoài cùng to & mờ nhất, lớp trong cùng nhỏ & đậm nhất.
    const int off = margin / 2;                 // độ lệch xuống-phải
    const int layers = qMax(1, blurRadius);     // số lớp ~ bán kính blur

    const int baseX = imgX + off;
    const int baseY = imgY + off;

    for (int i = layers - 1; i >= 0; --i) {
        // Lớp i mở rộng ra grow px mỗi cạnh quanh vùng ảnh.
        const int grow = i;
        QRect r(baseX - grow, baseY - grow, w + 2 * grow, h + 2 * grow);
        // Alpha mỗi lớp giảm dần theo độ rộng (giả vầng mờ).
        int a = shadow.alpha() / layers;
        if (a < 1) a = 1;
        QColor c = shadow;
        c.setAlpha(a);
        p.setBrush(c);
        p.drawRoundedRect(r, blurRadius, blurRadius);
    }

    // --- Ảnh gốc vẽ lên trên ---
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(imgX, imgY, img);
    p.end();

    return out;
}

} // namespace effects
