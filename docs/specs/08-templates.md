# SPEC 08 — Templates / Create (Ezsnagit)

> Nguồn: docs/research/05-library-share-templates.md · Module: `ezsnag_templates`

## Mục tiêu & phạm vi

Cụm **Create** gom các tính năng ghép nhiều capture thành asset hoàn chỉnh:

- **Create Image from Template / Combine Images** — ghép nhiều ảnh thành một visual guide/tutorial có layout chuyên nghiệp.
- **Apply Template** — áp template cho một ảnh đơn.
- **Create Video from Images** — thuyết minh giọng nói qua một/nhiều ảnh, annotate live, xuất `.mp4`.

Tất cả tận dụng **canvas object graph** của `ezsnag_canvas`: template là một layout chứa các **drop zone** (vùng thả ảnh) + object text/step number, sản phẩm vẫn editable khi còn ở `.ezsnagx`.

Ngoài phạm vi: Camtasia Online/Editor (pipeline video nặng) — chỉ ghi nhận là alternative, không clone.

---

## Tính năng

### 08.1 Create Image from Template / Combine Images

- **Mô tả:** ghép nhiều ảnh thành một visual guide có format. Dùng cho before/after side-by-side, giải thích quy trình/khái niệm, hoặc timeline sự kiện.
- **Tùy chọn nguồn template:**
  - Tập **built-in basic templates** ship sẵn.
  - Snagit có thêm layout qua TechSmith Assets (subscription) — với clone: **bộ template nội bộ + có thể nạp thêm từ thư mục/asset pack local** (không subscription).
  - Template hỗ trợ: **titles, themes, automatic step numbering, footer attribution**.
- **Flow A — từ đầu:**
  1. Create > **Image from Template** → mở dialog *Create Image from Template*.
  2. Chọn layout.
  3. Cấu hình: **image scaling** (*Fill entire drop zone* = phủ kín, có thể crop; *Fit entire image* = giữ trọn ảnh, có thể chừa khoảng trống) và **footer attribution**.
  4. **Create** → template mở trên canvas.
  5. **Kéo ảnh** từ Tray vào các drop zone; double-click + kéo để reposition trong zone; thêm **text/caption** và annotation.
- **Flow B — từ capture đã chọn:**
  - Trong Tray/Library chọn ảnh → right-click:
    - **Combine in Template** (nhiều ảnh) → chọn template, **reorder** thumbnail, thêm title/theme, bật **step numbering**.
    - **Apply Template** (một ảnh).
- **Edit lại template-based image:** mở lại sửa layout, swap ảnh, reorder, đổi theme… (giữ editable khi còn `.ezsnagx`).
- **Ưu tiên:** P1 (vài layout cơ bản + drop zone + scaling + step numbering); P2 (theme/title/footer nâng cao, asset pack ngoài).
- **Ghi chú clone C++/Qt:**
  - Template = file mô tả layout (JSON): danh sách **drop zone** (rect + scale mode), **text slot**, **step-number config**, **theme tham chiếu**, **footer**. Lưu trong `ezsnag_templates/templates/`.
  - Khi Create: dựng một document `.ezsnagx` mới từ template → drop zone là **ImageObject placeholder**; text slot là TextObject; step number là StepObject (tái dùng annotation của `ezsnag_canvas`).
  - **Scaling:** Fill = scale phủ + clip theo rect zone; Fit = scale giữ tỉ lệ trong rect, letterbox. Lưu mode per-zone để re-edit.
  - Drag ảnh từ Tray = drop event mang `captureId` → gán raster vào zone; reposition = pan ảnh trong viewport clip của zone.
  - **Auto step numbering:** khi nhiều ảnh/zone, sinh StepObject tăng dần theo thứ tự zone; reorder cập nhật số.

### 08.2 Apply Template (ảnh đơn)

- **Mô tả:** áp một template cho một ảnh đơn (frame/title/footer hóa một capture).
- **Ưu tiên:** P2.
- **Ghi chú clone C++/Qt:** trường hợp đặc biệt của 08.1 với layout 1 drop zone; tái dùng cùng engine dựng layout.

### 08.3 Create Video from Images

- **Mô tả:** record **voice narration** qua một/nhiều screenshot, annotate live khi nói → biến ảnh tĩnh thành video tutorial ngắn, output `.mp4`.
- **Flow:**
  1. Chọn 1+ ảnh (Ctrl/Cmd+click) trong Tray, **hoặc** Create > Video from Images, **hoặc** right-click selection → Create Video from Images.
  2. **Recording toolbar** hiện: chọn background color, cursor, **microphone**, **webcam**.
  3. Bắt đầu record — Shift+F9 (Win) / Ctrl+Shift+Space (Mac).
  4. Trong khi record: thêm **arrow, shape, step number, annotation** lên ảnh live.
  5. Dừng — Shift+F10 (Win) / Ctrl+Shift+V (Mac).
- **Ưu tiên:** P2 (phụ thuộc `ezsnag_video` đã có record + mic; annotate-live phức tạp).
- **Ghi chú clone C++/Qt:**
  - Ủy quyền capture/encode cho **`ezsnag_video`** (FFmpeg/Qt Multimedia): scene = ảnh hiển thị full-screen/canvas; record màn hình của khung trình chiếu + mic.
  - Annotate live = vẽ annotation object lên trên ảnh trong lúc record; recorder capture frame của khung đó.
  - Webcam PiP + cursor overlay tái dùng tính năng video ở SPEC 05.
  - Output `.mp4` (H.264) auto-save vào Library như video capture thường.

### 08.4 Tóm tắt Create

| Option | Input | Output |
|---|---|---|
| Image from Template / Combine in Template | Nhiều ảnh | Một ảnh guide ghép |
| Apply Template | Một ảnh | Ảnh đơn đã template hóa |
| Video from Images | Một/nhiều ảnh + narration | `.mp4` |

---

## Bảng ưu tiên

| ID | Tính năng | Ưu tiên |
|---|---|---|
| 08.1 | Combine Images: layout cơ bản + drop zone + scaling | P1 |
| 08.1 | Step numbering tự động + reorder | P1 |
| 08.1 | Title / theme / footer attribution nâng cao | P2 |
| 08.1 | Nạp asset pack template ngoài | P2 |
| 08.2 | Apply Template (ảnh đơn) | P2 |
| 08.3 | Video from Images (record + annotate live) | P2 |

## Điểm chưa chắc

- **Bộ layout built-in** ban đầu gồm bao nhiêu/những kiểu nào (side-by-side, grid, steps, timeline) — cần chốt danh sách tối thiểu.
- **Theme system** của template trùng/khác với Quick Styles Theme của Editor (SPEC 02) — cần thống nhất khái niệm "theme" để tránh nhầm.
- Định dạng file mô tả template (JSON nội bộ) cần version hóa để asset pack tương lai tương thích.
- **Video from Images** nặng và phụ thuộc `ezsnag_video`; annotate-live + đồng bộ với recorder là phần khó nhất — có thể ship sau khi video core ổn định.
- Cơ chế **download template** của Snagit (Assets subscription) không clone; thay bằng import thủ công thư mục template — cần quyết định UX import.
