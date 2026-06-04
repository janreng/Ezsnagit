#pragma once
#include <QColor>
#include <QImage>

// Hiệu ứng khung viền & bóng đổ — logic THUẦN (không gọi OS), test headless được.
// Mỗi hàm nhận QImage và trả về QImage MỚI (không sửa ảnh gốc).
namespace effects {

// Vẽ viền ĐẶC dày `width` px quanh ảnh, vẽ ĐÈ vào trong (không đổi kích thước ảnh
// để giữ toạ độ đơn giản). width<=0 -> trả về bản copy nguyên ảnh.
QImage addBorder(const QImage &img, int width, const QColor &color);

// Mở rộng canvas thêm `margin` mỗi cạnh (nền trong suốt), vẽ một bóng mờ lệch
// xuống-phải phía sau ảnh, rồi vẽ ảnh gốc lên trên.
// Kết quả size = (w + 2*margin, h + 2*margin).
QImage dropShadow(const QImage &img, int margin = 24, int blurRadius = 12,
                  const QColor &shadow = QColor(0, 0, 0, 160));

} // namespace effects
