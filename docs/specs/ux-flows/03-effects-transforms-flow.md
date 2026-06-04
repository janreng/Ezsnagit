# UX FLOW 03 — Effects & Transforms (Ezsnagit)

> Spec: docs/specs/03-effects-transforms.md · Module: `ezsnag_effects`

Tài liệu mô tả các luồng thao tác từng-bước trong Editor cho effect và transform. Tiền đề chung: người dùng đã có một capture mở trong Editor (canvas = ảnh nền + có thể có annotation object).

---

## Quy ước UI

- **Effects panel:** nút **Effects** trên thanh phải/trên Editor mở panel danh sách effect.
- **Apply flow:** chọn effect → chỉnh property trong panel → bấm **Apply**. Effect được nung vào canvas (lưu undo trước khi nung).
- **Quick Effects:** dải preset; bấm **+** để lưu tổ hợp hiện tại, bấm preset để áp lại 1 click.
- **Transforms** truy cập qua menu `Image` (Resize / Trim / Canvas Color / Rotate / Flip) hoặc tool trên toolbar (Crop, Cut Out).

---

## Flow A — Áp Border + Shadow (tổ hợp khung ảnh)

1. Mở **Effects** panel.
2. Chọn **Border** → đặt **màu** (eyedropper lấy màu từ ảnh hoặc nhập hex) → kéo **width** (vd 4px) → preview cập nhật trực tiếp.
3. Bấm **Apply** → border được nung; undo point được tạo.
4. Chọn tiếp **Shadow** → chọn **hướng** (vd dưới-phải) → chỉnh **offset / blur / opacity** → preview.
5. Bấm **Apply** → bóng đổ phía sau ảnh (canvas tự mở rộng để chứa bóng).
6. (Tùy chọn) Bấm **+** ở Quick Effects để lưu tổ hợp "Border+Shadow" thành preset áp lại sau này.
7. Nếu không ưng: `Ctrl+Z` lần lượt gỡ Shadow rồi Border.

**Edge case:** nếu canvas trong suốt (Canvas Color = transparent), Shadow vẫn render trên vùng alpha và export PNG giữ bóng.

---

## Flow B — Trim viền thừa (Auto-Trim)

1. Ảnh có viền/whitespace dư quanh nội dung (vd screenshot có lề trắng).
2. Menu `Image > Trim` (hoặc nút **Auto-Trim**).
3. (Tùy chọn) chỉnh **tolerance** màu và chọn **corner tham chiếu** (mặc định top-left).
4. Hệ thống quét từ mỗi cạnh vào trong, dừng nơi pixel lệch màu góc vượt tolerance → tính bounding box nội dung.
5. Preview hiển thị khung box đề xuất; bấm **Apply** → canvas crop về box, viền thừa biến mất.
6. Nếu trim quá tay (ăn vào nội dung): `Ctrl+Z`, tăng tolerance hoặc đổi corner rồi thử lại.

**Edge case:** viền không đồng màu (gradient) → Auto-Trim có thể không đủ; fallback dùng **Crop** thủ công.

---

## Flow C — Cut Out dải giữa (bỏ khoảng trống ảnh dài)

1. Ảnh dài (vd screenshot cuộn) có một khoảng trống lớn ở giữa cần bỏ.
2. Chọn tool **Cut Out** trên toolbar.
3. Chọn **Quick Style**: hướng **Horizontal** + kiểu mép.
   - **Straight edges** → ghép liền mạch.
   - **Decorative edges** → chừa gap + đường xé báo hiệu đã cắt.
4. **Click-drag** dọc theo ảnh để khoanh dải ngang cần xóa (full-width).
5. Thả chuột → hai phần trên/dưới được ghép lại theo kiểu mép đã chọn; ảnh ngắn lại.
6. Chuyển sang tool khác (vd Move) để **deactivate** Cut Out.

**Biến thể (Add Space):** chọn style **Add Horizontal/Vertical Space** rồi drag → ảnh *giãn ra* thêm dải nền (canvas color) thay vì cắt.
**Edge case:** áp lại Horizontal trong một gap decorative có sẵn → gap thu nhỏ lại.

---

## Flow D — Resize Image giữ tỉ lệ

1. Menu `Image > Resize Image` (hoặc click số kích thước ở đáy canvas).
2. Chọn **unit** (vd pixels). Bật **lock aspect ratio**.
3. Nhập width mới → height tự tính theo tỉ lệ.
4. (Tùy chọn nâng cao) đặt **DPI/Resolution** và bật **Resampling**; nếu upscale, hệ thống cảnh báo có thể giảm chất lượng vĩnh viễn.
5. Bấm **Apply** → ảnh scale (smooth). Undo khôi phục kích thước cũ.

---

## Flow E — Rotate & Flip (kèm mirror annotation)

1. **Rotate preset:** menu `Image > Rotate > 90° CW` → ảnh + mọi annotation xoay 90° không mất chất lượng.
2. **Rotate custom:** nhập góc (vd 12°) → canvas mở rộng chứa hình xoay; render smooth.
3. **Flip:** `Image > Flip Horizontal` → ảnh nền VÀ text/arrow/callout đều bị **lật gương** (không chỉ nền). Người dùng kiểm tra text không bị ngược nếu không mong muốn → `Ctrl+Z`.

**Lưu ý:** Flip phối hợp `ezsnag_canvas` biến đổi toạ độ object, vì vậy annotation cũng mirror đúng vị trí.

---

## Flow F — Watermark logo (Overlay vs Underlay)

1. Mở **Effects** → **Watermark**.
2. Chọn nguồn: **image** → load file logo (PNG có alpha), hoặc **text** → nhập chuỗi + chọn font.
3. Đặt **position grid** (vd góc dưới-phải) và **scale** slider.
4. Chọn mode:
   - **Overlay** → logo đè lên trên ảnh (chỉnh opacity nếu cần).
   - **Underlay** → logo dập chìm/emboss vào nền.
5. Bấm **Apply** → watermark nung vào ảnh.

---

## Flow G — Spotlight & Magnify một vùng

1. Dùng **Selection** tool khoanh vùng cần làm nổi.
2. Menu `Image > Effects > Spotlight and Magnify`.
3. Chỉnh: mức **magnify** (bật/tắt phóng to), độ **tối/mờ** nền xung quanh.
4. Preview: nền bị dim/blur, vùng chọn được phóng to & sắc nét.
5. Bấm **Apply** → nung thành ảnh (đây là baked effect, KHÔNG phải object Magnify sống).

**Phân biệt với annotation Magnify:** nếu cần một lens **vẫn edit được sau này**, dùng tool **Magnify** ở `ezsnag_annotations` (spec 02), không dùng effect này.

---

## Flow H — Canvas Color → nền trong suốt

1. Menu `Image > Canvas Color`.
2. Click ô preview màu → chọn màu, hoặc chọn **Transparent** (checkerboard).
3. Bấm **OK/Apply** → vùng trống canvas thành màu chọn hoặc alpha=0.
4. Export PNG → giữ alpha (nền trong suốt).

---

## Ghi chú trạng thái & undo

- Mọi effect/transform tạo **một undo point** trước khi nung → `Ctrl+Z` gỡ từng bước.
- Resize/Expand Canvas giữ lịch sử non-destructive qua `ezsnag_canvas`.
- Selection-scoped effect (chỉ áp lên vùng chọn) là tùy chọn nâng cao; mặc định mọi effect áp toàn canvas.
