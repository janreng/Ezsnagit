#pragma once

// RecentTray — khay (tray) hiển thị các ảnh chụp gần đây dưới dạng thumbnail.
// Một dải ngang các nút thumbnail bấm được, ảnh mới nhất nằm bên trái.
// Mỗi thumbnail lưu kèm ảnh full-res; bấm vào sẽ phát captureChosen(full).
// Có thể nhét vào dock hoặc đặt làm widget đáy của MainWindow.

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QImage>
#include <QIcon>
#include <QPixmap>
#include <QVector>

class QLabel;

class RecentTray : public QWidget {
    Q_OBJECT

public:
    explicit RecentTray(QWidget *parent = nullptr);

    // Thêm 1 ảnh chụp mới vào đầu khay (bên trái). Lưu ảnh full-res,
    // hiển thị thumbnail thu nhỏ. Giữ tối đa 12 ảnh gần nhất.
    void addCapture(const QImage &img);

    // Số lượng thumbnail đang có trong khay.
    int count() const;

signals:
    // Phát khi người dùng bấm 1 thumbnail; trả về ảnh full-res đã lưu.
    void captureChosen(const QImage &full);

private:
    QHBoxLayout *m_layout = nullptr;        // hàng ngang chứa các nút thumbnail
    QVector<QImage> m_images;               // ảnh full-res, song song với m_buttons
    QVector<QToolButton *> m_buttons;       // nút thumbnail, song song với m_images
    QLabel *m_hint = nullptr;               // nhãn gợi ý khi khay rỗng (optional)

    static constexpr int kMaxItems = 12;    // số ảnh tối đa giữ lại
    static constexpr int kThumbH = 56;      // chiều cao thumbnail (px)

    // Cập nhật hiện/ẩn nhãn gợi ý theo trạng thái rỗng.
    void updateHint();
};
