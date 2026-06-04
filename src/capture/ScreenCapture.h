#pragma once
#include <QImage>
#include <QRect>
#include <QVector>

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

// Liệt kê rect (pixel vật lý) của các cửa sổ top-level đang hiện, theo Z-order
// TRƯỚC→SAU (front-to-back). Dùng cho window-capture: hover để highlight cửa sổ.
// Bỏ qua cửa sổ ẩn/minimize/cloaked/tool-window. Nền tảng khác trả danh sách rỗng.
QVector<QRect> enumerateWindowRects();

} // namespace capture
