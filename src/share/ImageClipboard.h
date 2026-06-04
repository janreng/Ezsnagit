#pragma once
#include <QImage>
#include <QString>

// Xuất ảnh đã chụp — copy vào clipboard hệ thống và lưu ra file PNG.
// Phục vụ fast-path báo lỗi: chụp -> paste thẳng vào báo cáo.
namespace share {

// Copy ảnh vào clipboard hệ thống (QGuiApplication::clipboard()->setImage).
// Trả false nếu ảnh null hoặc không có clipboard.
bool copyImageToClipboard(const QImage &img);

// Lưu ảnh ra file PNG tại path. Trả false nếu ảnh null hoặc path rỗng.
bool saveImagePng(const QImage &img, const QString &path);

// Gợi ý tên file dạng "<prefix>-<seq>.png". Thuần, test được (không gọi Date::now).
QString suggestFileName(const QString &prefix, int seq);

} // namespace share
