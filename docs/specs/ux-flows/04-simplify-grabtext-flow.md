# UX FLOW 04 — Simplify, Smart Move & Grab Text (Ezsnagit)

> Nguồn: docs/research/03-effects-ocr.md · Spec: docs/specs/04-simplify-grabtext.md · Module: `ezsnag_simplify` (OpenCV) + `ezsnag_ocr` (Tesseract)

Các luồng thao tác từng-bước cho Simplify, Smart Move và OCR. Tiền đề: đã có capture mở trong Editor.

---

## Flow A — Simplify ảnh UI (Auto Simplify)

1. Chọn tool **Simplify** trên toolbar → mở Tool Properties.
2. Bật **Auto Simplify** → hệ thống (OpenCV) quét ảnh và **phủ shape trừu tượng** (thanh/khối/rounded rect màu) lên text & phần tử UI phát hiện được.
3. Kéo **Detail slider**:
   - kéo phải → nhiều object nhỏ, chi tiết hơn;
   - kéo trái → ít khối thô, gọn hơn.
   - Mỗi lần kéo → tập object **regenerate** lại.
4. Mở panel **Color Palette** → click dropdown màu để recolor các object cho khớp brand. (Gear icon → **Save Palette** để tái dùng cho ảnh khác.)
5. Tinh chỉnh thủ công nếu cần:
   - **Move Mode** → kéo dời / resize handle / Delete object thừa.
   - **Draw Mode** → kéo thả **Quick Style** thêm shape ở chỗ detector bỏ sót.
   - Sửa per-object: fill color, shape type, background color, text height, line spacing.
6. Bật **Show Original** + kéo opacity slider → thấy screenshot thật mờ bên dưới để canh shape cho khớp.
7. Tắt Show Original. Simplify object vẫn edit được tới khi export; **Export PNG** → flatten thành ảnh generic.

**Edge case:** UI phức tạp → detector bắt thiếu/thừa; chấp nhận chỉnh tay (xem điểm chưa chắc ở spec). Bật **Lock Objects** để tránh sửa nhầm khi chuyển tool.

---

## Flow B — Smart Move: dời/xóa phần tử trong screenshot phẳng

1. Chọn **Move** tool → trong Tool Properties bật **Smart Move**.
2. Hệ thống (OpenCV) phát hiện UI element; **hover** lên một phần tử → hiện selection handle.
3. Chỉnh **Detail slider** nếu nhóm sai (phải = tách nhỏ hơn, trái = gộp nhiều hơn).
4. **Dời:** click-drag phần tử (vd nút) sang vị trí mới → vùng cũ được **auto-fill** khớp nền xung quanh (inpaint).
5. **Xóa:** chọn phần tử → phím **Delete** → vùng trống cũng auto-fill nền.
6. Kiểm tra vùng auto-fill: nếu lộ vết (nền phức tạp), `Ctrl+Z` và cân nhắc che bằng annotation/Fill thủ công.

**Phân biệt:** Smart Move giữ *hình thức thật* của phần tử (chỉ dời/xóa). Nếu muốn *thay bằng shape trừu tượng* → dùng **Simplify** (Flow A).

---

## Flow C — Grab Text → copy

1. **Tăng độ chính xác:** dùng **Selection** tool khoanh sát vùng text cần lấy (vùng nhỏ chính xác hơn toàn ảnh).
2. **Right-click** vùng chọn → **Grab Text**.
3. Tesseract OCR vùng đó → mở dialog **Grab Text Results** với text nhận diện.
4. (Tùy chọn) bật **Plain text** để strip font/màu.
5. Bấm **Copy All** → text vào clipboard, paste sang nơi khác.

**Biến thể (toàn ảnh):** right-click ảnh (không cần selection) → Grab Text → OCR toàn canvas.
**Biến thể (capture-time):** ở Capture window tab Image → dropdown Selection chọn **Grab Text** → bấm Capture → drag chọn vùng màn hình → Results dialog mở trong Editor.
**Best practice:** contrast cao chữ/nền; light-on-light nhận kém → chọn vùng rõ ràng.

---

## Flow D — Edit Text tại chỗ (sửa label trên screenshot)

1. Dùng **Selection** tool khoanh vùng text cần sửa (vd một label/giá trị).
2. **Right-click** → **Edit Text**.
3. OCR (Tesseract) nhận text + ước lượng font/màu → tạo **text object edit được** đặt đúng chỗ; pixel chữ gốc được che bằng auto-fill nền.
4. Qua **Tool Properties** chỉnh **font / size / color**, hoặc click vào text **replace** từ, chèn text mới, xóa ký tự — ngay trên screenshot.
5. Click ra ngoài → text object vẫn vector, edit lại được sau. **Export** → flatten.

**Edge case:** nền dưới chữ phức tạp → auto-fill che chữ gốc có thể lộ; chọn vùng nền đồng màu cho kết quả tốt, hoặc che thêm bằng Fill/annotation.

---

## Flow E — Quản lý ngôn ngữ OCR

1. Preferences → mục **OCR Language** → chọn trong gói đã đóng: `eng` (mặc định), `deu`, `fra`, `spa`, `jpn`.
2. (Tùy chọn) thêm gói `traineddata` khác để mở rộng ngôn ngữ.
3. Lưu ý **Japanese** là hạn chế (cả gốc lẫn Tesseract) → cần ảnh chất lượng cao, contrast tốt.

---

## Ghi chú trạng thái

- Simplify object & Smart Move object là **object vector** trong canvas → có undo/redo, z-order, edit lại tới khi export flatten.
- Toàn bộ CV (OpenCV) và OCR (Tesseract) chạy **local/offline** — không upload ảnh, không dùng để train (bám nguyên tắc privacy của research).
- OCR/Simplify chất lượng phụ thuộc ảnh đầu vào; khuyến khích người dùng chọn vùng rõ ràng và chấp nhận chỉnh tay khi detector/inpaint chưa hoàn hảo.
