#pragma once
#include <QImage>
#include <QRect>

// Chụp màn hình (lớp platform). P1 slice 1: chụp toàn bộ virtual desktop (đa màn hình)
// ở pixel VẬT LÝ — nhờ process đã bật per-monitor DPI awareness (SPEC 12 §12.2).
// Triển khai Windows GDI; nền tảng khác trả QImage rỗng (sẽ bổ sung sau).
namespace capture {

// Giới hạn virtual desktop theo pixel vật lý (x, y có thể âm khi monitor phụ bên trái/trên).
QRect virtualDesktopRect();

// Chụp toàn bộ virtual desktop. QImage rỗng nếu lỗi.
QImage captureVirtualDesktop();

// Chụp một vùng (toạ độ pixel vật lý theo hệ virtual desktop). Tự kẹp vào giới hạn.
QImage captureRegion(const QRect &regionPx);

} // namespace capture
