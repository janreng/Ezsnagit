#pragma once
#include <QMainWindow>

class QLabel;
class QScrollArea;

// App shell — P1 slice 1: nút "Chụp toàn màn hình" -> hiện ảnh chụp trong app.
// (Editor/annotation thêm ở các phase sau; xem SPEC 00.)
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void captureFullScreen();
    void captureRegion();

private:
    void showCaptured(const QImage &img);

    QScrollArea *m_scroll = nullptr;
    QLabel *m_canvas = nullptr;   // hiển thị ảnh chụp (placeholder editor canvas)
};
