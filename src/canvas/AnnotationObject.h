#pragma once
#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPolygon>
#include <QRect>
#include <QString>
#include <QtMath>

// Một đối tượng annotation dạng vector (mũi tên, khung, highlight, text) — THUẦN dữ liệu
// + hàm vẽ. Toạ độ tính theo pixel ẢNH (cùng hệ với background của CanvasDocument).
namespace canvas {

// Box/Highlight/Text dùng rect chuẩn hoá. Arrow/Line dùng rect KHÔNG chuẩn hoá
// (topLeft=điểm đầu, bottomRight=điểm cuối) để giữ hướng. Step = badge số thứ tự
// (số lưu trong text). Blur KHÔNG phải object lưu trữ (được "nung" thẳng vào nền).
enum class ObjType { Arrow, Box, Highlight, Text, Line, Step, Blur };

struct AnnotationObject {
    ObjType type = ObjType::Box;
    QRect rect;                       // hình học (toạ độ ảnh)
    QColor color = QColor(220, 30, 30);
    int penWidth = 3;
    QString text;                     // cho ObjType::Text; với Step = số thứ tự
};

// Vẽ 1 object lên painter đã mở sẵn (đã set sang hệ toạ độ ảnh).
inline void paintObject(QPainter &p, const AnnotationObject &o) {
    p.save();
    switch (o.type) {
    case ObjType::Box: {
        // Khung viền, không tô nền.
        QPen pen(o.color);
        pen.setWidth(o.penWidth);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        p.drawRect(o.rect);
        break;
    }
    case ObjType::Highlight: {
        // Tô nền màu bán trong suốt (kiểu bút dạ quang).
        QColor fill = o.color;
        fill.setAlpha(80);
        p.setPen(Qt::NoPen);
        p.fillRect(o.rect, fill);
        break;
    }
    case ObjType::Arrow: {
        // Đường từ topLeft -> bottomRight, kèm đầu mũi tên ở điểm cuối.
        QPen pen(o.color);
        pen.setWidth(o.penWidth);
        pen.setJoinStyle(Qt::MiterJoin);
        pen.setCapStyle(Qt::RoundCap);
        p.setPen(pen);
        p.setBrush(o.color);

        const QPoint a = o.rect.topLeft();
        const QPoint b = o.rect.bottomRight();
        if (a == b) break;   // mũi tên dài 0 -> không vẽ (tránh đầu mũi tên rác)
        p.drawLine(a, b);

        // Đầu mũi tên: 2 cạnh xoè ra từ b, ngược hướng a->b.
        const double angle = std::atan2(double(b.y() - a.y()), double(b.x() - a.x()));
        const double headLen = 8.0 + o.penWidth * 2.0; // dài đầu mũi tên theo nét
        const double spread = M_PI / 7.0;               // độ xoè
        QPointF h1(b.x() - headLen * std::cos(angle - spread),
                   b.y() - headLen * std::sin(angle - spread));
        QPointF h2(b.x() - headLen * std::cos(angle + spread),
                   b.y() - headLen * std::sin(angle + spread));
        QPolygonF head;
        head << QPointF(b) << h1 << h2;
        p.drawPolygon(head);
        break;
    }
    case ObjType::Text: {
        // Vẽ chữ trong rect bằng màu object.
        p.setPen(o.color);
        p.drawText(o.rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, o.text);
        break;
    }
    case ObjType::Line: {
        // Đường thẳng từ topLeft -> bottomRight (không đầu mũi tên).
        QPen pen(o.color);
        pen.setWidth(o.penWidth);
        pen.setCapStyle(Qt::RoundCap);
        p.setPen(pen);
        p.drawLine(o.rect.topLeft(), o.rect.bottomRight());
        break;
    }
    case ObjType::Step: {
        // Badge tròn có số thứ tự (số nằm trong o.text). Vẽ tròn màu + số trắng.
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setPen(Qt::NoPen);
        p.setBrush(o.color);
        p.drawEllipse(o.rect);
        p.setPen(Qt::white);
        QFont f = p.font();
        f.setBold(true);
        f.setPixelSize(qMax(8, int(o.rect.height() * 0.55)));
        p.setFont(f);
        p.drawText(o.rect, Qt::AlignCenter, o.text);
        break;
    }
    case ObjType::Blur:
        break; // Blur được nung vào nền, không vẽ như object.
    }
    p.restore();
}

} // namespace canvas
