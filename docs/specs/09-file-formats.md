# SPEC 09 — File Formats (Ezsnagit)

> Module: `ezsnag_canvas`, `ezsnag_library`

## Mục tiêu & phạm vi

Định nghĩa các định dạng Ezsnagit đọc/ghi, trọng tâm là **`.ezsnagx`** — định dạng native riêng của Ezsnagit giữ **object vector** để re-edit — và **rule flatten** khi export sang raster/PDF/video. Insight cốt lõi: chỉ `.ezsnagx` đảm bảo edit lại được; mọi export khác là **phẳng** (baked-in pixel).

---

## Tính năng

### 09.1 `.ezsnagx` — native editable format

- **Mô tả:** định dạng native của Ezsnagit, **giữ toàn bộ trạng thái editable**. Cấu trúc logic:
  - **Ảnh nền (background raster)** — ảnh chụp gốc.
  - **Danh sách object vector** — text, callout, stamp, shape, line, highlight, step number, cursor, blur/magnify, **pasted image**… mỗi object lưu *kiểu + hình học + style + z-order*, **không nung vào pixel** → vẫn chỉnh/di chuyển/đổi z-order/xóa được.
  - **Metadata** — kích thước canvas, ngày tạo/sửa, source app/website (nếu có), thông tin template (nếu là asset ghép), danh sách tag/flag tham chiếu.
  - Đây là **định dạng duy nhất** đảm bảo re-edit sau này; Library auto-save dạng này (SPEC 06).
- **Ưu tiên:** P0 (nền tảng — không có nó thì Library/editor vô nghĩa).
- **Ghi chú implement C++/Qt (.ezsnagx store, serialize):**
  - Container đề xuất: **ZIP** (qua một zip lib, vd minizip/QuaZip) chứa:
    - `document.json` — canvas metadata + mảng object (type, geometry, style, z-index).
    - `background.png` (hoặc raster gốc, ưu tiên PNG lossless).
    - `assets/` — ảnh paste/stamp raster đi kèm (mỗi cái 1 file, object tham chiếu bằng id).
    - `thumbnail.png` — preview cho Tray/Library.
  - `ezsnag_canvas` chịu trách nhiệm **serialize/deserialize** object graph ↔ `document.json`; mỗi annotation type tự (de)serialize style của nó (`ezsnag_annotations`).
  - Ghi atomic: build vào temp → `QSaveFile` rename (đồng bộ với auto-save 06.1).
  - **Cross-platform:** JSON + PNG + ZIP đều portable → file tạo trên Win mở được trên Mac (`.ezsnagx` cross-platform theo thiết kế). Version field trong `document.json` để migrate.

### 09.2 Image formats — import & export

- **Mô tả (lọc theo khả thi Qt):**
  - **Import:** PNG, JPG, BMP, GIF, TIF, ICO (+ `.ezsnagx`). Các định dạng hiếm (RAS, EMF, PSD, TGA, WFX, WMF, WPG) → tùy plugin/thư viện, không bắt buộc bản đầu.
  - **Export:** PNG, JPG, BMP, GIF, TIF (raster) + PDF (export-only, hỗ trợ multi-page từ nhiều capture).
  - **Transparency:** hỗ trợ trên **PNG, GIF, TIF**; **không** trên JPG/BMP.
- **Ưu tiên:** P0 (PNG/JPG import+export); P1 (BMP/GIF/TIF, PDF); P2 (định dạng hiếm).
- **Ghi chú implement C++/Qt:**
  - Raster dùng `QImage`/`QImageWriter` (PNG/JPG/BMP/GIF/TIF có sẵn qua Qt image plugins).
  - PDF qua `QPdfWriter`/`QPainter`; multi-page = nhiều capture → nhiều `newPage()`.
  - Khi export sang JPG/BMP (không alpha) mà document có vùng trong suốt → flatten lên nền (mặc định trắng), cảnh báo nếu cần.

### 09.3 Video format

