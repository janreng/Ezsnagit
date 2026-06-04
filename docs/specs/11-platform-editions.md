# SPEC 11 — Nền tảng & Phạm vi (Ezsnagit)

> Module: —

## Mục tiêu

Chốt **nền tảng mục tiêu** và **ranh giới phạm vi** để khỏi sa lầy vào tính năng phụ thuộc bản quyền/nền tảng.

## 11.1 Nền tảng mục tiêu

- **Windows trước** (giống Ezcel — toolchain Qt6/MinGW ở `C:\Qt`). Lý do: capture/hotkey/loopback-audio API trên Windows rõ ràng, và máy phát triển là Windows 10.
- macOS: gác lại. Ezsnagit không cố parity 2 nền tảng giai đoạn đầu — tập trung làm tốt một nền tảng (Windows) trước.

## 11.2 Phạm vi tính năng (in-scope)

Ezsnagit nhắm một **superset hữu ích** các tính năng capture/annotate trên Windows, theo roadmap P0–P13:

- Capture ảnh (region/window/fullscreen/scrolling).
- Editor annotation đầy đủ, effects + transforms.
- Library + auto-save.
- Share file/clipboard/email.
- Grab Text OCR.
- Video trim/cut + GIF.
- Templates cơ bản.

## 11.3 Ngoài phạm vi / gác lại

- Tích hợp cloud có bản quyền & OAuth (các dịch vụ chia sẻ/đám mây bên thứ ba): gác lại.
- Smart Move / Simplify mức "AI" cao — chỉ làm bản OpenCV cơ bản.
- Video multitrack, hiệu ứng chuyển cảnh, narration nâng cao (thuộc phạm trù dựng video chuyên dụng, không phải chụp & annotate).
- Đồng bộ tài khoản online, marketplace asset.

## 11.4 Lựa chọn kỹ thuật cốt lõi

- Format gốc: **`.ezsnagx`**.
- OCR: **Tesseract**.
- Video/GIF: **FFmpeg**.
- Xử lý ảnh (Simplify/inpaint cơ bản): **OpenCV**.
- Library DB: **SQLite**.
- Tên & thương hiệu: **Ezsnagit** (đồng bộ họ "Ez*" với Ezcel).

## 11.5 Yêu cầu hệ thống (mục tiêu)

- Windows 10/11 64-bit. Qt6.8+, MinGW. Phụ thuộc runtime: Tesseract, OpenCV, FFmpeg (đóng gói kèm hoặc tải khi cài).

## Điểm chưa chắc
- Một số tính năng nâng cao (Smart Move, Simplify palette, plain-vs-formatted OCR) cần spike kỹ thuật trước khi chốt phạm vi chính xác.
- Target đóng gói component pack (tải khi cần vs bundle) sẽ điều chỉnh theo dung lượng thực tế khi build.
