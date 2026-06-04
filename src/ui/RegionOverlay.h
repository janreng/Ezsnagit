#pragma once
#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QRect>

// Overlay chọn vùng kiểu "screen-freeze": nhận ảnh đã chụp toàn virtual desktop, hiển thị
// full màn hình (làm tối), người dùng kéo chọn 1 vùng -> phát ảnh đã crop (pixel vật lý).
// Esc/chuột phải = hủy. (SPEC 01 region capture + screen-freeze.)
class RegionOverlay : public QWidget
{
    Q_OBJECT
public:
    // frozen = ảnh toàn virtual desktop (pixel vật lý). overlayGeometry = vùng logical phủ hết
    // các màn hình (QGuiApplication virtual geometry).
    explicit RegionOverlay(const QImage &frozen, const QRect &overlayGeometry,
                           QWidget *parent = nullptr);

signals:
    void regionSelected(const QImage &cropped); // vùng đã chọn (ảnh vật lý)
    void cancelled();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void closeEvent(QCloseEvent *) override;   // đảm bảo luôn phát 1 tín hiệu

private:
    QRect selectionWidgetRect() const; // vùng chọn theo toạ độ widget (chuẩn hoá)
    void updateMagnifier(const QPoint &cursor);

    QImage m_frozen;
    bool m_selecting = false;
    bool m_done = false;               // đã phát regionSelected/cancelled chưa
    QPoint m_start, m_cur;
    class Magnifier *m_mag = nullptr;  // kính lúp theo con trỏ
};
