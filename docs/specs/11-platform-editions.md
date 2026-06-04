# SPEC 11 — Platform & Phạm vi clone (Ezsnagit)

> Nguồn: docs/research/05-library-share-templates.md (§6), tổng hợp Win/Mac diff các spec · Module: —

## Mục tiêu

Chốt **nền tảng mục tiêu** và **ranh giới clone** để khỏi sa lầy vào tính năng phụ thuộc bản quyền/nền tảng.

## 11.1 Nền tảng mục tiêu

- **Windows trước** (giống Ezcel — toolchain Qt6/MinGW ở `C:\Qt`). Lý do: capture/hotkey/loopback-audio API trên Windows rõ ràng, và máy phát triển là Windows 10.
- macOS: gác lại. Snagit có khác biệt Win/Mac đáng kể (xem dưới); clone không cố parity 2 nền tảng giai đoạn đầu.

## 11.2 Khác biệt Snagit Windows vs Mac (tham khảo khi clone)

| Khu vực | Chỉ Windows | Chỉ Mac |
|---|---|---|
| Capture | Multiple Area, Time-lapse/Interval, Freehand, Printer capture | Webcam still-image, Mission Control capture |
| Effects | Page Curl, Color Replacement, Watermark, Spotlight & Magnify (effect), Lock Objects (Simplify) | Reflection, Fade, "Add Horizontal/Vertical Space" (Cut Out đảo) |
| Tools | Spotlight = Selection + effect | Spotlight = tool riêng |
| Video | (Remove audio: workaround) | Mirror camera, virtual background, "Make a Silent Copy", GIF Dynamic Colors |
| Share | Email, Clipboard, Program, Printer, OneDrive | Outlook, Pages/Keynote, macOS share-sheet |

→ **Chiến lược clone:** Ezsnagit gộp **superset hữu ích** trên một nền tảng (Windows), bỏ các mục trùng lặp/ít giá trị.

## 11.3 Phạm vi clone (in-scope vs gác lại)

**Làm (theo roadmap P0–P12):**
- Capture ảnh (region/window/fullscreen/scrolling), editor annotation đầy đủ, effects + transforms, library + auto-save, share file/clipboard/email, Grab Text OCR, video trim/cut + GIF, templates cơ bản.

**Gác lại / ngoài phạm vi:**
- Tích hợp cloud có bản quyền & OAuth: Screencast.com, Slack, Teams, Google Drive/Dropbox/OneDrive, YouTube.
- Smart Move / Simplify mức "AI" cao (chỉ làm bản OpenCV cơ bản).
- Video multitrack, hiệu ứng chuyển cảnh, narration nâng cao (đó là **Camtasia**, không phải Snagit).
- Đồng bộ tài khoản TechSmith, Assets marketplace.

## 11.4 Khác biệt có chủ đích với Snagit

- Format gốc: **`.ezsnagx`** (không phải `.snagx`).
- OCR: **Tesseract** (không phải ABBYY).
- Video/GIF: **FFmpeg**.
- Library DB: **SQLite**.
- Tên & thương hiệu: **Ezsnagit** (đồng bộ họ "Ez*" với Ezcel).

## 11.5 Yêu cầu hệ thống (mục tiêu)

- Windows 10/11 64-bit. Qt6.8+, MinGW. Phụ thuộc runtime: Tesseract, OpenCV, FFmpeg (đóng gói kèm hoặc tải khi cài).

## Điểm chưa chắc
- Số phiên bản "mới nhất" của Snagit dùng đánh số theo năm (2025/2026) — không có 1 con số cố định; tính năng tham chiếu theo dòng 2025–2026.
- Một số tính năng version-gated (Smart Move, Simplify palette, plain-vs-formatted OCR) xuất hiện ở bản mới; clone không cần bám đúng mốc version.
