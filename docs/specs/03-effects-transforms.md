# SPEC 03 — Effects & Transforms (Ezsnagit)

> Module: `ezsnag_effects`

## Mục tiêu & phạm vi

Module `ezsnag_effects` chịu trách nhiệm cho hai nhóm thao tác **raster trên toàn ảnh** (whole-canvas) của Editor:

1. **Effects** — tăng cường/trang trí ảnh: Border, Edges, Shadow, Perspective, Page Curl, Color Adjustment, Filters, Color Replacement, Watermark, Spotlight & Magnify (effect), Capture Info.
2. **Transforms** — biến đổi canvas/ảnh: Crop, Trim/Auto-Trim, Cut Out, Resize Image, Resize/Expand Canvas, Rotate, Flip, Canvas Color.

Phạm vi & ranh giới quan trọng (bám research):

- **Effects chỉ áp cho ảnh** (image-only). Effect chỉ tồn tại ở phía ảnh, không áp lên video object; chỉnh sửa video nặng thuộc về trình biên tập video chuyên dụng.
- **Effects áp lên toàn canvas đã flatten**, KHÔNG áp riêng từng annotation object (trừ trường hợp selection-scoped — xem dưới). Đây là khác biệt với annotation (object vector, thuộc `ezsnag_annotations`).
- **Phân biệt kiến trúc Magnify vs Spotlight&Magnify:** `Magnify` là một **annotation object sống** (thuộc `ezsnag_annotations`, không nằm trong spec này); còn `Spotlight & Magnify` là **baked effect/region operation** (thuộc spec này). Hai thứ giống nhau về thị giác nhưng khác bản chất.
- Ezsnagit là app một nền tảng (C++/Qt6, Windows-first) → ta **gộp tập tính năng đầy đủ nhất** và đánh dấu rõ mức độ chắc chắn của từng tính năng.

### Mô hình áp dụng effect

- Effect là phép biến đổi raster **non-destructive ở mức lịch sử undo**: lưu ảnh trước khi apply để Undo khôi phục. Trong Ezsnagit, undo/redo đi qua `ezsnag_canvas`.
- **Quick Effects:** lưu tổ hợp nhiều effect + tham số thành một preset, áp lại bằng 1 click. Lưu vào preferences (JSON).
- **Selection-scoped effect (tùy chọn):** nếu có vùng selection đang active, effect chỉ áp lên vùng đó (mask theo `QRegion`). MVP có thể làm sau; mặc định áp toàn canvas.
- Pipeline kỹ thuật: `QImage` nguồn → hàm filter thuần (không phụ thuộc Widgets, test headless được) → `QImage` đích. Lớp glue Qt ở `ui` chỉ gọi hàm + hiển thị preview.

---

## Tính năng

### 03.1 Border
- **Mô tả:** vẽ viền đặc bao quanh toàn ảnh (hoặc vùng selection).
- **Tùy chọn:** màu (swatch preset / palette / eyedropper / hex/RGB), độ rộng (width slider, px).
- **Ưu tiên:** **⭐MVP P4.**
- **Ghi chú implement C++/Qt:** `QPainter::drawRect` viền theo `QPen(color, width)` quanh biên ảnh; hoặc cấp phát `QImage` lớn hơn `2*width` mỗi chiều rồi fill nền màu border + vẽ ảnh gốc vào giữa (giữ pixel gốc nguyên vẹn). Cách thứ hai chuẩn hơn nếu border nằm ngoài ảnh.

### 03.2 Edges (edge effects)
- **Mô tả:** áp kiểu cạnh trang trí lên một hoặc nhiều cạnh ảnh.
- **Tùy chọn:** style ∈ { **drop shadow**, **torn**, **faded**, **wave**, **saw**, **shark tooth**, **beveled** }; chọn cạnh nào áp (top/bottom/left/right); **size** của cạnh; **shadow color & location**.
- **Ưu tiên:** MVP P4 (drop shadow + torn + faded); P5 (wave/saw/shark tooth/beveled).
- **Ghi chú implement C++/Qt:**
  - **drop shadow:** blur một mask offset (xem 03.3).
  - **torn / saw / shark tooth / wave:** sinh một **đường biên procedural** (random/răng cưa/sin) rồi dùng làm alpha mask cắt cạnh ảnh — `QPainterPath` + `setCompositionMode(QPainter::CompositionMode_DestinationIn)`.
  - **faded:** `QLinearGradient` alpha từ đặc → trong suốt ở cạnh.
  - **beveled:** vẽ highlight/shadow chéo ở rìa (gradient sáng-tối) để giả khối 3D.

