#pragma once
#include <QString>

// Lưu/đọc tuỳ chọn người dùng qua QSettings (org "Ez", app "Ezsnagit").
// Wrapper mỏng, getter/setter có kiểu; mỗi lời gọi tạo 1 QSettings riêng.
namespace core {

class Settings {
public:
    // Thư mục lưu mặc định (mặc định = Desktop của user nếu chưa đặt).
    static QString defaultSaveDir();
    static void setDefaultSaveDir(const QString &dir);

    // Thư mục dùng lần lưu/mở gần nhất.
    static QString lastDir();
    static void setLastDir(const QString &dir);

    // Bộ đếm tên file lưu-nhanh (tăng dần), trả giá trị hiện tại rồi tăng.
    static int nextQuickSaveSeq();

    // Tự copy clipboard sau khi chụp (mặc định true).
    static bool autoCopy();
    static void setAutoCopy(bool on);
};

} // namespace core
