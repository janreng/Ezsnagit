// Test headless cho ezsnag_capture — phần logic hình học thuần (ScreenGeometry).
// (Phần GDI capture cần màn hình thật nên không test ở đây.)
#include "capture/ScreenGeometry.h"
#include <QRect>
#include <QVector>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using capture::unionBounds;
using capture::clampToBounds;

int main() {
    // unionBounds: bao 2 monitor cạnh nhau.
    {
        QVector<QRect> r{ QRect(0,0,1920,1080), QRect(1920,0,1280,1024) };
        ok(unionBounds(r) == QRect(0,0,3200,1080), "union 2 monitor canh nhau");
    }
    // unionBounds: monitor phụ bên trái (toạ độ âm).
    {
        QVector<QRect> r{ QRect(0,0,1920,1080), QRect(-1280,0,1280,1024) };
        ok(unionBounds(r) == QRect(-1280,0,3200,1080), "union monitor ben trai (toa do am)");
    }
    // unionBounds rỗng.
    {
        ok(unionBounds({}).isNull(), "union rong -> null");
    }
    // clampToBounds: vùng nằm trong.
    {
        QRect b(0,0,1920,1080);
        ok(clampToBounds(QRect(100,100,200,200), b) == QRect(100,100,200,200), "clamp trong nguyen ven");
    }
    // clampToBounds: vùng tràn biên -> cắt.
    {
        QRect b(0,0,1920,1080);
        ok(clampToBounds(QRect(1800,0,400,200), b) == QRect(1800,0,120,200), "clamp tran phai -> cat");
    }
    // clampToBounds: vùng ngoài hẳn -> rỗng.
    {
        QRect b(0,0,1920,1080);
        ok(clampToBounds(QRect(5000,5000,100,100), b).isEmpty(), "clamp ngoai han -> rong");
    }

    std::printf("test_capture: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
