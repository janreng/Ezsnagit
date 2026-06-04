# SPEC 07 — Share / Outputs (Ezsnagit)

> Nguồn: docs/research/05-library-share-templates.md · Module: `ezsnag_share`

## Mục tiêu & phạm vi

Share/Outputs là lớp đưa capture **ra ngoài** Ezsnagit: copy clipboard, lưu file, gửi email, mở bằng program ngoài, và (gác lại) các đích cloud. Mỗi đích là một **destination** có cấu hình riêng, hiển thị trong **Share dropdown** ở Editor, thứ tự và việc bật/tắt do người dùng quản lý trong Preferences > Share.

**Ranh giới clone (quan trọng):**
- **Làm trước (P0–P1):** File, Clipboard, Email, Open-with / Program (custom external app).
- **Gác lại (P3+):** mọi đích cloud bản quyền cần OAuth (Screencast, Google Drive, Dropbox, Box, OneDrive/SharePoint, Slack, Teams, YouTube, Camtasia Online…). Định nghĩa khung plugin nhưng chưa triển khai auth.

Trước khi share, mọi đích raster sẽ **flatten** document `.ezsnagx` xuống ảnh phẳng theo định dạng đích (chi tiết flatten ở SPEC 09).

---

## Tính năng

### 07.1 Ba phương thức share

- **Mô tả:**
  1. **Copy / Copy All** → clipboard, paste bằng Ctrl+V (Win) / Cmd+V (Mac).
  2. **Share Link** → upload lên cloud host (mặc định Screencast của Snagit) và copy URL chia sẻ.
  3. **Share to destination** → chọn từ Share dropdown.
- **Ưu tiên:** Copy = P0; Share to destination = P0 (cho các đích local); Share Link = P3 (cần cloud).
- **Ghi chú clone C++/Qt:**
  - Copy: flatten → `QImage` → `QClipboard::setImage` (giữ alpha nếu định dạng hỗ trợ). "Copy All" áp cho nhiều capture chọn.
  - Share Link để khung interface nhưng disable trong bản đầu (cloud gác lại).

### 07.2 Danh sách Share destinations (outputs)

> Thứ tự/danh sách tùy nền tảng và đích nào được bật trong Preferences. Dưới là hợp nhất các đích Snagit tài liệu hóa, kèm trạng thái clone.

**Local & other (LÀM TRƯỚC)**
- **File** — lưu ra đĩa theo định dạng chọn (PNG/JPG/PDF/.ezsnagx… xem SPEC 09). → P0
- **Clipboard** (Win) — copy ra clipboard như một destination. → P0
- **Email** (Win) — đính kèm vào email. → P1
- **Program / Application** — gửi capture cho external app tùy chỉnh (xem 07.4). → P1
- **Printer** (Win; không có trên Windows-on-ARM). → P2
- **FTP** — upload file qua FTP. → P2

**Cloud & collaboration (GÁC LẠI — cần OAuth)**
- Screencast (đích Share Link mặc định), Google Drive, Dropbox, Box, Microsoft OneDrive (Win), Microsoft SharePoint (Win). → P3+

**Microsoft Office (GÁC LẠI / tùy chọn)**
- Word, PowerPoint, Excel, OneNote (Win), Outlook (Mac). → P3

**Communication (GÁC LẠI — cần OAuth/app)**
- Slack, Microsoft Teams, X/Twitter (Win). → P3+

**Video / TechSmith ecosystem (NGOÀI PHẠM VI)**
- YouTube, Camtasia Online, Camtasia Editor. → out of scope (đó là pipeline Camtasia).

**Apple-specific / Mac (TÙY CHỌN)**
- Pages, Keynote, Numbers; macOS share sheet: AirDrop, Mail, Messages, Notes, Photos, Freeform, Journal, Reminders. → P3 (qua native share sheet nếu build Mac).

