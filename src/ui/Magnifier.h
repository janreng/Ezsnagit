#pragma once

// Magnifier (loupe) — widget phóng to vùng dưới con trỏ khi capture,
// giúp chọn vùng chính xác tới từng pixel.
// Widget này KHÔNG tự di chuyển — overlay sẽ gọi move() để đặt nó cạnh con trỏ.

#include <QWidget>
#include <QImage>
#include <QPoint>

class Magnifier : public QWidget {
    Q_OBJECT

public:
    explicit Magnifier(QWidget *parent = nullptr);

    // Hiển thị vùng phóng to:
    //  - source: ảnh nguồn (thường là screenshot toàn màn hình)
    //  - focusImgPt: điểm focus tính theo pixel của ảnh nguồn (vị trí con trỏ)
    //  - zoom: hệ số phóng đại (mặc định 8x)
    // Lưu lại dữ liệu rồi trigger repaint. KHÔNG tự move().
    void showAt(const QImage &source, const QPoint &focusImgPt, int zoom = 8);

    // Đã có ảnh nguồn hợp lệ hay chưa.
    bool hasImage() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage m_source;            // ảnh nguồn để crop
    QPoint m_focusImgPt;        // điểm focus (toạ độ pixel trong m_source)
    int m_zoom = 8;             // hệ số phóng đại
};
