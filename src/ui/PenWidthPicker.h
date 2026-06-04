#pragma once

#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

// Widget chọn độ dày nét vẽ cho công cụ chú thích (annotation).
// Hiển thị một hàng nhỏ gọn: nhãn "Nét:" + combobox các độ dày px.
class PenWidthPicker : public QWidget {
    Q_OBJECT

public:
    explicit PenWidthPicker(QWidget *parent = nullptr);

    // Trả về độ dày nét đang được chọn (px).
    int width() const;

    // Chọn độ dày gần nhất với giá trị w trong danh sách có sẵn.
    void setWidth(int w);

signals:
    // Phát ra khi người dùng đổi độ dày nét.
    void widthChanged(int w);

private:
    QComboBox *m_combo;
};
