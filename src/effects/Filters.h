#pragma once
#include <QImage>

// Bộ lọc màu — logic THUẦN (không gọi OS), test headless được.
// Mỗi hàm nhận QImage và trả về QImage MỚI (không sửa ảnh gốc).
// Bên trong tự convert sang Format_ARGB32, giữ nguyên alpha của từng pixel.
namespace effects {

// Chuyển xám theo luma: gray = 0.299R + 0.587G + 0.114B (clamp 0..255). Giữ alpha.
QImage grayscale(const QImage &img);

// Đảo màu: (255-R, 255-G, 255-B). Giữ alpha.
QImage invert(const QImage &img);

// Tông nâu cổ điển (công thức sepia chuẩn, clamp 0..255). Giữ alpha.
QImage sepia(const QImage &img);

// Cộng delta (-255..255) vào R,G,B rồi clamp 0..255. Giữ alpha.
QImage adjustBrightness(const QImage &img, int delta);

// Chỉnh tương phản quanh 128: v' = clamp(128 + (v-128)*factor). Giữ alpha.
QImage adjustContrast(const QImage &img, double factor);

} // namespace effects
