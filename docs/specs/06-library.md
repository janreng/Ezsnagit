# SPEC 06 — Library (Ezsnagit)

> Nguồn: docs/research/05-library-share-templates.md · Module: `ezsnag_library`

## Mục tiêu & phạm vi

Library là **kho lưu trữ bền vững** của mọi capture (ảnh, video, GIF) mà người dùng từng tạo, được **tự lưu** (auto-save) ngay khi capture chứ không cần lệnh "Save" thủ công. Library gồm hai bề mặt:

- **Recent Captures Tray** — dải thumbnail ngang ở đáy Editor, chỉ hiển thị các item *gần đây*.
- **Library View** — tab riêng ở đầu Editor, hiển thị toàn bộ lịch sử với search/filter/sort/tag/flag.

Phạm vi spec này: hành vi auto-save, Tray, Library View, tag/flag, quản lý capture, vị trí lưu trữ, và lược đồ SQLite + cơ chế store file `.ezsnagx`.

Ngoài phạm vi (ở spec khác): định dạng `.ezsnagx` chi tiết → SPEC 09; Share/export đích → SPEC 07; Templates/Combine → SPEC 08.

---

## Tính năng

### 06.1 Auto-Save (khái niệm cốt lõi)

- **Mô tả:** mỗi capture ảnh được tự lưu thành file `.ezsnagx` trong Library; mỗi video capture chưa lưu được tự lưu thành `.mp4`. Người dùng **không cần** bấm Save để giữ capture — toàn bộ edit + annotation được persist tự động. Library do đó là lịch sử capture đầy đủ; capture nằm lại cho đến khi người dùng xóa rõ ràng.
- **Tùy chọn:** trong Preferences > Library có thể tắt *"Tự động lưu image capture mới vào Library"*. Nếu auto-save `.ezsnagx` thất bại → cảnh báo cho người dùng.
- **Ưu tiên:** P0 (nền tảng — không có auto-save thì không có Library).
- **Ghi chú clone C++/Qt:**
  - Khi Editor tạo/đổi document, `ezsnag_library` ghi/cập nhật file `.ezsnagx` vào **store** (thư mục do app quản lý) và upsert một bản ghi trong SQLite (`captures`).
  - Ghi an toàn: write tạm `*.tmp` → `QSaveFile` (atomic rename) để tránh hỏng file khi crash giữa chừng. Nếu rename thất bại → phát signal `autoSaveFailed(captureId)` cho UI hiện cảnh báo.
  - Auto-save chạy **debounce** (vd 1–2s sau thao tác cuối) trên thread riêng để không block UI.
  - Mô hình modern collapse "Unsaved vs Saved" của Snagit cũ: ở Ezsnagit mọi thứ auto-save dạng `.ezsnagx`; "Save As" chỉ là **export** một bản phẳng (PNG/JPG…) ra vị trí người dùng chọn (xem SPEC 09).

### 06.2 Recent Captures Tray

- **Mô tả:** dải ngang ở đáy Editor hiển thị image/video/GIF và file đã mở/edit gần đây. Click thumbnail → load lên canvas.
- **Tùy chọn:**
  - **Capacity:** số item hiển thị cấu hình được, **1–200** (Preferences > Library, "recent-captures count").
  - **Pinning:** ghim thumbnail về **bên trái** Tray để truy cập nhanh; capture mới xuất hiện bên phải các item đã ghim; click lại để bỏ ghim. Hỗ trợ pin cho image, GIF, video.
  - **Reordering:** kéo–thả thumbnail (đã ghim hoặc chưa) để sắp xếp lại — hữu ích khi gom asset của một project.
  - **Unsaved-changes indicator:** icon **"sunburst" sao màu cam** đánh dấu file có edit chưa export/lưu phẳng.
- **Quan hệ với Library:** Tray chỉ là *recent*; Library là kho đầy đủ. Bỏ/clear một item khỏi Tray **không** xóa nó — vẫn khôi phục được trong Library.
- **Ưu tiên:** P0 cho hiển thị + click load; P1 cho pin/reorder.
- **Ghi chú clone C++/Qt:**
  - Tray là `QListView` ngang (`setFlow(LeftToRight)`, `setWrapping(false)`) với một **`QAbstractListModel`** đọc từ `ezsnag_library` (query N item mới nhất theo `created_at`, cùng các item `pinned=1`).
  - Thumbnail dạng `QPixmap` cache (thumbnail cache trên đĩa + LRU in-memory) để cuộn mượt; render decouple khỏi load ảnh gốc.
  - Drag–drop dùng `QListView` internal move; thứ tự lưu vào cột `tray_order` (xem schema). Pin = cột `pinned` + sort key đặt các pinned trước.
  - Sunburst icon = overlay delegate khi `dirty=1` (document có thay đổi chưa flatten/export).

### 06.3 Library View

- **Mô tả:** tab ở đầu Editor mở kho toàn bộ capture: image, video, animated GIF, với search/filter/sort.
- **Search:** free-text theo **filename**; kết quả lọc trực tiếp (live).
- **Filter categories (tự sinh từ dữ liệu):**
  - **File Type** — Images / Videos / Animated GIFs
  - **Date** — gom theo năm, tháng, ngày tạo
  - **Applications** — mọi app đã chụp từ đó (Chrome, Word…)
  - **Websites** — mọi site đã chụp (URL-aware)
  - **Tags** — tag tùy chỉnh người dùng gán
  - **Flags** — visual marker người dùng gán (searchable)
  - **Favorites** — (Mac) mục đánh dấu yêu thích
