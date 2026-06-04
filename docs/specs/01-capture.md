# SPEC 01 — Capture (Ezsnagit)

> Nguồn: docs/research/01-capture.md · Module: `ezsnag_capture`, `ui/CaptureWindow`

## Mục tiêu & phạm vi

Spec này định nghĩa toàn bộ trải nghiệm **chụp màn hình** của Ezsnagit — bản clone C++/Qt6 của Snagit. Bao trùm:

- **Capture Window** (bề mặt khởi động chụp, thường trú, gọi bằng hotkey toàn cục) với 3 capture mode tabs: **All-in-One / Image / Video**.
- Quy trình chụp on-screen: crosshairs → selection → **on-screen toolbar (orange)** → finalize → mở capture trong Editor.
- Tất cả **Selection types / capture modes**: Region, Window, Fullscreen, Grab Text (OCR), Scrolling/Panoramic, và nhóm **Advanced** (Menu, Time Delay/Schedule/Interval, Multiple Area, Freehand, Webcam, Printer).
- **Scrolling capture** (Automatic + Manual/Panoramic).
- **Presets**, **share destination per preset**, **OneClick (Capture Widget)**.
- **Hotkeys** toàn cục + per-preset, tích hợp Print Screen (Windows).
- **Capture Preferences/Settings** liên quan tới chụp.
- Khác biệt **Windows vs Mac**.

Phần video chỉ chạm ở mức điểm vào (Video tab, nút Video Capture trên on-screen toolbar); chi tiết quay/biên tập nằm ở SPEC 05. Editor/annotation nằm ở SPEC 02. Grab Text/OCR engine chi tiết ở SPEC 04 (spec này chỉ mô tả luồng chụp Grab Text).

**Phạm vi clone (theo SPEC 00 §4):** làm tới capture ảnh + scrolling + OCR + video điểm vào. Tạm gác: tích hợp cloud bản quyền, Smart Move/Simplify AI cao. Khác có chủ đích: dùng Tesseract cho OCR, FFmpeg cho video.

---

## Tính năng

### 01.1 Capture Window — 3 capture mode tabs

- **Mô tả:** Cửa sổ khởi động chụp, phơi bày 3 mode dưới dạng tabs:
  - **All-in-One** — settings mặc định pre-selected để grab nhanh **ảnh hoặc video** on-the-fly. Quyết định image vs video vs scrolling **ngay tại thời điểm chụp** (sau khi kéo selection), không cần cam kết trước.
  - **Image** — chụp ảnh tùy biến: thêm automatic effects, chọn share destination, đặt time delay, chọn Selection type cụ thể (Region/Window/Fullscreen/Grab Text/Advanced…).
  - **Video** — quay video tùy biến: cấu hình audio (system/mic) + webcam device.
- **Tùy chọn/Settings:** mỗi tab có settings riêng (xem 01.13). Nút **Capture** trên tab nào sẽ chụp bằng settings hiện hành của tab đó.
- **Ưu tiên:** MVP (P1) — tab All-in-One + Image. Video tab điểm vào P1, chức năng quay P3 (SPEC 05).
- **Ghi chú clone C++/Qt:** `ui/CaptureWindow` = `QWidget` thường trú (ẩn xuống tray/menu-bar, không thoát app). Tabs = `QTabWidget` hoặc custom segmented control. State per-tab lưu trong data model `CaptureSettings` (struct trong `ezsnag_capture`). Capture Window có thể đổi thành **Capture Widget (OneClick)** qua preference (xem 01.16).

> Naming note (từ research): tài liệu chính thức nói "3 Capture modes: All-in-one, Image, Video". Toggle thứ ba trên luồng on-screen từng là **Panoramic**, build hiện tại đổi thành **Scrolling**. Lịch sử: "Panoramic" → "Scrolling"; "Scrolling" (auto) cũ → "Automatic Scrolling". **Cần verify** label trên build thật.

### 01.2 Bốn cách bắt đầu chụp (entry points)

- **Mô tả:** 4 entry point khởi động chụp:
  1. **Capture Window** — click nút **Capture** trên tab → chụp bằng settings của tab đó.
  2. **Hotkeys** — hotkey toàn cục (mặc định **Print Screen** trên Windows / **Control+Shift+C** trên Mac) chụp bằng settings hiện hành của Capture Window, từ bất kỳ app nào, khi Ezsnagit chạy nền. Preset cũng có thể có hotkey riêng.
  3. **Presets** — cấu hình chụp đã lưu, mỗi preset có hotkey riêng (xem 01.15).
  4. **OneClick (Capture Widget)** — tab/toolbar nổi ở mép màn hình, expand khi hover để chụp nhanh theo preset (xem 01.16).
