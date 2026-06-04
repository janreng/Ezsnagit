# SPEC 00 — Tổng quan & Kiến trúc (Ezsnagit)

Tài liệu nền: kiến trúc thông tin (IA) của app + đề xuất kiến trúc module C++/Qt6 để clone Snagit.
Các spec 01–11 mô tả chi tiết từng cụm tính năng và tham chiếu các module định nghĩa ở đây.

---

## 1. Information Architecture (3 bề mặt)

```
                         ┌────────────────────┐
                         │   Capture Window   │  (luôn thường trú, gọi bằng hotkey)
                         │  Image│Video│Scroll │
                         └─────────┬──────────┘
                       chụp xong → │ tạo capture
                                   ▼
   ┌──────────────────────────────────────────────────────────┐
   │                         Editor                            │
   │  Toolbar (annotation) · Canvas · Tool Properties · Effects│
   │  Recent Captures Tray (dưới) · Share toolbar (trên/phải)  │
   └─────────┬───────────────────────────────────┬────────────┘
             │ tự lưu .ezsnagx                    │ Share
             ▼                                    ▼
     ┌───────────────┐                   ┌──────────────────┐
     │   Library     │                   │  Outputs/Share   │
     │ search/tag/   │                   │ File·Clipboard·  │
     │ filter/recent │                   │ Email·Cloud·...  │
     └───────────────┘                   └──────────────────┘
```

- **Capture Window**: bề mặt khởi động chụp. Tối giản, gọi nhanh bằng phím tắt toàn cục.
- **Editor**: trung tâm — nơi người dùng ở lâu nhất. Canvas + annotation + effect + share.
- **Library**: kho mọi capture đã tạo (tự lưu), tìm kiếm và quản lý.

---

## 2. Kiến trúc module C++ (static lib per module — mô hình Ezcel)

Mỗi module là một static lib Qt, **logic thuần tách khỏi GUI** để test headless. Glue Qt/Widgets mỏng ở lớp `ui`.

| Module (lib) | Trách nhiệm | Phụ thuộc chính |
|---|---|---|
| `ezsnag_capture` | Engine chụp màn hình: region/window/fullscreen/scrolling/panoramic; lớp platform | Qt Gui; Win: Windows.Graphics.Capture / DXGI / GDI |
| `ezsnag_canvas` | Document model của editor: đồ thị **object vector**, z-order, undo/redo, serialize `.ezsnagx` | Qt Gui |
| `ezsnag_annotations` | Các loại annotation object + render: Arrow, Text, Callout, Shape, Line, Highlight, Step, Stamp, Pen, Blur, Magnify… | `ezsnag_canvas`, Qt Gui |
| `ezsnag_effects` | Effect raster + transform: Border, Shadow, Edge, Perspective, Color filter, Crop, Trim, Cut Out, Resize, Rotate, Flip, Watermark, Spotlight | Qt Gui |
| `ezsnag_simplify` | Simplify/SUI + Smart Move: phát hiện phần tử UI, thay bằng shape đơn giản | OpenCV |
| `ezsnag_ocr` | Grab Text: OCR ảnh → text | Tesseract |
| `ezsnag_video` | Quay video + biên tập nhẹ (trim/cut), Save Frame, export GIF/MP4 | FFmpeg, Qt Multimedia |
| `ezsnag_library` | Kho capture: DB, khay Recent, tag/flag, store `.ezsnagx` | SQLite, Qt Core |
| `ezsnag_share` | Đích xuất/chia sẻ: File, Clipboard, Email, Cloud (plugin) | Qt Network |
| `ezsnag_templates` | Create from Template / Combine Images / Video from Images | `ezsnag_canvas` |
| **App GUI** (`ui`) | `CaptureWindow`, `EditorWindow`, `LibraryView`, ToolPropertiesPanel, HotkeyManager, ShareToolbar | tất cả lib trên + Qt Widgets |

### Mô hình dữ liệu cốt lõi: Canvas Object Graph

- Mỗi annotation là một **object vector** (không nung vào pixel) → re-edit, di chuyển, đổi z-order.
- Background = raster (ảnh chụp). Annotation objects nằm trên các layer phía trên.
- `.ezsnagx` = ảnh nền + danh sách object (kiểu, hình học, style) + metadata → mở lại edit tiếp.
- Export PNG/JPG = **flatten** toàn bộ object xuống raster (mất khả năng edit, giống Snagit).
- Undo/redo trên thao tác object (thêm/sửa/xóa/di chuyển/đổi style) — mô hình giống SpreadsheetModel của Ezcel.

---

## 3. Bản đồ Spec ↔ Module

| Spec | Nội dung | Module |
|---|---|---|
| 01-capture | Mọi chế độ chụp, capture window, on-screen toolbar, hotkey | `ezsnag_capture`, `ui/CaptureWindow` |
| 02-editor-annotation | Canvas, layout editor, toàn bộ annotation tool, quick styles | `ezsnag_canvas`, `ezsnag_annotations`, `ui/EditorWindow` |
| 03-effects-transforms | Effects + biến đổi canvas/ảnh | `ezsnag_effects` |
| 04-simplify-grabtext | Simplify/SUI, Smart Move, Grab Text OCR | `ezsnag_simplify`, `ezsnag_ocr` |
| 05-video | Quay + biên tập video, GIF, PiP | `ezsnag_video` |
| 06-library | Library, Recent tray, tag/flag, auto-save | `ezsnag_library` |
| 07-share-outputs | Đích chia sẻ, custom output | `ezsnag_share` |
| 08-templates | Create from Template / Combine / Video from Images | `ezsnag_templates` |
| 09-file-formats | `.ezsnagx` + export raster/video | `ezsnag_canvas`, `ezsnag_library` |
| 10-preferences-hotkeys | Preferences, theme, hotkey customization | `ui` |
| 11-platform-editions | Khác biệt nền tảng, phạm vi clone | — |

---

## 3b. Technical precedents (study — KHÔNG copy code)

Các project có sẵn giải đúng những cơ chế Ezsnagit cần. Tham khảo pattern, **không copy mã** (lưu ý GPL):

- **Flameshot** (C++/Qt, GPL) — *tiền lệ kỹ thuật quan trọng nhất*, cùng stack. Học: overlay fullscreen trong suốt để chọn vùng, xử lý **đa màn hình**, vẽ annotation bằng QPainter/Qt, global hotkey, system tray. (research 07 §4)
- **ShareX** (C#, GPL) — học hành vi **stitch scrolling** và **after-capture task chain / upload pipeline** (kiến trúc destination pluggable). (research 07 §1)
- **Greenshot** (C#, GPL) — UX **destination picker** nhẹ.

⚠️ GPL: chỉ đọc để hiểu cách tiếp cận; Ezsnagit tự viết. Không nhúng code GPL.

## 4. Ranh giới clone (điều chỉnh kỳ vọng)

- **Làm tới**: capture ảnh + editor annotation + effects + library + share file/clipboard + OCR + scrolling + video trim/GIF cơ bản.
- **Tạm gác / khó**: tích hợp cloud bản quyền (Screencast.com, Slack/Teams OAuth), Smart Move/Simplify mức AI cao, multitrack video (đó là Camtasia).
- **Khác Snagit có chủ đích**: tên format `.ezsnagx`; OCR dùng Tesseract (không phải ABBYY); video encode bằng FFmpeg.

Xem `ROADMAP.md` cho thứ tự triển khai P0–P12.
