#include "capture/MonitorMap.h"
#include <QtMath>
#include <limits>

namespace capture {

int monitorAt(const QVector<Monitor> &mons, const QPoint &logicalPt) {
    for (int i = 0; i < mons.size(); ++i) {
        if (mons[i].logical.contains(logicalPt))
            return i;
    }
    return -1;
}

// Bình phương khoảng cách từ điểm tới tâm vùng logical của monitor (dùng cho fallback).
static double distSqToCenter(const Monitor &m, const QPoint &p) {
    const QPoint c = m.logical.center();
    const double dx = double(p.x()) - double(c.x());
    const double dy = double(p.y()) - double(c.y());
    return dx * dx + dy * dy;
}

// Nội suy/ngoại suy tuyến tính 1 điểm theo ô logical->physical của monitor m.
// Tránh chia 0 khi monitor có bề rộng/cao logical = 0 (giữ nguyên trục đó tại physical.left/top).
static QPoint interp(const Monitor &m, const QPoint &logicalPt) {
    double physX;
    if (m.logical.width() != 0) {
        const double sx = double(m.physical.width()) / double(m.logical.width());
        physX = double(m.physical.left()) + (double(logicalPt.x()) - double(m.logical.left())) * sx;
    } else {
        physX = double(m.physical.left());
    }
    double physY;
    if (m.logical.height() != 0) {
        const double sy = double(m.physical.height()) / double(m.logical.height());
        physY = double(m.physical.top()) + (double(logicalPt.y()) - double(m.logical.top())) * sy;
    } else {
        physY = double(m.physical.top());
    }
    return QPoint(qRound(physX), qRound(physY));
}

QPoint logicalToPhysical(const QVector<Monitor> &mons, const QPoint &logicalPt,
                         const QPoint &physVirtualOrigin) {
    // Danh sách rỗng -> mapping 1:1 (chỉ dời gốc).
    if (mons.isEmpty())
        return logicalPt - physVirtualOrigin;

    int idx = monitorAt(mons, logicalPt);
    if (idx < 0) {
        // Fallback: monitor gần nhất theo tâm.
        double best = std::numeric_limits<double>::max();
        idx = 0;
        for (int i = 0; i < mons.size(); ++i) {
            const double d = distSqToCenter(mons[i], logicalPt);
            if (d < best) { best = d; idx = i; }
        }
    }

    const QPoint phys = interp(mons[idx], logicalPt);
    return phys - physVirtualOrigin;
}

QRect logicalRectToImage(const QVector<Monitor> &mons, const QRect &logicalRect,
                         const QPoint &physVirtualOrigin) {
    const QPoint tl = logicalToPhysical(mons, logicalRect.topLeft(), physVirtualOrigin);
    const QPoint br = logicalToPhysical(mons, logicalRect.bottomRight(), physVirtualOrigin);
    return QRect(tl, br).normalized();
}

} // namespace capture
