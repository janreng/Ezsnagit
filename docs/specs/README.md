# Ezsnagit — Index Spec

Bộ tài liệu thiết kế Ezsnagit — công cụ chụp & annotate màn hình bằng C++/Qt6. Đọc `00` trước (kiến trúc & bản đồ module).

| # | Spec | Module C++ | UX flow |
|---|---|---|---|
| 00 | [Tổng quan & Kiến trúc](00-overview-architecture.md) | (tất cả) | — |
| 01 | [Capture](01-capture.md) | `ezsnag_capture`, `ui/CaptureWindow` | [flow](ux-flows/01-capture-flow.md) |
| 02 | [Editor & Annotation](02-editor-annotation.md) | `ezsnag_canvas`, `ezsnag_annotations` | [flow](ux-flows/02-editor-annotation-flow.md) |
| 03 | [Effects & Transforms](03-effects-transforms.md) | `ezsnag_effects` | [flow](ux-flows/03-effects-transforms-flow.md) |
| 04 | [Simplify & Grab Text (OCR)](04-simplify-grabtext.md) | `ezsnag_simplify`, `ezsnag_ocr` | [flow](ux-flows/04-simplify-grabtext-flow.md) |
| 05 | [Video](05-video.md) | `ezsnag_video` | [flow](ux-flows/05-video-flow.md) |
| 06 | [Library](06-library.md) | `ezsnag_library` | [flow](ux-flows/06-library-flow.md) |
| 07 | [Share & Outputs](07-share-outputs.md) | `ezsnag_share` | [flow](ux-flows/06-library-share-templates-flow.md) |
| 08 | [Templates / Create](08-templates.md) | `ezsnag_templates` | [flow](ux-flows/06-library-share-templates-flow.md) |
| 09 | [File Formats (.ezsnagx)](09-file-formats.md) | `ezsnag_canvas`, `ezsnag_library` | — |
| 10 | [Preferences & Hotkeys](10-preferences-hotkeys.md) | `ui` | — |
| 11 | [Nền tảng & Phạm vi](11-platform-editions.md) | — | — |
| 12 | [Non-Functional Requirements & Định vị](12-non-functional-requirements.md) ⭐ | (xuyên suốt) | — |

> **MVP định nghĩa theo workflow** (bug-report loop + doc-steps loop): kéo hotkey/Quick Styles/Repeat-Last
> lên MVP; DPI/đa màn hình + Library backup là nền bắt buộc; Smart Move/inpaint gate sau spike. Xem `ROADMAP.md`.

**Quy ước:** spec viết tiếng Việt có dấu, giữ tên feature + thuật ngữ kỹ thuật tiếng Anh. Mỗi feature ghi
Mô tả / Tùy chọn / Ưu tiên (theo phase roadmap) / Ghi chú kỹ thuật C++/Qt. Mục "Điểm chưa chắc" = cần verify trên app thật.
