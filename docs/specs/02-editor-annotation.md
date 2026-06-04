# SPEC 02 — Editor & Annotation (Ezsnagit)

> Module: `ezsnag_canvas`, `ezsnag_annotations`, `ui/EditorWindow`

---

## Mục tiêu & phạm vi

Editor là **bề mặt người dùng ở lâu nhất** trong Ezsnagit (xem SPEC 00 §1). Sau khi chụp, capture được mở trên Canvas để review, annotate, enhance, và share. Spec này đặc tả:

- **Layout** của Editor (toolbar, canvas, tool properties, recent tray, share toolbar).
- **Mô hình object vector** (re-edit / z-order / flatten) — ánh xạ vào `ezsnag_canvas` (Canvas Object Graph).
- **Toàn bộ annotation & editing tools** (Arrow, Text, Callout, Shape, Line, Highlight, Step, Stamp, Pen, Fill, Eraser, Selection, Move/Smart Move, Cut Out, Crop, Magnify, Blur/Pixelate, Spotlight, Eyedropper, Group/Arrange, Flip/Rotate).
- **Quick Styles / Themes / màu**, **Undo/redo / copy/paste / snapping**.

**Ngoài phạm vi** (đặt ở spec khác):
- Effects raster mức ảnh (Border, Edges, Color Adjustment, Spotlight & Magnify effect trên Windows, Watermark…) → **SPEC 03 (`ezsnag_effects`)**.
- **Simplify / SUI**, **Smart Move** (AI/OpenCV), **Grab Text / Edit Text (OCR)** → **SPEC 04 (`ezsnag_simplify`, `ezsnag_ocr`)**.
- Library, Recent tray (lưu trữ/tìm kiếm), Share destinations → SPEC 06 / 07.

> Tất cả annotation tool, Move tool, và Effects đều **chỉ áp dụng cho ảnh** (image-only). Video không annotate trong editor này (xử lý ở luồng video riêng); xem SPEC 05.

---

## Editor layout

```
┌───────────────────────────────────────────────────────────────┐
│ [Editor│Library]            Menu (File/Edit/Image/View)         │ ← tabs + menu bar
├───────────────────────────────────────────────────────────────┤
│ Toolbar: Arrow Text Callout Shape Line … [More ⋯]   Share ▸    │ ← toolbar (trên) + share (phải)
├──────────────────────────────────────────────┬────────────────┤
│                                              │ Quick Styles   │
│                                              │ ◻ ◻ ◻ ◻ + ★    │
│                  CANVAS                       ├────────────────┤
│            (workspace, drag handle           │ Tool Properties│
│             mép để mở rộng canvas)           │ Color/Width/…  │
│                                              │                │
├──────────────────────────────────────────────┴────────────────┤
│ Recent Captures Tray (thumbnails)                              │ ← tray (dưới)
└───────────────────────────────────────────────────────────────┘
```

| Region | Vị trí | Vai trò | Lớp implement |
|---|---|---|---|
| **Editor / Library tabs** | Trên-trái | Chuyển giữa Editor (sửa 1 capture) và Library (lịch sử) | `ui/EditorWindow` ↔ `ui/LibraryView` (SPEC 06) |
| **Toolbar** | Trên (ngang) | Chứa annotation/editing tools; nút **More (⋯)** mở tool ẩn; **customizable** (kéo thả) | `ui/AnnotationToolbar` (`QToolBar`) |
| **Tool Properties panel** | Phải | Hiển thị property của tool đang chọn (color, width, font, opacity, shadow…); đổi theo từng tool | `ui/ToolPropertiesPanel` (`QStackedWidget`, 1 page/tool) |
| **Quick Styles** | Phải, trên/trong Properties | Hàng preset style của tool đang chọn; lưu custom; thêm Favorites (★) | `ui/QuickStylesStrip` |
| **Effects** | Panel phải / nút Effects | Effect mức ảnh (xem SPEC 03) — **tách khỏi** per-object properties | `ezsnag_effects` (SPEC 03) |
| **Canvas (Workspace)** | Giữa | Bề mặt sửa; kéo handle mép canvas để **mở rộng vùng làm việc** (thêm canvas trắng quanh ảnh) | `ui/CanvasView` (`QGraphicsView`) |
| **Recent Captures Tray** | Dưới (ngang) | Thumbnail capture gần đây; thumbnail đang chọn hiển thị trên canvas | `ui/RecentTray` (SPEC 06) |
| **Share toolbar** | Trên-phải | Copy All, Share Link, dropdown đích chia sẻ | `ezsnag_share` (SPEC 07) |
| **Menu bar** | Trên | File/Edit/Image/View; chứa Canvas Snapping, Flatten All, Grab Text, undo/redo | `ui/EditorWindow` menus |
| **Assets panel** | Mở từ Editor | Thư viện assets (stamps/themes/templates) | Implement: thư viện stamp/theme local; cloud Assets = tạm gác (SPEC 00 §4) |

### Customizing toolbar
- **More → Customize Toolbar**: kéo tool vào/ra toolbar; tool ẩn vẫn truy cập qua **More**.
- Layout toolbar, Quick Styles, Favorites đều lưu theo user (preferences — SPEC 10).
- Implement: `QToolBar` cho phép ẩn/hiện action; lưu thứ tự + visibility vào `QSettings`.

