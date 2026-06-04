#pragma once
#include <QImage>
#include <QString>

// OCR — trích xuất text từ ảnh bằng cách gọi CLI Tesseract (tesseract.exe).
// Module tự co giãn theo môi trường: nếu KHÔNG cài Tesseract thì degrade gracefully
// (isAvailable() == false, grabText() trả "" ngay, không hang). Không phụ thuộc
// thư viện ngoài — chỉ shell-out qua QProcess.
namespace ocr {

// Tìm đường dẫn tới tesseract.exe. Ưu tiên trong PATH, sau đó các vị trí cài phổ biến.
// Trả "" nếu không tìm thấy.
QString tesseractPath();

// true nếu tìm thấy tesseract.exe (tức !tesseractPath().isEmpty()).
bool isAvailable();

// Chạy OCR trên ảnh, trả về text nhận dạng (UTF-8, đã trim).
// Trả "" nếu Tesseract không có sẵn, ảnh null, hoặc tiến trình lỗi/timeout (15s).
QString grabText(const QImage &img, const QString &lang = "eng");

} // namespace ocr