### 03.3 Shadow
- **Mô tả:** đổ bóng quanh ảnh ở các cạnh đã chọn hoặc toàn bộ.
- **Tùy chọn:** grid selector chọn hướng + size; **opacity / offset / blur**. Ezsnagit gộp đủ: hướng, size, opacity, offset, blur.
- **Ưu tiên:** **⭐MVP P4.**
- **Ghi chú implement C++/Qt:** tạo silhouette ảnh (alpha), tô đen theo opacity, dịch theo offset, làm mờ bằng blur (separable Gaussian hoặc `QGraphicsBlurEffect` render xuống `QImage`), composite phía sau ảnh gốc trên canvas mở rộng.

### 03.4 Perspective
- **Mô tả:** méo ảnh để giả góc nhìn/nghiêng.
- **Tùy chọn:** slider ngang/xoay (horizontal/rotation) và slider dọc/nghiêng (vertical/tilt).
- **Ưu tiên:** P5.
- **Ghi chú implement C++/Qt:** `QTransform` với hệ số projective (set m13/m23) hoặc `QTransform::quadToQuad` ánh xạ 4 góc ảnh sang tứ giác đích; render qua `QPainter` đã bật `SmoothPixmapTransform`. Canvas cần mở rộng để chứa hình méo.

### 03.5 Page Curl
- **Mô tả:** cuộn một góc ảnh tạo hiệu ứng "trang đang lật" 3D.
- **Tùy chọn:** preset đơn giản (chọn góc) + Apply.
- **Ưu tiên:** P6 (nice-to-have, ít dùng).
- **Ghi chú implement C++/Qt:** render góc theo mesh warp + gradient sáng/tối mô phỏng mặt cong; có thể xấp xỉ bằng `QTransform` + overlay gradient hình tam giác. Đây là effect khó làm đẹp → để sau.

### 03.6 Reflection
- **Mô tả:** thêm phản chiếu kiểu gương bên dưới ảnh.
- **Tùy chọn:** reflection **size %**, **opacity**.
- **Ưu tiên:** P6 (tùy chọn).
- **Ghi chú implement C++/Qt:** flip dọc một dải dưới ảnh + `QLinearGradient` alpha mờ dần. Đơn giản về kỹ thuật → ưu tiên thấp.

### 03.7 Fade
- **Mô tả:** làm nhòa/mờ dần các cạnh ảnh bằng gradient mượt.
- **Tùy chọn:** vùng/độ fade.
- **Ưu tiên:** P6 (tùy chọn).
- **Ghi chú implement C++/Qt:** alpha mask = `QRadialGradient`/`QLinearGradient` áp DestinationIn ở rìa.

