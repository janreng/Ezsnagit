// Test headless cho bộ lọc màu (effects/Filters) — logic THUẦN trên QImage.
// Cần QGuiApplication + nền tảng offscreen để QImage hoạt động không cần màn hình thật.
#include "effects/Filters.h"
#include <QGuiApplication>
#include <QImage>
#include <QColor>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using namespace effects;

// Ảnh 1 pixel với màu + alpha cho trước.
static QImage onePixel(int r, int g, int b, int a = 255) {
    QImage img(1, 1, QImage::Format_ARGB32);
    img.setPixelColor(0, 0, QColor(r, g, b, a));
    return img;
}

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    // grayscale: đỏ thuần (255,0,0) -> R==G==B, ~76 (0.299*255 = 76.245).
    {
        QColor c = grayscale(onePixel(255, 0, 0)).pixelColor(0, 0);
        ok(c.red() == c.green() && c.green() == c.blue(), "grayscale: R==G==B (xam)");
        ok(c.red() >= 75 && c.red() <= 77, "grayscale: do thuan ~76");
    }

    // invert: (10,20,30) -> (245,235,225).
    {
        QColor c = invert(onePixel(10, 20, 30)).pixelColor(0, 0);
        ok(c.red() == 245 && c.green() == 235 && c.blue() == 225, "invert: (10,20,30)->(245,235,225)");
    }

    // adjustBrightness(+50): (200,..) clamp <=255; (10,..) -> 60.
    {
        QColor hi = adjustBrightness(onePixel(200, 200, 200), 50).pixelColor(0, 0);
        ok(hi.red() == 250 && hi.red() <= 255, "brightness: 200+50=250 (<=255)");
        QColor cap = adjustBrightness(onePixel(220, 220, 220), 50).pixelColor(0, 0);
        ok(cap.red() == 255, "brightness: 220+50 clamp 255");
        QColor lo = adjustBrightness(onePixel(10, 10, 10), 50).pixelColor(0, 0);
        ok(lo.red() == 60 && lo.green() == 60 && lo.blue() == 60, "brightness: 10+50=60");
    }

    // adjustContrast(2.0): pixel 128 -> giữ 128; 200 -> tăng; clamp <=255.
    {
        QColor mid = adjustContrast(onePixel(128, 128, 128), 2.0).pixelColor(0, 0);
        ok(mid.red() == 128, "contrast: 128 giu nguyen");
        QColor up = adjustContrast(onePixel(200, 200, 200), 2.0).pixelColor(0, 0);
        ok(up.red() == 255, "contrast: 200 tang -> clamp 255"); // 128+(72*2)=272 -> 255
        QColor downClamp = adjustContrast(onePixel(40, 40, 40), 2.0).pixelColor(0, 0);
        ok(downClamp.red() == 0, "contrast: 40 -> clamp 0"); // 128+(-88*2)=-48 -> 0
    }

    // sepia: ảnh trả về không null, size giữ nguyên.
    {
        QImage src(4, 3, QImage::Format_ARGB32);
        src.fill(QColor(120, 80, 40));
        QImage out = sepia(src);
        ok(!out.isNull(), "sepia: khong null");
        ok(out.size() == src.size(), "sepia: size giu nguyen");
    }

    // Mọi hàm: size giữ nguyên + alpha giữ nguyên (alpha=128 vẫn 128 sau filter).
    {
        QImage src = onePixel(100, 150, 200, 128);
        ok(grayscale(src).size() == src.size(), "grayscale: size giu nguyen");
        ok(grayscale(src).pixelColor(0, 0).alpha() == 128, "grayscale: alpha giu 128");
        ok(invert(src).pixelColor(0, 0).alpha() == 128, "invert: alpha giu 128");
        ok(sepia(src).pixelColor(0, 0).alpha() == 128, "sepia: alpha giu 128");
        ok(adjustBrightness(src, 50).pixelColor(0, 0).alpha() == 128, "brightness: alpha giu 128");
        ok(adjustContrast(src, 2.0).pixelColor(0, 0).alpha() == 128, "contrast: alpha giu 128");
    }

    // Bonus: convert từ format khác (RGB32 không alpha) vẫn chạy, không null.
    {
        QImage rgb(2, 2, QImage::Format_RGB32);
        rgb.fill(QColor(50, 60, 70));
        ok(!grayscale(rgb).isNull(), "grayscale: convert tu RGB32 OK");
    }

    std::printf("test_filters: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
