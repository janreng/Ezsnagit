#include "capture/StitchMany.h"
#include "capture/Stitch.h"

namespace capture {

QImage stitchMany(const QVector<QImage> &frames) {
    QImage acc; // ảnh tích luỹ

    for (const QImage &f : frames) {
        if (f.isNull()) continue; // bỏ qua frame null

        if (acc.isNull()) {
            // Frame hợp lệ đầu tiên trở thành nền tích luỹ.
            acc = f;
        } else {
            // Tự dò overlap rồi nối frame kế tiếp vào bên dưới.
            acc = stitchAuto(acc, f);
        }
    }

    return acc; // rỗng nếu không có frame hợp lệ nào
}

} // namespace capture
