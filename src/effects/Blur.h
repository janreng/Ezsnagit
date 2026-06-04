#pragma once
#include <QImage>
#include <QRect>

// Làm mờ / pixelate một vùng ảnh — dùng cho mục đích che thông tin (redaction).
// Logic THUẦN (không gọi OS), test headless được.
// Mỗi hàm nhận QImage và trả về QImage MỚI (không sửa ảnh gốc).
namespace effects {

// Làm mờ CHỈ vùng region (kẹp trong biên ảnh) của một bản copy ảnh.
// Cách làm: crop vùng -> scale nhỏ xuống (width/strength, tối thiểu 1px)
// -> scale lại lên đúng kích thước vùng bằng SmoothTransformation -> vẽ đè.
// region rỗng / giao rỗng -> trả copy nguyên ảnh.
QImage blurRegion(const QImage &img, const QRect &region, int strength = 6);

// Mosaic (pixelate) CHỈ vùng region: scale nhỏ xuống (width/blockSize)
// rồi scale lên lại bằng FastTransformation (nearest) -> ô vuông pixel to -> vẽ đè.
// region rỗng / giao rỗng -> trả copy nguyên ảnh.
QImage pixelateRegion(const QImage &img, const QRect &region, int blockSize = 10);

} // namespace effects
