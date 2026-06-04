#include "CanvasWidget.h"
#include "effects/Blur.h"
#include "effects/Transforms.h"

#include <QPainter>
#include <QMouseEvent>
#include <QInputDialog>
#include <QLineEdit>
#include <QString>

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent)
{
    // Bật mouse-tracking để báo vị trí con trỏ ngay cả khi không giữ chuột.
    setMouseTracking(true);
}

void CanvasWidget::setImage(const QImage &img)
{
    m_doc.setBackground(img);
    m_doc.clear();   // ảnh nền mới -> xoá annotation cũ (tránh vẽ đè / nhân đôi sau xoay/lật)
    m_nextStep = 1;  // đánh số Step lại từ đầu cho ảnh mới
    // Widget khớp 1:1 với pixel ảnh -> toạ độ chuột chính là toạ độ ảnh.
    setFixedSize(img.size());
    resize(img.size());
    m_dragging = false;
    update();
}

void CanvasWidget::loadDocument(const canvas::CanvasDocument &doc)
{
    m_doc = doc;
    const QSize sz = m_doc.background().size();
    setFixedSize(sz);
    resize(sz);
    m_dragging = false;
    update();
}

bool CanvasWidget::undo()
{
    const bool ok = m_doc.undo();
    update();
    return ok;
}

bool CanvasWidget::redo()
{
    const bool ok = m_doc.redo();
    update();
    return ok;
}

QRect CanvasWidget::currentRect() const
{
    // Mũi tên & đường thẳng cần GIỮ hướng (start->cur) nên không normalize.
    // Các hình khác (Box/Highlight/Text) cần rect chuẩn hoá để vẽ đúng.
    if (m_tool == canvas::ObjType::Arrow || m_tool == canvas::ObjType::Line)
        return QRect(m_start, m_cur);
    return QRect(m_start, m_cur).normalized();
}

void CanvasWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // 1) Vẽ ảnh nền tại gốc (0,0).
    const QImage &bg = m_doc.background();
    if (!bg.isNull())
        p.drawImage(0, 0, bg);

    // 2) Vẽ tất cả annotation theo z-order (dưới -> trên).
    for (const canvas::AnnotationObject &o : m_doc.objects())
        canvas::paintObject(p, o);

    // 3) Preview live: object đang kéo (chưa commit) để người dùng thấy hình dạng.
    if (m_dragging) {
        canvas::AnnotationObject preview;
        preview.type = m_tool;
        preview.rect = currentRect();
        preview.color = m_color;
        preview.penWidth = m_penWidth;
        // Với tool Text chưa có chữ -> hiển thị placeholder để thấy vùng đặt chữ.
        if (m_tool == canvas::ObjType::Text)
            preview.text = QStringLiteral("Text...");
        canvas::paintObject(p, preview);
    }
}

void CanvasWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton || !hasImage()) {
        QWidget::mousePressEvent(e);
        return;
    }
    m_dragging = true;
    m_start = e->pos();
    m_cur = e->pos();
    update();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *e)
{
    emit cursorMoved(e->pos());   // toạ độ widget = toạ độ ảnh (1:1)
    if (!m_dragging) {
        QWidget::mouseMoveEvent(e);
        return;
    }
    m_cur = e->pos();
    update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton || !m_dragging) {
        QWidget::mouseReleaseEvent(e);
        return;
    }
    m_dragging = false;
    m_cur = e->pos();

    const QRect rect = currentRect();
    repaint(); // xoá preview NGAY (đồng bộ, trước khi dialog modal có thể mở)

    using canvas::ObjType;

    // Step: đặt badge số thứ tự tại điểm click (không cần kéo).
    if (m_tool == ObjType::Step) {
        const int sz = 30;
        const QRect badge(m_start.x() - sz / 2, m_start.y() - sz / 2, sz, sz);
        canvas::AnnotationObject obj{ ObjType::Step, badge, m_color, m_penWidth,
                                      QString::number(m_nextStep++) };
        m_doc.addObject(obj);
        update();
        emit documentChanged();
        return;
    }

    // Quá nhỏ -> coi như click nhầm, bỏ qua.
    const int dx = qAbs(m_cur.x() - m_start.x());
    const int dy = qAbs(m_cur.y() - m_start.y());
    if (m_tool == ObjType::Arrow || m_tool == ObjType::Line) {
        if (dx < 3 && dy < 3) return;            // đường: cho phép ngang/dọc, chỉ chặn click
    } else {
        if (dx < 3 || dy < 3) return;            // hình khối: cần đủ cả rộng lẫn cao
    }

    // Blur: nung trực tiếp vào nền (redaction — không lưu thành object đảo ngược được).
    if (m_tool == ObjType::Blur) {
        m_doc.setBackground(effects::blurRegion(m_doc.background(), rect.normalized()));
        update();
        return;
    }

    // Crop: cắt ảnh (đã gộp annotation) theo vùng chọn -> thành ảnh mới.
    if (m_tool == ObjType::Crop) {
        const QImage cropped = effects::crop(m_doc.renderFlattened(), rect.normalized());
        if (!cropped.isNull()) setImage(cropped);  // setImage tự clear annotation cũ
        return;
    }

    QString text;
    if (m_tool == ObjType::Text) {
        // Hỏi chuỗi cần chèn; rỗng/huỷ -> không thêm object.
        bool ok = false;
        text = QInputDialog::getText(this, tr("Chèn chữ"), tr("Nội dung:"),
                                     QLineEdit::Normal, QString(), &ok);
        if (!ok || text.isEmpty())
            return;
    }

    canvas::AnnotationObject obj{ m_tool, rect, m_color, m_penWidth, text };
    m_doc.addObject(obj);
    update();
    emit documentChanged();
}
