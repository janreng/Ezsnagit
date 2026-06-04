# SPEC 05 — Video (Ezsnagit)

> Nguồn: docs/research/04-video.md · Module: `ezsnag_video`
> Phụ thuộc: FFmpeg (encode/decode/mux), Qt Multimedia (preview/playback + audio I/O), lớp platform capture (Win: Windows.Graphics.Capture / DXGI; mic+system audio: WASAPI loopback). Glue UI ở `ui/CaptureWindow` (tab Video) và `ui/EditorWindow` (timeline video).

---

## 1. Mục tiêu & phạm vi

Clone phần video của Snagit là một **screen recorder + trình biên tập nhẹ single-track**: quay màn hình (region/window/fullscreen) có mic + system audio + webcam (PiP) + cursor effect + Screen Draw, rồi **trim / Cut Out / combine / Save Frame / export GIF**. **KHÔNG phải multitrack NLE** (đó là Camtasia).

**Trong phạm vi (làm tới):**
- Video Capture: recording toolbar, system audio + mic, webcam/PiP, cursor capture, Screen Draw, countdown.
- Video Editing: timeline + playhead, trim đầu/cuối, **Cut Out** đoạn giữa, Save selection as new video, **Add Videos** (combine), Save Frame as Image.
- Animated GIF from Video: presets + custom settings.
- Video from Images (narration over images) — chi tiết ở SPEC 08 templates, phần video do `ezsnag_video` đảm nhận.
- Output: MP4 (export chính) + GIF; import MP4/MOV/GIF/GIF.

**Ngoài phạm vi (gác lại / Camtasia):**
- Multitrack timeline, mix audio nhiều track, waveform editing, import audio rời.
- Reposition/resize webcam (PiP) **sau khi quay** (PiP là quyết định lúc capture — xem §6).
- Transition/animation, caption/subtitle track, template video.
- Tích hợp cloud bản quyền (Screencast.com, YouTube/Slack OAuth) — xem SPEC 07.

**Khác Snagit có chủ đích:** encode bằng **FFmpeg** (H.264/AAC trong MP4, GIF qua palettegen/paletteuse); preview/scrub bằng Qt Multimedia; format dự án ảnh là `.ezsnagx` (video lưu file MP4 riêng + tham chiếu trong Library).

---

## 2. Video Capture (quay video)

### 2.0 Luồng tổng quát
Capture Window → tab **Video** → chỉnh input (camera, mic, system audio, cursor, screen draw, "Open In", "Selection", "Share") → bấm **Capture**/hotkey → chọn vùng (crosshair) → cửa sổ **Ready to Record** → **Record** (có countdown) → **Stop** → mở trong Editor. Chi tiết từng bước ở `ux-flows/05-video-flow.md`.

### 2.1 Recording toolbar ("Ready to Record")
- **Mô tả:** Sau khi chọn vùng quay, hiện thanh toolbar nổi với các nút Record / Pause / Stop, đồng hồ elapsed-time, và các icon bật/tắt input (camera, mic, system audio, cursor, screen draw). Toolbar di chuyển được; trên dual-monitor nên đặt ở màn hình không quay; fullscreen single-monitor thì điều khiển bằng hotkey.
- **Tùy chọn / controls:**
  - **Record / Pause:** một nút toggle (Snagit Win dùng cùng `Shift+F9` cho cả record và pause).
  - **Stop:** kết thúc, video mở trong Editor (hoặc đích "Open In").
  - **Timer:** hiển thị thời lượng đang quay.
  - Input có thể **toggle on/off cả trước lẫn trong khi quay** (ví dụ mute mic giữa chừng).
- **Hotkey mặc định (clone, lấy theo Windows; cho cấu hình ở SPEC 10):** Capture/region = `Print Screen`; Record/Pause = `Shift+F9`; Stop = `Shift+F10`; switch PiP↔Fullscreen Webcam = `Ctrl+Shift+F`.
- **Ưu tiên:** **P10** (lõi của recording — không có thì không quay được).
- **Ghi chú clone C++/Qt:** Toolbar là `QWidget` frameless always-on-top (`Qt::Tool | FramelessWindowHint | WindowStaysOnTopHint`), độc lập với cửa sổ quay để không lọt vào frame. Trạng thái recording (idle/countdown/recording/paused) là một state machine trong `ezsnag_video` (logic thuần, test headless); toolbar chỉ là view. Elapsed-time lấy từ PTS thực của FFmpeg writer, không phải `QTimer` UI (tránh lệch khi máy giật).

