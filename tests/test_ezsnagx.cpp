// Test headless cho module I/O .ezsnagx (save/load native + export PNG).
// CanvasDocument dùng QImage/QPainter -> cần QGuiApplication với platform offscreen.
#include "canvas/CanvasDocument.h"
#include "canvas/AnnotationObject.h"
#include "canvas/EzsnagxFile.h"
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QSize>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

int main(int argc, char **argv) {
    // Bắt buộc trước khi tạo QGuiApplication: không cần màn hình thật.
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    const QString savePath = QDir::tempPath() + "/ez_test.ezsnagx";
    const QString pngPath  = QDir::tempPath() + "/ez_test.png";

    // --- Dựng document mẫu: nền trắng 60x40 + 1 Box + 1 Arrow ---
    canvas::CanvasDocument doc;
    {
        QImage bg(60, 40, QImage::Format_ARGB32);
        bg.fill(Qt::white);
        doc.setBackground(bg);

        canvas::AnnotationObject box;
        box.type = canvas::ObjType::Box;
        box.rect = QRect(5, 5, 20, 15);
        box.color = QColor(220, 30, 30);
        box.penWidth = 3;
        doc.addObject(box);

        canvas::AnnotationObject arrow;
        arrow.type = canvas::ObjType::Arrow;
        arrow.rect = QRect(10, 10, 30, 20);
        arrow.color = QColor(30, 90, 220);
        arrow.penWidth = 2;
        doc.addObject(arrow);
    }

    // --- saveEzsnagx -> true + file tồn tại ---
    ok(canvas::saveEzsnagx(doc, savePath), "save ezsnagx -> true");
    ok(QFile::exists(savePath), "file ezsnagx ton tai sau khi save");

    // --- loadEzsnagx vào doc mới -> true, đúng objects + background ---
    {
        canvas::CanvasDocument loaded;
        ok(canvas::loadEzsnagx(loaded, savePath), "load ezsnagx -> true");
        ok(loaded.objects().size() == 2, "loaded co dung 2 objects");
        ok(loaded.background().size() == QSize(60, 40), "loaded background 60x40");
    }

    // --- exportPng -> true, file tồn tại, load lại đúng 60x40 ---
    {
        ok(canvas::exportPng(doc, pngPath), "export png -> true");
        ok(QFile::exists(pngPath), "file png ton tai sau khi export");
        QImage back(pngPath);
        ok(!back.isNull() && back.size() == QSize(60, 40), "png load lai dung 60x40");
    }

    // --- saveEzsnagx path rỗng -> false ---
    ok(!canvas::saveEzsnagx(doc, QString()), "save path rong -> false");

    // --- defaultExtension ---
    ok(canvas::defaultExtension() == ".ezsnagx", "defaultExtension == .ezsnagx");

    // --- Dọn file tạm ---
    QFile::remove(savePath);
    QFile::remove(pngPath);

    std::printf("test_ezsnagx: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
