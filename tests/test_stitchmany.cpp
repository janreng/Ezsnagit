// Test headless cho capture::stitchMany — ghép nhiều frame chồng nhau thành panorama.
// Dựng ảnh "full" rồi cắt thành các frame chồng nhau, kiểm tra ghép lại khôi phục đúng.
#include "capture/StitchMany.h"
#include <QGuiApplication>
#include <QImage>
#include <QColor>
#include <QVector>
#include <cstdio>
#include <cstdlib>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using capture::stitchMany;

// Tạo ảnh cao `h`, rộng `w`, mỗi hàng y có màu xám (y%256, y%256, y%256).
// `y0` là offset hàng toàn cục (để các frame có pixel khớp nhau ở vùng chồng).
static QImage makeFrame(int y0, int h, int w) {
    QImage img(w, h, QImage::Format_ARGB32);
    for (int y = 0; y < h; ++y) {
        int g = (y0 + y) % 256;
        QRgb c = qRgb(g, g, g);
        QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (int x = 0; x < w; ++x) line[x] = c;
    }
    return img;
}

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    const int W = 30;

    // 3 frame chồng nhau, tái hiện ảnh full cao 120:
    //  f0: rows 0..49   (h50)
    //  f1: rows 30..79  (h50)  -> chồng 20 hàng với f0
    //  f2: rows 60..119 (h60)  -> chồng 20 hàng với f1
    QImage f0 = makeFrame(0,  50, W);
    QImage f1 = makeFrame(30, 50, W);
    QImage f2 = makeFrame(60, 60, W);

    // Ghép đủ 3 frame -> khôi phục đúng chiều cao 120, rộng 30.
    {
        QImage out = stitchMany({ f0, f1, f2 });
        ok(out.height() == 120, "stitchMany 3 frame -> height 120");
        ok(out.width() == W,    "stitchMany 3 frame -> width 30");
    }

    // frames rỗng -> ảnh rỗng.
    {
        QImage out = stitchMany({});
        ok(out.isNull(), "stitchMany rong -> null");
    }

    // 1 frame -> trả chính nó (cao 50).
    {
        QImage out = stitchMany({ f0 });
        ok(out.height() == 50 && out.width() == W, "stitchMany 1 frame -> chinh no (h50)");
    }

    // Có frame null xen giữa -> bỏ qua, vẫn ghép được phần còn lại.
    {
        QImage out = stitchMany({ f0, QImage(), f1, f2 });
        ok(out.height() == 120 && out.width() == W, "stitchMany null xen giua -> bo qua, van ghep");
    }

    std::printf("test_stitchmany: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
