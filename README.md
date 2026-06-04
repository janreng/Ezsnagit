# Ezsnagit

Phần mềm **chụp & chú thích màn hình** cho Windows — nhẹ, nhanh, **miễn phí**, chạy **offline**.
Xây bằng **C++ / Qt6**.

> **Miễn phí · không subscription · chạy offline · nhẹ & nhanh.**

## Ezsnagit làm được gì

Một ứng dụng desktop gồm các bề mặt chính:

1. **Capture** — chụp vùng tự chọn, chụp cửa sổ, chụp toàn màn hình, scrolling capture, quay video, lấy chữ từ ảnh (OCR).
2. **Editor** — biên tập với hệ **chú thích dạng vector** (mũi tên, text, callout, hình khối, đánh số bước, highlight, làm mờ…),
   **hiệu ứng** (viền, đổ bóng, phối cảnh, lọc màu…) và công cụ đơn giản hóa ảnh.
3. **Library & Share** — tự lưu mọi ảnh chụp, tìm kiếm/gắn thẻ, và chia sẻ ra nhiều đích.

## Tech stack

| Thành phần | Lựa chọn |
|---|---|
| Ngôn ngữ | C++20 |
| GUI | Qt6 (Widgets, Gui, Network, Multimedia) |
| Build | CMake + Ninja + MinGW |
| Screen capture | Windows.Graphics.Capture / DXGI Desktop Duplication / GDI |
| OCR (lấy chữ) | Tesseract |
| Đơn giản hóa ảnh / Smart Move | OpenCV |
| Video & GIF | FFmpeg |
| Thư viện ảnh | SQLite |
| Định dạng gốc | `.ezsnagx` (giữ chú thích vector, sửa lại được) |

## Cấu trúc tài liệu

```
docs/
├── specs/           # Đặc tả tính năng + ghi chú implement
│   └── ux-flows/    # Luồng thao tác từng bước
└── _assets/
ROADMAP.md           # Kế hoạch phân kỳ
```

## Nguyên tắc phát triển

- **Mỗi tính năng = 1 commit + 1 release riêng**, không gom.
- **Chia nhỏ file/function**, tách logic thuần ra module riêng, glue Qt mỏng ở lớp giao diện.
- **Test headless** cho logic thuần.
- CHANGELOG/release notes viết **tiếng Việt có dấu**, mô tả theo góc người dùng.

## Build

```
build.bat          REM cần Qt6.8.3 MinGW ở C:\Qt
package.bat        REM đóng gói portable + installer (cần Inno Setup)
```

## Trạng thái

🟢 Đang phát triển — đã chụp được toàn màn hình + chụp vùng tự chọn. Xem `ROADMAP.md`.
