# SPEC 12 — Non-Functional Requirements & Định vị (Ezsnagit)

> Module: xuyên suốt (cross-cutting) — ràng buộc mọi module.

## Vì sao spec này quan trọng nhất

**Cơ hội định vị của Ezsnagit nằm ở NFR + mô hình bản quyền, KHÔNG chỉ ở danh sách tính năng.**
Trọng tâm là giá/mô hình bản quyền và resource footprint, không phải lấp khoảng trống tính năng.
Các mục tiêu đo được: khởi động nhanh, gọn nhẹ, DPI/đa màn hình đúng, Library an toàn qua update, hoạt động offline,
miễn phí. Đây là **lý do tồn tại** của sản phẩm. Mỗi mục dưới là một **target đo được**.

## 12.1 Hiệu năng & footprint

| Yêu cầu | Target đo được | Vì sao |
|---|---|---|
| Cold start nhanh | **< 1 giây** mở app trên laptop đời 2020 | Khởi động tức thì là trải nghiệm cốt lõi |
| Cài đặt gọn | Base installer **< 60 MB** (chưa gồm gói OCR/FFmpeg/OpenCV tùy chọn) | Tránh "bloat", tải nhanh, cài gọn |
| Idle nhẹ | **Không** helper chạy nền ngốn CPU; RAM idle thấp | Không chiếm tài nguyên lúc rảnh |
| Chụp mượt | Capture overlay hiện **tức thì**, không lag trên ảnh lớn | Không freeze với capture lớn |

**Ghi chú kỹ thuật:** tách gói nặng (Tesseract/FFmpeg/OpenCV) thành **component pack tải khi cần** để base install gọn
(SPEC 11 §11.5). Lazy-init các module nặng; app shell + capture phải khởi động độc lập với OCR/video.

## 12.2 DPI & đa màn hình (ngày-một, không "verify sau")

- **Bắt buộc đúng** hình học capture trên setup **đa màn hình khác scale** (vd 150% + 100%). Đây là yêu cầu nền tảng, không được sai.
- Capture overlay phải phủ đúng mọi monitor; toạ độ region đúng theo per-monitor DPI.
- **Bỏ** khung "chỉ chụp 1 monitor một lúc" đã copy nhầm trong SPEC 01 — coi region span đa màn hình là tính năng P1, không phải "để sau".
- **Ghi chú kỹ thuật:** dùng per-monitor DPI awareness (`SetProcessDpiAwarenessContext` PER_MONITOR_V2), virtual desktop geometry cho overlay đa màn hình.

## 12.3 Độ tin cậy Library (trust dealbreaker)

- Library **sống sót qua update app** — không bao giờ mất dữ liệu sau khi cập nhật.
- **Backup/restore** dễ: auto-backup định kỳ (mỗi N lần ghi) + 1 nút export/import toàn kho.
- An toàn với cloud-sync (không hỏng khi thư mục store nằm trong OneDrive/Dropbox).
- Ghi atomic (`QSaveFile`), không bao giờ để file `.ezsnagx` dở dang.
- **→ Nâng backup/restore từ "ngoài scope" (SPEC 06) lên MVP.**

## 12.4 Crash-safety & phục hồi

- **Phục hồi capture chưa lưu sau crash** — không mất capture nếu app crash lúc finalize video.
- Video stream thẳng xuống đĩa khi quay; ảnh capture ghi tạm ngay khi tạo.

## 12.5 Hoạt động offline & bản quyền (định vị sản phẩm)

- **Khởi động + chụp KHÔNG cần mạng.** Không ép đăng nhập online, không phone-home.
- **Bản quyền: MIỄN PHÍ** (chốt với sếp 2026-06-04) — không subscription, không thu hồi license, không kích hoạt. Đây là khẩu hiệu định vị cốt lõi của sản phẩm.
- Không phone-home, không bắt đăng nhập.

## 12.6 UX & khám phá tính năng

- **Progressive disclosure**: không làm choáng người mới; tính năng nâng cao ẩn bớt.
- **Surface tính năng ẩn**: tính năng hay bị bỏ quên cần được làm nổi bật. Cân nhắc onboarding tips + command palette tìm lệnh.

## 12.7 Tiêu chí nghiệm thu theo workflow (đo được)

MVP phải đạt:
- **Bug-report loop** (capture → arrow+text → blur → copy) hoàn tất **< 5 giây**, **không cần mở editor nặng**.
- **Repeat Last Capture**: lặp capture trước bằng **1 phím**.
- Annotation thường dùng (arrow/text/box/highlight) đặt được bằng **phím đơn** + Quick Style.

## Bảng ưu tiên
| NFR | Phase |
|---|---|
| DPI/đa màn hình đúng · footprint gọn · offline-first · ghi atomic | **P0–P1 (nền)** |
| Library backup/restore · crash-recovery | MVP (P2–P5) |
| Command palette / onboarding discoverability · component packs | fast-follow |
| Khẩu hiệu bản quyền | xuyên suốt (README/marketing) |

## Điểm chưa chắc
- ~~Mô hình bản quyền~~ — **ĐÃ CHỐT: miễn phí** (2026-06-04).
- Target con số (1s, 60MB) là mục tiêu đề xuất — chỉnh theo thực tế build.
