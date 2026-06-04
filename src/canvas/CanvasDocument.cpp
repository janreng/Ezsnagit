#include "CanvasDocument.h"
#include <QBuffer>
#include <QByteArray>
#include <QJsonArray>
#include <QPainter>

namespace canvas {

// ---------------------------------------------------------------------------
// Helpers JSON cho các kiểu phụ.
// ---------------------------------------------------------------------------
namespace {

QJsonObject rectToJson(const QRect &r) {
    return QJsonObject{ {"x", r.x()}, {"y", r.y()}, {"w", r.width()}, {"h", r.height()} };
}

QRect rectFromJson(const QJsonObject &o) {
    return QRect(o.value("x").toInt(), o.value("y").toInt(),
                 o.value("w").toInt(), o.value("h").toInt());
}

// Màu lưu dạng "#AARRGGBB" để giữ alpha.
QString colorToJson(const QColor &c) { return c.name(QColor::HexArgb); }

QColor colorFromJson(const QString &s) {
    QColor c(s);
    return c.isValid() ? c : QColor(220, 30, 30);
}

QString typeToStr(ObjType t) {
    switch (t) {
    case ObjType::Arrow: return "arrow";
    case ObjType::Box: return "box";
    case ObjType::Highlight: return "highlight";
    case ObjType::Text: return "text";
    case ObjType::Line: return "line";
    case ObjType::Step: return "step";
    case ObjType::Blur: return "blur";
    }
    return "box";
}

ObjType typeFromStr(const QString &s) {
    if (s == "arrow") return ObjType::Arrow;
    if (s == "highlight") return ObjType::Highlight;
    if (s == "text") return ObjType::Text;
    if (s == "line") return ObjType::Line;
    if (s == "step") return ObjType::Step;
    if (s == "blur") return ObjType::Blur;
    return ObjType::Box;
}

} // namespace

// ---------------------------------------------------------------------------
// Nền + objects.
// ---------------------------------------------------------------------------
void CanvasDocument::setBackground(const QImage &img) {
    m_background = img;
}

void CanvasDocument::addObject(const AnnotationObject &o) {
    m_objects.append(o);
    m_redo.clear(); // mọi thao tác mới làm mất nhánh redo
}

void CanvasDocument::clear() {
    m_objects.clear();
    m_redo.clear();
}

// ---------------------------------------------------------------------------
// Undo / Redo (mỗi op = 1 object thêm/bớt ở cuối; m_objects chính là undo stack).
// ---------------------------------------------------------------------------
bool CanvasDocument::undo() {
    if (m_objects.isEmpty())
        return false;
    m_redo.append(m_objects.takeLast());
    return true;
}

bool CanvasDocument::redo() {
    if (m_redo.isEmpty())
        return false;
    m_objects.append(m_redo.takeLast());
    return true;
}

// ---------------------------------------------------------------------------
// Render flatten.
// ---------------------------------------------------------------------------
QImage CanvasDocument::renderFlattened() const {
    if (m_background.isNull())
        return QImage();
    QImage out = m_background.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    for (const AnnotationObject &o : m_objects)
        paintObject(p, o);
    p.end();
    return out;
}

// ---------------------------------------------------------------------------
// Serialize.
// ---------------------------------------------------------------------------
QJsonObject CanvasDocument::toJson() const {
    QJsonObject root;

    // Background -> PNG base64 (rỗng nếu không có ảnh).
    if (!m_background.isNull()) {
        QByteArray bytes;
        QBuffer buf(&bytes);
        buf.open(QIODevice::WriteOnly);
        m_background.save(&buf, "PNG");
        buf.close();
        root["background"] = QString::fromLatin1(bytes.toBase64());
    } else {
        root["background"] = QString();
    }

    QJsonArray arr;
    for (const AnnotationObject &o : m_objects) {
        QJsonObject jo;
        jo["type"] = typeToStr(o.type);
        jo["rect"] = rectToJson(o.rect);
        jo["color"] = colorToJson(o.color);
        jo["penWidth"] = o.penWidth;
        jo["text"] = o.text;
        arr.append(jo);
    }
    root["objects"] = arr;
    return root;
}

CanvasDocument CanvasDocument::fromJson(const QJsonObject &obj) {
    CanvasDocument doc;

    const QString bg = obj.value("background").toString();
    if (!bg.isEmpty()) {
        QByteArray bytes = QByteArray::fromBase64(bg.toLatin1());
        QImage img;
        img.loadFromData(bytes, "PNG");
        doc.m_background = img;
    }

    const QJsonArray arr = obj.value("objects").toArray();
    for (const QJsonValue &v : arr) {
        const QJsonObject jo = v.toObject();
        AnnotationObject o;
        o.type = typeFromStr(jo.value("type").toString());
        o.rect = rectFromJson(jo.value("rect").toObject());
        o.color = colorFromJson(jo.value("color").toString());
        o.penWidth = jo.value("penWidth").toInt(3);
        o.text = jo.value("text").toString();
        // Nạp trực tiếp (không qua undo stack — đây là load tài liệu).
        doc.m_objects.append(o);
    }
    return doc;
}

} // namespace canvas
