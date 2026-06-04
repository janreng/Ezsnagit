#pragma once

#include <QDialog>

// Khai báo trước để header nhẹ — chỉ cần con trỏ.
class QCheckBox;
class QLineEdit;

// Hộp thoại "Thiết lập" — đọc/ghi tuỳ chọn qua core::Settings.
class PreferencesDialog : public QDialog {
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

private slots:
    // Mở hộp chọn thư mục và cập nhật ô đường dẫn.
    void chooseSaveDir();
    // Ghi tuỳ chọn về Settings rồi accept().
    void onAccept();

private:
    QCheckBox *m_autoCopy = nullptr;     // Tự copy clipboard sau khi chụp.
    QLineEdit *m_saveDir = nullptr;      // Thư mục lưu mặc định (chỉ đọc).
};
