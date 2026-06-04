// Test headless cho effects::roundCorners — bo góc ảnh, ngoài vùng bo trong suốt.
// Cần QGuiApplication + nền tảng "offscreen" vì QPainter dùng paint engine của Qt Gui.
#include "effects/RoundedCorners.h"
#include <QGuiApplication>
#include <QImage>
#include <QColor>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using effects::roundCorners;

int main(int argc, char **argv) {
    // Bắt buộc chạy headless trước khi tạo QGuiApplication.
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    // Ảnh 40x40 đỏ đặc (alpha 255).
    QImage img(40, 40, QImage::Format_ARGB32);
    img.fill(QColor(255, 0, 0, 255));

    // --- roundCorners(img, 12) ---
    {
        QImage out = roundCorners(img, 12);
        ok(out.size() == QSize(40, 40), "round12: giu size 40x40");
        ok(out.hasAlphaChannel(), "round12: co alpha channel");

        // Góc (0,0) phải bị bo -> trong suốt alpha=0.
        ok(qAlpha(out.pixel(0, 0)) == 0, "round12: goc (0,0) trong suot");

        // Trung tâm (20,20) vẫn đỏ đặc alpha 255.
        QColor c = out.pixelColor(20, 20);
        ok(c.alpha() == 255 && c.red() == 255 && c.green() == 0 && c.blue() == 0,
           "round12: trung tam do dac");
    }

    // --- radius <= 0 -> giữ nguyên (góc vẫn đỏ đặc) ---
    {
        QImage out = roundCorners(img, 0);
        ok(out.size() == QSize(40, 40), "round0: giu size");
        QColor c = out.pixelColor(0, 0);
        ok(c.alpha() == 255 && c.red() == 255, "round0: goc van do dac");

        QImage out2 = roundCorners(img, -5);
        ok(out2.pixelColor(0, 0).alpha() == 255, "round neg: goc van do dac");
    }

    // --- radius rất lớn (1000) trên 40x40 -> kẹp về 20, không crash ---
    {
        QImage out = roundCorners(img, 1000);
        ok(out.size() == QSize(40, 40), "round big: giu size, khong crash");
        ok(out.hasAlphaChannel(), "round big: co alpha");
        // Với r=20 ảnh thành hình tròn/oval -> góc (0,0) vẫn trong suốt.
        ok(qAlpha(out.pixel(0, 0)) == 0, "round big: goc (0,0) trong suot");
    }

    std::printf("test_rounded: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
