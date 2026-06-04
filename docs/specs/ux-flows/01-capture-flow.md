# UX Flows — Capture (Ezsnagit)

> Tham chiếu: docs/specs/01-capture.md · Nguồn: docs/research/01-capture.md
> Phím tắt ghi theo Windows; Mac trong ngoặc khi khác. Mỗi flow liệt kê từng bước 1→2→3 kèm thay đổi trạng thái UI.

---

## Flow A — All-in-One capture (đường nhanh mặc định)

1. Mở Capture Window (icon tray/menu-bar, hoặc nút **Capture** trong Editor). → *Capture Window hiện, đang ở tab.*
2. Chọn tab **All-in-One**. → *Settings pre-selected của tab load.*
3. Click nút **Capture** (hoặc **Print Screen** / Mac **Control+Shift+C**). → *Capture Window ẩn; màn hình **dim nhẹ**; **orange crosshairs** khóa theo cursor; **magnifier loupe** + **dimension readout** đi theo cursor.*
4. Đặt selection, một trong hai:
   - **Hover window/UI element** → viền **orange dashed** snap quanh vùng phát hiện. → *click để chọn vùng đó.*
   - **Click & drag** vẽ region chữ nhật → *readout hiện live W×H* → thả để chốt.
5. → *On-screen toolbar (orange) hiện cạnh selection: Image / Video / Scrolling (+ likely Grab Text / Dimensions / Effects / Cancel).*
6. Click **Image Capture** (chốt finalize, hoặc **Enter**). → *Capture mở trong **Editor** để preview/edit/share* (hoặc đi thẳng tới share destination nếu đã cấu hình).
- **Hủy bất kỳ lúc nào:** **Esc** → *thoát chụp, trả về desktop, Capture Window trở lại nền.*

---

## Flow B — Chụp vùng (Region)

1. Capture Window → tab **Image**. → *Image settings hiện.*
2. Selection dropdown = **Region**. → *mode Region active.*
3. Click **Capture** (hoặc **Print Screen**; hoặc **Shift+Enter** = Start Region Selection). → *crosshairs + magnifier hiện.*
4. **Click & drag** chọn vùng. → *readout live W×H.*
5. Thả chuột (hoặc **Enter** để finalize). → *capture chốt, mở trong Editor.*
- **Tùy chọn:** mở **Selection properties** nhập W×H + tọa độ chính xác trước khi thả.

---

## Flow C — Chụp Window

1. Capture Window → **Image** → Selection = **Window**. → *mode Window active.*
2. Click **Capture**. → *crosshairs hiện.*
3. **Hover** window mục tiêu tới khi viền **orange dashed** bao quanh nó. → *highlight window detect được.*
4. **Click** window. → *chụp window, mở trong Editor.*

---

## Flow D — Fullscreen

1. Capture Window → **Image** → Selection = **Fullscreen**. → *mode Fullscreen active.*
2. Click **Capture** (hoặc hotkey; Mac **F**). → *chụp **monitor dưới cursor / active** (chỉ 1 monitor), mở trong Editor.*

---

## Flow E — Grab Text capture (OCR)

1. Capture Window → **Image** → Selection = **Grab Text**. → *mode Grab Text active.*
2. Click **Capture** (**Print Screen** / Mac **Shift+Ctrl+C** / **Shift+Command+O**). → *crosshairs hiện.*
3. **Click & drag** vùng có text. → *readout W×H.*
4. Thả. → *dialog **Grab Text Results** mở trong Editor với text nhận diện.*
5. Click **Copy All** (hoặc select + copy). → *text vào clipboard, dán nơi khác được.*
- **Kết hợp:** bật scrolling trước để OCR cả nội dung dài (vd spreadsheet) thành dữ liệu editable.

---

## Flow F — Scrolling capture: Automatic (arrows)

1. Mở nội dung cần chụp (web page / doc dài / spreadsheet rộng).
2. Capture Window → **All-in-One** (hoặc **Image**) → click **Capture**. → *crosshairs hiện.*
3. Đưa cursor lên window scrollable. → ***directional scrolling arrows** hiện trên/cạnh window (nếu app hỗ trợ auto-scroll).*
4. Click arrow phù hợp (hoặc hotkey):
   - **Horizontal** — full width — **H** (Mac **R**).
   - **Vertical** — full height — **V** (Mac **D**).
   - **Both/diagonal** — toàn trang — **B** (Mac **X**).
   → *Ezsnagit **tự auto-scroll + stitch** nội dung.*
5. → *Ảnh ráp xong mở trong Editor.*
- *(Mac hotkeys R/D/X — cần verify build.)*

---

## Flow G — Scrolling capture: Manual / Panoramic

