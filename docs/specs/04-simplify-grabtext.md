# SPEC 04 — Simplify, Smart Move & Grab Text (Ezsnagit)

> Nguồn: docs/research/03-effects-ocr.md · Module: `ezsnag_simplify` (OpenCV) + `ezsnag_ocr` (Tesseract)

## Mục tiêu & phạm vi

Spec này gom ba tính năng "auto-detect" cao cấp — chia làm hai module:

1. **`ezsnag_simplify` (OpenCV):**
   - **Simplify / SUI (Simplified User Interface):** thay text + phần tử UI chi tiết bằng **shape trừu tượng** sạch sẽ → ảnh generic, dễ localize, version-proof, ít nhiễu. Đây là tính năng đặc trưng của Snagit.
   - **Smart Move:** biến phần tử UI trong **screenshot phẳng** thành **object edit được** để rearrange/remove (auto-fill nền sau khi dời/xóa).
2. **`ezsnag_ocr` (Tesseract):**
   - **Grab Text:** OCR ảnh → text (capture-time hoặc trong Editor; toàn ảnh hoặc vùng chọn).
   - **Edit Text:** chuyển text nhận diện thành **text object edit được tại chỗ** trên ảnh.

### Khác biệt cốt lõi (bám research)

- **Smart Move** = làm cho UI *tĩnh* edit được để **dời/xóa** (giữ nguyên hình thức). **Simplify** = thay UI bằng **shape trừu tượng**. Cùng "auto-detect UI element" nhưng output khác nhau.
- **Magnify/annotation** không thuộc spec này (xem spec 02/03).
- **OCR engine:** Snagit gốc dùng ABBYY (license gia hạn qua update 2026-05-01). **Ezsnagit dùng Tesseract** (khác có chủ đích, xem overview §4) — open-source, chạy local, không phụ thuộc license bên thứ ba.
- **Privacy:** Smart Move/Simplify chạy **local** bằng computer-vision (Snagit dẫn OpenCV). Không upload ảnh, không dùng để train. Ezsnagit giữ nguyên nguyên tắc này (toàn bộ CV + OCR chạy offline).

---

## Tính năng

### Phần A — Simplify / SUI (`ezsnag_simplify`, OpenCV)

### 04.1 Auto Simplify (auto-detect)
- **Mô tả:** bật **Auto Simplify** trong Tool Properties → quét ảnh và **phủ shape đơn giản** (thanh/khối/hình màu) lên text & phần tử UI phát hiện được (button, field, text run...).
- **Tùy chọn:** **Detail slider** chỉnh **độ nhạy phát hiện**; kéo slider **regenerate** lại tập object. Detail cao = nhiều object nhỏ; thấp = ít khối thô.
- **Ưu tiên:** P7 (sau MVP core). Auto-detect là phần khó nhất.
- **Ghi chú clone (OpenCV):**
  - Tiền xử lý: grayscale → adaptive threshold / Canny edge.
  - Phát hiện vùng: morphological dilation gộp ký tự thành text-run, `findContours` + `boundingRect` lấy bounding box; lọc theo kích thước/tỉ lệ để phân biệt button/field/text.
  - **Detail slider ↔ merge threshold:** kernel dilation lớn / threshold gộp cao = ít object thô (detail thấp); kernel nhỏ = nhiều object nhỏ (detail cao). Kéo slider → chạy lại detector.
  - Mapping **box → shape primitive:** mỗi box → một simplify object (rounded rect cho button/field, bar cho text run) lưu vào canvas object graph (`ezsnag_canvas`), tô màu theo palette.

### 04.2 Color Palette
- **Mô tả:** panel **Color Palette** quản lý màu cho simplify object; dropdown màu để recolor object trên canvas.
- **Tùy chọn:** **Save Palette** (gear icon) → tạo mới hoặc thay palette cũ → đồng bộ brand/product qua nhiều ảnh.
- **Ưu tiên:** P7.
- **Ghi chú clone:** palette = danh sách màu lưu JSON trong preferences; áp màu là cập nhật fill của simplify object (object vector, không nung pixel cho tới khi export).

