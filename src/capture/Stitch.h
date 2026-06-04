#pragma once
#include <QImage>

// Ghép ảnh dọc (vertical stitching) — logic THUẦN (không gọi OS), test headless được.
// Nền tảng cho chụp trang dài bằng cách cuộn (scrolling/panoramic capture): các ảnh
// chụp liên tiếp CHỒNG nhau theo chiều dọc. Ta phát hiện vùng chồng (số hàng dưới của
// ảnh trên trùng với số hàng trên của ảnh dưới) rồi nối lại mà không lặp vùng chồng.
namespace capture {

// Tìm số HÀNG chồng nhau giữa đáy của `top` và đỉnh của `bottom`.
// Thử các giá trị overlap từ lớn xuống nhỏ; với mỗi overlap, so khớp `overlap` hàng
// cuối của `top` với `overlap` hàng đầu của `bottom` (so từng pixel, đếm % giống).
// Trả overlap LỚN NHẤT có >= 99% pixel giống. maxOverlap<=0 -> min(top.height, bottom.height).
// Trả 0 nếu không tìm thấy chồng lắp. Khác width -> trả 0.
int detectVerticalOverlap(const QImage &top, const QImage &bottom, int maxOverlap = 0);

// Ghép dọc: ảnh kết quả cao = top.height + bottom.height - overlap, rộng = width.
// Vẽ `top` ở (0,0), vẽ phần `bottom` từ hàng `overlap` trở xuống ngay bên dưới `top`.
// overlap được kẹp về [0, bottom.height].
QImage stitchVertical(const QImage &top, const QImage &bottom, int overlap);

// Tiện ích: tự phát hiện overlap rồi ghép.
QImage stitchAuto(const QImage &top, const QImage &bottom);

} // namespace capture