---

## Mô hình object vector (re-edit, z-order, flatten) — ánh xạ vào `ezsnag_canvas`

Đây là **xương sống** của hành vi annotation. Tham chiếu SPEC 00 §2 (Canvas Object Graph).

### Nguyên tắc object vector
- **Vector objects stay editable.** Text, Callout, Arrow, Shape, Line, Stamp, Step, ảnh paste… là các **vector object** riêng nằm trên ảnh nền; chọn/di chuyển/resize/restyle/reorder/xóa bất kỳ lúc nào bằng **Move tool**.
- **Editable format:** lưu mọi object + state → mở lại edit tiếp. Ezsnagit dùng **`.ezsnagx`** (SPEC 09).
- **Flatten = thành raster.** Export PNG/JPEG hoặc **Flatten All** → mọi object nung vào bitmap, **không còn edit** riêng.
- **Layering / z-order:** mỗi object 1 layer; object mới vẽ đè lên trên; reorder qua arrange controls + context menu (xem §02.20).
- **Raster-only tools** thao tác trực tiếp lên pixel đã flatten (KHÔNG tạo vector object): **Fill, Eraser, Blur*, Crop, Cut Out**, và **Selection**-based pixel edits.
- **Pen** và **Line** có thể vector hoặc raster tùy toggle **Create as Vector** (Win) / **Create Editable Line** (Mac).

> *Blur có thể coi gần nhóm raster, nhưng nó còn re-editable như object (chọn lại đổi intensity). Ezsnagit: ưu tiên hiện thực **Blur/Pixelate là object vector** (re-edit được), flatten khi export. Xem §02.17.

### Ánh xạ implement C++/Qt — quyết định kiến trúc

> **Quyết định nền tảng (giữ nhất quán toàn spec):** Canvas hiện thực bằng **`QGraphicsScene` + `QGraphicsView`**. Mỗi annotation = một **`QGraphicsItem` subclass** trong `ezsnag_annotations`. Ảnh nền = một `QGraphicsPixmapItem` ở z thấp nhất. Lý do: QGraphicsScene cho sẵn hit-testing, selection handles, z-ordering (`setZValue`), transform (rotate/scale), bounding rect, và serialize hình học — đúng nhu cầu object graph. Tránh tự vẽ mọi thứ trên một `QWidget` custom (sẽ phải tái phát minh hit-test, z-order, selection).

`ezsnag_canvas` (logic thuần, test headless) định nghĩa **document model** độc lập GUI:

```
CanvasDocument
 ├─ backgroundRaster : QImage            // ảnh nền (raster)
 ├─ objects : QList<AnnotationObject*>   // theo thứ tự z (cuối = trên cùng)
 ├─ canvasSize : QSize                   // có thể > ảnh (mở rộng canvas trắng)
 ├─ undoStack : QUndoStack
 └─ metadata (source, ts, tags…)
```

- `AnnotationObject` (model thuần): `type`, `geometry` (điểm/rect/path), `style` (color/width/opacity/shadow/font…), `zIndex`, `rotation`, `transform`. **Không** phụ thuộc Qt Widgets — chỉ Qt Gui (`QPainterPath`, `QColor`, `QImage`).
- Lớp **render/GUI** (`ezsnag_annotations` + `ui`): mỗi `AnnotationObject` được map sang một `QGraphicsItem` subclass để hiển thị + tương tác. Tách biệt cho phép render headless (paint ra `QImage`) khi flatten/export mà không cần view.
- **Undo/redo:** `QUndoStack` với các `QUndoCommand` (AddObject, RemoveObject, MoveObject, ChangeStyle, ReorderZ, EditText…). Mô hình giống SpreadsheetModel của Ezcel (SPEC 00 §2).
- **Z-order:** `zIndex` trong model ↔ `QGraphicsItem::setZValue`. Reorder = `ReorderZCommand`.
- **Flatten:** render tuần tự background → từng object theo z lên một `QImage` (qua `QPainter` headless). Dùng cho export PNG/JPG và **Flatten All** (biến tất cả thành background raster, xóa danh sách object).
- **Serialize `.ezsnagx`:** JSON (object list: type/geometry/style/z) + PNG nền nhúng/đính kèm. Chi tiết format ở SPEC 09.

### Vector vs raster trong Ezsnagit
- **Vector objects** (re-edit): Arrow, Text, Callout, Shape, Line, Highlight, Step, Stamp, Pen (khi Create-as-Vector), Magnify, Blur/Pixelate. → `QGraphicsItem` subclass. *(Spotlight KHÔNG ở đây — là effect baked, xem SPEC 03 §03.13.)*
- **Raster ops** (sửa thẳng background pixel, có undo nhưng không phải object): Fill (bucket), Eraser, Crop, Cut Out, Selection-paste/delete, Pen (khi raster). → thao tác trên `backgroundRaster` (QImage) qua `QUndoCommand` lưu before/after region.

---

## Annotation tools