### 2.2 Chọn vùng quay (region / window / fullscreen)
- **Mô tả:** Sau **Capture**, hiện crosshair màu cam. Click+drag vẽ region tùy ý; **Fullscreen** quay cả màn hình; **Window** snap theo biên cửa sổ ứng dụng. Tinh chỉnh bằng kéo handle hoặc nhập chính xác **Width/Height**.
- **Tùy chọn:**
  - **Aspect-ratio lock khi kéo:** giữ `Ctrl+Shift` để khóa **16:9** (tham chiếu thêm **4:3** trên Windows).
  - **Dimension guide chuẩn:** 1920×1080, 1280×720, 640×480, 320×240.
  - **Selection mode** chọn trước ở tab Video: Region / Window / Fullscreen.
- **Ưu tiên:** **P10** (region) · **P10** (fullscreen) · **P9** (window snap) · **P8** (aspect lock + dimension input).
- **Ghi chú clone C++/Qt:** Tái dùng region selector của `ezsnag_capture` (SPEC 01) nhưng output là **rect + monitor + DPI scale** để cấu hình nguồn capture video. **Độ phân giải output = kích thước vùng quay** (Snagit không có dropdown bitrate/quality riêng — xem §8). Nên ép width/height về số chẵn (yêu cầu của H.264 yuv420p).

### 2.3 Audio: system audio + microphone
- **Mô tả:** **System Audio** thu âm thanh do máy phát ra (alert app, loa, audio webinar/Zoom). **Microphone Audio** thu từ mic tích hợp hoặc USB. Hai nguồn thu **đồng thời** được (workflow webinar/Zoom: vừa mic vừa system audio).
- **Tùy chọn:**
  - **Microphone Options:** chọn input device + **background noise removal**.
  - **System Audio:** bật/tắt.
  - Toggle mic/system audio cả trước và trong khi quay.
- **Ưu tiên:** **P10** (mic) · **P9** (system audio) · **P7** (noise removal).
- **Ghi chú clone C++/Qt:**
  - **Mic:** `QMediaDevices::audioInputs()` để liệt kê device; thu PCM qua `QAudioSource`/Qt Multimedia hoặc trực tiếp FFmpeg `dshow` (Windows). Cho chọn device trong dropdown.
  - **System audio (loopback):** Qt Multimedia **không** expose loopback; dùng **WASAPI loopback** (Windows Core Audio) hoặc FFmpeg `dshow`/`wasapi` để bắt audio đầu ra. Đây là phần platform-specific cần lớp riêng trong `ezsnag_video::platform`.
  - **Trộn mic + system audio:** dùng FFmpeg `amix` (hoặc trộn PCM thủ công) → một audio stream AAC trong MP4. (Snagit gộp 1 track — phù hợp model single-track.)
  - **Background noise removal:** dùng FFmpeg filter `afftdn`/`arnndn` (RNNoise) trên track mic. Đánh dấu là tùy chọn nâng cao (P7).

### 2.4 Webcam / Picture-in-Picture (PiP)
- **Mô tả:** Toggle **Camera/Webcam** thu video webcam. Hai chế độ hiển thị: **Picture-in-Picture** (webcam ở góc, overlay lên screen recording) và **Fullscreen Webcam** (webcam chiếm toàn frame). Chuyển PiP↔Fullscreen **trong khi quay** bằng nút hoặc `Ctrl+Shift+F`.
- **Tùy chọn (Camera Options):**
  - **Placement:** Top-left / Top-right / Bottom-left / Bottom-right.
  - **Shape:** Circle / Square / Rounded Square / Rectangle / Rounded Rectangle.
  - **Size:** kéo góc preview to/nhỏ (cả lúc preview lẫn lúc quay).
  - **Reposition:** kéo preview giữa các góc.
  - **(Mac-only, ưu tiên thấp cho clone Win):** Mirror (lật ngang), Border màu tùy chọn, Virtual background / blur.