- **Tùy chọn/Settings:** Preference "Capture interface" chọn Capture Window vs Capture Widget làm bề mặt chính.
- **Ưu tiên:** ⭐MVP (P1) — Capture Window + Hotkeys (MVP-A). Presets P2. OneClick P2.
- **Ghi chú clone C++/Qt:** `ui/HotkeyManager` đăng ký global hotkey qua `RegisterHotKey` (Win32) cho mọi nền (Qt không có global hotkey native). App chạy nền = `QSystemTrayIcon` + không gọi `quit` khi đóng window.

### 01.3 All-in-One Capture (crosshairs / magnifier / dimensions)

- **Mô tả:** Đường chụp nhanh mặc định. Bắt đầu chụp → kéo region → chọn loại capture (image/video/scrolling) từ on-screen toolbar. Khi bắt đầu: màn hình **dim nhẹ**, **orange crosshairs** khóa theo cursor; **magnifier loupe** + **dimension readout** (pixel W×H và/hoặc tọa độ cursor) đi theo cursor để đặt điểm chính xác từng pixel.
  - Hover một window/UI element → **automatic region detection**: viền **orange dashed** snap quanh window/control phát hiện được; click để chụp vùng đó.
  - Hoặc **click & drag** vẽ region chữ nhật tùy ý; thả để chốt. Khi kéo, readout hiện live W×H.
- **Tùy chọn/Settings:**
  - **Orange crosshairs** — báo trạng thái chụp đang active; cursor thành crosshair.
  - **Magnifier** — loupe zoom cạnh cursor cho chọn cạnh chính xác pixel; bật/tắt qua preference **Show magnifier**.
  - **Simplified crosshairs** — preference làm gọn cursor cho hiệu năng.
  - **Dimension readout** — live W×H khi kéo; có thể nhập số chính xác qua **Selection properties** (kích thước cố định + tọa độ vị trí).
  - **Magnifier + crosshair + dimension readout polish** — combo loupe zoom + crosshair khóa cursor + readout W×H live là **chuẩn chất lượng cảm nhận** (CleanShot-grade), không chỉ tiện ích phụ. Nâng thành ⭐MVP: phải mượt, đọc pixel chính xác, hiện/ẩn theo preference nhưng mặc định bật.
- **Ưu tiên:** ⭐MVP (P1) — MVP-A.
- **Ghi chú clone C++/Qt:** Lớp overlay chụp = `QWidget` fullscreen, frameless, `Qt::WindowStaysOnTopHint`, vẽ dim + crosshairs + magnifier bằng `QPainter`. Magnifier đọc pixel từ screen grab (DXGI/`Windows.Graphics.Capture` hoặc fallback GDI `BitBlt`). Region detection: trên Windows enum window/control qua UIAutomation hoặc `WindowFromPoint` + `GetWindowRect`/`DwmGetWindowAttribute` (loại bỏ shadow). Data model `Selection { QRect rect; enum DetectKind }`.

### 01.4 Region / Selection capture

- **Mô tả:** Chụp một vùng chữ nhật người dùng tự vẽ.
- **Tùy chọn/Settings:** Selection = **Region**; hỗ trợ nhập kích thước chính xác qua Selection properties.
- **Ưu tiên:** ⭐MVP (P1) — MVP-A.
- **Ghi chú clone C++/Qt:** crop từ screen grab theo `QRect` đã chọn. Phím tắt liên quan: **Shift+Enter** = Start Region Selection, **Enter** = finalize, **Esc** = cancel (Windows).

### 01.5 Window capture

- **Mô tả:** Chụp một window mở (auto-detect).
- **Tùy chọn/Settings:** Selection = **Window**; hover window tới khi viền orange dashed hiện → click.
- **Ưu tiên:** ⭐MVP (P1) — MVP-A.
- **Ghi chú clone C++/Qt:** dùng region detection của 01.3; crop theo bounding rect của window. Cân nhắc DWM-aware bounds để loại viền/đổ bóng.

### 01.6 Fullscreen capture

