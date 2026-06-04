// Test headless cho ghép ảnh dọc (capture::Stitch) — logic thuần, dùng ảnh synthetic.
// Cần QGuiApplication + nền tảng offscreen vì QImage/QPainter thuộc Qt Gui.
#include "capture/Stitch.h"
#include <QGuiApplication>
#include <QImage>
#include <QColor>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using capture::detectVerticalOverlap;
using capture::stitchVertical;
using capture::stitchAuto;

// Ảnh gradient theo hàng: mỗi hàng y tô màu QColor(y,y,y) -> mỗi hàng khác nhau.
static QImage makeGradient(int w, int h) {
    QImage img(w, h, QImage::Format_ARGB32);
    for (int y = 0; y < h; ++y) {
        const QRgb c = qRgb(y, y, y);
        for (int x = 0; x < w; ++x) img.setPixel(x, y, c);
    }
    return img;
}

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    // Ảnh nguồn cao 100, rộng 30.
    const QImage full = makeGradient(30, 100);
    // top = hàng 0..59 ; bottom = hàng 40..99 -> chồng 20 hàng (40..59).
    const QImage top = full.copy(0, 0, 30, 60);
    const QImage bottom = full.copy(0, 40, 30, 60);

    // Phát hiện overlap đúng = 20.
    ok(detectVerticalOverlap(top, bottom) == 20, "detect overlap = 20");

    // stitchAuto khôi phục đúng chiều cao gốc 100 và rộng 30.
    const QImage merged = stitchAuto(top, bottom);
    ok(merged.height() == 100, "stitchAuto height = 100");
    ok(merged.width() == 30, "stitchAuto width = 30");

    // Pixel hàng giữa khớp full (hàng 50 màu (50,50,50)).
    ok(merged.pixel(0, 50) == full.pixel(0, 50), "hang 50 khop full");
    ok(qRed(merged.pixel(15, 50)) == 50, "hang 50 mau (50,50,50)");
    // Kiểm thêm vài hàng biên để chắc nội dung đúng vị trí.
    ok(merged.pixel(0, 0) == full.pixel(0, 0), "hang 0 khop full");
    ok(merged.pixel(0, 99) == full.pixel(0, 99), "hang 99 khop full");

    // Khác width -> detect trả 0.
    {
        const QImage narrow = makeGradient(20, 60);
        ok(detectVerticalOverlap(top, narrow) == 0, "khac width -> overlap 0");
    }

    // Bottom KHÔNG chồng top (2 đoạn rời) -> overlap 0, stitch nối thẳng.
    {
        const QImage a = full.copy(0, 0, 30, 30);   // hàng 0..29
        const QImage b = full.copy(0, 60, 30, 30);  // hàng 60..89 (rời hẳn)
        ok(detectVerticalOverlap(a, b) == 0, "khong chong -> overlap 0");
        const QImage cat = stitchAuto(a, b);
        ok(cat.height() == 60, "noi thang height = tong");
        ok(cat.width() == 30, "noi thang width = 30");
        // Nửa trên = a, nửa dưới = b.
        ok(cat.pixel(0, 0) == a.pixel(0, 0), "noi thang: hang dau = a");
        ok(cat.pixel(0, 30) == b.pixel(0, 0), "noi thang: hang 30 = b[0]");
    }

    // stitchVertical kẹp overlap vượt biên về [0, bottom.height].
    {
        const QImage big = stitchVertical(top, bottom, 9999);
        ok(big.height() == top.height(), "overlap vuot bien -> chi con top height");
    }

    std::printf("test_stitch: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
