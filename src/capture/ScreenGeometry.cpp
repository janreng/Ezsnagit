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

QRect mapWidgetRectToImage(const QRect &sel, const QSize &widgetSize, const QSize &imageSize) {
    if (widgetSize.isEmpty() || imageSize.isEmpty()) return QRect();
    const double sx = double(imageSize.width())  / widgetSize.width();
    const double sy = double(imageSize.height()) / widgetSize.height();
    const QRect mapped(qRound(sel.x() * sx), qRound(sel.y() * sy),
                       qRound(sel.width() * sx), qRound(sel.height() * sy));
    return mapped.intersected(QRect(QPoint(0, 0), imageSize));
}

QPoint mapWidgetPointToImage(const QPoint &p, const QSize &widgetSize, const QSize &imageSize) {
    if (widgetSize.isEmpty() || imageSize.isEmpty()) return QPoint();
    const double sx = double(imageSize.width())  / widgetSize.width();
    const double sy = double(imageSize.height()) / widgetSize.height();
    return QPoint(qRound(p.x() * sx), qRound(p.y() * sy));
}

int topWindowAt(const QVector<QRect> &rectsFrontToBack, const QPoint &p) {
    for (int i = 0; i < rectsFrontToBack.size(); ++i)
        if (rectsFrontToBack.at(i).contains(p)) return i;
    return -1;
}

} // namespace capture