- **Mô tả:** Chụp toàn màn hình.
- **Tùy chọn/Settings:** Selection = **Fullscreen**; chụp bằng nút hoặc hotkey. **Multi-monitor:** chụp được monitor dưới cursor / active, hoặc toàn bộ virtual desktop. Hình học capture phải **đúng trên đa màn hình khác scale (mixed-DPI)** — đây là **nền tảng P1**, không phải "verify sau" (xem SPEC 12 §12.2).
- **Ưu tiên:** ⭐MVP (P1) — MVP-A.
- **Ghi chú clone C++/Qt:** `QScreen` của monitor active; grab toàn screen. Per-monitor DPI awareness (PER_MONITOR_V2) để toạ độ đúng khi scale khác nhau. Mac fullscreen hotkey = **F**.

### 01.7 Grab Text capture (OCR)

- **Mô tả:** OCR — trích text có thể chỉnh sửa từ một vùng màn hình.
- **Tùy chọn/Settings:** Selection = **Grab Text** → Capture (Print Screen / Mac **Shift+Ctrl+C**) → click & drag vùng → dialog **Grab Text Results** mở trong Editor với text nhận diện → **Copy All** / copy để dán nơi khác. Có thể kết hợp scrolling để biến ảnh spreadsheet phẳng thành dữ liệu editable.
- **Ưu tiên:** P2 (luồng chụp); engine OCR ở SPEC 04.
- **Ghi chú clone C++/Qt:** luồng chụp ở `ezsnag_capture` tạo ảnh region rồi đẩy sang `ezsnag_ocr` (Tesseract). Mac Grab Text hotkey = **Shift+Command+O**.

### 01.8 Advanced → Menu Capture

- **Mô tả:** Chụp drop-down/cascading menu đang mở (vốn biến mất khi click chỗ khác).
- **Tùy chọn/Settings:** Selection = **Advanced > Menu** → (Windows) tùy chọn click **Menu Properties** để cấu hình sub-menu/menu options → tùy chọn bật **Time Delay** → Capture → mở menu mục tiêu trong lúc đếm ngược để nó được chụp.
- **Ưu tiên:** P3.
- **Ghi chú clone C++/Qt:** dựa trên time-delay capture; trong lúc delay không vẽ overlay che để menu hệ thống mở được. **(Mac: Menu Properties đơn giản hơn / khác — cần verify availability trên Mac.)**

### 01.9 Advanced → Time Delay / Schedule / Interval

- **Mô tả:** Delay chụp N giây (để dàn dựng màn hình), schedule theo ngày/giờ, hoặc chụp lặp theo interval (time-lapse).
- **Tùy chọn/Settings:** chọn Selection type → bật **Time Delay** → chọn:
  - **Delay** — số giây trước khi chụp.
  - **Schedule** — chụp tại ngày/giờ xác định.
  - **Interval** — chụp time-lapse lặp lại đều đặn. **(Windows only — "Time-lapse capture")**
  - **Countdown** hiện ở **góc dưới-phải màn hình** trước khi chụp.
- **Ưu tiên:** Delay P2; Schedule/Interval P4.
- **Ghi chú clone C++/Qt:** `QTimer` cho delay/interval; widget countdown overlay nhỏ ở bottom-right. Schedule = `QTimer`/scheduler với `QDateTime`.

### 01.10 Advanced → Multiple Area

- **Mô tả:** Chụp nhiều vùng/window rời rạc gộp vào một capture.
- **Tùy chọn/Settings:**
  - **Windows:** Selection = **Advanced > Multiple Area** → Capture → orange crosshairs → click windows và/hoặc drag regions để add từng cái → click nút **Finish** ở đầu màn hình.
  - **Mac:** All-in-One → Capture → **giữ Command** và drag chọn nhiều vùng → thả Command để kết thúc.
- **Ưu tiên:** P3.
- **Ghi chú clone C++/Qt:** thu thập list `QRect`/window-rects, composite thành 1 canvas. Cần modifier-key handling khác nhau theo nền tảng (Win: nút Finish; Mac: phím Command).

### 01.11 Advanced → Freehand

- **Mô tả:** Chụp hình bất kỳ vẽ tay (không chữ nhật). **(Windows)**
- **Tùy chọn/Settings:** Selection = **Advanced > Freehand** → Capture → cursor thành **scissors** → click & drag quanh vùng để outline → thả để chụp hình bao kín.
- **Ưu tiên:** P4.
- **Ghi chú clone C++/Qt:** thu `QPainterPath` polygon, mask alpha vùng ngoài. **(Mac availability — cần verify.)**

