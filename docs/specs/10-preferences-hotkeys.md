# SPEC 10 — Preferences & Hotkeys (Ezsnagit)

> Module: `ui` (app shell)

## Mục tiêu & phạm vi

Trung tâm cấu hình của app: Preferences theo tab + hệ phím tắt toàn cục có thể tùy biến + theme.
Đây là phần "glue" của app, không phải module logic riêng — sống trong lớp `ui`/app shell.

## Tính năng

### 10.1 Preferences dialog (theo tab)
- **Mô tả:** hộp thoại cấu hình chia tab: **Capture, Editor, Share, Library, Hotkeys/Keyboard, Advanced, Save As**.
- **Tùy chọn:**
  - *Capture:* cursor capture on/off, delay mặc định, preview-in-editor sau chụp, multi-monitor.
  - *Editor:* màu canvas mặc định, grid/snapping, đơn vị (px), auto-flatten cảnh báo.
  - *Share:* bật/tắt + sắp xếp các đích chia sẻ (xem SPEC 07).
  - *Library:* vị trí thư mục store `.ezsnagx`, dung lượng, tự dọn.
  - *Advanced:* tăng tốc phần cứng, ghi log, reset.
- **Ưu tiên:** khung dialog + Capture/Editor tab = P1–P2; còn lại theo phase tính năng tương ứng.
- **Ghi chú kỹ thuật C++/Qt:** `QDialog` + `QTabWidget`; lưu qua `QSettings` (registry/INI trên Windows). Mỗi tab là 1 `QWidget` con. Tách struct `AppSettings` thuần để test.

### 10.2 Hotkeys toàn cục (global)
- **Mô tả:** phím tắt gọi chụp ngay cả khi app ở nền. Mặc định trên Windows:
  | Hành động | Mặc định (Win) |
  |---|---|
  | Global Capture (mở chọn vùng) | `Print Screen` |
  | Bắt đầu chọn region | `Shift+Enter` |
  | Chốt capture | `Enter` |
  | Hủy | `Esc` |
  | Repeat Last Capture | `Ctrl+Shift+R` |
- **Tùy chọn:** cho phép **đổi/gán lại** mọi hotkey; cảnh báo xung đột (vd `Print Screen` bị tiện ích chụp màn hình sẵn của Windows 11 chiếm); bật tích hợp `Print Screen`.
- **Ưu tiên:** **⭐MVP P1** — cả 1 hotkey global cơ bản LẪN khả năng **đổi/gán lại**. Hotkey là "linh hồn" tốc độ, KHÔNG để P12. Người dùng phải đổi được vì `Print Screen` hay đụng tiện ích chụp màn hình sẵn của Windows 11 — chính spec này ghi nhận xung đột đó. Default Repeat-Last-Capture = `Ctrl+Shift+R` (khớp SPEC 01 §01.22).
- **Ghi chú kỹ thuật C++/Qt:** đăng ký global hotkey trên Windows qua `RegisterHotKey` (Win32) — Qt không có API global hotkey sẵn, cần lớp platform (hoặc thư viện `QHotkey`). Lưu mapping trong `QSettings`.

### 10.3 Theme (giao diện app)
- **Mô tả:** Light / Dark / theo hệ thống.
- **Ưu tiên:** P13 (polish).
- **Ghi chú kỹ thuật C++/Qt:** Qt stylesheet (QSS) hoặc `QStyleHints::colorScheme` (Qt 6.5+); palette tùy biến.

### 10.4 Brand Themes & Quick Styles (khác với theme app)
- **Mô tả:** bộ màu thương hiệu + font tạo ra Quick Styles cho annotation (xem SPEC 02 §Quick Styles). Khác với theme giao diện app.
- **Ưu tiên:** P13.
- **Ghi chú kỹ thuật:** lưu file `.ezsnagtheme` (JSON), nạp vào palette tool properties.

### 10.5 Cập nhật (auto-update)
- **Mô tả:** kiểm tra bản mới (như updater của Ezcel C++).
- **Ưu tiên:** sau MVP.
- **Ghi chú kỹ thuật:** tái dùng pattern `Updater`/`VersionCompare` từ Ezcel (GitHub Releases).

## Bảng ưu tiên
| Tính năng | Phase |
|---|---|
| Khung Preferences + Capture/Editor tab | P1–P2 |
| Global hotkey chụp + **đổi/gán lại hotkey** | ⭐MVP P1 |
| Share/Library tab | P5–P6 |
| Theme app (light/dark), Quick Style themes nâng cao, command palette | P13 |
| Auto-update | sau MVP |

## Điểm chưa chắc / cần verify
- Phím mặc định "Repeat Last Capture" trên Windows — đề xuất `Ctrl+Shift+R`, xác nhận khi build.
- Ezsnagit Windows-first; các nền tảng khác tạm bỏ qua.
- Danh sách tab Preferences cuối cùng có thể tinh chỉnh; verify trên app thật.
