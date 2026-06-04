// Test headless cho module effects::Transforms — biến đổi ảnh thuần.
// QImage chạy không cần app, nhưng để an toàn ta set offscreen + QGuiApplication.
#include "effects/Transforms.h"
#include <QGuiApplication>
#include <QImage>
#include <QColor>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

// Tạo ảnh đồng màu để test.
static QImage solid(int w, int h, const QColor &c) {
    QImage img(w, h, QImage::Format_ARGB32);
    img.fill(c);
    return img;
}

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    // crop trong biên -> đúng size.
    {
        QImage img = solid(100, 100, Qt::red);
        QImage c = effects::crop(img, QRect(10, 10, 50, 40));
        ok(c.size() == QSize(50, 40), "crop trong bien -> 50x40");
    }
    // crop ngoài biên -> kẹp đúng.
    {
        QImage img = solid(100, 100, Qt::red);
        QImage c = effects::crop(img, QRect(80, 80, 50, 50)); // tràn -> còn 20x20
        ok(c.size() == QSize(20, 20), "crop tran bien -> kep 20x20");
    }
    // crop giao rỗng -> ảnh rỗng.
    {
        QImage img = solid(100, 100, Qt::red);
        QImage c = effects::crop(img, QRect(500, 500, 10, 10));
        ok(c.isNull(), "crop ngoai han -> rong");
    }
    // rotate90 của ảnh 100x50 -> 50x100.
    {
        QImage img = solid(100, 50, Qt::green);
        QImage r = effects::rotate90(img, true);
        ok(r.size() == QSize(50, 100), "rotate90 100x50 -> 50x100");
    }
    // flipHorizontal 2 lần == ảnh gốc.
    {
        QImage img = solid(30, 20, Qt::blue);
        // đặt vài pixel đặc trưng để so sánh có nghĩa.
        img.setPixelColor(0, 0, Qt::white);
        img.setPixelColor(29, 19, Qt::black);
        QImage twice = effects::flipHorizontal(effects::flipHorizontal(img));
        ok(twice == img, "flipHorizontal 2 lan == goc");
    }
    // flipVertical 2 lần == ảnh gốc (bonus sanity).
    {
        QImage img = solid(16, 24, Qt::cyan);
        img.setPixelColor(3, 5, Qt::magenta);
        QImage twice = effects::flipVertical(effects::flipVertical(img));
        ok(twice == img, "flipVertical 2 lan == goc");
    }
    // scaledTo(20,20) -> 20x20.
    {
        QImage img = solid(100, 60, Qt::yellow);
        QImage s = effects::scaledTo(img, QSize(20, 20));
        ok(s.size() == QSize(20, 20), "scaledTo -> 20x20");
    }
    // autoTrim: nền trắng 50x50, ô đen 10x10 ở (20,20) -> vùng (20,20,10,10).
    {
        QImage img = solid(50, 50, Qt::white);
        for (int y = 20; y < 30; ++y)
            for (int x = 20; x < 30; ++x)
                img.setPixelColor(x, y, Qt::black);
        QImage t = effects::autoTrim(img);
        ok(t.size() == QSize(10, 10), "autoTrim -> size 10x10");
        ok(t.pixelColor(0, 0) == QColor(Qt::black), "autoTrim -> goc la o den");
    }
    // autoTrim: ảnh đồng màu -> trả nguyên ảnh.
    {
        QImage img = solid(40, 30, Qt::gray);
        QImage t = effects::autoTrim(img);
        ok(t.size() == QSize(40, 30), "autoTrim dong mau -> nguyen anh");
    }

    std::printf("test_effects: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