- **Mô tả:** **`.mp4`** (H.264) — định dạng video duy nhất; video capture chưa lưu auto-save dạng `.mp4` trong Library (SPEC 06). GIF animated là output raster động (từ video, xem SPEC 05).
- **Ưu tiên:** P0 cho video core (qua `ezsnag_video`/FFmpeg).
- **Ghi chú implement C++/Qt:** encode FFmpeg (H.264). Không có định dạng video native editable — video là output phẳng.

### 09.4 Rule flatten (quan trọng)

- **Mô tả:**
  - Export/share sang **PNG, JPG, TIFF, GIF, BMP, PDF** → **flatten**: mọi annotation/object thành pixel nung sẵn, **không còn editable**.
  - (Tham chiếu hành vi Mac trên một số công cụ tương tự) khi quit, file **non-`.ezsnagx`** đang mở bị flatten, annotation cũ không sửa được nữa.
  - **Best practice:** hoàn tất mọi edit ở `.ezsnagx` trước; giữ một bản `.ezsnagx` nếu có thể cần edit lại; duplicate trước khi edit lớn; dùng định dạng hỗ trợ trong suốt (PNG/GIF/TIF) nếu phải export ngoài `.ezsnagx`.
- **Ưu tiên:** P0 (flatten là bước bắt buộc của mọi export raster).
- **Ghi chú implement C++/Qt (rule flatten khi export PNG/JPG):**
  1. Lấy `QImage` nền (background raster) ở kích thước canvas đầy đủ.
  2. `QPainter` lên QImage đó, render lần lượt mọi object theo **z-order** tăng dần (gọi `paint()` của từng annotation/effect — cùng pipeline render màn hình nhưng vẽ ra QImage thay vì widget).
  3. Áp effect raster (border/shadow/filter…) theo đúng thứ tự.
  4. Kết quả là một QImage **phẳng** → `QImageWriter` ghi PNG/JPG/…
  5. **Không** chạm vào file `.ezsnagx` gốc trong Library → bản editable vẫn còn; export chỉ là **copy phẳng** ra đích.
  - JPG/BMP: nếu có alpha, composite lên nền đặc trước khi ghi.
  - Flatten là hàm thuần (`flatten(document) -> QImage`) trong `ezsnag_canvas`, test headless được (không cần widget).

---

## Bảng ưu tiên

| ID | Tính năng | Ưu tiên |
|---|---|---|
| 09.1 | `.ezsnagx` native (ZIP: json + bg + assets + thumb) | P0 |
| 09.2 | Import/Export PNG, JPG | P0 |
| 09.2 | BMP/GIF/TIF + PDF (multi-page) | P1 |
| 09.2 | Định dạng hiếm (PSD/TGA/EMF/WMF…) | P2 |
| 09.3 | `.mp4` (H.264) | P0 (qua video) |
| 09.4 | Flatten pipeline khi export raster/PDF | P0 |

## Điểm chưa chắc

- **Container `.ezsnagx`**: ZIP(json+png) đề xuất ở đây vs binary đơn — ZIP dễ debug/portable, chọn ZIP; cần chốt zip lib (QuaZip/minizip).
- Ezsnagit chỉ làm **một** định dạng native (`.ezsnagx`), không có legacy format.
- Danh sách định dạng **import hiếm** (RAS/EMF/PSD/TGA/WFX/WMF/WPG) phụ thuộc thư viện ngoài — chưa cam kết.
- **PDF multi-page**: từ nhiều capture trong Library — UX chọn nhiều rồi "Export as PDF" cần định nghĩa (liên quan SPEC 06/07).
- Hành vi **flatten-on-quit** kiểu Mac có nên áp dụng không, hay luôn giữ `.ezsnagx` editable bất kể nền tảng — đề xuất **luôn giữ editable**, không áp dụng flatten-on-quit (an toàn hơn cho user).
- Có lưu **effect raster** (border/shadow) dạng editable param trong `.ezsnagx` hay nung sẵn — nên lưu param để re-edit (đồng bộ `ezsnag_effects`).
