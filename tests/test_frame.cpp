// Test headless cho effects::Frame — viền (addBorder) & bóng đổ (dropShadow).
// Cần QGuiApplication + offscreen platform vì QPainter dùng backend đồ hoạ.
#include "effects/Frame.h"
#include <QGuiApplication>
#include <QImage>
#include <QColor>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using effects::addBorder;
using effects::dropShadow;

// Ảnh đặc một màu để dễ kiểm tra pixel.
static QImage solid(int w, int h, QColor c) {
    QImage img(w, h, QImage::Format_ARGB32);
    img.fill(c);
    return img;
}

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    const QColor blue(0, 0, 255, 255);
    const QColor red(255, 0, 0, 255);

    // addBorder: giữ nguyên kích thước, góc thành đỏ, giữa không đổi.
    {
        QImage src = solid(50, 50, blue);
        QImage out = addBorder(src, 4, red);
        ok(out.size() == QSize(50, 50), "addBorder giu kich thuoc 50x50");
        ok(QColor(out.pixelColor(0, 0)) == red, "addBorder goc (0,0) == do");
        ok(QColor(out.pixelColor(25, 25)) == blue, "addBorder giua (25,25) khong doi");
    }

    // addBorder width<=0 -> ảnh không đổi.
    {
        QImage src = solid(50, 50, blue);
        QImage out = addBorder(src, 0, red);
        ok(out.size() == src.size(), "addBorder width=0 giu kich thuoc");
        ok(QColor(out.pixelColor(0, 0)) == blue, "addBorder width=0 goc khong doi");
        QImage out2 = addBorder(src, -5, red);
        ok(QColor(out2.pixelColor(0, 0)) == blue, "addBorder width<0 goc khong doi");
    }

    // dropShadow: size = (w+2m, h+2m); có ảnh gốc ở giữa; có bóng ở rìa.
    {
        QImage src = solid(40, 30, blue);
        QImage out = dropShadow(src, 20);
        ok(out.size() == QSize(80, 70), "dropShadow size == (80,70)");
        // Tâm canvas (40,35) nằm trong vùng ảnh gốc -> phải là màu xanh đặc.
        ok(QColor(out.pixelColor(40, 35)) == blue, "dropShadow giua co pixel anh goc");
        // Tìm một pixel bóng: vùng ngoài ảnh nhưng có alpha > 0 (không trong suốt hẳn).
        bool hasShadow = false;
        for (int y = 0; y < out.height() && !hasShadow; ++y) {
            for (int x = 0; x < out.width(); ++x) {
                // Bỏ qua vùng ảnh gốc [20..60) x [20..50).
                bool inImage = (x >= 20 && x < 60 && y >= 20 && y < 50);
                if (inImage) continue;
                if (out.pixelColor(x, y).alpha() > 0) { hasShadow = true; break; }
            }
        }
        ok(hasShadow, "dropShadow co vung bong o ria");
    }

    // Không crash với ảnh nhỏ 1x1.
    {
        QImage tiny = solid(1, 1, red);
        QImage b = addBorder(tiny, 3, blue);
        ok(!b.isNull() && b.size() == QSize(1, 1), "addBorder 1x1 khong crash");
        QImage s = dropShadow(tiny, 5);
        ok(!s.isNull() && s.size() == QSize(11, 11), "dropShadow 1x1 khong crash");
    }

    std::printf("test_frame: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
