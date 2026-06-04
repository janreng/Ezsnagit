# SPEC 10 — Preferences & Hotkeys (Ezsnagit)

> Nguồn: docs/research/05-library-share-templates.md (§5), 01-capture.md (§8–9) · Module: `ui` (app shell)

## Mục tiêu & phạm vi

Trung tâm cấu hình của app: Preferences theo tab + hệ phím tắt toàn cục có thể tùy biến + theme.
Đây là phần "glue" của app, không phải module logic riêng — sống trong lớp `ui`/app shell.

## Tính năng

### 10.1 Preferences dialog (theo tab)
- **Mô tả:** hộp thoại cấu hình chia tab giống Snagit: **Capture, Editor, Share, Library, Hotkeys/Keyboard, Advanced, Save As**.
- **Tùy chọn:**
  - *Capture:* cursor capture on/off, delay mặc định, preview-in-editor sau chụp, multi-monitor.
  - *Editor:* màu canvas mặc định, grid/snapping, đơn vị (px), auto-flatten cảnh báo.
  - *Share:* bật/tắt + sắp xếp các đích chia sẻ (xem SPEC 07).
  - *Library:* vị trí thư mục store `.ezsnagx`, dung lượng, tự dọn.
  - *Advanced:* tăng tốc phần cứng, ghi log, reset.
- **Ưu tiên:** khung dialog + Capture/Editor tab = P1–P2; còn lại theo phase tính năng tương ứng.
- **Ghi chú clone C++/Qt:** `QDialog` + `QTabWidget`; lưu qua `QSettings` (registry/INI trên Windows). Mỗi tab là 1 `QWidget` con. Tách struct `AppSettings` thuần để test.

### 10.2 Hotkeys toàn cục (global)
- **Mô tả:** phím tắt gọi chụp ngay cả khi app ở nền. Mặc định kiểu Snagit (Windows):
  | Hành động | Mặc định (Win) |
  |---|---|
  | Global Capture (mở chọn vùng) | `Print Screen` |
  | Bắt đầu chọn region | `Shift+Enter` |
  | Chốt capture | `Enter` |
  | Hủy | `Esc` |
  | Repeat Last Capture | (cần verify — research §10) |
- **Tùy chọn:** cho phép **đổi/gán lại** mọi hotkey; cảnh báo xung đột (vd Print Screen bị Snipping Tool Win11 chiếm); bật tích hợp Print Screen.
- **Ưu tiên:** **⭐MVP P1** — cả 1 hotkey global cơ bản LẪN khả năng **đổi/gán lại**. (REVIEW finding 1: hotkey là "linh hồn" tốc độ, KHÔNG để P12. Người dùng phải đổi được vì Print Screen hay đụng Snipping Tool Win11 — chính spec này ghi nhận xung đột đó.) Default Repeat-Last-Capture = `Ctrl+Shift+R` (khớp SPEC 01 §01.22).
- **Ghi chú clone C++/Qt:** đăng ký global hotkey trên Windows qua `RegisterHotKey` (Win32) — Qt không có API global hotkey sẵn, cần lớp platform (hoặc thư viện `QHotkey`). Lưu mapping trong `QSettings`.

### 10.3 Theme (giao diện app)
- **Mô tả:** Light / Dark / theo hệ thống — như Snagit.
- **Ưu tiên:** P13 (polish).
- **Ghi chú clone C++/Qt:** Qt stylesheet (QSS) hoặc `QStyleHints::colorScheme` (Qt 6.5+); palette tùy biến.

### 10.4 Brand Themes & Quick Styles (khác với theme app)
- **Mô tả:** bộ màu thương hiệu + font tạo ra Quick Styles cho annotation (xem SPEC 02 §Quick Styles). Khác với theme giao diện app.
- **Ưu tiên:** P13.
- **Ghi chú clone:** lưu file `.ezsnagtheme` (JSON), nạp vào palette tool properties.

### 10.5 Cập nhật (auto-update)
- **Mô tả:** kiểm tra bản mới (như updater của Ezcel C++).
- **Ưu tiên:** sau MVP.
- **Ghi chú clone:** tái dùng pattern `Updater`/`VersionCompare` từ Ezcel (GitHub Releases).

## Bảng ưu tiên
| Tính năng | Phase |
|---|---|
| Khung Preferences + Capture/Editor tab | P1–P2 |
| Global hotkey chụp + **đổi/gán lại hotkey** | ⭐MVP P1 |
| Share/Library tab | P5–P6 |
| Theme app (light/dark), Quick Style themes nâng cao, command palette | P13 |
| Auto-update | sau MVP |

## Điểm chưa chắc / cần verify
- Phím mặc định "Repeat Last Capture" trên Windows (research §10 chưa chốt).
- Mac có hệ hotkey + một số tab Preferences khác — clone Windows-first nên tạm bỏ qua.
- Danh sách tab Preferences chính xác của bản mới nhất có thể khác; cần verify trên app thật.