- **Ưu tiên:** **P8** (PiP overlay + placement/shape/size) · **P7** (Fullscreen Webcam + switch khi quay) · **P4** (Mirror/Border/virtual background — gác, ưu tiên Windows trước).
- **Ghi chú clone C++/Qt:**
  - Webcam frame qua `QCamera` + `QVideoSink` (Qt Multimedia) hoặc FFmpeg `dshow video=...`.
  - **PiP là compositing lúc quay:** mỗi frame output = screen frame + webcam frame được scale/mask theo shape rồi blit vào góc theo placement. Compositing nên làm trên GPU (QPainter/QImage hoặc shader) trước khi đẩy vào encoder. Shape bo góc/tròn = alpha mask.
  - Vì compositing xảy ra **trước encode**, PiP nung thẳng vào pixel → **không** sửa được sau khi quay (đúng như Snagit; xem §6, §11).
  - State PiP (mode/placement/shape/size) là một struct config trong `ezsnag_video`; switch khi quay = đổi config runtime của compositor.

### 2.5 Cursor capture
- **Mô tả:** Toggle **Cursor** quyết định con trỏ có xuất hiện trong recording không. Hiệu ứng con trỏ: **Highlight Cursor** (vòng nhấn mạnh quanh con trỏ) và **Click Animation** (phản hồi thị giác khi click chuột).
- **Tùy chọn:** Cursor on/off · Highlight Cursor · Click Animation.
- **Ưu tiên:** **P9** (hiện cursor) · **P7** (highlight + click animation).
- **Ghi chú clone C++/Qt:** Một số API capture (DXGI/WGC) cho chọn có vẽ cursor hay không; nếu nguồn không kèm cursor thì tự vẽ overlay con trỏ tại vị trí `GetCursorPos`. Highlight/Click animation = overlay vẽ thêm vào frame trong compositor (vòng tròn quanh con trỏ; hook click chuột để bật animation). Compositing chung pipeline với PiP/Screen Draw.

### 2.6 Screen Draw (vẽ khi đang quay)
- **Mô tả:** Cho phép **annotate trực tiếp trên màn hình trong lúc quay** để highlight (shape, arrow, step number). Có **fade duration** (giây) để annotation tự biến mất sau một thời gian. Bật từ toggle tab Video và từ recording toolbar.
- **Tùy chọn:** chọn công cụ (arrow/shape/step) · màu · **fade duration** (giây) on/off.
- **Ưu tiên:** **P6**.
- **Ghi chú clone C++/Qt:** Overlay window trong suốt phủ vùng quay, nhận input vẽ; nét vẽ được composit vào frame trước encode (giống PiP/cursor). Fade = giảm alpha theo timer rồi xóa object. Tái dùng renderer của `ezsnag_annotations` cho hình arrow/shape/step để đồng bộ style.

### 2.7 Countdown trước khi quay
- **Mô tả:** Trước khi capture bắt đầu có **countdown** kiểu 3-2-1.
- **Tùy chọn:** bật/tắt + (đề xuất clone) chọn 0/3/5/10 giây.
- **Ưu tiên:** **P7**.
- **Ghi chú clone C++/Qt:** Overlay đếm ngược toàn màn hình; **không** ghi frame countdown vào file (writer chỉ start khi countdown xong). **Điểm chưa chắc:** thời lượng countdown thật của Snagit và việc có cho cấu hình không — research flag uncertain; clone chốt mặc định 3s, cho cấu hình (xem §9).

### 2.8 Giới hạn thời lượng / tips
- TechSmith khuyên **tránh quay quá ~1 giờ**; nên quay nhiều clip ngắn rồi **combine** trong Editor.
- **Hard max recording length không có tài liệu — (uncertain).** Clone không đặt giới hạn cứng nhưng nên: ghi trực tiếp ra file (segmented muxing), cảnh báo khi >1h hoặc khi đĩa sắp đầy.
- **Ghi chú clone:** Encode **trong lúc quay** (streaming ra MP4), không giữ raw frame trong RAM — bắt buộc với clip dài. Dùng `faststart` khi finalize để MP4 phát được ngay.

