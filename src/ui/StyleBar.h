#pragma once

// StyleBar — thanh Quick Styles: mỗi preset annotation là 1 nút bấm.
// Bấm 1 nút sẽ phát signal styleChosen để MainWindow set tool/color/penWidth
// cho canvas. Có thể nhét vào QToolBar qua addWidget hoặc đặt dưới toolbar.

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QPixmap>
#include <QIcon>

#include "canvas/QuickStyle.h"

class StyleBar : public QWidget {
    Q_OBJECT

public:
    explicit StyleBar(QWidget *parent = nullptr);

    // Dựng lại toàn bộ nút từ danh sách style mới.
    void setStyles(const QVector<canvas::QuickStyle> &styles);

signals:
    // Phát khi người dùng bấm 1 style preset.
    void styleChosen(const canvas::QuickStyle &style);

private:
    QHBoxLayout *m_layout = nullptr;   // hàng ngang chứa các nút

    // Xoá hết nút hiện có (dùng trước khi rebuild).
    void clearButtons();
};