### 04.3 Manual simplify objects
- **Mô tả:** thêm/sửa shape thủ công.
- **Tùy chọn:** **Quick Styles** (preset shape kéo thả); **Draw Mode** (thêm) vs **Move Mode** (dời); per-object: kéo handle resize, dời, **Delete**, sửa **fill color / shape type / background color / text height / line spacing**; **Select All (Shape)** batch-edit mọi simplify shape.
- **Ưu tiên:** P7 (Draw/Move + per-object props); Select All P8.
- **Ghi chú clone:** simplify object là loại object trong `ezsnag_canvas` (giống annotation) → tận dụng undo/redo, z-order, handle resize sẵn có.

### 04.4 Show Original
- **Mô tả:** **Show Original** lộ screenshot thật bên dưới lớp simplify, có **opacity slider** để canh shape theo nguồn khi chỉnh.
- **Ưu tiên:** P7.
- **Ghi chú clone:** render ảnh nền gốc với opacity dưới lớp simplify object (chỉ ở chế độ hiển thị, không ảnh hưởng export).

### 04.5 Integration với annotation
- **Mô tả:** simplify object cùng tồn tại với annotation thường (arrow, callout...) trên cùng canvas; chuyển tool tự do. Shape vẫn edit được tới khi flatten/export.
- **Ưu tiên:** P7 (đến từ thiết kế object graph chung).
- **Ghi chú clone:** dùng chung object graph → không cần layer riêng.

### 04.6 Lock Objects *(nguồn: Windows-only)*
- **Mô tả:** **Lock Objects** chống sửa nhầm simplify object khi chuyển tool.
- **Tùy chọn:** toggle lock.
- **Ưu tiên:** P8 (tiện ích).
- **Ghi chú clone:** thêm cờ `locked` trên object → bỏ qua hit-test khi chọn/di chuyển. Mac gốc không có → ta cấp tùy chọn này cho tiện.

---

### Phần B — Smart Move (`ezsnag_simplify`, OpenCV)

### 04.7 Move tool (nền tảng)
- **Mô tả:** chọn/dời/sửa/xóa annotation **object** trên canvas. Sau khi vẽ bằng tool khác, chuyển Move để kéo/resize/xóa hoặc double-click sửa property.
- **Ưu tiên:** đây là tool nền của editor (spec 02). Smart Move là chế độ bật thêm.

### 04.8 Smart Move (auto-detect UI trong screenshot phẳng)
- **Mô tả:** biến phần tử UI của **screenshot đã flatten** thành object edit được. Hover một object → hiện selection handle. Cho phép rearrange/remove.
- **Tùy chọn:**
  - **Bật:** (Windows) bật **Smart Move** trong Tool Properties của Move tool; (Mac) nút **Smart Move** ở Actions panel. Ezsnagit: toggle trong Tool Properties của Move.
  - **Detail slider:** chỉnh granularity. Phải = phát hiện object/line/text nhỏ hơn, gộp ít; trái = gộp nhiều.
  - **Actions per object:** **Move** (click-drag), **Delete** (phím Delete), **Edit Text** (double-click text box sửa tại chỗ).
  - **Auto-fill background:** khi dời/xóa object, tự fill vùng trống khớp nền xung quanh (cùng engine auto-fill của Selection tool).
- **Ưu tiên:** P7 (core detect + move/delete); Edit-Text-in-place P8.
- **Ghi chú clone (OpenCV):**
  - Detector tương tự Auto Simplify (edge/contour/bounding box) nhưng output là **object có thể dời** giữ ảnh con (crop của box) làm texture, không thay bằng shape trừu tượng.
  - **Auto-fill (inpaint):** dùng `cv::inpaint` (Telea/Navier-Stokes) hoặc texture-fill từ viền lân cận để lấp vùng vacate. Đây là điểm kỹ thuật then chốt.
  - **Detail slider ↔** kích thước kernel gộp (giống 04.1).
  - Edit Text in-place dùng OCR (`ezsnag_ocr`) để lấy text box → chuyển thành text object.