### 01.12 Advanced → Webcam image / Printer / Mission Control

- **Mô tả:**
  - **Webcam image capture** — chụp ảnh tĩnh từ webcam/camera. **(Tài liệu là Mac-only cho image webcam.)** Mac flow: Image tab → Selection = **Webcam** → Capture → chọn camera device từ dropdown → kích hoạt. Trên cả 2 nền tảng webcam còn dùng cho **Picture-in-Picture video** (xem SPEC 05).
  - **Printer Capture** — **(Windows only)** đẩy nội dung vào Editor qua tùy chọn **Print** của app khác (Ezsnagit xuất hiện như một printer). Chi tiết flow chưa cover — verify trên build.
  - **Mission Control** — **(Mac only)** target trong Selection dropdown.
- **Tùy chọn/Settings:** chọn device webcam; printer = virtual printer driver.
- **Ưu tiên:** Webcam image P4; Printer P5 (low); Mission Control — bỏ qua (Mac-specific, out of scope MVP).
- **Ghi chú clone C++/Qt:** Webcam qua `QCamera`/Qt Multimedia. Printer capture = virtual printer driver (phức tạp, platform-specific Win) — đánh dấu khó.

### 01.13 On-screen capture toolbar (orange toolbar)

- **Mô tả:** Sau khi chọn region (drag-release hoặc window auto-detect), Ezsnagit hiện toolbar on-screen **trước khi** finalize. Đây là điểm quyết định của All-in-One.
- **Tùy chọn/Settings:**
  - **Confirmed buttons:** **Image Capture** (chốt thành screenshot tĩnh), **Video Capture** (chuyển sang quay region đã chọn → mở video options/countdown), **Scrolling Capture** (bắt đầu scrolling/panoramic của window đã chọn — xem 01.14).
  - **Likely-present (verify placement):** **Grab Text** (OCR selection), **Dimensions / Selection properties** (W×H chính xác, snap-to-window), **Effects** quick-apply, **Cancel (Esc)**. **(uncertain — vị trí trên toolbar vs chỉ ở Selection dropdown.)**
  - Khi hover window scrollable lúc selection → **directional scrolling arrows** (horizontal/vertical/both) hiện ngay trên/cạnh selection (xem 01.14).
- **Ưu tiên:** MVP (P1) — Image/Video/Scrolling toggles. Grab Text/Effects/Dimensions trên toolbar P2.
- **Ghi chú clone C++/Qt:** toolbar = `QWidget` nổi nhỏ, position cạnh `Selection.rect` (clamp trong screen). Render trên cùng overlay layer. Mỗi nút emit signal đổi `CaptureKind`. **Cần verify** inventory + thứ tự nút thực tế.

### 01.14 Scrolling Capture (Automatic + Manual/Panoramic)

- **Mô tả:** Chụp nội dung dài/rộng hơn vùng nhìn thấy (full web page, doc dài, spreadsheet rộng). Hai phương pháp:
  - **Automatic Scrolling (arrows):** Ezsnagit detect window scrollable, **tự scroll + stitch** toàn nội dung. Arrows hiện khi hover (nếu app hỗ trợ auto-scroll):
    - **Horizontal arrow** — trái/phải, chụp full width.
    - **Vertical arrow** — lên/xuống, chụp full height.
    - **Both/diagonal arrow** — chụp **toàn trang** (full W + H).
    - Flow: mở nội dung → All-in-One (hoặc Image) → Capture → đưa cursor lên window → click arrow phù hợp → auto-scroll + ráp → mở trong Editor.
    - Hotkeys (Windows): **H** = horizontal, **V** = vertical, **B** = both/diagonal. **(Mac: R / D / X — uncertain, verify per build.)**
  - **Manual / Panoramic Scrolling:** người dùng tự scroll, chụp đúng phần muốn — chính xác hơn auto, dùng được nơi auto fail (parallax sites). Flow: All-in-One → Capture (Print Screen / **Ctrl+Shift+C**) → drag chọn vùng trong window → click nút **Scrolling Capture** trên toolbar → **tự scroll** xuống/lên/ngang bằng **scroll bar** (ưu tiên) hoặc mouse wheel → click **Done** → ảnh stitched mở trong Editor.
  - **Full-page capture:** chọn arrow both-directions (auto) hoặc scroll hết trang (manual) → ảnh full-page (toàn web/doc/spreadsheet trong 1 ảnh).
