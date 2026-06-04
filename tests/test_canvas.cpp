// Test headless cho editor document model (CanvasDocument + AnnotationObject).
// Cần QGuiApplication offscreen vì renderFlattened()/paintObject dùng QImage/QPainter.
#include "canvas/CanvasDocument.h"
#include <QGuiApplication>
#include <QImage>
#include <QJsonObject>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using canvas::AnnotationObject;
using canvas::CanvasDocument;
using canvas::ObjType;

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    // --- add / undo / redo ---
    {
        CanvasDocument doc;
        AnnotationObject a;
        a.type = ObjType::Box;
        a.rect = QRect(10, 10, 40, 30);
        AnnotationObject b;
        b.type = ObjType::Arrow;
        b.rect = QRect(0, 0, 50, 50);

        doc.addObject(a);
        doc.addObject(b);
        ok(doc.objects().size() == 2, "addObject x2 -> size 2");

        ok(doc.undo(), "undo tra true");
        ok(doc.objects().size() == 1, "sau undo -> size 1");
        ok(doc.canRedo(), "sau undo -> canRedo true");

        ok(doc.redo(), "redo tra true");
        ok(doc.objects().size() == 2, "sau redo -> size 2");
        ok(!doc.canRedo(), "het redo -> canRedo false");
    }

    // --- renderFlattened trên background 100x80 ---
    {
        CanvasDocument doc;
        QImage bg(100, 80, QImage::Format_ARGB32);
        bg.fill(Qt::white);
        doc.setBackground(bg);

        AnnotationObject box;
        box.type = ObjType::Box;
        box.rect = QRect(5, 5, 30, 20);
        doc.addObject(box);

        QImage out = doc.renderFlattened();
        ok(!out.isNull(), "renderFlattened khong null");
        ok(out.width() == 100 && out.height() == 80, "renderFlattened 100x80");
    }

    // --- toJson -> fromJson round-trip ---
    {
        CanvasDocument doc;
        QImage bg(20, 15, QImage::Format_ARGB32);
        bg.fill(Qt::black);
        doc.setBackground(bg);

        AnnotationObject a;
        a.type = ObjType::Highlight;
        a.rect = QRect(3, 4, 12, 9);
        a.color = QColor(10, 200, 50);
        a.penWidth = 7;
        a.text = "xin chao";
        doc.addObject(a);

        AnnotationObject b;
        b.type = ObjType::Text;
        b.rect = QRect(1, 2, 30, 8);
        b.text = "label";
        doc.addObject(b);

        QJsonObject json = doc.toJson();
        CanvasDocument loaded = CanvasDocument::fromJson(json);

        ok(loaded.objects().size() == doc.objects().size(), "json: size objects bang nhau");
        ok(loaded.objects()[0].rect == a.rect, "json: object[0].rect giu nguyen");
        ok(loaded.objects()[0].color == a.color, "json: object[0].color giu nguyen");
        ok(loaded.objects()[0].type == a.type, "json: object[0].type giu nguyen");
        ok(loaded.objects()[0].penWidth == a.penWidth, "json: object[0].penWidth giu nguyen");
        ok(loaded.objects()[0].text == a.text, "json: object[0].text giu nguyen");
        ok(!loaded.background().isNull(), "json: background round-trip khong null");
    }

    // --- clear ---
    {
        CanvasDocument doc;
        AnnotationObject a;
        doc.addObject(a);
        doc.addObject(a);
        doc.clear();
        ok(doc.objects().isEmpty(), "clear -> objects rong");
        ok(!doc.canUndo(), "clear -> canUndo false");
        ok(!doc.canRedo(), "clear -> canRedo false");
    }

    std::printf("test_canvas: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