- **Sorting:** theo **date modified, date created, name, size**; click heading để sort / đảo chiều.
- **Ưu tiên:** P0 cho search filename + filter File Type/Date + sort; P1 cho Applications/Websites/Tags/Flags; P2 cho Favorites (Mac).
- **Ghi chú clone C++/Qt:**
  - View = grid thumbnail (`QListView` IconMode) + sidebar filter (cây category). Backing model là proxy trên dữ liệu SQLite.
  - Filter categories **auto-generated**: thực thi `SELECT DISTINCT`/`GROUP BY` trên các cột metadata (`source_app`, `source_url`, `file_type`, `created_at`) và bảng `tags`/`flags`. Date group bằng query nhóm theo `strftime` năm/tháng/ngày.
  - Search live: debounce input → `WHERE filename LIKE ?`; kết hợp với filter đang chọn bằng `AND`.
  - Sort: ánh xạ heading → `ORDER BY` cột tương ứng; lưu trạng thái sort cuối.

### 06.4 Tags & Flags (tổ chức)

- **Mô tả:**
  - **Tags:** chọn 1+ capture (Ctrl/Cmd+click nhiều) → nút **Tag** → gõ tag mới hoặc chọn tag có sẵn. Xem mọi tag qua category **Tags** trong filter. Tag dùng để gom/tìm lại capture.
  - **Flags:** visual marker gắn vào capture để nổi bật trong Tray/Library. Flag lọc/tìm được như tag nhưng là **tập marker cố định** (không phải free text).
- **Ưu tiên:** P1.
- **Ghi chú clone C++/Qt:**
  - Tags là quan hệ nhiều–nhiều: bảng `tags(id, name)` + `capture_tags(capture_id, tag_id)`. Free text → upsert tag theo `name` (unique, case-insensitive).
  - Flags là tập enum cố định (vd: Red/Yellow/Green/Important…); lưu bitmask `flags INTEGER` trên `captures` hoặc bảng `capture_flags` nếu cho nhiều flag. Chọn bitmask cho gọn vì tập cố định.
  - Gán hàng loạt (multi-select) → một transaction.

### 06.5 Quản lý Capture

- **Mô tả:**
  - **Open:** double-click thumbnail → load vào Editor.
  - **Select:** Ctrl/Cmd+click (nhiều); Shift+click (range).
  - **Delete:** right-click → **Delete** (Win) / **Move to Trash** (Mac). Hỗ trợ multi-select. Có thể có prompt xác nhận (Preferences).
  - **Metadata:** right-click (Win) / Ctrl+click (Mac) xem metadata (ngày, source app/website).
  - **Combine / templates:** right-click → **Combine in Template** (nhiều), **Apply Template** (một), hoặc **Create Video from Images** (xem SPEC 08).
- **Ưu tiên:** P0 (open/select/delete); P1 (metadata view, context-menu vào Templates).
- **Ghi chú clone C++/Qt:**
  - Delete = xóa bản ghi SQLite + file `.ezsnagx`/`.mp4` trong store (có thể đẩy vào thư mục Trash nội bộ để undo). Confirm prompt theo setting.
  - Context menu là `QMenu`; entry Templates ủy quyền sang `ezsnag_templates`.

### 06.6 Vị trí lưu trữ (Storage)

- **Mô tả:**
  - Capture auto-save nằm trong **Library store** — thư mục ẩn do app quản lý mặc định.
  - Vị trí store có thể **đổi** qua Preferences > Library ("Library location management").
  - Bản export (PNG/JPG…) đi tới thư mục đích người dùng chọn lúc export/share (không nằm trong store).
  - Lưu store lên cloud sync folder (OneDrive/Drive/Dropbox) cho backup + đa thiết bị — nhưng *Library gốc là local*.
- **Ưu tiên:** P0 cho store mặc định; P2 cho di chuyển location.
- **Ghi chú clone C++/Qt:**
  - Mặc định store ở `QStandardPaths::AppDataLocation`/`<app>/Library/` chứa DB `library.db` (SQLite) + thư mục `store/` chứa các file `.ezsnagx`/`.mp4` (đặt tên theo UUID).
  - Di chuyển location = move file + cập nhật đường dẫn base (DB chỉ lưu **đường dẫn tương đối** trong store để move không vỡ tham chiếu).

---

## Bảng ưu tiên

| ID | Tính năng | Ưu tiên |
|---|---|---|
| 06.1 | Auto-save `.ezsnagx`/`.mp4` + cảnh báo lỗi | P0 |
| 06.2 | Recent Captures Tray (hiển thị + load) | P0 |
| 06.2 | Tray pin / reorder | P1 |
| 06.3 | Library View: search filename + File Type/Date filter + sort | P0 |
| 06.3 | Filter Applications/Websites/Tags/Flags | P1 |
| 06.3 | Favorites (Mac) | P2 |
| 06.4 | Tags | P1 |
| 06.4 | Flags | P1 |
| 06.5 | Open/Select/Delete | P0 |
| 06.5 | Metadata view + context-menu Templates | P1 |
| 06.6 | Store mặc định + DB | P0 |
| 06.6 | Di chuyển Library location | P2 |

## Điểm chưa chắc

- **Default Tray count** so với max 200 có thể khác theo version Snagit; ta chọn mặc định hợp lý (vd 25) trong khoảng 1–200.
- **Favorites** là Mac-only ở Snagit; với clone có thể nâng thành flag chung "Favorite" thay vì giữ phân biệt nền tảng — cần quyết định.
- **Flags** lưu bitmask (tập cố định) vs bảng riêng (cho phép custom flag tương lai) — chọn bitmask trước, để ngỏ migration.
- Metadata **source_app / source_url** phụ thuộc engine capture có lấy được context cửa sổ/URL không (liên quan `ezsnag_capture`); có thể null trong bản đầu.
- Cơ chế **backup import** (Preferences > Library) chưa scope ở spec này — cần spec riêng nếu làm.