---

## 3. Video Editing (Editor)

Khi video mở, Editor hiện video trên **canvas** + **playback toolbar** + **timeline** bên dưới. Single-track.

### 3.1 Timeline, scrubber & playhead
- **Mô tả:** **Playhead trắng** quyết định frame đang hiển thị. Kéo playhead để scrub/nhảy bất kỳ điểm nào. **Next/Previous Frame** (hoặc phím **←/→**) di chuyển đúng một frame. **Green Start handle** + **Red End handle** trên timeline để chọn segment (dùng cho Cut, Save-as-clip, GIF).
- **Tùy chọn:** Play/Pause · Previous/Next Frame · volume/mute · readout elapsed-time / total-duration · kéo Start/End handle.
- **Ưu tiên:** **P10**.
- **Ghi chú clone C++/Qt:** Preview bằng **Qt Multimedia** (`QMediaPlayer` + `QVideoWidget`/`QGraphicsVideoItem` trên canvas). Frame-by-frame chính xác cần seek theo frame: lấy `frameRate` + duration để map thời gian↔frame; nếu `QMediaPlayer` seek không đủ chính xác, fallback decode frame qua FFmpeg để hiển thị đúng frame. Timeline là custom `QWidget` vẽ thanh + 2 handle + playhead; logic chọn (start/end/playhead time) là model thuần trong `ezsnag_video`.

### 3.2 Trim & Cut Out (xóa đầu / cuối / giữa)
- **Mô tả (Trim đầu/cuối):** đặt Start/End handle bao đoạn cần xóa → (tùy chọn) kéo playhead để preview đoạn sẽ bị xóa → bấm **Cut Out** → **đường vàng** trên timeline đánh dấu chỗ vừa xóa.
- **Mô tả (Cut Out đoạn giữa):** Start handle đặt đầu đoạn thừa, End handle đặt cuối đoạn thừa → **Cut Out** → đoạn giữa biến mất, hai nửa nối lại, **đường vàng** đánh dấu mối nối.
- **Label nút chính xác:** **"Cut Out"**.
- **Tùy chọn:** preview trước khi cắt · undo/redo cắt.
- **Ưu tiên:** **P10** (Cut Out là tính năng biên tập lõi).
- **Ghi chú clone C++/Qt:** Mô hình **edit list không phá hủy**: giữ danh sách "kept segments" `[(t0,t1), (t2,t3), ...]`; mỗi Cut Out = xóa khoảng `[start,end]` khỏi danh sách và merge các đoạn kề. Preview phát theo edit list (skip đoạn đã cắt). **Đường vàng** = marker tại mỗi điểm nối. Khi **export** mới render thật: FFmpeg cắt theo segment rồi `concat` (lưu ý keyframe — cần re-encode quanh điểm cắt để cắt chính xác frame, không chỉ cắt copy theo keyframe). Undo/redo dùng chung cơ chế undo của canvas.

### 3.3 Save selection as new video
- **Mô tả:** chia recording dài thành clip ngắn. Kéo Start/End chọn một phần → review bằng playhead → **Save As New Video** ("Save selection as new video") → clip mới mở trên canvas như một capture riêng.
- **Ưu tiên:** **P8**.
- **Ghi chú clone C++/Qt:** Export segment `[start,end]` ra MP4 mới bằng FFmpeg (re-encode quanh keyframe để cắt chính xác), tạo capture mới trong Library và mở trong Editor.

