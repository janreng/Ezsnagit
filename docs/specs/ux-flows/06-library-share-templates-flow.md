# UX FLOW (gộp) — Library · Share · Templates (Ezsnagit)

> Nguồn: docs/research/05-library-share-templates.md
> Spec liên quan: 06-library.md · 07-share-outputs.md · 08-templates.md · 09-file-formats.md
> Module: `ezsnag_library`, `ezsnag_share`, `ezsnag_templates`, `ezsnag_canvas`

Luồng từng-bước xuyên ba cụm: **Library** (tìm/lọc/tag) → **Share** (chọn đích → xuất) → **Templates** (Create from Template, Combine Images). Xem `06-library-flow.md` cho luồng Library chi tiết hơn.

---

## PHẦN 1 — LIBRARY: tìm / lọc / tag

### 1.1 Mở Library và tìm
1. Click tab **Library** ở đầu Editor.
2. Gõ ô **Search** → lọc **live** theo filename.

### 1.2 Lọc theo category (sidebar auto-generated)
1. Chọn một category: **File Type / Date / Applications / Websites / Tags / Flags / Favorites**.
2. Kết hợp nhiều category + ô Search để thu hẹp (AND).
3. **Sort** bằng click heading: date modified / date created / name / size; click lại đảo chiều.

### 1.3 Tag & Flag
1. Chọn 1+ capture (Ctrl/Cmd+click nhiều; Shift+click range).
2. **Tag** → gõ tag mới / chọn có sẵn → áp cho cả nhóm.
3. **Flag** → chọn marker cố định → gắn để nổi bật.
4. Lọc lại qua category **Tags**/**Flags**.

→ Từ một capture đã chọn, người dùng có thể đi tiếp sang **Share** (Phần 2) hoặc **Templates** (Phần 3).

---

## PHẦN 2 — SHARE: chọn đích → xuất

### 2.1 Copy nhanh ra clipboard
1. Ở Editor, bấm **Copy** (hoặc Copy All cho nhiều capture).
2. Hệ thống **flatten** document → ảnh phẳng → đặt vào clipboard.
3. Người dùng paste bằng Ctrl+V (Win) / Cmd+V (Mac).

### 2.2 Share tới một destination
1. Mở **Share dropdown** ở Editor → thấy danh sách destination đã bật (theo thứ tự trong Preferences).
2. Chọn đích:
   - **File** → chọn định dạng (PNG/JPG/PDF/.ezsnagx…) + thư mục → ghi ra đĩa.
   - **Clipboard** (Win) → như 2.1.
   - **Email** (Win) → soạn email kèm file đính kèm.
   - **Program/Application** → bàn giao file capture cho external app đã cấu hình.
3. Với mọi đích **raster**, hệ thống flatten document trước (object → pixel); **file `.ezsnagx` gốc trong Library không đổi** (bản editable vẫn còn).

### 2.3 Custom output (Program / Application)
1. Share dropdown → **Program > Manage > Add Program** (Win) / **Preferences > Add Share > Application** (Mac).
2. Chọn `.exe` / startup file của app.
3. Lần sau chọn program đó trong Share → Ezsnagit export ra file tạm rồi mở bằng app đó.

### 2.4 Cấu hình destination
1. Preferences > **Share** tab.
2. **Win:** tick/untick để bật/tắt; kéo để reorder thứ tự trong dropdown.
3. **Mac:** **+** thêm đích từ list, **−** gỡ; reorder; đặt tên/shortcut tùy chỉnh.
4. Per-destination: set account / privacy / default folder.

> Ghi chú phạm vi: File/Clipboard/Email/Program làm trước. Các đích cloud (Screencast, Drive, Dropbox, OneDrive, Slack, Teams, YouTube…) cần OAuth → gác lại; hiện disabled trong dropdown.

---

## PHẦN 3 — TEMPLATES: Create from Template & Combine Images

### 3.1 Create Image from Template (từ đầu)
1. **Create > Image from Template** → mở dialog.
2. Chọn **layout** template.
3. Cấu hình:
   - **Image scaling:** *Fill entire drop zone* (phủ kín, có thể crop) hoặc *Fit entire image* (giữ trọn ảnh, có thể chừa trống).
   - **Footer attribution** bật/tắt.
4. **Create** → template mở trên canvas với các **drop zone** trống.
5. **Kéo ảnh từ Tray** vào drop zone; double-click + kéo để reposition trong zone.
6. Thêm **text/caption**, annotation, **step number**.
7. Kết quả auto-save dạng `.ezsnagx` → vào Library; vẫn re-edit được (swap ảnh, reorder, đổi theme).

### 3.2 Combine in Template (từ capture đã chọn)
1. Trong Tray/Library chọn **nhiều ảnh** (Ctrl/Cmd+click).
2. Right-click → **Combine in Template**.
3. Chọn template → **reorder** thumbnail theo ý.
4. Thêm **title / theme**, bật **automatic step numbering** (reorder cập nhật số).
5. Xác nhận → ảnh ghép mở trên canvas, editable.

### 3.3 Apply Template (ảnh đơn)
1. Chọn **một ảnh** → right-click → **Apply Template**.
2. Chọn layout 1-zone (frame/title/footer) → áp lên ảnh.

### 3.4 Create Video from Images
1. Chọn 1+ ảnh trong Tray, **hoặc** Create > Video from Images, **hoặc** right-click → Create Video from Images.
2. **Recording toolbar** hiện: chọn background color, cursor, **microphone**, **webcam**.
3. Start record (Shift+F9 Win / Ctrl+Shift+Space Mac).
4. Khi record: thêm arrow/shape/step number/annotation **live** trên ảnh.
5. Stop (Shift+F10 Win / Ctrl+Shift+V Mac) → xuất `.mp4`, auto-save vào Library.

---

## Liên kết giữa ba cụm

- **Library → Share:** chọn capture trong Library/Tray → Share dropdown → đích.
- **Library → Templates:** chọn capture → right-click → Combine/Apply/Video.
- **Templates → Library:** sản phẩm ghép auto-save dạng `.ezsnagx` (ảnh) hoặc `.mp4` (video) → quay lại là item Library mới.
- **Mọi export raster** (Share File PNG/JPG/PDF, Copy clipboard) → **flatten**; bản `.ezsnagx` editable luôn được giữ trong Library (SPEC 09).

## Khác biệt nền tảng cần nhớ (Win vs Mac)

- **Win-only:** Email output, Clipboard destination, Program (custom .exe), Printer, X/Twitter, OneNote, Evernote, OneDrive/SharePoint.
- **Mac-only:** Outlook output, Pages/Keynote/Numbers, native share sheet (AirDrop/Mail/Messages/Notes/Photos/Freeform/Journal/Reminders), tên & shortcut tùy chỉnh per-destination; Favorites trong Library.
- **Hành vi:** (Snagit) trên Mac file non-`.ezsnagx` flatten khi quit — clone **đề xuất luôn giữ editable**, không bắt chước.
- Settings: Win = Edit > Editor Preferences; Mac = Snagit menu > Settings.