- **Tùy chọn/Settings:**
  - Guidance TechSmith: scroll **mượt** (không quá nhanh/chậm); ưu tiên scroll bar hơn wheel để tránh chụp hover state; không gồm preview/control bar vào ảnh.
  - Preference **"Require Start button for scrolling capture"** — bắt đầu auto vs chờ Start tường minh (Win + Mac).
- **Ưu tiên:** Automatic P3, Manual/Panoramic P3.
- **Ghi chú clone C++/Qt:** lõi = capture nhiều frame khi window scroll + **image stitching** (template matching/overlap detection, có thể OpenCV) ráp seamless. Auto-scroll: gửi scroll message tới target window (Win: `WM_MOUSEWHEEL`/`SendMessage`) + detect end-of-content. Đây là phần khó nhất của module capture.

### 01.15 Presets

- **Mô tả:** Cấu hình chụp đã lưu: capture type + selection + cursor + delay + effects + share destination, đóng gói thành một preset.
- **Tùy chọn/Settings:** mỗi preset trigger từ Capture Window và có thể gán **hotkey riêng**. Mỗi preset có thể gồm một **Share** output (clipboard/file/đích tích hợp) để route tự động khi finalize. Ví dụ TechSmith: preset scrolling-capture; preset "video with webcam overlay"; preset "copy to clipboard"; preset "save as JPG/PNG". Quảng bá là **"Custom capture presets" + "Custom keyboard shortcuts"**.
- **Ưu tiên:** P2.
- **Ghi chú clone C++/Qt:** `Preset` = serialize `CaptureSettings` + optional hotkey + share target. Lưu trong settings store (`QSettings`/JSON). Hotkey per-preset đăng ký qua `HotkeyManager`.

### 01.16 OneClick (Capture Widget)

- **Mô tả:** Tab/toolbar nổi ở mép màn hình, expand khi hover, chụp nhanh theo preset.
- **Tùy chọn/Settings:** settings **cố định** khi chụp qua OneClick: Capture Type **All-in-One**, **Preview in Editor: On**, **Capture Cursor: On**, **Delay: None**, **Effects: None**, **Share: None**. OneClick **không** kế thừa settings hiện hành của Capture Window (gotcha có tài liệu).
- **Ưu tiên:** P2.
- **Ghi chú clone C++/Qt:** `QWidget` nổi dock mép screen, edge-hide/expand animation. Preference "Capture interface" chuyển giữa Capture Window và Widget.

### 01.17 Per-tab capture settings

- **Mô tả:** Trong mỗi tab cấu hình: Selection type, **Capture Cursor** (gồm/loại con trỏ), **Time Delay**, automatic **Effects**, **Preview in Editor** (on/off), **Copy to Clipboard**, **Share** destination.
- **Tùy chọn/Settings:** như trên; Capture Cursor cũng đặt được per-preset.
- **Ưu tiên:** P1 (Cursor, Preview, Copy to Clipboard); Effects/Share theo SPEC 03/07.
- **Ghi chú clone C++/Qt:** data model `CaptureSettings { SelectionType; bool captureCursor; Delay; QList<Effect>; bool previewInEditor; bool copyToClipboard; ShareTarget; }`.

### 01.18 Capture Preferences / Settings (capture-relevant)

- **Mô tả:** Mục preferences liên quan chụp.
- **Tùy chọn/Settings:**
  - **Windows (Preferences):** Run Ezsnagit when Windows starts · Capture interface (Window vs Widget) · **Show magnifier** · **Simplified crosshairs** · **Require Start button for scrolling capture** · Support HDR color settings · **Hotkeys tab** (đổi hotkey + **Allow override other hotkey assignments**) · **Capture cursor** (gồm/loại con trỏ).
  - **Mac (Settings):** **Hide Ezsnagit and controls when capturing** · **Hide desktop icons during video capture** · **Require Start button for scrolling capture** · **Keyboard tab** (hotkey tùy biến) · Webcam device selection; video frame rate/encoding (video-side).
  - **Multiple monitors:** Fullscreen chụp monitor active/cursor hoặc toàn virtual desktop; Region/Window/Scrolling chạy đúng trên mọi monitor. **Region được phép span nhiều monitor có scale khác nhau** — hình học mixed-DPI đúng là **nền tảng P1** (xem SPEC 12 §12.2), không phải "verify sau".
  - **Delays:** Time Delay (giây), Schedule (ngày/giờ), Interval/time-lapse (Windows). Countdown ở bottom-right.