- **Privacy:** chạy local, không upload (nguyên tắc bám research).

---

### Phần C — Grab Text & Edit Text OCR (`ezsnag_ocr`, Tesseract)

### 04.9 Grab Text khi chụp (capture-time)
- **Mô tả:** trong Capture window, tab **Image** → chọn **Grab Text** ở dropdown **Selection** → bấm Capture (phím tắt) → drag chọn vùng màn hình → dialog **Grab Text Results** mở trong Editor với text nhận diện.
- **Ưu tiên:** P7.
- **Ghi chú clone:** phối hợp `ezsnag_capture` chụp vùng → đẩy `QImage` qua Tesseract → mở Results dialog.

### 04.10 Grab Text từ ảnh có sẵn (trong Editor)
- **Mô tả:**
  - **Toàn bộ text:** right-click ảnh trên canvas → **Grab Text**.
  - **Text một phần (chính xác hơn):** Selection tool khoanh vùng text nhỏ trước → right-click → **Grab Text**. Giới hạn vùng → tăng độ chính xác.
- **Ưu tiên:** MVP của OCR — P7 (đây là đường dùng phổ biến nhất).
- **Ghi chú clone (Tesseract):** truyền `QImage` (hoặc crop vùng chọn) sang Tesseract API; nên upscale + tăng contrast trước khi OCR để cải thiện kết quả.

### 04.11 Grab Text Results dialog
- **Mô tả:** dialog hiện text nhận diện.
- **Tùy chọn:** **Copy All** copy ra clipboard; toggle **Plain text** / **Make Plain Text** strip font & màu rồi Copy All (lựa chọn plain-vs-formatted có từ Snagit 2024).
- **Ưu tiên:** P7.
- **Ghi chú clone:** Tesseract trả plain text mặc định; "formatted" giữ layout/dòng theo `hOCR`/TSV của Tesseract (toạ độ box) — nếu chưa cần thì làm plain trước. `QClipboard::setText` cho Copy All.
- **Điểm chưa chắc:** auto-copy không click không phải toggle có sẵn ở bản cũ; Ezsnagit có thể thêm tùy chọn "auto-copy sau Grab Text".

### 04.12 Edit Text (sửa text screenshot tại chỗ)
- **Mô tả:** chuyển text nhận diện thành **text object edit được** trên ảnh thay vì chỉ copy.
- **Tùy chọn:** Selection tool khoanh vùng text → right-click → **Edit Text**; rồi qua Tool Properties đổi **font / size / color**, chèn text mới, **replace** từ đã chọn, hoặc xóa ký tự — ngay trên screenshot (cập nhật label/giá trị không cần chụp lại).
- **Ưu tiên:** P8 (khó: cần OCR + che text gốc + render text mới khớp nền).
- **Ghi chú clone:** OCR lấy box + text + ước lượng màu chữ/nền; tạo text object đặt đúng vị trí; che pixel chữ gốc bằng auto-fill nền (cùng inpaint của Smart Move) rồi vẽ text object lên trên (object vector edit tiếp được).

### 04.13 Languages & accuracy
- **Ngôn ngữ (theo support docs gốc):** English, German, French, Japanese, Spanish — lưu ý **ký tự Japanese** là hạn chế.
- **Ghi chú clone (Tesseract):** Tesseract hỗ trợ 100+ ngôn ngữ qua `traineddata`. Ezsnagit nên đóng gói tối thiểu: `eng`, `deu`, `fra`, `spa`, `jpn` (khớp tập gốc), cho phép thêm gói sau. Cho người dùng chọn ngôn ngữ OCR trong preferences (mặc định eng).
- **Best practices độ chính xác (bám research):** contrast cao giữa chữ và nền là then chốt (light-on-light nhận kém); chọn vùng nhỏ thay vì toàn ảnh để tăng độ chính xác → khuyến khích luồng "Selection → Grab Text".
- **Tính năng cùng họ OCR (tham chiếu, không bắt buộc MVP):** Smart Redact (auto-detect + che text nhạy cảm), Translate Text.