> **Universal UX flow**: chọn tool trên toolbar → Quick Styles + Tool Properties hiện bên phải → chọn Quick Style hoặc chỉnh property → click (hoặc click-drag) trên canvas tạo object → gõ/điều chỉnh → chuyển sang **Move tool** để chọn lại, di chuyển, restyle, hoặc xóa.

> **Convention implement:** trừ khi ghi rõ là raster, mọi tool tạo một `QGraphicsItem` subclass (đặt tên `Ezsnag<Tool>Item`) backed bởi một `AnnotationObject` trong model. Selection handles (white=resize, green=rotate, yellow/blue=tail…) hiện thực bằng các child item / overlay handle khi item được chọn ở Move tool.

### 02.1 Arrow
- **Mô tả:** mũi tên chỉ hướng chú ý. Vẽ click-drag **tail → head**; giữ **Shift** ràng buộc H/V/45°.
- **Properties:** Color (theme/HEX/RGB/eyedropper) · End Style (kiểu đầu mũi) · Line Style (solid/dashed/dotted) · Width (px, slider) · Start/End size (tỉ lệ đầu mũi) · Opacity · Shadow (direction + advanced: color/distance/opacity/blur; angle trên Win) · **Bezier curve** (kéo white handle uốn cong; Win+Mac).
- **Ưu tiên:** **⭐MVP P3** (tool cốt lõi, không có bezier ở MVP — bezier P3+/sau).
- **Implement C++/Qt:** `EzsnagArrowItem : QGraphicsItem`. Hình học = 2 điểm (tail/head) + tham số đầu mũi; `paint()` dựng `QPainterPath` thân + polygon đầu mũi. Bezier = thêm control point → `QPainterPath::cubicTo`. Shadow = vẽ path offset/blur (hoặc `QGraphicsDropShadowEffect` cho bản đơn giản).

### 02.2 Text
- **Mô tả:** thêm caption/heading/label dạng chữ trực tiếp (**không** có hộp nền). Click-drag tạo text box → gõ. Cần hộp nền/đuôi chỉ → dùng **Callout**.
- **Properties:** Font · Font size · B/I/U/Strikethrough (tùy nền tảng) · **Fill color** (ruột chữ) + **Outline color** + outline width (0 = không viền) · Alignment (H/V) · Opacity · Shadow · **Padding** (Win) · **Select All Text Objects** (bulk-edit, bản mới).
- **Ưu tiên:** **⭐MVP P3.**
- **Implement C++/Qt:** `EzsnagTextItem : QGraphicsTextItem` (hoặc subclass tự quản layout). Editing in-place qua `QTextCursor`/`setTextInteractionFlags`. Outline chữ = vẽ `QPainterPath` từ glyph (`QPainterPath::addText`) rồi stroke; fill + outline 2 lượt. Select-All = lệnh chọn mọi item cùng type trong scene.

### 02.3 Callout (speech bubbles)
- **Mô tả:** text box có shape + fill + **đuôi (tail)** chỉ. **Shape Style** dropdown (rounded rect, speech/thought bubble, oval…). Click-drag vẽ → gõ.
- **Handles:** white = resize; **yellow** = di đầu đuôi; **blue** = thêm đuôi; **green** = rotate; **anchor point** dời thân callout so với đuôi.
- **Properties:** Fill color (theme/HEX/RGB/eyedropper; Mac thêm gradient) · Outline color · Shadow · **Arrow/Tail style** (line style + end style + size + color) · Text (font, color, size, B/I/U/strike, align H/V, padding + opacity trên Win).
- **Ưu tiên:** **⭐MVP P3** (gradient fill, multi-tail = sau).
- **Implement C++/Qt:** `EzsnagCalloutItem : QGraphicsItem` chứa một child `QGraphicsTextItem` cho text. Body = `QPainterPath` theo shape style; tail = path tam giác nối anchor→tip; multi-tail = list tip points. Custom handle items cho yellow/blue/green/anchor.

