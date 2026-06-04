# Phát hành Ezsnagit (quy trình release lên GitHub)

Theo đúng quy trình của Ezcel. Auto-update (làm ở phase sau) sẽ đọc **GitHub Releases** của repo
`<OWNER>/Ezsnagit` — ⚠️ **chờ sếp tạo repo + xác nhận owner** (dự kiến `janreng/Ezsnagit`), rồi cập nhật vào updater.

## 1. Tăng số phiên bản
Sửa cùng một số ở 2 nơi:
- `CMakeLists.txt` → `project(Ezsnagit VERSION x.y.z ...)`
- `installer.iss` → `MyAppVersion` (hoặc truyền qua `-DMyAppVersion` lúc ISCC)

> Quy ước: **mỗi tính năng = 1 commit + 1 bump + 1 release riêng** (kế thừa Ezcel).

## 2. Build app + đóng gói installer
```bat
build.bat        REM tao build\Ezsnagit.exe
package.bat      REM windeployqt -> dist\Ezsnagit\, roi ISCC -> installer\Ezsnagit-Setup-x.y.z.exe
```
Cần **Inno Setup 6** để ra file `.exe` cài đặt (không có thì chỉ ra bản portable trong `dist\`).

## 3. Tạo GitHub Release
```bat
gh release create vX.Y.Z "installer\Ezsnagit-Setup-X.Y.Z.exe" --title "vX.Y.Z" --notes-file CHANGELOG.md
```
Yêu cầu:
- **Tag** theo phiên bản, vd `v0.2.0` (app bỏ chữ `v` khi so sánh — xem `src/core/Version.cpp`).
- **Đính kèm** `Ezsnagit-Setup-X.Y.Z.exe` làm asset (updater tìm file `.exe`).
- Repo phải **public** để updater đọc release ẩn danh.

## 4. CHANGELOG / release notes
- Viết **tiếng Việt CÓ DẤU**, mô tả theo góc người dùng (KHÔNG ghi kỹ thuật module/test).
- Commit/tag thì ASCII.

## Trạng thái hiện tại
- ⏳ Chưa có git remote — chờ sếp tạo repo GitHub rồi `git remote add origin ...` + `git push -u origin master`.
- ⏳ Updater (đọc Releases) làm ở phase sau; hiện đã có sẵn `ezsnag_core` so sánh semver.
