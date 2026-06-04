#pragma once
#include <QMainWindow>

// App shell P0 — cửa sổ chính rỗng (placeholder cho EditorWindow ở các phase sau).
// Theo SPEC 00: đây là lớp `ui`, sẽ host Capture/Editor/Library dần theo roadmap.
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
};
