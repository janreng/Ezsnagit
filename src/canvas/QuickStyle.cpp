#include "QuickStyle.h"

namespace canvas {

namespace {

// Map ObjType <-> chuỗi để serialize JSON cho ổn định (không phụ thuộc số enum).
QString typeToString(ObjType t) {
    switch (t) {
    case ObjType::Arrow:     return QStringLiteral("arrow");
    case ObjType::Box:       return QStringLiteral("box");
    case ObjType::Highlight: return QStringLiteral("highlight");
    case ObjType::Text:      return QStringLiteral("text");
    }
    return QStringLiteral("box");
}

ObjType typeFromString(const QString &s) {
    const QString k = s.trimmed().toLower();
    if (k == QLatin1String("arrow"))     return ObjType::Arrow;
    if (k == QLatin1String("highlight")) return ObjType::Highlight;
    if (k == QLatin1String("text"))      return ObjType::Text;
    return ObjType::Box; // mặc định / không nhận dạng được
}

} // namespace

AnnotationObject applyStyle(const QuickStyle &s, const AnnotationObject &base) {
    // Giữ nguyên hình học (rect) và text của object, chỉ đổi thuộc tính từ style.
    AnnotationObject out = base;
    out.type = s.type;
    out.color = s.color;
    out.penWidth = s.penWidth;
    return out;
}

QVector<QuickStyle> defaultQuickStyles() {
    QVector<QuickStyle> v;
    v.push_back(QuickStyle{ QStringLiteral("Mui ten do"),  ObjType::Arrow,     QColor(220, 30, 30),  3 });
    v.push_back(QuickStyle{ QStringLiteral("O do"),        ObjType::Box,       QColor(220, 30, 30),  3 });
    v.push_back(QuickStyle{ QStringLiteral("Highlight vang"), ObjType::Highlight, QColor(255, 235, 60), 1 });
    v.push_back(QuickStyle{ QStringLiteral("Text den"),    ObjType::Text,      QColor(0, 0, 0),      2 });
    return v;
}

QJsonArray stylesToJson(const QVector<QuickStyle> &styles) {
    QJsonArray arr;
    for (const QuickStyle &s : styles) {
        QJsonObject o;
        o.insert(QStringLiteral("name"), s.name);
        o.insert(QStringLiteral("type"), typeToString(s.type));
        // Màu lưu dạng "#AARRGGBB" để giữ cả alpha.
        o.insert(QStringLiteral("color"), s.color.name(QColor::HexArgb));
        o.insert(QStringLiteral("penWidth"), s.penWidth);
        arr.append(o);
    }
    return arr;
}

QVector<QuickStyle> stylesFromJson(const QJsonArray &arr) {
    QVector<QuickStyle> v;
    v.reserve(arr.size());
    for (const QJsonValue &val : arr) {
        if (!val.isObject()) continue;
        const QJsonObject o = val.toObject();
        QuickStyle s;
        s.name = o.value(QStringLiteral("name")).toString();
        s.type = typeFromString(o.value(QStringLiteral("type")).toString());
        s.color = QColor(o.value(QStringLiteral("color")).toString());
        s.penWidth = o.value(QStringLiteral("penWidth")).toInt(3);
        v.push_back(s);
    }
    return v;
}

} // namespace canvas