### 3.4 Combine / Add Videos
- **Mô tả:** bấm **Add Videos** trên playback toolbar để merge nhiều clip từ **Library** thành một video. Clip **reorder bằng kéo-thả**. Đây là cách khuyến nghị để ghép nhiều recording ngắn thành một video dài.
- **Ưu tiên:** **P8**.
- **Ghi chú clone C++/Qt:** Dialog/tray chọn clip + sắp xếp thứ tự; export = FFmpeg `concat`. Cần **chuẩn hóa** trước khi concat (cùng resolution/fps/pixfmt/codec/sample rate) — nếu khác thì re-encode về profile chung (scale + pad + fps + aresample). Mismatch resolution → pad letterbox theo clip lớn nhất.

### 3.5 Save Frame as Image (trích một frame thành ảnh)
- **Mô tả:** kéo **playhead** tới đúng frame (dùng frame-by-frame cho chính xác) → bấm **Save Frame as Image** (icon máy ảnh) → frame mở thành **ảnh mới** trên canvas, annotate/edit/share như ảnh chụp thường. Hoạt động với recording của Snagit và **MP4/MOV/GIF** import; cũng có thể dùng quick-capture hotkey để grab frame từ video đang phát ở app/browser khác.
- **Ưu tiên:** **P9**.
- **Ghi chú clone C++/Qt:** Decode frame tại thời điểm playhead (FFmpeg seek + decode chính xác frame, không lấy frame Qt preview vì có thể downscale), tạo capture ảnh mới (`ezsnag_canvas`) → mở trong Editor ảnh để annotate.

---

## 4. Animated GIF from Video

### 4.1 Luồng
- **Mô tả:** mở video → kéo green/red handle chọn segment (hoặc chọn cả video sau) → preview (kéo playhead về green handle, Play) → bấm **Save As Animated GIF** (nút **GIF**) → chọn **selection** vs **entire video** → chọn **optimization preset** (hoặc Custom) trong dialog **Create GIF** → tạo.
- **Khuyến nghị:** giữ segment GIF **< 30 giây**, target **≤ 1 MB**.
- **Ưu tiên:** **P9**.

### 4.2 Optimization presets (Create GIF dialog)
| Preset | Ý nghĩa |
|---|---|
| **Screen Video (Default)** | tốt cho screen recording. |
| **High Motion Video** | cho footage thực tế hoặc game. |
| **Reduced File Size** | scale video về **max width/height 200 px**. |
| **Custom** | điều khiển thủ công (§4.3). |

### 4.3 Custom settings
| Setting | Tác dụng |
|---|---|
| **Frame Rate** | cao = mượt hơn nhưng file lớn hơn. |
| **Dimensions** (W/H hoặc scale %) | nhỏ hơn = file nhẹ hơn. |
| **Aspect Ratio Lock** | khuyên bật để tránh méo. |
| **Dithering** | giảm color banding; tăng dung lượng. |
| **Looping** | lặp vô hạn / không lặp / **lặp số lần cụ thể**. |
| **Dynamic Colors** (Mac) | tăng file; khuyên dùng cho hầu hết video. |
| **Fade to Black** | thêm transition ở frame cuối. |

- Có thể combine nhiều clip rồi export thành một GIF.
- GIF bị mờ → tạo lại với Custom + frame rate cao hơn (đổi lại file lớn hơn).
- **Ưu tiên:** presets **P9** · Custom (frame rate/dimensions/aspect lock) **P8** · dithering/looping-count/fade-to-black **P6** · Dynamic Colors **P3** (Mac).
- **Ghi chú clone C++/Qt:** Pipeline GIF chuẩn FFmpeg 2 pass — `palettegen` (tạo palette tối ưu) rồi `paletteuse` (`dither=bayer`/`sierra2_4a` cho dithering on/off). Map preset → tham số:
  - *Screen Video:* `palettegen` mặc định, dither nhẹ, giữ fps gốc (giới hạn ~15).
  - *High Motion:* fps cao hơn + `stats_mode=diff` cho chuyển động.
  - *Reduced File Size:* `scale=200:200:force_original_aspect_ratio=decrease` (max 200px).
  Looping: header `-loop 0` (vô hạn) / `-loop 1` (một lần) / số lần cụ thể. Fade to Black: filter `fade=t=out` ở cuối. Hiển thị **ước lượng dung lượng** và cảnh báo khi >1MB hoặc >30s.

---

