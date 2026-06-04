#pragma once
#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPoint>
#include <QRect>
#include "canvas/CanvasDocument.h"

// Canvas editor widget: hiển thị ảnh nền + danh sách annotation từ canvas::CanvasDocument,
// cho phép kéo chuột để vẽ annotation mới (Box/Arrow/Highlight/Text). Kích thước widget khớp
// đúng pixel ảnh (1:1) để toạ độ chuột = toạ độ ảnh. Thay thế cho QLabel canvas cũ trong MainWindow.
class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent = nullptr);

    // --- Nạp / lấy dữ liệu ---
    void setImage(const QImage &img);                       // đặt ảnh nền mới, resize widget theo ảnh
    void loadDocument(const canvas::CanvasDocument &doc);    // thay toàn bộ document hiện tại
    const canvas::CanvasDocument &document() const { return m_doc; }

    // --- Cấu hình công cụ vẽ cho annotation MỚI ---
    void setTool(canvas::ObjType t) { m_tool = t; }
    void setColor(const QColor &c) { m_color = c; }
    void setPenWidth(int w) { m_penWidth = w; }

    // --- Undo / Redo (uỷ thác cho document) ---
    bool undo();
    bool redo();

    bool hasImage() const { return !m_doc.background().isNull(); }

signals:
    void documentChanged(); // phát mỗi khi thêm annotation mới (để MainWindow cập nhật trạng thái)
    void cursorMoved(const QPoint &imgPt); // vị trí con trỏ theo pixel ảnh (cho thanh trạng thái)

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    QRect currentRect() const; // QRect(start,cur) đã chuẩn hoá

    canvas::CanvasDocument m_doc;

    // Công cụ vẽ hiện hành cho annotation mới.
    canvas::ObjType m_tool = canvas::ObjType::Box;
    QColor m_color = QColor(220, 30, 30);
    int m_penWidth = 3;

    // Trạng thái kéo chuột.
    bool m_dragging = false;
    QPoint m_start;
    QPoint m_cur;

    int m_nextStep = 1;   // số thứ tự kế tiếp cho công cụ Step
};
