# UX Flows — Editor & Annotation (Ezsnagit)

> Bổ trợ cho SPEC 02 (`docs/specs/02-editor-annotation.md`). Các luồng thao tác từng-bước trong Editor.
> Quy ước: **Move tool** là tool re-edit chung; mọi vector object tạo ra đều chọn lại được bằng Move tool.

---

## Flow chung (Universal) — vẽ một annotation

1. Mở capture trên Canvas (từ chụp xong hoặc chọn thumbnail ở Recent Tray).
2. Click tool trên **Toolbar** (ví dụ Arrow). → **Quick Styles** + **Tool Properties** hiện bên phải.
3. Chọn một **Quick Style** preset, hoặc chỉnh Color / Width / Opacity / Shadow… trong Properties.
4. **Click** (hoặc **click-drag**) trên Canvas để tạo object.
5. Gõ/điều chỉnh nếu là tool có text (Text, Callout, Step).
6. Chuyển sang **Move tool** để chọn lại, di chuyển, resize, restyle, hoặc xóa.
7. (Tùy chọn) Lưu style vừa dùng thành Quick Style mới: click **+** dưới hàng Quick Styles.

---

## Flow: Dùng tool Arrow

1. Toolbar → **Arrow**.
2. Properties bên phải: chọn Color, End Style (kiểu đầu mũi), Line Style (solid/dashed/dotted), Width, Opacity, Shadow.
3. Trên Canvas: **click-drag từ tail → head**. Mũi tên hướng theo chiều kéo.
4. Giữ **Shift** khi kéo để ràng buộc ngang/dọc/45°.
5. (Tùy chọn) Bật **Bezier curve** trong Properties → kéo các **white handle** để uốn cong thân mũi tên.
6. Thả chuột → mũi tên thành object.
7. **Re-edit:** Move tool → click mũi tên → kéo endpoint để đổi hướng/độ dài, đổi property, hoặc Del để xóa.

---

## Flow: Tạo Callout (speech bubble)

1. Toolbar → **Callout**.
2. Properties: chọn **Shape Style** (rounded rect, speech bubble, oval…), Fill color, Outline color, Tail style (line/end/size/color), Shadow, và thuộc tính Text (font/size/align).
3. Trên Canvas: **click-drag** vẽ thân callout.
4. **Gõ text** ngay trong callout (cursor đặt sẵn).
5. Điều chỉnh đuôi: kéo **yellow handle** = di đầu đuôi tới đối tượng cần chỉ; **anchor point** = dời thân so với đuôi.
6. (Tùy chọn) Kéo **blue handle** thêm một đuôi thứ hai; **green handle** xoay callout; **white handle** resize.
7. **Re-edit:** Move tool → double-click để sửa text; kéo handle reshape/di tail/rotate; restyle qua Properties.

---

## Flow: Step tool đánh số (sequence)

1. Toolbar → **Step**.
2. Properties: chọn **Type** (numbers 1,2,3 / letters A,B,C / Roman), Fill color, Text color, Shape (circle/square), Shadow.
3. Trên Canvas: **click** chỗ đầu tiên → thả badge **1**.
4. Click chỗ tiếp theo → tự thành **2**, click nữa → **3**… (auto-increment).
5. Sai số? Right-click một step → **Edit Value** để đặt số/chữ thủ công.
6. Cần bắt đầu chuỗi mới? Properties → **Restart Sequence** → các click sau đếm lại từ đầu.
7. **Re-edit:** Move tool → kéo step để reposition; nếu xóa một step, hệ thống **renumber** các step còn lại hợp lý.

---

## Flow: Blur vùng nhạy cảm (redact)

1. Toolbar → **Blur**.
2. Properties: chọn **Type**:
   - **Blur** — làm mờ kiểu ống kính (che nhẹ).
   - **Pixelate** — ô vuông lớn (tốt cho mặt/ID).
   - **Solid shape** — phủ đặc hoàn toàn → **dùng cho redact thật** (không reverse được).
