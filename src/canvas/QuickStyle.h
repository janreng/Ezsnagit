#pragma once
#include "AnnotationObject.h"
#include <QColor>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QVector>

// Quick Styles — preset annotation cho markup nhanh: chọn 1 style là set sẵn
// type/color/penWidth, chỉ cần kéo hình. THUẦN dữ liệu, không phụ thuộc GUI.
namespace canvas {

struct QuickStyle {
    QString name;            // tên hiển thị, vd "Mui ten do"
    ObjType type = ObjType::Box;
    QColor color = QColor(220, 30, 30);
    int penWidth = 3;
};

// Áp style vào 1 object: giữ nguyên rect/text của base, chỉ đổi type/color/penWidth.
AnnotationObject applyStyle(const QuickStyle &s, const AnnotationObject &base);

// Bộ preset mặc định hay dùng (>=4 style).
QVector<QuickStyle> defaultQuickStyles();

// Serialize list style ra/khôi phục từ JSON (lưu preset người dùng).
QJsonArray stylesToJson(const QVector<QuickStyle> &styles);
QVector<QuickStyle> stylesFromJson(const QJsonArray &arr);

} // namespace canvas
