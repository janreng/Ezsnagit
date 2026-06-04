# UX Flows 05 — Video (Ezsnagit)

> Đi kèm `docs/specs/05-video.md` · Module: `ezsnag_video` · Bề mặt: Capture Window (tab Video) + Editor (timeline video).
> Hotkey lấy theo Windows (cho cấu hình ở SPEC 10): Capture/region `Print Screen` · Record/Pause `Shift+F9` · Stop `Shift+F10` · switch PiP↔Fullscreen Webcam `Ctrl+Shift+F` · frame-by-frame `←/→`.

---

## Flow A — Quay một vùng màn hình có mic

**Mục tiêu:** quay region màn hình kèm narration mic, mở trong Editor để chỉnh.

1. Mở **Capture Window** → chọn tab **Video**.
2. Bật toggle **Microphone** → mở **Microphone Options** → chọn input device (và bật **noise removal** nếu muốn).
3. (Tùy chọn) bật **System Audio** nếu cần thu cả tiếng máy; bật **Cursor** nếu muốn con trỏ hiện trong video.
4. Đặt **Selection = Region** và **Open In = Editor**.
5. Bấm **Capture** (hoặc `Print Screen`) → crosshair cam hiện ra.
6. **Click + drag** vẽ vùng quay (hoặc giữ `Ctrl+Shift` khóa 16:9; hoặc nhập W/H chính xác).
7. Thả chuột → cửa sổ **Ready to Record** hiện với nút Record/Pause/Stop, timer và các icon input.
8. (Tùy chọn) kéo toolbar sang màn hình khác để không lọt vào frame.
9. Bấm **Record** (hoặc `Shift+F9`) → **countdown 3-2-1** chạy → bắt đầu ghi; timer đếm.
10. Nói/thao tác. Có thể **mute mic giữa chừng** bằng cách click icon mic; bấm `Shift+F9` để **Pause/Resume**.
11. Bấm **Stop** (hoặc `Shift+F10`).
12. Video mở trong **Editor** trên canvas, sẵn sàng trim/edit/share.

*Clone notes:* writer FFmpeg chỉ start sau khi countdown xong (frame countdown không vào file); mic qua QAudioSource/dshow trộn AAC vào MP4; toolbar là cửa sổ frameless always-on-top tách khỏi vùng quay.

---

## Flow B — Trim đầu/cuối (xóa phần thừa ở đầu hoặc cuối)

**Mục tiêu:** bỏ đoạn dư đầu hoặc cuối clip.

1. Mở video trong **Editor** (timeline hiện bên dưới canvas).
2. Kéo **green Start handle** và **red End handle** sao cho **bao đúng đoạn cần XÓA** (ví dụ từ 0:00 đến điểm thật sự bắt đầu).
3. (Tùy chọn) kéo **playhead** qua vùng chọn để **preview** đoạn sẽ bị xóa.
4. Bấm **Cut Out**.
5. Đoạn được bỏ; **đường vàng** trên timeline đánh dấu chỗ vừa cắt.
6. Lặp lại cho đầu/cuối còn lại nếu cần. `Ctrl+Z` để undo nếu cắt nhầm.
7. Xuất qua Flow F (export MP4) khi xong.

*Clone notes:* cắt là **không phá hủy** — cập nhật danh sách "kept segments", preview phát theo edit list, chỉ render thật khi export.

---

## Flow C — Cut Out đoạn giữa

**Mục tiêu:** bỏ một đoạn ở giữa, hai nửa nối lại.

1. Trong **Editor**, kéo **playhead** tới đầu đoạn thừa; dùng `←/→` để canh chính xác frame.
2. Đặt **green Start handle** ở đầu đoạn thừa.
3. Kéo tới cuối đoạn thừa, đặt **red End handle** ở đó.
4. (Tùy chọn) preview vùng chọn bằng cách kéo playhead qua đoạn.
5. Bấm **Cut Out** → đoạn giữa biến mất, phần trước và sau **tự nối lại**; **đường vàng** đánh dấu mối nối.
6. Kiểm tra mối nối bằng Play; `Ctrl+Z` nếu cần.

*Clone notes:* khi export, FFmpeg cắt theo từng segment giữ lại rồi `concat`; re-encode quanh điểm cắt để cắt đúng frame (không lệ thuộc keyframe).

---

## Flow D — Export Animated GIF

**Mục tiêu:** chuyển một đoạn video thành GIF nhẹ.

1. Mở video trong **Editor** (hoặc chọn từ Recent Captures).
2. Kéo **green/red handle** chọn segment muốn thành GIF (khuyến nghị **< 30s**).
3. Kéo **playhead** về green handle → bấm **Play** để preview đoạn.
4. Bấm **Save As Animated GIF** (nút **GIF**).
5. Chọn **Selection** (đoạn vừa chọn) hoặc **Entire Video**.
6. Trong dialog **Create GIF**, chọn preset:
   - **Screen Video (Default)** cho screen recording,
   - **High Motion Video** cho footage/game,
   - **Reduced File Size** (scale max 200px),
   - hoặc **Custom** → chỉnh **Frame Rate / Dimensions / Aspect Ratio Lock / Dithering / Looping (vô hạn / 1 lần / số lần) / Fade to Black**.
