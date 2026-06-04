#include "capture/ScreenGeometry.h"

namespace capture {

QRect unionBounds(const QVector<QRect> &rects) {
    QRect out;
    for (const QRect &r : rects) {
        if (r.isNull()) continue;
        out = out.isNull() ? r : out.united(r);
    }
    return out;
}

QRect clampToBounds(const QRect &r, const QRect &bounds) {
    const QRect g = r.intersected(bounds);
    return g.isEmpty() ? QRect() : g;
}

} // namespace capture