- **Ưu tiên:** P1 (Show magnifier, Simplified crosshairs, Capture cursor, Run at startup, Capture interface); HDR + Require Start P3.
- **Ghi chú clone C++/Qt:** chi tiết Preferences UI ở SPEC 10; spec này chỉ liệt kê item liên quan chụp. Lưu qua `QSettings`.

### 01.19 Global Hotkeys & PrintScreen integration

- **Mô tả:** Hotkey toàn cục + per-preset; tích hợp Print Screen trên Windows.
- **Tùy chọn/Settings:**
  - **Default Windows capture shortcuts:** Global Capture = **Print Screen** · Start Region Selection = **Shift+Enter** · Take Capture (finalize) = **Enter** · Cancel = **Esc** · Start/Pause/Resume Recording = **Shift+F9** · Stop Recording = **Shift+F10** · Scrolling H/V/Both = **H/V/B** (during capture, on hover) · Repeat Last Capture = **Ctrl+Shift+R** (chốt default, khớp Snagit Mac).
  - **Default Mac capture shortcuts:** Global Capture = **Control+Shift+C** · All-in-One = **Control+Shift+O** · Image = **Control+Shift+S** · Video = **Control+Shift+V** · Take Capture = **Command+Return** · Cancel = **Esc** · Fullscreen = **F** · Grab Text = **Shift+Command+O** · Repeat Last Capture = **Control+Shift+R** · Scrolling H/V/Diagonal = **R/D/X** · Start/Pause Recording = **Control+Shift+Spacebar**.
  - Global hotkey **editable**: mở Capture Window → click **Shortcut** field → bấm tổ hợp. Preset hotkey đặt per-preset.
  - **PrintScreen integration (Windows):** bind Print Screen làm trigger chụp toàn cục. **Conflict Windows 11:** Print Screen có thể mở Snipping Tool native; để nhường key cho Ezsnagit → tắt Windows **Settings → Accessibility → Keyboard → "Use the Print screen key to open screen capture"**, restart. Có option **"Capture Directly to Image Using Print Screen"** (Print Screen ra ảnh trực tiếp).
- **Ưu tiên:** ⭐MVP P1 (Global Capture rebindable, Region, finalize, Cancel, Print Screen bind, **Repeat Last Capture** = Ctrl+Shift+R — rẻ mà cực được yêu thích, ship ngay MVP-A); recording hotkeys theo SPEC 05.
- **Ghi chú clone C++/Qt:** `ui/HotkeyManager` qua Win32 `RegisterHotKey`. Print Screen có thể bị OS chặn → cần fallback/hướng dẫn user tắt Snipping Tool. "Allow override other hotkey assignments" = cố `RegisterHotKey` ưu tiên, báo conflict nếu fail.

### 01.20 Screen-freeze before selection