## 5. Video from Images (ảnh → video có narration)

> Tính năng thuộc **SPEC 08 (templates / `ezsnag_templates`)**; phần ghi video + narration do `ezsnag_video` đảm nhận. Tóm tắt ở đây để khép kín spec video.

- **Mô tả:** thu **voice narration trên một/nhiều ảnh tĩnh**, có annotate live — cách nhanh biến screenshot thành video how-to không cần NLE.
- **Luồng:** chọn nhiều ảnh trong Recent Captures/Library (`Ctrl+click`) → **Create > Video from Images** → recording toolbar hiện ra, reorder ảnh bằng kéo-thả → cấu hình **Background Color** (dark/light), **Cursor**, **Microphone** (narrate), **Webcam** (tùy chọn) → **Record** (`Shift+F9`) và thuyết minh → chuyển ảnh bằng **←/→** hoặc Next/Previous → **annotate khi đang quay/pause** (arrow/shape/step từ Favorites, kéo-thả lên ảnh) → Pause/resume → **Stop** (`Shift+F10`) → video mở trên canvas để trim/edit/share thành **MP4**.
- **Tùy chọn:** Background Color · Cursor · Microphone · Webcam · reorder · annotate live.
- **Ưu tiên:** **P6** (toàn cụm — sau khi recording + editing lõi đã chạy).
- **Ghi chú clone C++/Qt:** Compositor render mỗi ảnh thành frame (nền theo Background Color, ảnh canh giữa) + overlay annotation đang vẽ; ghi liên tục ra MP4 cùng track mic giống recording màn hình. Chuyển ảnh = đổi nguồn ảnh của compositor (không tạo transition). Annotate dùng renderer `ezsnag_annotations`.

---

## 6. Picture-in-Picture / Audio (biên tập sau quay)

### 6.1 PiP sau khi quay
- PiP được cấu hình **lúc capture** (placement/shape/size/mode — §2.4) và **nung vào pixel** khi compositing trước encode.
- **Biên tập PiP sau quay** (di chuyển/resize webcam trong video đã lưu) **không có trong Snagit Editor** (editor là trim/cut/combine). Để reposition camera track sau, TechSmith trỏ sang Camtasia (camera ở track riêng). **(uncertain — clone coi PiP là capture-time only.)**
- **Quyết định clone:** PiP **chỉ chỉnh được lúc quay**. Không làm post-edit PiP ở bản đầu (multitrack = ngoài phạm vi).

### 6.2 Audio editing
- **Thu audio:** system audio + mic lúc quay (§2.3); narration qua Video from Images (§5).
- **Remove audio:** Snagit chỉ có trên **Mac** (`Video > Make a Silent Copy` tạo bản không tiếng, giữ bản gốc). Windows không có lệnh remove-audio in-editor tài liệu hóa. **(uncertain.)**
- **Quyết định clone:** làm **"Make a Silent Copy"** cho cả Windows (rẻ với FFmpeg: `-an` hoặc drop audio stream) — **P6**. Không multitrack mixing/waveform editing (Camtasia).
- **Ghi chú clone C++/Qt:** Silent copy = FFmpeg copy video stream + `-an`. Mute trong preview là Qt Multimedia volume, không đổi file.

---

## 7. Sharing / Output (MP4 / GIF)

- **MP4** — export video chính (lưu local/network drive). Codec clone: **H.264 (libx264) + AAC**, `yuv420p`, `+faststart`.
- **Animated GIF** — qua GIF export (§4), pipeline palettegen/paletteuse.
- Editor **import/đọc** thêm **MOV** (và GIF).
- **Đích chia sẻ:** File (lưu MP4 local/network) là chính cho clone. Screencast.com / Google Drive / Dropbox / YouTube / Slack là tích hợp cloud bản quyền — **gác** (xem SPEC 07). Snagit auto-copy share link vào clipboard sau upload.
- **Resolution/quality:** output resolution = **kích thước vùng quay**; Snagit **không** expose dropdown bitrate/CRF/frame-rate cho MP4 trong UI tài liệu hóa. **(uncertain — không tìm thấy control bitrate/CRF/fps chính thức.)**
- **Quyết định clone:** giữ UI tối giản như Snagit (không phơi CRF cho user thường) nhưng **bên trong** đặt `libx264 -crf 20 -preset medium`, fps = fps capture thực. Có thể để control nâng cao trong Preferences (SPEC 10) cho power user, mặc định ẩn.
- **Ưu tiên:** export MP4 (File) **P10** · export GIF **P9** · cloud destinations **P3** (gác).

