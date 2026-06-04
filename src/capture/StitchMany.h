#pragma once
#include <QImage>
#include <QVector>

// Ghép NHIỀU ảnh dọc thành một panorama cao — logic THUẦN (không gọi OS), test headless được.
// Mở rộng từ Stitch.h (ghép 2 ảnh) sang ghép cả một dãy frame chụp khi cuộn trang dài.
namespace capture {

// Ghép tuần tự một dãy frame thành một ảnh cao duy nhất.
// Bắt đầu từ frames[0], lần lượt stitchAuto(acc, frames[i]) cho từng frame kế tiếp
// (tự dò vùng chồng + nối, không lặp vùng chồng).
//  - frames rỗng        -> QImage rỗng.
//  - 1 frame hợp lệ     -> trả chính frame đó.
//  - frame null xen kẽ  -> bỏ qua, vẫn ghép phần còn lại.
//  - khác width         -> stitchAuto nối thẳng (overlap 0), chấp nhận.
QImage stitchMany(const QVector<QImage> &frames);

} // namespace capture
