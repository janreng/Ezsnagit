// Test headless cho MonitorMap — ánh xạ logical<->physical đa màn hình mixed-DPI.
// Logic THUẦN (không cần màn hình thật), chạy được không cần GUI.
#include "capture/MonitorMap.h"
#include <QRect>
#include <QPoint>
#include <QVector>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using capture::Monitor;
using capture::monitorAt;
using capture::logicalToPhysical;
using capture::logicalRectToImage;

int main() {
    // Single monitor 1:1, gốc (0,0): logical (100,100) -> image (100,100).
    {
        QVector<Monitor> mons{ { QRect(0,0,1920,1080), QRect(0,0,1920,1080) } };
        ok(logicalToPhysical(mons, QPoint(100,100), QPoint(0,0)) == QPoint(100,100),
           "single 1:1 -> image (100,100)");
    }

    // Single monitor scale 2 (logical 960x540 = physical 1920x1080): logical (100,100) -> (200,200).
    {
        QVector<Monitor> mons{ { QRect(0,0,960,540), QRect(0,0,1920,1080) } };
        ok(logicalToPhysical(mons, QPoint(100,100), QPoint(0,0)) == QPoint(200,200),
           "single scale=2 -> image (200,200)");
    }

    // Hai monitor khác DPI: mon0 100% (logical 0,0,1920,1080 = phys 0,0,1920,1080),
    // mon1 200% (logical 1920,0,1280,720 = phys 1920,0,2560,1440).
    // Điểm trên mon1 tại logical (1920+100,100):
    //   physX = 1920 + 100*(2560/1280) = 1920 + 200 = 2120 ; image x = 2120 (origin 0).
    //   physY = 0    + 100*(1440/720)  = 200            ; image y = 200.
    // Phải map theo scale của mon1 (x2), KHÔNG theo mon0 (x1).
    {
        QVector<Monitor> mons{
            { QRect(0,0,1920,1080),    QRect(0,0,1920,1080)    },
            { QRect(1920,0,1280,720),  QRect(1920,0,2560,1440) },
        };
        QPoint img = logicalToPhysical(mons, QPoint(1920+100,100), QPoint(0,0));
        ok(img == QPoint(2120,200), "mon1 dung scale mon1 (x2) -> image (2120,200)");
        // Kiểm tra rõ KHÔNG dùng scale mon0: nếu nhầm dùng mon0 (x1) sẽ ra x=2020.
        ok(img.x() != 2020, "mon1 KHONG dung scale mon0");

        // Điểm trên mon0 vẫn 1:1.
        ok(logicalToPhysical(mons, QPoint(100,100), QPoint(0,0)) == QPoint(100,100),
           "mon0 van 1:1 -> image (100,100)");
    }

    // monitorAt: đúng index và -1 khi ngoài.
    {
        QVector<Monitor> mons{
            { QRect(0,0,1920,1080),    QRect(0,0,1920,1080)    },
            { QRect(1920,0,1280,720),  QRect(1920,0,2560,1440) },
        };
        ok(monitorAt(mons, QPoint(50,50)) == 0,        "monitorAt -> mon0");
        ok(monitorAt(mons, QPoint(2000,100)) == 1,     "monitorAt -> mon1");
        ok(monitorAt(mons, QPoint(9000,9000)) == -1,   "monitorAt ngoai -> -1");
    }

    // logicalRectToImage trên monitor scale 2: rect kích thước nhân đôi.
    // logical rect (10,20,100,50): topLeft(10,20)->(20,40); bottomRight = (109,69) (Qt inclusive)
    //   ->(218,138). QRect(QPoint(20,40),QPoint(218,138)) = (20,40, 199x99).
    {
        QVector<Monitor> mons{ { QRect(0,0,960,540), QRect(0,0,1920,1080) } };
        QRect r = logicalRectToImage(mons, QRect(10,20,100,50), QPoint(0,0));
        ok(r == QRect(QPoint(20,40), QPoint(218,138)),
           "rect scale=2 -> size nhan doi");
        // Xác nhận kích thước ~ gấp đôi (chiều rộng/cao xấp xỉ 2x).
        ok(r.width() == 199 && r.height() == 99, "rect scale=2 kich thuoc nhan doi");
    }

    // Fallback: điểm ngoài mọi monitor -> dùng monitor gần nhất (ngoại suy tuyến tính).
    {
        QVector<Monitor> mons{ { QRect(0,0,960,540), QRect(0,0,1920,1080) } };
        // Điểm (-10,-10) ngoài monitor: ngoại suy scale 2 -> (-20,-20).
        ok(logicalToPhysical(mons, QPoint(-10,-10), QPoint(0,0)) == QPoint(-20,-20),
           "fallback ngoai -> monitor gan nhat (x2)");
    }

    // physVirtualOrigin khác (0,0): trừ gốc ảnh.
    {
        QVector<Monitor> mons{ { QRect(0,0,1920,1080), QRect(-100,-50,1920,1080) } };
        // logical (100,100) -> phys (-100+100, -50+100) = (0,50); origin (-100,-50) -> image (100,100).
        ok(logicalToPhysical(mons, QPoint(100,100), QPoint(-100,-50)) == QPoint(100,100),
           "tru physVirtualOrigin");
    }

    std::printf("test_monitormap: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
