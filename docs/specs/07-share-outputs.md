# SPEC 07 — Share / Outputs (Ezsnagit)

> Nguồn: docs/research/05-library-share-templates.md · Module: `ezsnag_share`

## Mục tiêu & phạm vi

Share/Outputs là lớp đưa capture **ra ngoài** Ezsnagit: copy clipboard, lưu file, gửi email, mở bằng program ngoài, và (gác lại) các đích cloud. Mỗi đích là một **destination** có cấu hình riêng, hiển thị trong **Share dropdown** ở Editor, thứ tự và việc bật/tắt do người dùng quản lý trong Preferences > Share.

**Ranh giới clone (quan trọng):**
- **⭐MVP (P6):** File (PNG/JPG/PDF), Clipboard, Email (MAPI), Open-with / Program (custom external app), và **interface `IShareDestination` pluggable** (xem 07.2a).
- **Differentiator (P7):** after-capture task chain (xem 07.5).
- **Gác lại (P-sau):** mọi đích cloud bản quyền cần OAuth (Screencast, Google Drive, Dropbox, Box, OneDrive/SharePoint, Slack, Teams, YouTube, Camtasia Online…). Khung plugin định nghĩa sớm nhưng OAuth/auth triển khai sau; **Share Link hosted là hạng mục cloud ưu tiên #1**.

Trước khi share, mọi đích raster sẽ **flatten** document `.ezsnagx` xuống ảnh phẳng theo định dạng đích (chi tiết flatten ở SPEC 09).

---

## Tính năng

### 07.1 Ba phương thức share

- **Mô tả:**
  1. **Copy / Copy All** → clipboard, paste bằng Ctrl+V (Win) / Cmd+V (Mac).
  2. **Share Link** → upload lên cloud host (mặc định Screencast của Snagit) và copy URL chia sẻ.
  3. **Share to destination** → chọn từ Share dropdown.
