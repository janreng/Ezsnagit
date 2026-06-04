// Test headless cho effects::blurRegion / pixelateRegion (redaction).
// Cần QGuiApplication + nền offscreen vì QPainter trên QImage chạm vào hệ thống vẽ của Qt.
#include "effects/Blur.h"
#include <QGuiApplication>
#include <QImage>
#include <QColor>
#include <QRect>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using effects::blurRegion;
using effects::pixelateRegion;

// Ảnh 60x60: nửa trái trắng, nửa phải đen.
static QImage makeSplitImage() {
    QImage img(60, 60, QImage::Format_ARGB32);
    img.fill(Qt::white);
    for (int y = 0; y < 60; ++y)
        for (int x = 30; x < 60; ++x)
            img.setPixelColor(x, y, QColor(Qt::black));
    return img;
}

int main(int argc, char **argv) {
    // Headless cho chắc.
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    const QImage src = makeSplitImage();
    const QRect region(20, 20, 20, 20);

    // --- blurRegion ---
    {
        QImage out = blurRegion(src, region, 6);
        ok(out.size() == QSize(60, 60), "blur giu nguyen size 60x60");
        // Pixel ngoài vùng (0,0) — góc trắng — không đổi.
        ok(out.pixelColor(0, 0) == src.pixelColor(0, 0), "blur pixel ngoai vung (0,0) giu nguyen");
    }

    // --- pixelateRegion ---
    {
        QImage out = pixelateRegion(src, region, 10);
        ok(out.size() == QSize(60, 60), "pixelate giu nguyen size 60x60");
        ok(out.pixelColor(0, 0) == src.pixelColor(0, 0), "pixelate pixel ngoai vung (0,0) giu nguyen");
    }

    // --- region ngoài biên -> trả ảnh đúng kích thước, không crash ---
    {
        QImage out = blurRegion(src, QRect(1000, 1000, 50, 50), 6);
        ok(out.size() == QSize(60, 60), "blur region ngoai bien -> size goc");
        QImage out2 = pixelateRegion(src, QRect(1000, 1000, 50, 50), 10);
        ok(out2.size() == QSize(60, 60), "pixelate region ngoai bien -> size goc");
    }

    // --- region rỗng -> trả ảnh đúng kích thước ---
    {
        QImage out = blurRegion(src, QRect(), 6);
        ok(out.size() == QSize(60, 60), "blur region rong -> size goc");
        QImage out2 = pixelateRegion(src, QRect(), 10);
        ok(out2.size() == QSize(60, 60), "pixelate region rong -> size goc");
    }

    // --- strength / blockSize = 1 -> không crash, size giữ nguyên ---
    {
        QImage out = blurRegion(src, region, 1);
        ok(out.size() == QSize(60, 60), "blur strength=1 -> khong crash");
        QImage out2 = pixelateRegion(src, region, 1);
        ok(out2.size() == QSize(60, 60), "pixelate blockSize=1 -> khong crash");
    }

    std::printf("test_blur: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