- **Ghi chú clone C++/Qt:** Mỗi destination implement chung interface `IShareDestination` (`name`, `icon`, `isAvailable()`, `configure()`, `share(captureRef, options)`). Đích cloud là plugin nạp sau; đích local là built-in. Khác biệt Win/Mac (Email/Clipboard/Printer/X là Win-only; Outlook/Pages/share-sheet là Mac-only) thể hiện bằng `isAvailable()` theo `#ifdef Q_OS_*`.

### 07.3 Cấu hình / bật–tắt outputs

- **Mô tả:**
  - **Windows:** Editor → Edit > Editor Preferences > **Share** tab → tick/untick để bật/tắt; reorder để đổi thứ tự trong Share dropdown.
  - **Mac:** Snagit menu > Settings > **Share** tab → **+** thêm đích từ list, **−** gỡ; reorder tùy ý.
  - Per-destination preferences: đặt **account**, **privacy level**, **default folder**, **name** tùy chỉnh (Mac), **keyboard shortcut** (Mac).
- **Ưu tiên:** P1 (UI bật/tắt + reorder); per-destination prefs theo từng đích.
- **Ghi chú clone C++/Qt:**
  - Cấu hình lưu trong `QSettings` (hoặc bảng config trong SQLite của library): danh sách destination enabled + thứ tự + tham số mỗi đích.
  - Share dropdown render từ danh sách enabled theo thứ tự đã lưu; reorder = drag trong list settings.
  - Per-destination prefs là dialog do chính destination cung cấp (`configure()` mở widget riêng).

### 07.4 Custom outputs (Program / Application)

- **Mô tả:**
  - **Windows:** Share dropdown → **Program > Manage > Add Program** → chọn `.exe`. Ezsnagit bàn giao file capture cho program đó.
  - **Mac:** Share dropdown → **Preferences > Add Share > Application** → chọn startup file của app.
  - Cho phép tải thêm destination plugin; enterprise có thể pre-config qua deployment.
- **Ưu tiên:** P1.
- **Ghi chú clone C++/Qt:**
  - "Program" destination = flatten/export ra file tạm (định dạng cấu hình, mặc định PNG) → `QProcess::startDetached(exePath, {tempFile})`.
  - Quản lý danh sách program (path + tên + định dạng truyền) trong settings; mỗi program là một entry destination động.
  - Mac: tương tự, mở bằng app chỉ định (`open -a App tempFile`).

---

## Bảng ưu tiên

| ID | Đích / tính năng | Ưu tiên |
|---|---|---|
| 07.1 | Copy / Copy All (clipboard) | P0 |
| 07.2 | File destination (export ra đĩa) | P0 |
| 07.2 | Clipboard destination | P0 |
| 07.2 | Email (Win) | P1 |
| 07.2 | Program/Application (custom) | P1 |
| 07.3 | Bật/tắt + reorder destination | P1 |
| 07.2 | Printer / FTP | P2 |
| 07.1 | Share Link | P3 (cloud) |
| 07.2 | Cloud/Office/Comm (OAuth) | P3+ (gác) |
| 07.2 | Mac share sheet / Pages/Keynote | P3 (nếu build Mac) |

## Điểm chưa chắc

- **Email** trên Win: dùng MAPI / mailto với attachment? mailto không đính kèm file được → cần MAPI (`MAPISendMail`) hoặc mở Outlook qua COM. Cần quyết định cách triển khai.
- **Share dropdown** mặc định bật những đích nào (subset) chưa cố định — admin/enterprise có thể đổi; ta đề xuất mặc định: File, Clipboard, Email, Program.
- Per-destination **keyboard shortcut** là Mac-only ở Snagit; có thể nâng thành cross-platform trong clone — để ngỏ.
- Khung **plugin cloud** (interface + cơ chế nạp) thiết kế sớm hay đợi tới P3 — đề xuất định nghĩa interface ngay, hoãn implement.
- Định dạng file mặc định khi "Program" bàn giao (PNG vs giữ `.ezsnagx`) tùy app đích — cho cấu hình per-program.