### 03.8 Capture Info
- **Mô tả:** in metadata chụp lên ảnh: tên app, version, ngày/giờ, OS, URL, kích thước (dimensions), + ô text tùy chỉnh.
- **Tùy chọn:** màu nền, thuộc tính font.
- **Ưu tiên:** **⭐MVP P4.**
- **Core của bug-report workflow:** đây là tính năng "ký" lên capture các thông tin timestamp / app / url / dimensions — nằm trên critical path của luồng báo lỗi (workflow #2). Vì vậy kéo lên MVP, không để P5.
- **Ghi chú implement C++/Qt:** đọc metadata từ capture (`ezsnag_capture` lưu vào canvas metadata), `QPainter::drawText` vào dải nền ở cạnh ảnh. Lưu ý giờ hệ thống chạy PST → khi format timestamp cần đúng định dạng người dùng mong đợi.

### 03.9 Color Adjustment
- **Mô tả:** điều chỉnh tông màu theo kênh.
- **Tùy chọn:** **Brightness**, **Contrast**, **Hue**, **Saturation**, **Gamma** (5 slider).
- **Ưu tiên:** MVP P4.
- **Ghi chú implement C++/Qt:** duyệt pixel `QImage` (format `ARGB32`):
  - brightness/contrast: phép tuyến tính `out = (in - 0.5)*contrast + 0.5 + bright`.
  - hue/saturation: chuyển RGB→HSL (hoặc `QColor::toHsl`) → dịch H, scale S → về RGB.
  - gamma: LUT 256 phần tử `out = 255*(in/255)^(1/gamma)`.
  - Dùng LUT + xử lý theo scanline để nhanh; cân nhắc OpenMP cho ảnh lớn.

### 03.10 Filters (Color Filters)
- **Mô tả:** bộ lọc dựng sẵn.
- **Tùy chọn (gộp):** **soften**, **sharpen**, **opacity**, **monochrome**, **grayscale**, **invert colors**, **sepia**.
- **Ưu tiên:** MVP P4 (grayscale/invert/opacity/sharpen/soften); P5 (monochrome/sepia).
- **Ghi chú implement C++/Qt:**
  - grayscale: luma `0.299R+0.587G+0.114B`.
  - invert: `255 - c`.
  - opacity: scale kênh alpha.
  - sharpen/soften: convolution 3×3 (unsharp / box-blur) — separable nếu được.
  - sepia: ma trận màu sepia chuẩn.
  - monochrome: grayscale rồi tô theo 1 màu chủ đạo.
- **Điểm chưa chắc:** danh sách sub-option có thể mở rộng; coi list trên là đại diện, không phải bất biến.

### 03.11 Color Replacement
- **Mô tả:** thay một hoặc nhiều màu cụ thể bằng màu khác (recolor UI / đổi brand color).
- **Tùy chọn:** danh sách **color pair** (source → replacement); thêm nhiều cặp trước khi Apply; nên có **tolerance** quanh màu nguồn.
- **Ưu tiên:** P5.
- **Ghi chú implement C++/Qt:** với mỗi pixel, nếu khoảng cách màu tới một `source` ≤ tolerance thì thay bằng `replacement` (giữ độ sáng tương đối nếu muốn mượt). Eyedropper để chọn source từ canvas.

### 03.12 Watermark
- **Mô tả:** phủ text, logo, hoặc copyright lên ảnh.
- **Tùy chọn:** nguồn = **image (logo)** hoặc **text**; **position grid** (9 ô); **scale** slider; mode hiển thị **Underlay** (chìm/emboss vào nền) vs **Overlay** (đè lên trên).
- **Ưu tiên:** P5.
- **Ghi chú implement C++/Qt:** load logo `QImage` (giữ alpha), scale theo slider, đặt theo grid (tính toạ độ 9 vị trí). **Overlay:** composite `SourceOver` với opacity. **Underlay:** chuyển logo sang grayscale/emboss + blend mode mô phỏng dập chìm. Text watermark: render `QPainter::drawText` với font/opacity.

### 03.13 Spotlight & Magnify (effect)
- **Mô tả:** **baked effect** làm nổi một vùng đã chọn: làm mờ/tối nền xung quanh (dim/blur background) + **tùy chọn** phóng to vùng (magnify region) trong một thao tác gộp.
- **Đây là canonical home của "Spotlight & Magnify".** Mục này là nơi đặc tả chính thức của effect dim/blur-nền (+ magnify tùy chọn). Phân biệt rõ với object sống ở SPEC 02:
  - **Magnify (live object)** → SPEC 02 §02.16 — annotation re-edit được, sống trong canvas.
  - **Spotlight & Magnify (baked effect)** → mục này (SPEC 03 §03.13) — phép raster nung vào ảnh.
  - **Editor KHÔNG được surface hai control "magnify" gây nhầm lẫn cạnh nhau:** một control là live-object Magnify (SPEC 02), control kia là baked Spotlight&Magnify effect (ở đây). Đặt chúng ở hai chỗ rõ ràng (toolbar annotation vs panel Effects), label khác nhau.
- **Tùy chọn:** chọn vùng bằng Selection tool trước; thuộc tính: **magnify on/off** + mức magnify, độ tối/mờ nền (dim opacity + blur).
- **Ưu tiên:** P5.
- **Ghi chú implement C++/Qt:** baked operation: (1) làm tối/mờ toàn ảnh (overlay đen opacity + blur), (2) nếu magnify on: vẽ lại vùng selection đã scale-up vào vị trí (clip theo shape). Khác với annotation Magnify (object sống, thuộc `ezsnag_annotations`).
- **Quyết định (đã chốt):** ranh giới tool-vs-effect KHÔNG còn deferred. Spotlight&Magnify là **baked effect** và sống ở mục này; live Magnify object là SPEC 02 §02.16. Hai thứ tồn tại song song có chủ đích (xem SPEC 02 §6.5). Effect có cờ "magnify on/off" để bao cả trường hợp chỉ Spotlight (không phóng to).

---

### Transforms

### 03.14 Crop
- **Mô tả:** xén bỏ phần rìa ngoài vùng chọn.
- **Tùy chọn:** kéo **blue handles** đặt vùng giữ; kéo selection để dời; click **Crop** để xóa ngoài vùng. Circle/round crop qua Selection tool + kéo **yellow diamond** bo góc (avatar tròn).
- **Ưu tiên:** **⭐MVP P4.**
- **Ghi chú implement C++/Qt:** `QImage::copy(QRect)` theo vùng giữ. Round crop: tạo alpha mask `QPainterPath` (rounded rect/ellipse) áp DestinationIn rồi crop bbox. Canvas thu nhỏ về kích thước vùng giữ.

### 03.15 Trim / Auto-Trim
- **Mô tả:** tự động bỏ padding/whitespace ở rìa (phát hiện viền đồng màu).
- **Tùy chọn:** tolerance màu; (tùy chọn) chọn cạnh nào trim.
- **Ưu tiên:** **⭐MVP P4** (đặc trưng hữu ích).
- **Ghi chú implement C++/Qt (auto-detect viền):** lấy màu góc (corner color) làm tham chiếu; **quét từ mỗi cạnh vào trong**, dừng tại scanline/cột đầu tiên có pixel lệch màu góc vượt tolerance; ghép thành bounding box rồi crop về box đó. Cho phép chọn corner tham chiếu (mặc định top-left) và tolerance.
- **Điểm chưa chắc:** entry point UI chưa chốt. Ta đặt menu `Image > Trim` + nút Auto-Trim.

### 03.16 Cut Out
- **Mô tả:** xóa một **dải ngang full-width** hoặc **dải dọc full-height** rồi ghép hai phần còn lại — bỏ khoảng trống lớn trong ảnh dài mà vẫn giữ hai đầu.
- **Tùy chọn:** hướng **Horizontal / Vertical**; **Quick Style** quy định hướng + kiểu mép; click-drag để định dải cần bỏ.
- **Kiểu mép:**
  - **Straight edges:** ghép **liền mạch** không thấy đường nối.
  - **Decorative edges:** chừa một **gap nhỏ + đường xé/cắt** giữa hai phần để báo hiệu đã cắt nội dung.
- **Add Horizontal/Vertical Space:** *mở rộng* ảnh thêm số px (đảo ngược của cut); áp lại Horizontal/Vertical trong gap có sẵn sẽ thu nhỏ gap.
- **Ưu tiên:** MVP P4 (straight stitch); P5 (decorative torn line + add space).
- **Ghi chú implement C++/Qt (ghép dải):** với cut ngang dải `[y1,y2]`: tạo ảnh mới cao `H-(y2-y1)`, copy phần trên `[0,y1)` rồi nối phần dưới `[y2,H)` ngay dưới (paste bằng `QPainter::drawImage`). **Straight:** ghép sát. **Decorative:** chừa gap px + vẽ đường răng/xé procedural giữa hai phần. Vertical làm tương tự theo cột. Add Space: chèn dải nền (canvas color) thay vì xóa.

### 03.17 Resize Image
- **Mô tả:** đổi kích thước pixel thật của ảnh.
- **Tùy chọn:** **units** ∈ { percent, pixels, inches, centimeters }; **lock aspect ratio**; **Resolution (DPI)** + **Resampling** (resample bật có thể giảm chất lượng vĩnh viễn); **pixel density** trước khi resize.
- **Ưu tiên:** **⭐MVP P4** (pixels/percent + aspect lock); P5 (inch/cm/DPI/resample).
- **Ghi chú implement C++/Qt:** `QImage::scaled(w,h, aspectMode, Qt::SmoothTransformation)`. Inch/cm ↔ px qua DPI. DPI lưu vào `QImage::setDotsPerMeterX/Y`. Cảnh báo người dùng khi upscale (resample) làm giảm chất lượng.

### 03.18 Resize Canvas / Expand Canvas
- **Mô tả:** đổi vùng làm việc (canvas) mà **không scale pixel** — thêm khoảng quanh ảnh, hoặc thu nhỏ để xén.
- **Tùy chọn:** kéo cạnh canvas ra ngoài (thêm vùng màu canvas), hoặc dialog Canvas Size nhập kích thước + anchor.
- **Ưu tiên:** P5.
- **Ghi chú implement C++/Qt:** tạo `QImage` mới kích thước canvas, fill canvas color, vẽ ảnh gốc vào theo anchor. Giữ lịch sử non-destructive qua undo của `ezsnag_canvas`.
- **Điểm chưa chắc:** UI có thể là kéo cạnh hoặc dialog Canvas Size. Ezsnagit cấp cả hai (kéo cạnh + dialog).

### 03.19 Rotate
- **Mô tả:** xoay ảnh.
- **Tùy chọn:** preset **90° CW / 90° CCW / 180°**; **custom angle** (số nguyên độ); kéo xoay tương tác qua control circle.
- **Ưu tiên:** **⭐MVP P4** (90/180/custom).
- **Ghi chú implement C++/Qt:** 90/180 = hoán vị scanline (không nội suy, không mất chất lượng). Custom angle = `QTransform().rotate(deg)` + `QPainter` smooth; canvas mở rộng để chứa bbox xoay. Kéo xoay tương tác để sau (P5).

### 03.20 Flip
- **Mô tả:** lật ảnh.
- **Tùy chọn:** **Flip Horizontal** (trái↔phải), **Flip Vertical** (trên↔dưới).
- **Ưu tiên:** **⭐MVP P4.**
- **Ghi chú implement C++/Qt:** `QImage::mirrored(horizontal, vertical)`. **Quan trọng:** flip cũng phải **mirror cả annotation/text** trên canvas (chúng bị lật, không chỉ nền) → cần phối hợp `ezsnag_canvas` biến đổi toạ độ object, không chỉ raster nền.

### 03.21 Canvas Color
- **Mô tả:** đổi màu nền canvas; hỗ trợ **transparent** (checkerboard) → export PNG có alpha.
- **Tùy chọn:** chọn màu hoặc transparent.
- **Ưu tiên:** MVP P4.
- **Ghi chú implement C++/Qt:** với vùng "trống" của canvas (ngoài ảnh, hoặc nơi alpha=0), fill bằng màu chọn; nếu chọn transparent thì giữ alpha=0 và render checkerboard chỉ ở khâu hiển thị (không nung vào pixel). Export PNG giữ alpha.

---

## Bảng ưu tiên

| Tính năng | Nhóm | Ưu tiên |
|---|---|---|
| Border | Effect | ⭐MVP P4 |
| Shadow (opacity/offset/blur) | Effect | ⭐MVP P4 |
| Capture Info (core bug-report) | Effect | ⭐MVP P4 |
| Crop (+ round crop) | Transform | ⭐MVP P4 |
| Trim / Auto-Trim | Transform | ⭐MVP P4 |
| Resize Image (px/percent + aspect) | Transform | ⭐MVP P4 |
| Rotate (90/180/custom) | Transform | ⭐MVP P4 |
| Flip H/V (mirror annotation) | Transform | ⭐MVP P4 |
| Edges (drop shadow/torn/faded) | Effect | MVP P4 |
| Edges (wave/saw/shark tooth/beveled) | Effect | P5 |
| Color Adjustment (5 slider) | Effect | MVP P4 |
| Filters: grayscale/invert/opacity/sharpen/soften | Effect | MVP P4 |
| Filters: monochrome/sepia | Effect | P5 |
| Cut Out (straight stitch, H/V) | Transform | MVP P4 |
| Cut Out (decorative torn + add space) | Transform | P5 |
| Resize Image (inch/cm/DPI/resample) | Transform | P5 |
| Canvas Color (incl. transparent) | Transform | MVP P4 |
| Perspective | Effect | P5 |
| Color Replacement | Effect | P5 |
| Watermark (overlay/underlay) | Effect | P5 |
| Spotlight & Magnify (effect) | Effect | P5 |
| Resize/Expand Canvas | Transform | P5 |
| Page Curl | Effect | P6 |
| Reflection | Effect | P6 |
| Fade | Effect | P6 |

## Điểm chưa chắc

- **Filter sub-options:** danh sách trong 03.10 là đại diện, không bất biến.
- **Trim entry point** chưa chốt. Phương án dự phòng documented: Fill tool (thay viền đặc thành trong suốt) + Crop thủ công.
- **Resize/Expand Canvas UI:** kéo cạnh vs dialog Canvas Size — cần chốt theo thiết kế đích.
- **Sepia:** Ezsnagit cấp toggle "Sepia" tên riêng cho tiện (thay vì đạt qua monochrome/color adjustment).
- **Page Curl / Reflection / Fade** là effect ít dùng → P6, có thể bỏ nếu hẹp thời gian.
- Ezsnagit dùng format riêng `.ezsnagx` một nền tảng (Windows-first).