### 02.4 Shape
- **Mô tả:** thêm shape / khung vùng. Click-drag **rectangle**/**ellipse**; chọn **Polygon** trong properties (kéo cạnh đầu, click chốt mỗi góc). Giữ **Shift** = square/circle hoàn hảo.
- **Properties:** Fill color (kể cả **transparent** → khung rỗng) · Outline color · Line width (px) + Line style · Opacity · Shadow.
- **Ưu tiên:** **⭐MVP P3.**
- **Implement C++/Qt:** `EzsnagShapeItem : QGraphicsItem` với enum `{Rect, Ellipse, Polygon}`; `paint()` theo loại. Polygon = `QPolygonF` + chế độ nhập tuần tự (click thêm điểm, double-click kết thúc). Green handle rotate qua `setTransformOriginPoint` + `setRotation`.

### 02.5 Line
- **Mô tả:** đường thẳng/cong (đầu mũi tùy chọn). Click-drag; **Shift** = H/V.
- **Properties:** End style (mũi tùy chọn) · Line style (solid/dashed) · Color · Width (px) · Opacity · Shadow · **Bezier curve** (kéo white handle).
- **Vector/raster:** chịu toggle **Create as Vector / Create Editable Line** giống Pen (xem §02.9).
- **Ưu tiên:** **MVP P3** (bezier sau).
- **Implement C++/Qt:** `EzsnagLineItem : QGraphicsItem` (chia sẻ nhiều logic với Arrow — có thể chung base `EzsnagStrokeItem`). Khi raster mode → vẽ thẳng vào `backgroundRaster`.

### 02.6 Highlight (Highlighter)
- **Mô tả:** highlight bán trong suốt phủ vùng, vẫn thấy nội dung dưới (như bút dạ). Click-drag vùng cần highlight.
- **Properties:** Color · Opacity (semi-transparent) · **Shape** (Mac only: rect, rounded rect, oval, polygon) · **Apply only to text** (Mac only).
- **Phân biệt:** đây là **Highlighter annotation tool** (tạo object highlight mờ), KHÁC **Spotlight & Magnify effect** (làm tối/nhấn — SPEC 03). Trên Windows nhấn mạnh dạng shape làm qua Selection + effect.
- **Ưu tiên:** **⭐MVP P3** (chỉ rect; shape khác + apply-only-to-text = sau, tùy nền tảng).
- **Implement C++/Qt:** `EzsnagHighlightItem : QGraphicsItem`, fill màu với alpha thấp + composition mode `Multiply` (`QPainter::CompositionMode_Multiply`) để giống bút dạ phủ lên text.

### 02.7 Step (numbered/lettered sequence)
- **Mô tả:** click thả badge số/chữ **tự tăng** để minh hoạ chuỗi/quy trình. Mỗi click sau tăng tiếp.
- **Properties:** **Type** (numbers 1,2,3 / letters A,B,C / a,b,c / **Roman**) · Fill color · Text color · **Shape** (circle/square…) · Shadow · Opacity · **Restart Sequence** · **Select All** (bản mới).
- **Manual:** right-click step → **Edit Value** đổi số/chữ. Move tool reposition → sequence renumber hợp lý.
- **Ưu tiên:** **⭐MVP-B (P3)** (numbers + circle; letters/roman + custom shape sau). Đây là **Step tool thủ công** — phân biệt với **Step-Capture** (auto chụp khi click) ở §02.7b.
- **Implement C++/Qt:** `EzsnagStepItem : QGraphicsItem` (badge = shape + text giữa). Sequence state quản ở document level (`StepSequence` trong `ezsnag_canvas`): danh sách step theo thứ tự, renumber khi thêm/xóa/Restart. Edit Value override giá trị thủ công.

### 02.7b Step-Capture (auto chụp khi click)
- **Mô tả:** tính năng documentation chủ lực — **tự chụp một screenshot mỗi lần user click**, đồng thời **dò tên UI element** đang được click và **đánh số tự động** theo thứ tự thao tác. Tạo nhanh tài liệu hướng dẫn từng-bước (step-by-step guide) mà không phải chụp + đánh số thủ công.
- **Phân biệt:** đây **KHÁC** Step tool thủ công (§02.7). Step tool = thả badge số trên một capture có sẵn; Step-Capture = **engine chụp tự động** theo từng click, sinh ra chuỗi capture đã đánh số.
- **Ưu tiên:** **P10 (fast-follow)** — không thuộc MVP; OS-integration nặng nên làm sau khi nền capture/editor đã ổn.
- **Ghi chú implement C++/Qt:** cần **hook chuột toàn cục** (global mouse hook) để bắt mỗi click; trên Windows dùng **UI Automation (UIA)** để lấy **tên control** đang được click; mỗi click → trigger capture + ghi tên element + tăng số đếm.

### 02.8 Stamp (stickers / graphics)
- **Mô tả:** thả stamp/sticker đồ hoạ (cursor, emoji, icon, badge…). Chọn **category** dropdown hoặc **search** → click đặt → kéo/handle resize.
- **Custom stamps:** import **JPG/PNG/GIF**. Mac có thể lưu ảnh thành custom stamp.
- **Assets:** thư viện stamp đóng gói sẵn (Win: **Download More**; Mac: browse/import). → Implement: thư viện stamp **local** đóng gói; cloud Assets tạm gác (SPEC 00 §4).
- **Properties:** Shadow · Opacity. Lưu Quick Styles.
- **Ưu tiên:** **MVP P3** (đặt stamp từ thư viện local + import PNG; GIF động + cloud = sau).
- **Implement C++/Qt:** `EzsnagStampItem : QGraphicsPixmapItem` (giữ ảnh gốc để re-scale nét). Animated GIF = tạm dùng frame đầu ở MVP.

### 02.9 Pen (Freehand)
- **Mô tả:** vẽ tay tự do. Click-drag; **Shift** = thẳng H/V.
- **Vector vs raster:** **Create as Vector** (Win) / **Create Editable Line** (Mac) → bật: stroke là object edit được; tắt: nung thẳng vào bitmap.
- **Properties:** Color · Width (px) · Opacity · **Shape** (Win: nhiều; Mac: chỉ circle) · Shadow · **Antialias** + **smooth curve** · **Select All**.
- **Ưu tiên:** **MVP P3** (vector mode mặc định; raster mode + smoothing = sau).
- **Implement C++/Qt:** `EzsnagPenItem : QGraphicsItem` giữ `QPainterPath` (sample điểm khi drag; smoothing = simplify path / Catmull-Rom). Raster mode → `QPainter` vẽ path thẳng vào `backgroundRaster`. Round-cap pen shape = `Qt::RoundCap`.

### 02.10 Fill (bucket) — RASTER
- **Mô tả:** flood-fill vùng màu liền kề (ẩn object bằng cách phủ pixel cùng màu). Chọn fill color (eyedropper sample) → click vùng.
- **Properties:** Color (eyedropper) · **Tolerance** slider (cao = phủ rộng, thấp = chính xác).
- **Ưu tiên:** **sau** (P4+). Không cốt lõi cho MVP annotation.
- **Implement C++/Qt:** **raster op** trên `backgroundRaster`. Scanline/queue flood fill trên `QImage`, so màu theo tolerance (khoảng cách RGB). `QUndoCommand` lưu before/after bounding region. KHÔNG phải `QGraphicsItem`.

### 02.11 Eraser — RASTER
- **Mô tả:** xoá vùng flattened để lộ canvas dưới. Drag qua pixel cần xoá.
- **Ưu tiên:** **sau** (P4+).
- **Implement C++/Qt:** raster op — vẽ trong suốt (clear alpha) lên `backgroundRaster` dọc đường drag với brush tròn. Undo lưu region.
- **Điểm chưa chắc:** kích thước/shape brush của Eraser **không** rõ trong docs gốc → Ezsnagit tự đề xuất brush size slider + round brush; verify sau.

### 02.12 Selection — RASTER
- **Mô tả:** chọn phần ảnh để **cut/copy/move/delete/apply effect** (raster selection).
- **Shapes:** Rectangle, Ellipse, Polygon (click góc, double-click xong), **Freehand/Lasso** (drag). Shift = square/circle.
- **Actions:** Move (drag) · Delete (Del) · Copy/Paste (Ctrl/Cmd+C/V) · **Grab Text** (right/control-click — SPEC 04) · **Effects** (SPEC 03) · **Invert** (chọn phần còn lại).
- **Snap to Object:** tự snap selection vào mép shape/icon gần nhất trong vùng.
- **Background fill khi move/delete:** **Transparent** (lộ canvas) hoặc **Auto-Fill** (lấp thông minh theo xung quanh).
- **Ưu tiên:** **sau** (P4+); Auto-Fill (content-aware) = khó, gác lâu hơn.
- **Implement C++/Qt:** vùng chọn = `QPainterPath`/mask trên `backgroundRaster`. Cut/copy → `QImage` sub-region vào clipboard (`QClipboard`). Move/delete + Transparent = clear pixel; **Auto-Fill** = inpainting (cân nhắc OpenCV `inpaint`; tạm gác). Snap to Object = edge detection nhẹ.

### 02.13 Move (và Smart Move / Grab Text move)
- **Mô tả (cơ bản):** chọn/di chuyển/resize/rotate/restyle/xóa các **vector object** đã thêm. Double-click object để edit (text/property); Del xóa. **Đây là tool chuyển sang sau khi tạo bất kỳ annotation.**
- **Smart Move:** AI tự phát hiện object **trong chính screenshot** (button, text, UI) và làm chúng edit được tại chỗ — rearrange button, delete/replace text, di UI. Chạy **local OpenCV** (không upload). Hover hiện handle; **Detail** slider chỉnh độ chi tiết; double-click text để sửa. **Add Narration** (Mac).
- **Re-editing model:** Move là cổng vào mọi sửa hậu-tạo, gồm layering/arrange (§02.20).
- **Ưu tiên:** **Move cơ bản = MVP P3** (bắt buộc). **Smart Move = sau** (SPEC 04, OpenCV; SPEC 00 §4 xếp AI cao là "tạm gác").
- **Implement C++/Qt:** Move tool = chế độ tương tác của `CanvasView`: bật `QGraphicsItem::ItemIsSelectable | ItemIsMovable`, hiện selection handles (resize/rotate) cho item chọn, route Del/double-click. **Smart Move** thuộc `ezsnag_simplify` (SPEC 04) — không hiện thực trong MVP của spec này.

### 02.14 Cut Out — RASTER (transform)
- **Mô tả:** xoá **dải ngang/dọc** khỏi ảnh và nối 2 phần còn lại (bỏ whitespace/gap dài). Chọn Quick Style (set **direction** + **edge style**) → click-drag qua dải cần bỏ.
- **Edge:** **straight-edge** → stitch liền 2 phía; **decorative-edge** → để **gap** (không chỉnh được) với mép trang trí.
- **Ưu tiên:** **sau** (P4+) — thuộc nhóm transform ảnh, gần SPEC 03.
- **Implement C++/Qt:** raster transform trên `backgroundRaster`: cắt 2 nửa, ghép lại (đổi canvasSize). Vì làm đổi geometry nền, mọi object vector phải dịch theo nếu nằm sau dải cắt. `QUndoCommand` lưu ảnh + offset object.

### 02.15 Crop — RASTER (transform)
- **Mô tả:** xén mép thừa của ảnh. Kéo **blue handle** định vùng giữ → reposition → click **Crop** áp dụng.
- **Note:** đổi kích thước canvas/ảnh (destructive với pixel bị xén sau khi áp dụng).
- **Ưu tiên:** **⭐MVP P3** (thao tác cơ bản, hay dùng ngay sau chụp).
- **Implement C++/Qt:** overlay crop rect (`QGraphicsView` rubber-band riêng) → áp dụng = `QImage::copy(rect)` + dịch tọa độ mọi object vector về gốc mới + đổi `canvasSize`. Undo lưu ảnh + geometry trước.

### 02.16 Magnify
- **Mô tả:** phóng to một vùng tại chỗ để khoe chi tiết (magnified inset). Click-drag chọn vùng → render phóng to; reposition inset qua **anchor point**; white handle resize.
- **Properties:** **Magnification %** (slider/value) · Outline color · Line width (px) · **Shape** · **Select All (Magnification)** (bản mới).
- **Ưu tiên:** **sau** (P4+).
- **Implement C++/Qt:** `EzsnagMagnifyItem : QGraphicsItem`. Khi paint: lấy vùng nguồn từ ảnh nền (+ object dưới), scale theo %, vẽ vào inset rect với outline. Cần access composite-below → render-on-demand từ document; cân nhắc cache `QPixmap`.

### 02.17 Blur (và Pixelate / Redact)
- **Mô tả:** che thông tin nhạy cảm/gây nhiễu. Click-drag vùng cần che.
- **Types (properties):** **Blur** (mờ ống kính) · **Pixelate** (ô vuông lớn — tốt cho mặt/ID) · **Solid shape** (phủ đặc hoàn toàn — **khuyến nghị cho redact thật** vì không reverse được).
- **Intensity** slider · **Shape** dropdown.
- **Smart Redact:** AI tự phát hiện + che dữ liệu nhạy cảm (email, phone, credit-card…). → thuộc nhóm AI, **tạm gác / SPEC 04**.
- **⚠️ Security / Redaction note (LOUD):** Blur được giữ dạng **object có thể đảo ngược TỚI KHI flatten** (chọn lại đổi/gỡ intensity, hoặc lấy lại pixel gốc từ file `.ezsnagx`). Vì vậy Blur/Pixelate **KHÔNG an toàn** cho dữ liệu nhạy cảm. **Redaction AN TOÀN = Solid shape** (phủ đặc, không reverse được) **hoặc flatten ngay** sau khi che. Đây là **security note cho UI** (cảnh báo user khi redact info nhạy cảm), không chỉ là vấn đề format/serialize.
- **Ưu tiên:** **⭐MVP P3** (Blur + Pixelate + Solid; **Smart Redact = sau**).
- **Implement C++/Qt:** `EzsnagObscureItem : QGraphicsItem` với enum `{Blur, Pixelate, Solid}`. Khi paint: lấy region ảnh nền dưới item, áp filter (Gaussian blur / downscale-upscale cho pixelate / fill đặc) theo intensity, vẽ vào bounds. Re-editable (đổi type/intensity) → giữ là object, **flatten khi export**. (Có thể coi gần raster, nhưng object hoá tiện re-edit — xem ghi chú ở mục Mô hình vector.)

### 02.18 Spotlight
- **Spotlight:** xem **SPEC 03 §03.13** (Spotlight & Magnify effect) — Ezsnagit dùng mô hình **effect baked**; ở đây **không định nghĩa lại**. (Khác với **Magnify §02.16**, vốn là object vector sống.)

### 02.19 Eyedropper (Color picker)
- **Mô tả:** KHÔNG phải tool canvas độc lập — là **control dùng chung** trong mọi color dropdown. Click eyedropper → sample pixel bất kỳ trên màn hình/ảnh → áp vào color hiện tại của tool (fill/outline/text…).
- **Ưu tiên:** **MVP P3** (sample trong canvas). Sample **toàn màn hình** (ngoài app) = sau, cần screen grab.
- **Implement C++/Qt:** widget eyedropper trong color picker. Trong canvas: `QImage::pixelColor` tại điểm click. Toàn màn hình: grab screen (`QScreen::grabWindow`) rồi pick — nền tảng-specific.

### 02.20 Group / Arrange (layering & z-order)
- **Mô tả:** object có thể **group** và **arrange** z-order (bring forward / send backward / to front / to back) qua context menu (right-click) + arrange controls.
- **Ưu tiên:** **arrange z-order = MVP P3** (cần cho object graph); **group = sau**.
- **Implement C++/Qt:** z-order ↔ `zIndex` trong model + `setZValue`. Commands: BringToFront / SendToBack / BringForward / SendBackward (mỗi cái = `ReorderZCommand` trên undo stack). Group = item cha `QGraphicsItemGroup` chứa child; transform chung.
- **Điểm chưa chắc:** menu label + shortcut group/arrange chính xác **không** rõ trong docs → Ezsnagit đặt label riêng (Bring to Front…), verify sau.

### 02.21 Flip / Rotate
- **Mô tả:** object vector lẻ rotate qua **green handle**. Toàn ảnh rotate/flip qua **Image menu** (rotate left/right, flip H/V).
- **Ưu tiên:** **per-object rotate = MVP P3**; **image-level rotate/flip = MVP P3** (cơ bản); per-object flip = sau.
- **Implement C++/Qt:** per-object rotate = `setRotation` quanh `transformOriginPoint`. Image flip/rotate = transform `backgroundRaster` (`QImage::mirrored`, `QTransform().rotate(90)`) + dịch/transform mọi object theo. Image-level thuộc `ezsnag_effects` về bản chất (SPEC 03) nhưng menu ở Editor.
- **Điểm chưa chắc:** per-object flip vs image-level flip label chưa rõ — verify.

---

## Quick Styles / Themes / màu

### Quick Styles (tool presets) — ⭐MVP P3
- Mỗi tool có **Quick Styles** mặc định (tổ hợp property cho 1-click). **Lưu custom:** chỉnh property → preview style mới hiện dưới → click **+/save**. **Reorder:** kéo. **Delete:** right-click → Delete Quick Style.
- **Ưu tiên:** **⭐MVP P3** — đây là **linh hồn tốc độ** của annotation. Làm ngay cùng các tool mà nó style, không để "sau".
- **Implement C++/Qt:** `ezsnag_annotations` định nghĩa `ToolStyle` (struct property theo tool) serialize JSON; UI `QuickStylesStrip` render preview. Lưu vào preferences (SPEC 10).

### Favorites — ⭐MVP P3
- Click **star** trên Quick Style/Quick Effect → thêm vào **Favorites** (bộ chung, cross-tool). Star vàng = đã lưu; click lại bỏ.
- **Ưu tiên:** **⭐MVP P3** (đi kèm Quick Styles — speed soul của annotation).
- **Implement C++/Qt:** flag `favorite` trên `ToolStyle`; view Favorites lọc cross-tool.

### Themes (brand/theme colors)
- **Theme** = palette **4–8 màu** + **1 font** (+ optional drop shadow on/off) để annotation nhất quán toàn project/brand.
- Tạo theme → Ezsnagit **auto-generate Quick Styles** từ màu theme, xuất hiện ở hầu hết tool.
- **Tạo:** chọn màu (preset/palette/HEX/RGB/eyedropper) → pick font → đặt tên → Create. **Edit/Export:** gear icon → export **.ezsnagtheme** chia sẻ; import; tải theme pro từ Assets.
- **Đây là cơ chế brand colors** của Ezsnagit — định nghĩa 1 lần, lan ra Quick Styles khắp nơi.
- **Ưu tiên:** **sau** (P4+) — color picker cơ bản đủ cho MVP; theme system + auto-gen Quick Styles = mở rộng.
- **Implement C++/Qt:** `Theme` (palette + font + shadow flag) serialize **`.ezsnagtheme`**; generator tạo `ToolStyle` từ màu theme. Cloud theme download = tạm gác.

### Color control (dùng chung)
- Color dropdown mọi tool: preset **theme colors** · **HEX/RGB** entry · **Eyedropper** · (Mac) gradient ở vài tool (Callout fill).
- **Implement C++/Qt:** `ui/ColorPickerWidget` dùng lại mọi nơi; gradient = sau.

---

## Undo/redo, copy/paste, snapping/alignment

### Undo / Redo
- Chuẩn: Ctrl/Cmd+Z / Ctrl+Y (hoặc Cmd+Shift+Z); cũng ở Edit menu/toolbar.
- **Implement C++/Qt:** `QUndoStack` trong `CanvasDocument`; mỗi thao tác object = `QUndoCommand`. Edit/View menu nối `createUndoAction/createRedoAction`. **MVP P3** (cốt lõi).

### Copy / Paste / Duplicate
- Ctrl/Cmd+C/V trên selection + object; **Copy All** (Share) copy ảnh flatten vào clipboard. Paste ảnh → object edit mới. Duplicate qua copy-paste.
- **Implement C++/Qt:** copy object → serialize JSON vào clipboard (mime riêng) + raster vào `QClipboard`; paste tạo object/`QGraphicsPixmapItem` mới offset. **MVP P3.**
- **Điểm chưa chắc:** phím Duplicate riêng (Ctrl/Cmd+D?) **không** xác nhận trong docs → Ezsnagit tự gán Ctrl+D, verify.

### Snapping / Alignment
- **Canvas Snapping:** tự align object với nhau / với guide khi kéo. Toggle qua **Image menu → Canvas Snapping**.
- **Shift-constrain:** giữ Shift khi vẽ → square/circle/đường H/V.
- **Snap to Object** (Selection): snap vào mép shape/icon gần nhất.
- **Implement C++/Qt:** snapping = khi kéo, so cạnh/tâm bounding rect các item, hút trong ngưỡng px, vẽ guide line tạm. Toggle lưu preference. **MVP P3** (Shift-constrain bắt buộc; smart guide snapping P3+).
- **Điểm chưa chắc:** lệnh **align/distribute** rõ ràng (ngoài Canvas Snapping) **không** xác nhận → Ezsnagit có thể thêm align/distribute riêng, verify.

---

## Bảng ưu tiên (MVP vs sau)

| Tool / tính năng | Ưu tiên | Ghi chú |
|---|---|---|
| **Canvas Object Graph** (model, z-order, undo/redo, flatten, serialize) | **MVP P3** | Nền tảng — làm trước mọi tool |
| Move tool (cơ bản: select/move/resize/rotate/delete/restyle) | **MVP P3** | Cổng re-edit, bắt buộc |
| Arrow | **⭐MVP P3** | MVP-A; Bezier = sau |
| Text | **⭐MVP P3** | MVP-A; Outline/Select-All cơ bản |
| Callout | **⭐MVP P3** | MVP-A; Gradient/multi-tail = sau |
| Shape / Box (rect/ellipse/polygon) | **⭐MVP P3** | MVP-A |
| Line | **MVP P3** | Bezier = sau |
| Highlight | **⭐MVP P3** | MVP-A; chỉ rect ở MVP |
| Step (thủ công — numbers/circle) | **⭐MVP-B (P3)** | Letters/Roman/shape = sau |
| **Step-Capture (auto chụp khi click)** | **P10 (fast-follow)** | Flagship doc feature; OS-integration nặng (mouse hook + UIA). KHÁC Step tool |
| Stamp (local lib + import PNG) | **MVP P3** | GIF động + cloud Assets = sau |
| Pen (vector mode) | **MVP P3** | Raster mode + smoothing = sau |
| Blur / Pixelate / Solid | **⭐MVP P3** | MVP-A; redact AN TOÀN = solid; Smart Redact = sau |
| Crop | **⭐MVP P3** | MVP-A |
| Per-object rotate + image rotate/flip | **MVP P3** | Per-object flip = sau |
| Eyedropper (trong canvas) | **MVP P3** | Toàn màn hình = sau |
| Arrange z-order (front/back/forward/backward) | **MVP P3** | |
| Undo/redo, copy/paste, Shift-constrain | **MVP P3** | |
| Color picker (HEX/RGB/theme) | **MVP P3** | Gradient = sau |
| Quick Styles + Favorites (save/reorder/delete) | **⭐MVP P3** | Speed soul của annotation |
| Magnify (object sống) | **sau (P4+)** | |
| Spotlight | **→ SPEC 03 §03.13** | Effect baked; không định nghĩa ở SPEC 02 |
| Fill (bucket) | **sau (P4+)** | Raster |
| Eraser | **sau (P4+)** | Raster, brush chưa rõ |
| Selection (rect/ellipse/poly/lasso) | **sau (P4+)** | Auto-Fill inpaint = gác lâu |
| Cut Out | **sau (P4+)** | Transform |
| Group objects | **sau (P4+)** | |
| Themes (palette + auto-gen Quick Styles + export) | **sau (P4+)** | `.ezsnagtheme` |
| Smart Move (OpenCV) | **gác / SPEC 04** | AI cao |
| Smart Redact (AI) | **gác / SPEC 04** | AI |
| Snap-to-guide thông minh, align/distribute | **P4+** | Cần verify lệnh |

---

## Điểm chưa chắc / cần verify

Giữ nguyên các uncertainty từ research §9 + bổ sung quyết định implement:

1. **Eraser** — kích thước/shape brush không rõ trong docs (§9.1). Ezsnagit tự đề xuất brush-size slider + round brush; verify build hiện tại.
2. **Group / Arrange** — menu label + shortcut chính xác chưa rõ (§9.2, §9.6). Ezsnagit đặt label riêng (Bring to Front / Send to Back…); verify.
3. **Per-object Flip vs image-level Flip/Rotate** — label/menu chưa rõ (§9.3). Verify.
4. **Duplicate shortcut** — phím riêng (Ctrl/Cmd+D?) chưa xác nhận (§7, §9.4). Ezsnagit tạm gán Ctrl+D.
5. **Align / Distribute** — lệnh rõ ràng ngoài Canvas Snapping chưa xác nhận (§6, §9.5). Verify.
6. **Z-order command wording** (bring to front / send to back) chưa chốt (§9.6).
7. **Blur là raster hay object** — có thể coi gần raster nhưng re-editable. Ezsnagit chọn **object hoá** (re-edit, flatten khi export); xác nhận hành vi mong muốn. **Lưu ý security:** vì object-blur reversible tới khi flatten → redact AN TOÀN phải là **solid shape** (xem §02.17).
8. ~~Spotlight tool-vs-effect~~ — **ĐÃ CHỐT:** Spotlight là **effect baked**, ownership ở **SPEC 03 §03.13**; SPEC 02 chỉ cross-reference (§02.18). Magnify (§02.16) vẫn là object sống. *(Ambiguity cũ đã resolve.)*
9. **Cross-platform effect loss** (format editable Mac → Windows mất effect) (§2 caveat) — Ezsnagit single-codebase Qt nên không gặp; nhưng `.ezsnagx` phải định nghĩa rõ effect serialization (SPEC 03/09).
10. **Recent features version-gated** (Select-All-like-objects, Smart Move, Smart Redact, Simplify color palettes) — xác nhận với build mục tiêu 2026.

> **Quyết định kiến trúc cần review:** chọn `QGraphicsScene/QGraphicsItem` cho toàn bộ object graph (thay vì custom `QWidget` paint). Nếu sau này cần custom blending/effect pipeline phức tạp vượt khả năng QGraphicsView, cân nhắc render layer riêng — nhưng MVP đi với QGraphicsScene để tận dụng hit-test/z-order/transform sẵn có.
