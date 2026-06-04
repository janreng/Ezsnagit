// Test headless cho QuickStyle — preset annotation style + round-trip JSON.
// QColor/QJson không cần app, nhưng set offscreen + QGuiApplication cho chắc.
#include "canvas/QuickStyle.h"
#include <QGuiApplication>
#include <QJsonArray>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using canvas::ObjType;
using canvas::AnnotationObject;
using canvas::QuickStyle;
using canvas::applyStyle;
using canvas::defaultQuickStyles;
using canvas::stylesToJson;
using canvas::stylesFromJson;

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    // defaultQuickStyles có ít nhất 4 preset.
    {
        ok(defaultQuickStyles().size() >= 4, "default >= 4 style");
    }

    // applyStyle: base Box xanh, áp "Arrow do" -> type/color đổi, rect giữ nguyên.
    {
        AnnotationObject base;
        base.type = ObjType::Box;
        base.color = QColor(0, 0, 255);          // xanh
        base.penWidth = 5;
        base.rect = QRect(10, 20, 100, 50);
        base.text = QStringLiteral("giu nguyen");

        QuickStyle s{ QStringLiteral("Arrow red"), ObjType::Arrow, QColor(255, 0, 0), 3 };
        AnnotationObject out = applyStyle(s, base);

        ok(out.type == ObjType::Arrow,            "applyStyle: type -> Arrow");
        ok(out.color == QColor(255, 0, 0),        "applyStyle: color -> red");
        ok(out.penWidth == 3,                     "applyStyle: penWidth -> 3");
        ok(out.rect == base.rect,                 "applyStyle: rect giu nguyen");
        ok(out.text == base.text,                 "applyStyle: text giu nguyen");
    }

    // Round-trip JSON: size bằng nhau, style[0] giữ nguyên name/type/color/penWidth.
    {
        QVector<QuickStyle> in = defaultQuickStyles();
        QJsonArray arr = stylesToJson(in);
        QVector<QuickStyle> out = stylesFromJson(arr);

        ok(out.size() == in.size(),               "json round-trip: size bang nhau");
        ok(out[0].name == in[0].name,             "json round-trip: name[0] giu nguyen");
        ok(out[0].type == in[0].type,             "json round-trip: type[0] giu nguyen");
        ok(out[0].color == in[0].color,           "json round-trip: color[0] giu nguyen");
        ok(out[0].penWidth == in[0].penWidth,     "json round-trip: penWidth[0] giu nguyen");
    }

    // Round-trip giữ alpha (highlight có alpha mặc định 255, test 1 màu có alpha).
    {
        QVector<QuickStyle> in{ QuickStyle{ QStringLiteral("Alpha"), ObjType::Highlight, QColor(255, 235, 60, 128), 1 } };
        QVector<QuickStyle> out = stylesFromJson(stylesToJson(in));
        ok(out.size() == 1 && out[0].color == QColor(255, 235, 60, 128), "json round-trip: giu alpha");
    }

    std::printf("test_quickstyle: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
