# SPEC 12 — Non-Functional Requirements & Định vị (Ezsnagit)

> Nguồn: docs/research/06 (forum), 08 (review/feature-request), 07 (đối thủ) · docs/REVIEW.md (finding 3–5, §3)
> Module: xuyên suốt (cross-cutting) — ràng buộc mọi module.

## Vì sao spec này quan trọng nhất

Research kết luận: **cơ hội cạnh tranh của Ezsnagit nằm ở NFR + mô hình bản quyền, KHÔNG phải ở danh sách tính năng**
(research 08 §1 nguyên văn: "almost entirely about price/licensing model and resource footprint, not feature gaps").
Snagit thua đúng ở mấy điểm đo được: khởi động chậm, nặng, DPI/đa màn hình sai, mất Library sau update, ép đăng nhập online,
chuyển sang subscription + thu hồi license. Đây là **lý do tồn tại** của bản clone. Mỗi mục dưới là một **target đo được**.

## 12.1 Hiệu năng & footprint

| Yêu cầu | Target đo được | Vì sao |
|---|---|---|
| Cold start nhanh | **< 1 giây** mở app trên laptop đời 2020 | Snagit bị chê khởi động chậm (08 §5) |
| Cài đặt gọn | Base installer **< 60 MB** (chưa gồm gói OCR/FFmpeg/OpenCV tùy chọn) | Snagit ~420MB bị chê "bloat" (06 §2.2) |
| Idle nhẹ | **Không** helper chạy nền ngốn CPU; RAM idle thấp | Snagit ngốn CPU lúc rảnh (08 §5) |
| Chụp mượt | Capture overlay hiện **tức thì**, không lag trên ảnh lớn | Snagit freeze với capture lớn (08 §5) |

**Ghi chú clone:** tách gói nặng (Tesseract/FFmpeg/OpenCV) thành **component pack tải khi cần** để base install gọn
(SPEC 11 §11.5). Lazy-init các module nặng; app shell + capture phải khởi động độc lập với OCR/video.

## 12.2 DPI & đa màn hình (ngày-một, không "verify sau")

- **Bắt buộc đúng** hình học capture trên setup **đa màn hình khác scale** (vd 150% + 100%). Đây là bug #1 chưa fix của Snagit (08 §5/§7).
- Capture overlay phải phủ đúng mọi monitor; toạ độ region đúng theo per-monitor DPI.
- **Bỏ** khung "chỉ chụp 1 monitor một lúc" đã copy nhầm trong SPEC 01 — coi region span đa màn hình là tính năng P1, không phải "để sau".
- **Ghi chú clone:** dùng per-monitor DPI awareness (`SetProcessDpiAwarenessContext` PER_MONITOR_V2), virtual desktop geometry; tham khảo Flameshot (cùng Qt) cho overlay đa màn hình.

## 12.3 Độ tin cậy Library (trust dealbreaker)

- Library **sống sót qua update app** (Snagit từng mất sạch Library sau update — 06 §2.4).
- **Backup/restore** dễ: auto-backup định kỳ (mỗi N lần ghi) + 1 nút export/import toàn kho.
- An toàn với cloud-sync (không hỏng khi thư mục store nằm trong OneDrive/Dropbox).
- Ghi atomic (`QSaveFile`), không bao giờ để file `.ezsnagx` dở dang.
- **→ Nâng backup/restore từ "ngoài scope" (SPEC 06) lên MVP.**

## 12.4 Crash-safety & phục hồi

- **Phục hồi capture chưa lưu sau crash** (Snagit mất capture khi crash lúc finalize video — 08 §9).
- Video stream thẳng xuống đĩa khi quay; ảnh capture ghi tạm ngay khi tạo.

## 12.5 Hoạt động offline & bản quyền (định vị sản phẩm)

- **Khởi động + chụp KHÔNG cần mạng.** Không ép đăng nhập online, không phone-home (06 §2.6, 08 §7).
- **Bản quyền: MIỄN PHÍ** (chốt với sếp 2026-06-04) — không subscription, không thu hồi license, không kích hoạt. Đây là khẩu hiệu định vị, đối lập trực tiếp lời chê to nhất của Snagit (08 §4).
- Không phone-home, không bắt đăng nhập.

## 12.6 UX & khám phá tính năng

- **Progressive disclosure**: không làm choáng người mới (08 §6); tính năng nâng cao ẩn bớt.
- **Surface tính năng ẩn**: ngay cả TechSmith thừa nhận feature hay bị bỏ quên (06 §3). Cân nhắc onboarding tips + command palette tìm lệnh.

## 12.7 Tiêu chí nghiệm thu theo workflow (đo được)

Lấy từ research 09 §6 — MVP phải đạt:
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
