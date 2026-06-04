#pragma once
#include <QImage>
#include <QRect>
#include <QSize>

// Biến đổi ảnh — logic THUẦN (không gọi OS), test headless được.
// Mỗi hàm nhận QImage và trả về QImage MỚI (không sửa ảnh gốc).
namespace effects {

// Cắt vùng r (kẹp trong biên ảnh). Trả ảnh rỗng nếu giao rỗng.
QImage crop(const QImage &img, const QRect &r);

// Xoay 90 độ — clockwise=true xoay theo chiều kim đồng hồ, ngược lại ngược chiều.
QImage rotate90(const QImage &img, bool clockwise);

// Lật ngang (trái↔phải).
QImage flipHorizontal(const QImage &img);

// Lật dọc (trên↔dưới).
QImage flipVertical(const QImage &img);

// Resize giữ chất lượng (smooth, bỏ qua tỉ lệ — đúng đúng size yêu cầu).
QImage scaledTo(const QImage &img, const QSize &size);

// Cắt bỏ viền đồng màu quanh ảnh: tìm bounding box các pixel KHÁC màu góc trên-trái,
// trả vùng đó. Nếu cả ảnh đồng màu (hoặc rỗng) trả nguyên ảnh.
QImage autoTrim(const QImage &img);

} // namespace effects