- **Mô tả:** Đóng băng màn hình ngay lúc kích hoạt chụp — chụp 1 frame tĩnh và để người dùng chọn vùng **trên ảnh đứng yên** thay vì trên nội dung động (video đang chạy, animation, tooltip biến mất). Một usability win lớn (research 07 §3/§10 Tier-1 #4).
- **Tùy chọn/Settings:** mặc định bật; preference tắt nếu muốn chọn trên màn hình live. Magnifier/crosshair đọc pixel từ chính frame đã đóng băng nên ổn định.
- **Ưu tiên:** ⭐MVP (P1) — MVP-A.
- **Ghi chú clone C++/Qt:** rẻ trong Qt — grab 1 screenshot toàn virtual desktop lúc kích hoạt, hiển thị làm **fullscreen overlay tĩnh** (`QWidget` frameless, `Qt::WindowStaysOnTopHint`), vẽ dim + crosshairs lên trên. Region detection (01.3) vẫn chạy trên geometry thật (window rects), chỉ phần hiển thị là frame đóng băng.

### 01.21 Self-timer / delay capture

- **Mô tả:** Hẹn giờ chụp sau N giây để dàn dựng màn hình (mở menu, hover state, sắp cửa sổ). Trùng cơ chế **Time Delay** ở 01.9 nhưng đề cao như một quick-action MVP.
- **Tùy chọn/Settings:** chọn số giây; **countdown** hiện góc dưới-phải trước khi chụp (như 01.9).
- **Ưu tiên:** ⭐MVP (P1) — MVP-A (đã có Time Delay ở 01.9; đánh dấu phần delay cơ bản là MVP).
- **Ghi chú clone C++/Qt:** `QTimer` đếm ngược + widget countdown overlay nhỏ ở bottom-right (chung hạ tầng với 01.9).

### 01.22 Repeat Last Capture

- **Mô tả:** Lặp lại đúng capture trước (cùng selection type + vùng + settings) bằng **1 phím** — daily-driver được yêu thích, rẻ để build (research 09 §3/§6).
- **Tùy chọn/Settings:** default key **Ctrl+Shift+R** (đổi được qua Hotkeys; xem 01.19). Lặp lại lần chụp gần nhất không cần qua Capture Window.
- **Ưu tiên:** ⭐MVP (P1) — MVP-A (chốt, bỏ "uncertain").
- **Ghi chú clone C++/Qt:** lưu `CaptureSettings` + `Selection.rect` của lần chụp cuối; hotkey trigger replay thẳng. Đăng ký qua `HotkeyManager`.

### 01.23 Preset copy-thẳng-clipboard (không mở editor)

- **Mô tả:** Preset chụp → **copy thẳng vào clipboard**, KHÔNG mở Editor nặng. Đây là **enabler của bug-report loop** (luồng tần suất cao nhất: chụp → dán <5 giây, research 09 §6 #1).
- **Tùy chọn/Settings:** preset với **Preview in Editor: Off** + **Copy to Clipboard: On** + **Share: None**; trigger bằng hotkey riêng của preset (xem 01.15).
- **Ưu tiên:** ⭐MVP (P1) — MVP-A (kéo từ P2 lên: là fast-path enabler).
- **Ghi chú clone C++/Qt:** sau finalize, bỏ qua mở `CaptureWindow`/Editor; đẩy `QImage` vào `QClipboard` rồi đóng overlay. Dùng chung pipeline `CaptureSettings { previewInEditor=false; copyToClipboard=true }` (01.17).

### 01.24 Pinned / floating screenshots

- **Mô tả:** Ghim ảnh vừa chụp thành **cửa sổ nổi trên mọi cửa sổ khác** để đối chiếu khi làm việc (CleanShot-grade, research 07 §3/§10 Tier-2 #5). Differentiator UX.
- **Tùy chọn/Settings:** chỉnh opacity, kéo-thả vị trí, click-through lock (cho click xuyên qua xuống app dưới), đóng nhanh.
- **Ưu tiên:** P7 (differentiator — xem ROADMAP P7).
- **Ghi chú clone C++/Qt:** `QWidget` frameless `Qt::WindowStaysOnTopHint` chứa `QLabel`/pixmap; opacity qua `setWindowOpacity`; click-through qua `Qt::WindowTransparentForInput` (toggle). Nhiều pin = nhiều widget độc lập.

### 01.25 Quick Access Overlay

- **Mô tả:** Khay capture-gần-đây **thoáng qua** hiện ngay sau khi chụp, không cần mở Editor — kéo-thả/copy/share nhanh rồi tự ẩn (research 07 §10 Tier-2). Khác với Recent Tray của Editor (vốn cần Editor đang mở).
- **Tùy chọn/Settings:** thumbnail vài capture gần nhất; hover để hành động (copy / save / share / mở editor); tự ẩn sau thời gian rảnh.
- **Ưu tiên:** P7 (differentiator — xem ROADMAP P7).
- **Ghi chú clone C++/Qt:** `QWidget` nổi transient (góc màn hình), danh sách thumbnail từ store capture gần nhất; auto-hide timer. Độc lập với cửa sổ Editor.

### 01.26 After-capture chain

- **Mô tả:** Sau khi chụp, nối chuỗi xử-lý-sau gọn: **effects → save → copy-link** (hoặc editor → OCR → upload) tự động theo cấu hình, kiểu ShareX. Per-preset destination ở đây chỉ là điểm vào; chuỗi đầy đủ + upload layer pluggable thuộc share spec.
- **Tùy chọn/Settings:** preset chỉ định các bước nối tiếp khi finalize (xem 01.15 cho share-per-preset).
- **Ưu tiên:** P7 (differentiator) — chi tiết pipeline + destination-picker **trỏ SPEC 07 (Share)**.
- **Ghi chú clone C++/Qt:** thiết kế **upload/destination layer dạng interface pluggable** ngay từ đầu (xem SPEC 07); module capture chỉ phát sự kiện "capture finalized" + `CaptureSettings.chain`. Tránh hard-code danh sách đích.

---

## Bảng ưu tiên (MVP vs sau)

| # | Feature | Ưu tiên |
|---|---|---|
| 01.1 | Capture Window — All-in-One + Image tabs | **P1** |
| 01.1 | Video tab (chức năng quay) | P3 (điểm vào P1) |
| 01.2 | Entry: Capture Window + Hotkeys | ⭐**P1** (MVP-A) |
| 01.2 | Entry: Presets | P2 |
| 01.2 | Entry: OneClick | P2 |
| 01.3 | All-in-One: crosshairs / magnifier / dimensions / region detect | ⭐**P1** (MVP-A) |
| 01.4 | Region capture | ⭐**P1** (MVP-A) |
| 01.5 | Window capture | ⭐**P1** (MVP-A) |
| 01.6 | Fullscreen capture | ⭐**P1** (MVP-A) |
| 01.7 | Grab Text capture (luồng) | P2 (OCR engine SPEC 04) |
| 01.8 | Advanced → Menu | P3 |
| 01.9 | Time Delay | P2 |
| 01.9 | Schedule / Interval (time-lapse) | P4 |
| 01.10 | Multiple Area | P3 |
| 01.11 | Freehand | P4 |
| 01.12 | Webcam image | P4 |
| 01.12 | Printer capture (Win) | P5 |
| 01.12 | Mission Control (Mac) | bỏ qua |
| 01.13 | On-screen toolbar (Image/Video/Scrolling) | ⭐**P1** (MVP-A) |
| 01.13 | Toolbar: Grab Text / Effects / Dimensions | P2 |
| 01.14 | Automatic Scrolling (arrows + stitch) | P3 |
| 01.14 | Manual / Panoramic Scrolling | P3 |
| 01.15 | Presets | P2 |
| 01.16 | OneClick widget | P2 |
| 01.17 | Per-tab settings (cursor/preview/clipboard) | ⭐**P1** (MVP-A) |
| 01.18 | Capture preferences cơ bản | **P1** |
| 01.18 | HDR / Require Start button | P3 |
| 01.19 | Global hotkeys + Print Screen bind | ⭐**P1** (MVP-A) |
| 01.20 | Screen-freeze before selection | ⭐**P1** (MVP-A) |
| 01.21 | Self-timer / delay capture | ⭐**P1** (MVP-A) |
| 01.22 | Repeat Last Capture (Ctrl+Shift+R) | ⭐**P1** (MVP-A) |
| 01.23 | Preset copy-thẳng-clipboard (no editor) | ⭐**P1** (MVP-A) |
| 01.24 | Pinned / floating screenshots | P7 (differentiator) |
| 01.25 | Quick Access Overlay | P7 (differentiator) |
| 01.26 | After-capture chain (trỏ SPEC 07) | P7 (differentiator) |

---

## Điểm chưa chắc / cần verify trên app thật

(Bảo toàn từ research §10 + các điểm uncertain rải rác.)

1. **Inventory + thứ tự nút** trên on-screen (orange) toolbar — vị trí Grab Text / Effects / Dimensions (chỉ trên toolbar, hay chỉ ở Selection dropdown?).
2. Toggle thứ ba on-screen build hiện tại là **"Panoramic"** hay **"Scrolling"**?
3. **Mac availability** của Freehand và Menu advanced modes.
4. "All-in-One / Image / Video" xuất hiện như on-screen toggle hay chỉ là tab của Capture Window?
5. **Mac scrolling arrow hotkeys** R / D / X — xác nhận theo build.
6. Naming note: "3 Capture modes" vs "Image/Video/Panoramic" trên các trang marketing/legacy.
7. Chi tiết flow **Printer capture** (Windows) chưa được cover.

> **Đã giải quyết (không còn "verify"):** phím **Repeat Last Capture** = **Ctrl+Shift+R** (chốt default, 01.19/01.22); hành vi **multi-monitor / region span đa màn hình mixed-DPI** = nền tảng **P1**, đúng hình học theo SPEC 12 §12.2 (không còn là "verify sau").
