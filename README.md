# Ezsnagit

Clone của **TechSmith Snagit** — phần mềm chụp/quay màn hình + biên tập ảnh chú thích.
Xây bằng **C++ / Qt6** (đồng bộ stack với dự án Ezcel để tái dùng kinh nghiệm & toolchain).

> Tên nội bộ: **Ezsnagit**. Mục tiêu: tái tạo trải nghiệm Snagit (Capture → Editor → Library → Share)
> ở mức tính năng cốt lõi trước, mở rộng dần theo roadmap.

## Định vị (vì sao làm bản clone này)

Research cộng đồng/đối thủ chỉ ra: điểm yếu lớn nhất của Snagit **không phải thiếu tính năng** mà là
**mô hình bản quyền + footprint** (chuyển sang subscription + thu hồi license, nặng ~420MB, khởi động chậm,
DPI/đa màn hình sai, ép đăng nhập online, mất Library sau update). Ezsnagit nhắm đúng khe hở đó:

> **Sở hữu vĩnh viễn · không subscription · chạy offline · nhẹ & nhanh.**

Đây là kim chỉ nam, không chỉ là khẩu hiệu — xem các target đo được ở `docs/specs/12-non-functional-requirements.md`
(cold start < 1s, base install < 60MB, DPI/đa màn hình đúng ngày-một, Library backup an toàn). Ngoài ra nhắm tới
khe "UX kiểu CleanShot + automation kiểu ShareX" mà thị trường đang để trống (xem `docs/research/07-competitors.md`).

## Ezsnagit là gì

Một ứng dụng desktop gồm 3 bề mặt chính, giống Snagit:

1. **Capture Window** — chụp vùng/cửa sổ/toàn màn hình, scrolling capture, quay video, Grab Text.
2. **Editor** — canvas biên tập với hệ **annotation object dạng vector** (mũi tên, text, callout, shape,
   step, highlight, blur…), **effects** (border, shadow, perspective, color filter…), **Simplify** & **OCR**.
3. **Library** — tự lưu mọi capture (`.ezsnagx`), khay Recent, tìm kiếm/tag, và **Share** ra nhiều đích.

## Tech stack

| Thành phần | Lựa chọn |
|---|---|
| Ngôn ngữ | C++20 |
| GUI | Qt6 (Widgets, Gui, Network, Multimedia) |
| Build | CMake + Ninja + MinGW (giống Ezcel, toolchain ở `C:\Qt`) |
| Screen capture | Win: Windows.Graphics.Capture / DXGI Desktop Duplication / GDI BitBlt |
| OCR (Grab Text) | Tesseract |
| Simplify / Smart Move | OpenCV (phát hiện phần tử UI) |
| Video encode + GIF | FFmpeg |
| Library store | SQLite |
| File format gốc | `.ezsnagx` (giữ object vector, editable) |

## Cấu trúc tài liệu

```
docs/
├── research/        # Research thô từ docs chính thức TechSmith (5 cụm tính năng)
├── specs/           # Spec clone: mỗi tính năng + ghi chú implement C++/Qt
│   └── ux-flows/    # Luồng thao tác (UX follow) từng bước
└── _assets/         # Ảnh tham chiếu UI
ROADMAP.md           # Kế hoạch phân kỳ P0..P12
docs/specs/00-overview-architecture.md  # Kiến trúc module C++ + IA app
```

## Nguyên tắc phát triển (kế thừa từ Ezcel)

- **Mỗi tính năng = 1 commit + 1 release riêng**, không gom.
- **Chia nhỏ file/function**, tách logic thuần ra module riêng, glue Qt mỏng ở window.
- **Test headless** cho logic thuần (capture/effect/ocr/serialize có thể test không cần GUI).
- CHANGELOG/release notes viết **tiếng Việt có dấu**, mô tả theo góc người dùng.

## Trạng thái

🟢 **P0 xong** — khung CMake/Qt6 dựng được: `Ezsnagit.exe` (app shell mở cửa sổ) + module `ezsnag_core` (version/semver) + test harness headless (`test_core` 9/9 pass) + per-monitor DPI awareness. Tiếp theo: **P1 — Capture nền** (xem `ROADMAP.md`).

### Build
```
build.bat        REM Qt6.8.3 MinGW ở C:\Qt (giống Ezcel)
```