- **Ưu tiên:** Copy = ⭐MVP (P6); Share to destination = ⭐MVP (P6) (cho các đích local); Share Link = P-sau (cần cloud).
- **Ghi chú ưu tiên Share Link:** một luồng **"copy link" hosted share chạy ngon, robust** là **flow chia sẻ hiện đại được yêu cầu nhiều nhất** (research 09 §6 #8 — "covers the dominant modern flow"). Vì thế khi tới giai đoạn cloud (**P-sau**), Share Link là **hạng mục cloud ưu tiên số 1** — làm trước các đích Office/Comm OAuth khác. Có thể cân nhắc một bản link tối giản (self-host/local-server) sớm hơn nếu khả thi.
- **Ghi chú clone C++/Qt:**
  - Copy: flatten → `QImage` → `QClipboard::setImage` (giữ alpha nếu định dạng hỗ trợ). "Copy All" áp cho nhiều capture chọn.
  - Share Link để khung interface nhưng disable trong bản đầu (cloud gác lại).

### 07.2 Danh sách Share destinations (outputs)

> Thứ tự/danh sách tùy nền tảng và đích nào được bật trong Preferences. Dưới là hợp nhất các đích Snagit tài liệu hóa, kèm trạng thái clone.

**Local & other (LÀM TRƯỚC)**
- **File** — lưu ra đĩa theo định dạng chọn (PNG/JPG/PDF/.ezsnagx… xem SPEC 09). → ⭐MVP (P6)
- **Clipboard** (Win) — copy ra clipboard như một destination. → ⭐MVP (P6)
- **Email** (Win) — đính kèm vào email. → ⭐MVP (P6)
  - **Cơ chế (đã chốt):** dùng **MAPI làm cơ chế chính** trên Windows — `MAPISendMail` (Simple MAPI) cho phép **đính kèm file** capture trực tiếp và mở mail client mặc định với attachment sẵn. **`mailto:` là fallback** khi máy không có MAPI provider khả dụng (`isAvailable()` MAPI = false) — mở compose window với subject/body nhưng **không kèm được file**, người dùng tự đính kèm tay. Không phụ thuộc Outlook-qua-COM (tránh ràng buộc Office).
- **Program / Application** — gửi capture cho external app tùy chỉnh (xem 07.4), tức "open-with-program". → ⭐MVP (P6)
- **Printer** (Win; không có trên Windows-on-ARM). → P-sau
- **FTP / SFTP** — upload file qua FTP/SFTP, là uploader cắm vào `IShareDestination` (xem 07.2a). → fast-follow (sau P6)

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

#### 07.2a Lớp share/upload pluggable (`IShareDestination`) — thiết kế từ ngày đầu

- **Quyết định thiết kế:** lớp share/upload là một **interface `IShareDestination` pluggable ngay từ ngày đầu**, KHÔNG hard-code danh sách đích. Mọi destination (local built-in lẫn cloud/upload nạp sau) đều đi qua cùng một interface, được đăng ký vào một registry và render động ra Share dropdown.
- **Lý do:** đây là **ShareX power-feature** — sự linh hoạt về upload-destination là khoảng trống cạnh tranh lớn nhất của Snagit (research 07: "limited upload-destination flexibility, no real plugin/extension ecosystem"). Thiết kế interface sớm cho phép sau này cắm thêm **custom HTTP uploader** (URL + headers + parse response regex để lấy link) và **FTP/SFTP** mà không phải sửa core.
- **Ưu tiên:**
  - **Interface `IShareDestination` + registry + render dropdown động → P6** (làm cùng các đích local MVP).
  - **Custom HTTP uploader + FTP/SFTP → fast-follow** (sau P6, trước khi đụng cloud OAuth nặng). Đây là các uploader cắm thêm, không block MVP.
- **Hợp đồng tối thiểu:** `name`, `icon`, `isAvailable()`, `configure()` (mở widget cấu hình riêng), `share(captureRef, options)` (trả về kết quả: đường dẫn/URL/lỗi). Uploader bổ sung chỉ cần implement đúng interface này là plug được.

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

### 07.5 After-capture task chain (post-capture pipeline) — differentiator

- **Mô tả:**
  - Một **pipeline sau-chụp gọn nhẹ kiểu ShareX**: sau khi chụp, capture chạy qua một **chuỗi bước do người dùng cấu hình** thay vì một flow cố định. Chuỗi mặc định gọn: **effects → save → copy-link** (ví dụ: áp effect/border → lưu file PNG → copy đường dẫn/URL vào clipboard).
  - Người dùng tự **bật/tắt và sắp xếp thứ tự** từng bước trong chuỗi (giống reorder destination ở 07.3). Mỗi bước là một hành động nhỏ, composable: áp effect, mở editor, lưu file, copy ra clipboard, gửi tới một `IShareDestination`, copy link sau khi share.
  - Đây là **half thứ hai của định vị "CleanShot UX + ShareX automation"** (research 07 §10): Snagit không có chuỗi automation tương đương. Giữ **nhẹ** — không phải workflow engine đầy đủ của ShareX, chỉ một chuỗi tuyến tính các bước có sẵn.
- **Ưu tiên:** **P7 (differentiator)** — xem ROADMAP P7 ("After-capture chain + Pinned screenshots"). Không thuộc MVP P6; là fast-follow tạo khác biệt.
- **Ghi chú clone C++/Qt:**
  - Chuỗi lưu dưới dạng danh sách step (kiểu + tham số) trong settings; runner duyệt tuần tự, mỗi step nhận/đẩy `captureRef`.
  - Tái dùng đúng các khối đã có: effect engine (SPEC 03), File destination, Clipboard, và `IShareDestination` (07.2a) cho bước upload/share. Step "copy-link" lấy URL trả về từ `share()`.
  - Cross-reference: ROADMAP **P7**; lớp upload pluggable ở **07.2a**.

---

## Bảng ưu tiên

| ID | Đích / tính năng | Ưu tiên |
|---|---|---|
| 07.1 | Copy / Copy All (clipboard) | ⭐MVP (P6) |
| 07.2 | File destination (export ra đĩa) | ⭐MVP (P6) |
| 07.2 | Clipboard destination | ⭐MVP (P6) |
| 07.2 | Email (Win) — MAPI chính, mailto fallback | ⭐MVP (P6) |
| 07.2 | Program/Application (open-with, custom) | ⭐MVP (P6) |
| 07.2a | Interface `IShareDestination` pluggable + registry | ⭐MVP (P6) |
| 07.3 | Bật/tắt + reorder destination | P6 |
| 07.2a | Custom HTTP uploader + FTP/SFTP | fast-follow (sau P6) |
| 07.5 | After-capture task chain (pipeline) | P7 (differentiator) |
| 07.2 | Printer | P-sau |
| 07.1 | Share Link (hosted copy-link) | **P-sau — ưu tiên #1 khi làm cloud** |
| 07.2 | Cloud/Office/Comm (OAuth) | P-sau (gác) |
| 07.2 | Mac share sheet / Pages/Keynote | P-sau (nếu build Mac) |

## Điểm chưa chắc

- **Share dropdown** mặc định bật những đích nào (subset) chưa cố định — admin/enterprise có thể đổi; ta đề xuất mặc định: File, Clipboard, Email, Program.
- Per-destination **keyboard shortcut** là Mac-only ở Snagit; có thể nâng thành cross-platform trong clone — để ngỏ.
- Định dạng file mặc định khi "Program" bàn giao (PNG vs giữ `.ezsnagx`) tùy app đích — cho cấu hình per-program.

**Đã chốt (không còn để ngỏ):**
- **Email (Win):** **MAPI làm cơ chế chính** (`MAPISendMail`, đính kèm được), **`mailto:` fallback** khi không có MAPI. Không dùng Outlook-qua-COM. (xem 07.2 Email)
- **Khung plugin / lớp upload:** thiết kế **interface `IShareDestination` pluggable ngay từ ngày đầu** (P6); custom HTTP uploader + FTP/SFTP là fast-follow. (xem 07.2a)
- **After-capture chain** đưa vào **P7** như differentiator. (xem 07.5)