3. Chỉnh **Intensity** slider và **Shape** (dropdown).
4. Trên Canvas: **click-drag** phủ vùng cần che (email, số thẻ, mặt…).
5. **Cảnh báo bảo mật:** với dữ liệu thực sự nhạy cảm, KHÔNG dùng Blur/Pixelate đơn thuần (có thể đảo ngược một phần) — chọn **Solid shape**.
6. **Re-edit:** Move tool → chọn object → đổi type/intensity hoặc resize vùng. Object được **flatten** khi export PNG/JPG.

> *(Smart Redact — tự phát hiện email/phone/credit-card — là tính năng AI, xem SPEC 04; không thuộc luồng MVP này.)*

---

## Flow: Re-edit một object đã vẽ

1. Toolbar → **Move tool**.
2. Trên Canvas: **click** object cần sửa → hiện **selection handles**.
   - **White handle** = resize.
   - **Green handle** = rotate.
   - Handle đặc thù tool (Callout: yellow/blue tail; Arrow/Line: endpoint).
3. Sửa:
   - **Di chuyển:** kéo thân object (Canvas Snapping tự align nếu bật).
   - **Resize:** kéo white handle.
   - **Rotate:** kéo green handle.
   - **Restyle:** Properties panel bên phải (color/width/opacity/shadow…).
   - **Edit text** (Text/Callout/Step): **double-click** để đặt cursor và sửa.
   - **Xóa:** phím **Delete**.
4. **Bulk edit:** với tool hỗ trợ, Properties có **Select All (<tool>)** → sửa mọi object cùng loại một lần (Step, Text, Arrow, Magnify, Pen).
5. Sai thì **Ctrl/Cmd+Z** (Undo) — mọi thao tác object đều undo/redo được.

---

## Flow: Đổi z-order (layering)

1. Toolbar → **Move tool** → click object cần đổi lớp.
2. **Right-click** object → menu **Arrange** (hoặc dùng arrange controls):
   - **Bring to Front** — đưa lên trên cùng.
   - **Bring Forward** — lên 1 lớp.
   - **Send Backward** — xuống 1 lớp.
   - **Send to Back** — xuống dưới cùng (ngay trên ảnh nền).
3. Object được vẽ lại theo z-order mới (object trên đè object dưới).
4. **Undo** (Ctrl/Cmd+Z) nếu nhầm.

> Ghi chú implement: z-order ↔ `zIndex` trong `CanvasDocument` + `QGraphicsItem::setZValue`; mỗi lệnh là một `ReorderZCommand` trên `QUndoStack`. Label menu (Bring to Front…) là đề xuất — cần verify (SPEC 02, Điểm chưa chắc §2/§6).

---

## Flow phụ: Group objects (sau MVP)

1. Move tool → giữ Shift / rubber-band chọn nhiều object.
2. Right-click → **Group** → các object thành một nhóm; di chuyển/transform chung.
3. **Ungroup** để tách lại.

> Ưu tiên: sau MVP (SPEC 02). Implement: `QGraphicsItemGroup`.

---

## Flow phụ: Crop ảnh

1. Toolbar → **Crop**.
2. Trên Canvas: kéo **blue handle** định vùng giữ lại; kéo thân để reposition vùng.
3. Click nút **Crop** áp dụng → ảnh/canvas đổi kích thước; pixel ngoài vùng bị xén.
4. **Undo** nếu cần (Crop là destructive sau khi áp dụng).

---

## Flow phụ: Mở rộng canvas (thêm vùng trắng)

1. Trên Canvas: kéo **handle ở mép canvas** ra ngoài → thêm vùng canvas trắng quanh ảnh.
2. Dùng vùng trắng mới để đặt thêm annotation/text ngoài ảnh gốc.

---

## Flow phụ: Lưu & flatten

1. **Lưu editable:** File → Save → định dạng **`.ezsnagx`** (giữ mọi object để mở lại edit tiếp).
2. **Export raster:** File → Export → PNG/JPG → mọi object **flatten** vào bitmap (không còn edit riêng).
3. **Flatten All** (menu): nung tất cả object xuống nền raster ngay trong editor (chủ động, không reverse — cân nhắc trước khi dùng).
4. **Copy All** (Share toolbar): copy ảnh đã flatten vào clipboard.
