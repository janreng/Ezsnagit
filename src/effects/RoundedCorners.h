#pragma once
#include <QImage>

// Bo tròn 4 góc của một ảnh — vùng ngoài rounded-rect trở thành TRONG SUỐT (alpha=0).
// Logic THUẦN (chỉ dùng Qt Gui, không gọi OS), test headless được.
// Trả về QImage MỚI định dạng ARGB32 (không sửa ảnh gốc).
namespace effects {

// Trả ảnh ARGB32 cùng kích thước với 4 góc được bo tròn bán kính `radius`.
// Cách làm: tạo ảnh ARGB32 trong suốt cùng size, QPainter clip bằng
// QPainterPath::addRoundedRect rồi drawImage ảnh gốc -> ngoài vùng bo = trong suốt.
// radius <= 0  -> trả copy ARGB32 nguyên ảnh (không bo).
// radius được tự kẹp <= min(w,h)/2 để không vượt quá nửa cạnh ngắn.
QImage roundCorners(const QImage &img, int radius = 16);

} // namespace effects
