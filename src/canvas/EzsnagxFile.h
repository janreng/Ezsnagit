#pragma once
#include "CanvasDocument.h"
#include <QString>

// Module I/O cho định dạng tài liệu native .ezsnagx (JSON) + export ảnh PNG.
// Thuần dữ liệu, không phụ thuộc UI — dùng được từ headless test.
namespace canvas {

// Lưu doc ra file .ezsnagx (JSON) một cách NGUYÊN TỬ (QSaveFile commit).
// Trả false nếu path rỗng hoặc lỗi ghi/commit.
bool saveEzsnagx(const CanvasDocument &doc, const QString &path);

// Đọc file .ezsnagx, parse JSON, nạp vào out. Trả false nếu lỗi đọc/parse.
bool loadEzsnagx(CanvasDocument &out, const QString &path);

// Render flatten rồi lưu PNG. Trả false nếu ảnh render null hoặc lưu lỗi.
bool exportPng(const CanvasDocument &doc, const QString &path);

// Phần mở rộng mặc định của định dạng native.
QString defaultExtension();

} // namespace canvas
