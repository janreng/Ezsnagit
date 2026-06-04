# UX FLOW 06 — Library (Ezsnagit)

> Spec: 06-library.md · Module: `ezsnag_library`

Luồng từng bước cho Library: từ lúc capture được auto-save, qua Recent Captures Tray, tới Library View (tìm/lọc/sort), và tag/flag/quản lý.

---

## Flow A — Capture được auto-save vào Library

1. Người dùng hoàn tất một capture (ảnh/video) ở Capture Window.
2. Capture mở trong Editor trên canvas.
3. **Hệ thống tự lưu** (không cần lệnh Save):
   - Ảnh → ghi `.ezsnagx` vào Library store + upsert bản ghi SQLite.
   - Video chưa lưu → ghi `.mp4` vào store + bản ghi SQLite.
4. Thumbnail capture xuất hiện ở **bên phải Recent Captures Tray**.
5. Mọi edit/annotation tiếp theo → auto-save debounce cập nhật lại `.ezsnagx`.
6. Nếu auto-save lỗi → hiện cảnh báo "Không lưu được vào Library".

→ Capture giờ là item bền vững trong Library cho đến khi xóa rõ ràng.

---

## Flow B — Dùng Recent Captures Tray

1. Người dùng nhìn dải Tray ở đáy Editor.
2. **Mở lại:** click thumbnail → capture load lên canvas để edit tiếp.
3. **Ghim:** click icon pin trên thumbnail → item neo về **trái** Tray; capture mới vẫn vào bên phải.
4. **Sắp xếp:** kéo–thả thumbnail để đổi thứ tự (gom asset một project).
5. **Nhận biết chưa lưu:** thumbnail có edit chưa export hiện icon **sunburst cam**.
6. **Gỡ khỏi Tray:** clear một item khỏi Tray → item **không bị xóa**, vẫn nằm trong Library View.

---

## Flow C — Mở Library View và tìm/lọc

1. Click tab **Library** ở đầu Editor → mở grid toàn bộ capture.
2. **Tìm theo tên:** gõ vào ô Search → kết quả lọc **live** theo filename.
3. **Lọc theo category** (sidebar tự sinh từ dữ liệu):
   - File Type (Images / Videos / Animated GIFs)
   - Date (năm → tháng → ngày)
   - Applications (app đã chụp từ đó)
   - Websites (URL)
   - Tags / Flags / Favorites
4. **Kết hợp:** chọn một/nhiều category + ô Search → thu hẹp kết quả (AND).
5. **Sort:** click heading để sort theo date modified / date created / name / size; click lại để đảo chiều.
6. Click thumbnail xem trước; **double-click** → load vào Editor.

---

## Flow D — Tag & Flag

1. Trong Tray/Library, chọn 1+ capture (Ctrl/Cmd+click nhiều, Shift+click range).
2. **Tag:** bấm nút **Tag** → gõ tag mới hoặc chọn tag có sẵn → áp cho toàn bộ mục đã chọn (một transaction).
3. **Flag:** chọn một flag từ tập marker cố định → gắn lên capture (nổi bật trong Tray/Library).
4. Lọc lại sau: vào category **Tags** / **Flags** trong sidebar Library để gom các capture đã gán.

---

## Flow E — Quản lý capture (open / metadata / delete)

1. **Open:** double-click thumbnail → load Editor.
2. **Xem metadata:** right-click (Win) / Ctrl+click (Mac) → ngày tạo, source app/website.
3. **Delete:** right-click → Delete (Win) / Move to Trash (Mac).
   - Multi-select để xóa hàng loạt.
   - Nếu bật prompt xác nhận → hỏi trước khi xóa.
   - Xóa = bỏ bản ghi SQLite + file trong store.
4. **Vào Templates:** right-click selection → Combine in Template / Apply Template / Create Video from Images (chuyển sang flow Templates).

---

## Flow F — Di chuyển vị trí Library (storage)

1. Preferences > Library → "Library location management".
2. Chọn thư mục đích mới.
3. Hệ thống move file store + cập nhật base path (DB lưu đường dẫn **tương đối** nên tham chiếu không vỡ).
4. (Tùy chọn) đặt thư mục đó trong cloud sync (OneDrive/Drive/Dropbox) để backup + đa thiết bị — Library gốc vẫn local.

---

## Trạng thái & lỗi cần xử lý

- Auto-save thất bại → cảnh báo, giữ document trong RAM, cho retry.
- Xóa nhầm → (đề xuất) Trash nội bộ để undo trước khi xóa vĩnh viễn.
- File store mất/hỏng nhưng bản ghi DB còn → đánh dấu "missing", không crash grid.
- Library rỗng lần đầu → empty state hướng dẫn capture đầu tiên.
