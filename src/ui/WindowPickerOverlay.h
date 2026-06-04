#pragma once
#include <QWidget>
#include <QImage>
#include <QRect>
#include <QVector>

// Overlay chọn CỬA SỔ kiểu screen-freeze: hiển thị ảnh đã chụp (làm tối), di chuột tới đâu
// thì highlight cửa sổ dưới con trỏ; click = chụp đúng cửa sổ đó. Esc/chuột phải = hủy.
// windowRectsImg: rect các cửa sổ theo toạ độ ẢNH (pixel), Z-order trước→sau.
class WindowPickerOverlay : public QWidget
{
    Q_OBJECT
public:
    WindowPickerOverlay(const QImage &frozen, const QVector<QRect> &windowRectsImg,
                        const QRect &overlayGeometry, QWidget *parent = nullptr);

signals:
    void windowSelected(const QImage &cropped);
    void cancelled();

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void closeEvent(QCloseEvent *) override;   // đảm bảo luôn phát 1 tín hiệu

private:
    QImage m_frozen;
    QVector<QRect> m_winImg;   // rect cửa sổ theo toạ độ ảnh
    int m_hover = -1;          // chỉ số cửa sổ đang highlight
    bool m_done = false;       // đã phát windowSelected/cancelled chưa
};
