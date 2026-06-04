# Ezsnagit — Roadmap (v2, sau review)

> Phiên bản này đã chỉnh theo `docs/REVIEW.md`: **MVP định nghĩa theo WORKFLOW**, không theo "hoàn thành module".
> Nguyên tắc: mỗi tính năng = 1 release. Ưu tiên đặt theo "daily-driver" mà research 06–09 chỉ ra, không theo thứ tự panel.

Ký hiệu: 🟥 chưa làm. Cột **MVP** = thuộc bản dùng-được tối thiểu.

---

## Định nghĩa MVP (theo workflow, không theo module)

MVP = **2 workflow chủ lực chạy thật nhanh**, kèm nền NFR:

- **MVP-A — Bug-report loop** (luồng tần suất cao nhất): chụp region/window/fullscreen + **global hotkey (đổi được)** +
  **Repeat Last Capture** + Arrow/Text/Callout/Box/Highlight có **Quick Styles** + Blur/redact + Crop +
  **preset copy-thẳng-clipboard-không-mở-editor** + lưu PNG + Recent tray + tìm theo tên/ngày + **Capture Info overlay**.
  → Tiêu chí: hoàn tất **< 5 giây**.
- **MVP-B — Doc-steps loop**: + **Step tool** (thủ công) + bộ **Combine-in-Template** nhỏ (cạnh nhau / đánh số / before-after) + export PDF/ảnh.

Nền bắt buộc kèm MVP: **NFR** (SPEC 12) — DPI/đa màn hình đúng, footprint gọn, offline-first, Library atomic + backup.

---

## Phases

### P0 — Khung dự án + chốt nền 🟥
- CMake + Qt6 + Ninja/MinGW skeleton (như Ezcel), app shell.
- **Chốt NFR (SPEC 12)** làm ràng buộc: per-monitor DPI awareness, lazy-init module nặng, ghi atomic.
- Hệ test headless.

### P1 — Capture nền + Quick Capture fast-path 🟥 ⭐MVP
- Chụp **Region / Window / Full screen**; overlay **đa màn hình + mixed-DPI ĐÚNG** (NFR §12.2) — nền tảng, không "để sau".
- **Global hotkey đổi được** + **Repeat Last Capture** (default `Ctrl+Shift+R`).
- **Screen-freeze** trước khi chọn vùng + magnifier/crosshair + self-timer (UX win rẻ, research 07).
- **Preset copy-thẳng-clipboard** (không mở editor) — enabler của bug-report loop.
- `ezsnag_capture` (lớp platform Windows).

### P2 — Editor canvas + document model 🟥 ⭐MVP
- Canvas + `ezsnag_canvas` object graph vector (QGraphicsScene), z-order, undo/redo.
- Lưu/mở **`.ezsnagx`** (atomic, recoverable) + export PNG/JPG (flatten).
- **Capture Info overlay** (kéo lên sớm — core của bug-report).

### P3 — Annotation cốt lõi + Quick Styles 🟥 ⭐MVP
- Arrow, Text, Callout, Shape/Box, Line, Highlight, **Step (thủ công)**, Blur/redact (8+ tool).
- **Quick Styles + Favorites** ngay từ đầu (đây là "linh hồn" tốc độ, không để P12).
- Tool đặt được bằng **phím đơn**. Redact an toàn = solid shape (cảnh báo blur reversible tới khi flatten).

### P4 — Effects & transforms 🟥 ⭐MVP (phần lõi)
- Crop, Trim, Resize, Rotate/Flip, Border, Shadow, Color filter, **Cut Out**.

### P5 — Library + Recent + Backup 🟥 ⭐MVP
- Tự lưu mọi capture; Recent tray; tìm theo **tên/ngày/type** (đủ cho MVP).
- **Backup/restore + an toàn cloud-sync** (NFR §12.3 — nâng từ "ngoài scope" lên MVP).
- Lọc nâng cao (app/website/tag/flag) = fast-follow, không gate MVP.

### P5.5 — Templates / Combine (nhỏ) 🟥 ⭐MVP-B
- Combine-in-Template tối thiểu (cạnh nhau / đánh số / before-after) → hoàn tất MVP-B doc-steps.
  *(Sửa mâu thuẫn cũ: spec ghi P1, roadmap ghi P11 — chốt: bộ nhỏ vào MVP-B, bộ đầy đủ ở P11.)*

### P6 — Share / Outputs 🟥
- File (PNG/JPG/PDF/GIF), Clipboard, **Email (chốt cơ chế MAPI/COM)**, mở bằng app ngoài.
- **Upload layer dạng interface pluggable** ngay từ đầu (để sau cắm custom HTTP/FTP).

### P7 — After-capture chain + Pinned screenshots 🟥 (differentiator)
- Chuỗi xử-lý-sau-chụp gọn (effects→editor→save→copy-link).
- Pinned/floating screenshots.

### P8 — Scrolling capture 🟥 (rủi ro cao — spike trước)
- **Panoramic/manual stitch trước** (xác định), auto-scroll sau. "Stitch xấu còn tệ hơn không có."

### P9 — Grab Text (OCR) 🟥
- `ezsnag_ocr` qua Tesseract. **Làm TRƯỚC Simplify/Smart Move** (giá trị cao, rủi ro thấp hơn).

### P10 — Step-Capture (auto-on-click) 🟥 (flagship doc feature)
- Tự chụp mỗi click + dò tên UI element. OS-integration nặng → fast-follow sau MVP.

### P11 — Templates đầy đủ + Video 🟥
- Create from Template đầy đủ, Combine, Create Video from Images.
- **Video** (quay + trim/cut + GIF) qua FFmpeg — giữ NHẸ (không multitrack = trình biên tập video chuyên dụng). Rủi ro: system-audio loopback.

### P12 — Simplify / Smart Move 🟥
- `ezsnag_simplify` (OpenCV). **Smart Move + inpaint = rủi ro cao nhất** → gate sau feasibility spike; cân nhắc **cắt khỏi v1** hoặc chỉ manual-simplify. Edit-Text-in-place có thể bỏ v1.

### P13 — Polish 🟥
- Themes (light/dark), brand-color theme nâng cao, command palette/onboarding (discoverability), Preferences đầy đủ, auto-update.

---

## Sequencing theo rủi ro (từ REVIEW §4)
1. **Khoá sớm**: overlay mixed-DPI/đa màn hình + Library atomic/recoverable (P1–P2) — nền của mọi thứ + chính là NFR wedge.
2. **Spike trước khi cam kết**: scrolling stitcher.
3. **OCR trước Simplify/Smart Move**.
4. **Smart Move/inpaint = optional research**, không nằm critical path.
5. Giữ base install gọn bằng component pack tải-khi-cần (bảo vệ NFR footprint).

## Định vị (lead bằng cái này, không phải feature)
**"Sở hữu vĩnh viễn · không subscription · chạy offline · nhẹ"** — đây là khẩu hiệu, vì lời chê to nhất của các công cụ thương mại cùng phân khúc là bản quyền + nặng.