1. Capture Window → **All-in-One** → **Capture** (**Print Screen** / **Ctrl+Shift+C**). → *crosshairs hiện.*
2. **Drag chọn vùng** trong window. → *selection chốt, on-screen toolbar hiện.*
3. Click nút **Scrolling Capture** trên toolbar. → *vào chế độ panoramic; (nếu preference "Require Start button" bật → chờ **Start**).*
4. **Tự scroll** xuống/lên/ngang bằng **scroll bar** (ưu tiên hơn mouse wheel để tránh hover state). → *Ezsnagit capture từng phần khi cuộn.*
5. Click **Done**. → *ảnh stitched mở trong Editor.*
- **Lưu ý:** scroll mượt, không gồm preview/control bar vào ảnh.

---

## Flow H — Đổi mode trên on-screen toolbar (decision point All-in-One)

1. Đang ở bước on-screen toolbar (sau Flow A bước 5). → *toolbar hiện: Image / Video / Scrolling.*
2. Chọn một toggle:
   - **Image Capture** → *finalize thành screenshot tĩnh → Editor.*
   - **Video Capture** → *chuyển sang quay region đã chọn → mở video options / countdown* (chi tiết SPEC 05).
   - **Scrolling Capture** → *vào panoramic (tiếp tục như Flow G bước 4)*, hoặc nếu hover ra arrows → auto-scroll (Flow F).
3. → *Trạng thái UI đổi theo toggle; **Esc** hủy về desktop.*

---

## Flow I — Time Delay capture

1. Capture Window → chọn Selection type bất kỳ → bật **Time Delay** → chọn **Delay** + nhập số giây. → *delay armed.*
2. Click **Capture** (hoặc **Print Screen**). → *Capture Window ẩn; **countdown** hiện ở **góc dưới-phải** màn hình.*
3. Trong lúc đếm: dàn dựng màn hình (vd mở menu cho Menu capture). → *màn hình tương tác bình thường.*
4. Countdown hết → *chụp fire tự động → mở trong Editor.*
- **Biến thể:** **Schedule** (chụp tại ngày/giờ) · **Interval** (time-lapse lặp, Windows only).

---

## Flow J — Menu capture

1. Capture Window → **Image** → Selection = **Advanced > Menu**. → *mode Menu active.*
2. (Windows) tùy chọn click **Menu Properties** cấu hình sub-menu/menu options. → *dialog properties.*
3. Bật **Time Delay** (đủ giây để mở menu). → *delay armed.*
4. Click **Capture** → *countdown bottom-right.*
5. Mở menu drop-down mục tiêu trong lúc đếm. → *menu đang mở.*
6. Countdown hết → *chụp gồm cả menu → Editor.*

---

## Flow K — Multiple Area

**Windows:**
1. Capture Window → **Image** → Selection = **Advanced > Multiple Area** → **Capture**. → *orange crosshairs hiện.*
2. Click windows và/hoặc drag regions, lần lượt. → *mỗi vùng add vào selection set.*
3. Click nút **Finish** ở đầu màn hình. → *gộp tất cả thành 1 capture → Editor.*

**Mac:**
1. **All-in-One** → **Capture** → **giữ Command** và drag chọn nhiều vùng. → *các vùng tích lũy.*
2. Thả Command. → *kết thúc, gộp → Editor.*

---

## Flow L — Freehand (Windows)

1. Capture Window → **Image** → Selection = **Advanced > Freehand** → **Capture**. → *cursor thành **scissors**.*
2. **Click & drag** quanh vùng để outline hình bất kỳ. → *đường viền vẽ theo.*
3. Thả. → *chụp hình bao kín → Editor.*

---

## Flow M — Webcam image capture (Mac)

1. Capture Window → **Image** → Selection = **Webcam** → **Capture**. → *preview webcam.*
2. Chọn camera device từ dropdown. → *device active.*
3. Kích hoạt capture. → *ảnh tĩnh từ webcam → Editor.*

---

## Flow N — Chụp qua OneClick (Capture Widget)

1. Hover **tab/toolbar nổi** ở mép màn hình. → *widget expand hiện preset buttons.*
2. Click một preset. → *chụp với settings cố định (All-in-One, Preview On, Cursor On, Delay None, Effects None, Share None).*
3. → *capture mở trong Editor (Preview On).*
- **Lưu ý:** OneClick **không** kế thừa settings hiện hành của Capture Window.

---

## Flow O — Đổi global hotkey

1. Mở Capture Window → click **Shortcut** field. → *field chờ nhận tổ hợp.*
2. Bấm tổ hợp phím mong muốn. → *hotkey mới ghi nhận.*
3. (Windows 11, nếu chọn Print Screen) nếu Print Screen mở Snipping Tool → vào **Windows Settings → Accessibility → Keyboard** tắt **"Use the Print screen key to open screen capture"** → restart. → *Print Screen nhường cho Ezsnagit.*
- Preset hotkey đặt riêng trong cấu hình từng preset.

---

## Flow P — Repeat Last Capture

1. Sau một lần chụp, bấm **Repeat Last Capture** (Mac **Control+Shift+R**; Windows key **uncertain — verify**). → *lặp lại lần chụp gần nhất với cùng settings.*
