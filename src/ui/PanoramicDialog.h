#pragma once
#include <QDialog>
#include <QImage>
#include <QVector>

class QLabel;

// Hộp thoại chụp ghép dọc (panoramic thủ công): người dùng chụp lần lượt nhiều khung
// (vùng), app tích luỹ rồi ghép thành một ảnh dài. An toàn (không tự cuộn màn hình).
// Modeless: bấm "Chụp thêm khung" -> phát captureRequested, MainWindow lo chụp rồi
// gọi addFrame(); "Ghép xong" -> accept() (MainWindow đọc frames() và ghép).
class PanoramicDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PanoramicDialog(QWidget *parent = nullptr);

    void addFrame(const QImage &img);
    const QVector<QImage> &frames() const { return m_frames; }

signals:
    void captureRequested();

private:
    void updateInfo();

    QVector<QImage> m_frames;
    QLabel *m_info = nullptr;
    class QPushButton *m_done = nullptr;
};
