# Ezsnagit — Roadmap

Kế hoạch phân kỳ clone Snagit bằng C++/Qt6. Nguyên tắc: **mỗi tính năng = 1 release**, đi từ
xương sống (capture → editor → save) rồi mở rộng. Mỗi phase nên kết thúc bằng một bản chạy được.

> Ký hiệu: 🟥 chưa làm · 🟨 đang làm · 🟩 xong. Hiện tại toàn bộ 🟥 (mới xong tài liệu).

## P0 — Khung dự án 🟥
- CMake + Qt6 + Ninja/MinGW skeleton (giống Ezcel), app shell, cửa sổ rỗng.
- Hệ test headless, CI build cơ bản.

## P1 — Capture cốt lõi 🟥
- Chụp **Region / Window / Full screen** → mở ảnh trong Editor.
- Capture Window tối giản + 1 hotkey toàn cục.
- `ezsnag_capture` (lớp platform Windows trước).

## P2 — Editor canvas + document model 🟥
- Canvas hiển thị ảnh nền; `ezsnag_canvas` với **object graph vector**, z-order, undo/redo.
- Lưu/mở **`.ezsnagx`** (ảnh nền + objects + metadata).
- Export PNG/JPG (flatten).

## P3 — Annotation cốt lõi 🟥
- Arrow, Text, Callout, Shape, Line, Highlight, Step, Blur (8 tool MVP).
- Tool Properties panel + Quick Styles cơ bản.

## P4 — Effects & transforms 🟥
- Crop, Trim, Resize, Rotate/Flip, Border, Shadow, Color filter.
- Cut Out (cắt dải + nối).

## P5 — Library & Recent 🟥
- Tự lưu mọi capture vào kho `.ezsnagx`; khay Recent Captures.
- Library view: tìm theo tên, lọc theo ngày/app, tag/flag (SQLite).

## P6 — Share / Outputs 🟥
- File (PNG/JPG/PDF/GIF), Clipboard, Email cơ bản, mở bằng app ngoài.
- Cấu trúc share-destination dạng plugin.

## P7 — Scrolling capture 🟥
- Auto scrolling (dọc/ngang/cả trang) + panoramic thủ công, ghép panorama.

## P8 — Grab Text (OCR) 🟥
- `ezsnag_ocr` qua Tesseract: trích text từ ảnh → clipboard; Edit Text cơ bản.

## P9 — Simplify & Smart Move 🟥
- `ezsnag_simplify` qua OpenCV: phát hiện phần tử UI → thay shape; Smart Move.

## P10 — Video 🟥
- Quay màn hình (system audio + mic + webcam/PiP) qua FFmpeg.
- Biên tập: trim/cut, Save Frame, export **GIF** + MP4.

## P11 — Templates / Create 🟥
- Create Image from Template, Combine Images, Video from Images.

## P12 — Polish 🟥
- Themes (light/dark), brand colors/Quick Styles nâng cao, tùy biến hotkey, Preferences đầy đủ.

---

### Thứ tự ưu tiên đề xuất
**MVP dùng được** = P0→P6 (chụp ảnh, chú thích, lưu, chia sẻ). Đây là 80% giá trị hằng ngày của Snagit.
Các phase OCR/Simplify/Video/Template là mở rộng nâng cao, làm sau khi xương sống ổn.