---

## 8. Bảng ưu tiên

| Tính năng | Ưu tiên | Ghi chú |
|---|---|---|
| Recording toolbar (Record/Pause/Stop/timer) | P10 | state machine lõi |
| Chọn vùng region / fullscreen | P10 | tái dùng selector SPEC 01 |
| Window snap selection | P9 | |
| Aspect lock 16:9 + nhập W/H + dimension guide | P8 | |
| Mic audio (+ chọn device) | P10 | QAudioSource / dshow |
| System audio (loopback) | P9 | WASAPI loopback (platform) |
| Background noise removal | P7 | FFmpeg afftdn/arnndn |
| Cursor capture | P9 | |
| Highlight cursor + click animation | P7 | overlay compositor |
| Webcam PiP (placement/shape/size) | P8 | composit trước encode |
| Fullscreen webcam + switch khi quay (`Ctrl+Shift+F`) | P7 | |
| Webcam Mirror/Border/virtual background | P4 | Mac-first, gác cho Win |
| Screen Draw (+ fade) | P6 | overlay annotation |
| Countdown | P7 | mặc định 3s, cho cấu hình |
| Timeline + playhead + frame-by-frame | P10 | Qt Multimedia + FFmpeg seek |
| Trim / Cut Out (đầu/cuối/giữa) + đường vàng | P10 | edit list không phá hủy |
| Save selection as new video | P8 | |
| Add Videos (combine) | P8 | FFmpeg concat + normalize |
| Save Frame as Image | P9 | FFmpeg decode frame |
| GIF presets | P9 | palettegen/paletteuse |
| GIF custom (fps/dimensions/aspect lock) | P8 | |
| GIF dithering/looping-count/fade-to-black | P6 | |
| GIF Dynamic Colors (Mac) | P3 | |
| Video from Images | P6 | với SPEC 08 |
| Export MP4 (File) | P10 | libx264+AAC, faststart |
| Export GIF (File) | P9 | |
| Make a Silent Copy (remove audio) | P6 | FFmpeg -an, cả Win |
| Cloud share (Screencast/Drive/YouTube/Slack) | P3 | gác, SPEC 07 |
| Post-edit PiP reposition | — | ngoài phạm vi (Camtasia) |

---

## 9. Điểm chưa chắc (uncertainties — giữ nguyên từ research)

- **Countdown:** thời lượng chính xác trước khi quay và có cấu hình được không. → Clone chốt mặc định **3s, cho cấu hình** (0/3/5/10s) trong Preferences.
- **Max recording length:** chỉ có khuyến nghị "giữ dưới ~1 giờ", không có giới hạn cứng tài liệu hóa. → Clone không giới hạn cứng, encode streaming + cảnh báo >1h / sắp đầy đĩa.
- **Bitrate / quality / frame-rate cho MP4:** Snagit không phơi control nào trong UI chính thức. → Clone ẩn mặc định (`crf 20`, fps theo capture), control nâng cao tùy chọn trong Preferences.
- **Remove audio trên Windows:** không có lệnh in-editor tài liệu hóa (chỉ Mac có "Make a Silent Copy"). → Clone **làm cho cả Windows**.
- **Reposition webcam/PiP sau quay:** dường như capture-time only ở Snagit (Camtasia mới post-edit). → Clone coi PiP **capture-time only**.
- **Mac record hotkey** `Ctrl+Shift+Space` vs `Cmd+Shift+Space`: lệch giữa các bản docs, likely version-dependent. → Clone Windows-first dùng `Shift+F9`/`Shift+F10`; hotkey cho cấu hình ở SPEC 10.
