#pragma once
#include <QRect>
#include <QVector>

// Hình học màn hình — logic THUẦN (không gọi OS), test headless được.
// Dùng cho capture đa màn hình / mixed-DPI (NFR SPEC 12 §12.2): tính bao virtual
// desktop và kẹp vùng chọn vào trong giới hạn.
namespace capture {

// Bao toàn bộ rects thành 1 QRect (giới hạn virtual desktop). Danh sách rỗng -> QRect rỗng.
QRect unionBounds(const QVector<QRect> &rects);

// Kẹp (giao) r vào trong bounds. Trả QRect rỗng nếu không giao nhau.
QRect clampToBounds(const QRect &r, const QRect &bounds);

// Ánh xạ vùng chọn từ toạ độ WIDGET (logical) sang toạ độ ẢNH (pixel vật lý), theo
// tỉ lệ imageSize/widgetSize — dùng cho region-capture kiểu screen-freeze: overlay hiển
// thị ảnh đã chụp scale vừa khít, người dùng kéo chọn trên widget, ta crop trên ảnh gốc.
// Kết quả được kẹp vào trong ảnh. Trả QRect rỗng nếu kích thước không hợp lệ.
QRect mapWidgetRectToImage(const QRect &sel, const QSize &widgetSize, const QSize &imageSize);

// Như trên nhưng cho một điểm (toạ độ con trỏ widget -> pixel ảnh). Không kẹp biên.
QPoint mapWidgetPointToImage(const QPoint &p, const QSize &widgetSize, const QSize &imageSize);

// Tìm cửa sổ TRÊN CÙNG chứa điểm p, trong danh sách rects xếp theo Z-order TRƯỚC→SAU
// (front-to-back). Trả chỉ số cửa sổ đầu tiên chứa p, hoặc -1 nếu không cửa sổ nào.
int topWindowAt(const QVector<QRect> &rectsFrontToBack, const QPoint &p);

} // namespace capture