7. Xem **ước lượng dung lượng** (cảnh báo nếu >1MB); bấm **Create**.
8. Chọn nơi lưu → GIF được xuất.

*Clone notes:* pipeline FFmpeg `palettegen` → `paletteuse`; Reduced File Size = `scale=200:200:force_original_aspect_ratio=decrease`; looping qua `-loop`; nếu GIF mờ thì làm lại Custom + fps cao hơn.

---

## Flow E — Save Frame thành ảnh

**Mục tiêu:** trích đúng một frame của video ra ảnh để annotate.

1. Mở video trong **Editor**.
2. Kéo **playhead** tới gần frame mong muốn.
3. Dùng **Next/Previous Frame** hoặc phím `←/→` để canh **chính xác từng frame**.
4. Bấm **Save Frame as Image** (icon máy ảnh).
5. Frame mở thành **ảnh mới** trên canvas — annotate (arrow/shape/text), edit, share như screenshot thường.

*Clone notes:* decode frame tại thời điểm playhead bằng FFmpeg (seek chính xác, không lấy frame downscale từ preview); tạo capture ảnh mới trong `ezsnag_canvas`. Hoạt động cả với video import MP4/MOV/GIF.

---

## Flow F — Export MP4 (chia sẻ File)

**Mục tiêu:** xuất video đã chỉnh thành file MP4.

1. Sau khi trim/Cut Out xong, bấm **Share / Save** → chọn **File**.
2. Chọn thư mục + tên file `.mp4`.
3. Hệ thống render edit list (cắt + concat các đoạn giữ lại) và encode.
4. File MP4 được lưu; (tùy chọn) copy đường dẫn vào clipboard.

*Clone notes:* encode `libx264 -crf 20 -preset medium`, AAC audio, `yuv420p`, `+faststart`; fps theo capture thực; không phơi CRF cho user thường (ẩn trong Preferences nếu cần — SPEC 10).

---

## Flow G — Combine nhiều clip (Add Videos)

**Mục tiêu:** ghép nhiều recording ngắn thành một video.

1. Mở một video trong **Editor**.
2. Trên playback toolbar bấm **Add Videos**.
3. Chọn các clip từ **Library**.
4. **Kéo-thả** sắp xếp lại thứ tự clip.
5. Xác nhận → các clip ghép thành một video trên canvas.
6. Trim/Cut Out tiếp nếu cần → export qua Flow F.

*Clone notes:* FFmpeg `concat`; **normalize** trước (cùng resolution/fps/pixfmt/codec/sample rate), mismatch resolution thì pad letterbox theo clip lớn nhất.

---

## Flow H — Video from Images (ảnh → video có narration)

**Mục tiêu:** biến nhiều screenshot thành video how-to có giọng thuyết minh.

1. Trong **Editor**, mở **Recent Captures** / **Library**.
2. **Multi-select** ảnh bằng `Ctrl+click`.
3. Bấm **Create > Video from Images** → recording toolbar hiện ra.
4. **Kéo-thả** sắp xếp thứ tự ảnh trong tray.
5. Cấu hình: **Background Color** (dark/light), **Cursor**, **Microphone** (để narrate), **Webcam** (tùy chọn).
6. Bấm **Record** (`Shift+F9`) → bắt đầu thuyết minh ảnh đầu tiên.
7. Chuyển ảnh bằng `←/→` hoặc **Next/Previous**.
8. **Annotate khi đang quay** (hoặc khi Pause): chọn arrow/shape/step từ **Favorites**, kéo-thả lên ảnh hiện tại.
9. **Pause/Resume** bằng `Shift+F9` khi cần.
10. Bấm **Stop** (`Shift+F10`).
11. Video mở trên canvas để trim/edit và export **MP4** (Flow F).

*Clone notes:* thuộc SPEC 08 (`ezsnag_templates`), phần ghi do `ezsnag_video`; compositor render mỗi ảnh + overlay annotation thành frame, ghi cùng track mic; chuyển ảnh = đổi nguồn compositor (không transition).

---

## Flow phụ — Webcam PiP khi quay (tham chiếu)

1. Ở tab **Video** bật **Camera/Webcam** → mở **Camera Options** → chọn **Placement** (góc), **Shape** (tròn/vuông/bo góc), **Size**.
2. Chọn chế độ **Picture-in-Picture** (overlay góc) hoặc **Fullscreen Webcam**.
3. Quay như Flow A.
4. **Trong khi quay**, bấm `Ctrl+Shift+F` (hoặc nút Fullscreen Webcam / PiP) để **chuyển qua lại**; kéo góc preview để resize, kéo preview giữa các góc để reposition.

*Lưu ý:* PiP **chỉ chỉnh lúc quay** — sau khi quay không sửa được vị trí/kích thước webcam trong clone (đó là phạm vi Camtasia). Xem SPEC 05 §6.