### 04.14 Platform notes (tham chiếu)
- Grab Text/Edit Text core chạy cả Win & Mac; Mac grab được từ frame video tạm dừng; phím tắt capture khác nhau (PrintScreen Win / Shift+Ctrl+C Mac). Ezsnagit một nền tảng → dùng một bộ phím tắt cấu hình được (spec 10).

---

## Bảng ưu tiên

| Tính năng | Module | Ưu tiên | Ghi chú |
|---|---|---|---|
| Grab Text từ ảnh trong Editor (toàn/vùng) | `ezsnag_ocr` | P7 | đường dùng chính, Tesseract |
| Grab Text capture-time | `ezsnag_ocr` | P7 | phối hợp `ezsnag_capture` |
| Grab Text Results (Copy All + plain) | `ezsnag_ocr` | P7 | clipboard |
| Language packs (eng/deu/fra/spa/jpn) | `ezsnag_ocr` | P7 | đóng gói traineddata |
| Auto Simplify + Detail slider | `ezsnag_simplify` | P7 | OpenCV detect, khó nhất |
| Color Palette (+ save) | `ezsnag_simplify` | P7 | JSON preset |
| Manual simplify (Draw/Move, per-object props) | `ezsnag_simplify` | P7 | object graph chung |
| Show Original underlay | `ezsnag_simplify` | P7 | opacity overlay |
| Smart Move (detect + move/delete + auto-fill) | `ezsnag_simplify` | P7 | OpenCV + inpaint |
| Edit Text in-place (Smart Move) | `ezsnag_simplify`+`ezsnag_ocr` | P8 | OCR + inpaint + text object |
| Edit Text (từ Grab Text) | `ezsnag_ocr` | P8 | che nền + text object |
| Select All (Shape) batch-edit | `ezsnag_simplify` | P8 | |
| Lock Objects | `ezsnag_simplify` | P8 | cờ locked (Win-only gốc) |
| Auto-copy sau Grab Text | `ezsnag_ocr` | P8 | tùy chọn thêm |
| Smart Redact / Translate Text | `ezsnag_ocr` | sau (ngoài MVP) | họ OCR, optional |

## Điểm chưa chắc

- **Auto-detect quality (Simplify & Smart Move):** Snagit dùng CV chuyên biệt; clone bằng OpenCV thuần (contour/morphology) sẽ **kém chính xác hơn** trên UI phức tạp. Overview §4 đã gác "Smart Move/Simplify mức AI cao" → MVP làm detect cơ bản, chấp nhận chỉnh tay nhiều.
- **Auto-fill / inpaint:** chất lượng `cv::inpaint` trên nền phức tạp (gradient, texture) hạn chế → vùng vacate có thể lộ. Đây là rủi ro kỹ thuật chính của Smart Move/Edit Text.
- **Detail slider mapping:** ánh xạ slider → tham số detector (kernel/threshold) cần tuning thực nghiệm; chưa có công thức chuẩn.
- **OCR engine khác gốc:** Tesseract ≠ ABBYY → độ chính xác/định dạng khác; đặc biệt **Japanese** vốn đã là hạn chế ở gốc, Tesseract `jpn` cũng cần ảnh chất lượng cao.
- **Plain vs formatted:** giữ định dạng (font/màu/layout) qua Tesseract cần hOCR/TSV; MVP ưu tiên plain text.
- **Auto-copy clipboard:** không phải toggle chuẩn ở Snagit cũ; ta tự thêm nếu muốn.
- **Editions:** research không có ma trận đầy đủ AI feature theo tier; giả định Simplify ở base. Ezsnagit không phân tier → cấp toàn bộ.
