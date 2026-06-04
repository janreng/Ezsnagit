#pragma once
#include <QMainWindow>
#include <QImage>

class QScrollArea;
class CanvasWidget;
namespace hotkey { class GlobalHotkey; }

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
    void captureWindow();
    void copyToClipboard();
    void rotateRight();
    void flipHorizontal();
    void openFile();
    void saveFile();
    void exportPng();

private:
    void showCaptured(const QImage &img);

    QScrollArea *m_scroll = nullptr;
    CanvasWidget *m_canvas = nullptr;   // editor canvas (ảnh nền + annotation vector)
    hotkey::GlobalHotkey *m_hotkey = nullptr;
};
