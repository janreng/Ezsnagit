// Test headless cho module share/ImageClipboard.
// Clipboard cần QGuiApplication -> dùng platform "offscreen" để chạy không màn hình.
#include "share/ImageClipboard.h"
#include <QGuiApplication>
#include <QImage>
#include <QDir>
#include <QFile>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using share::copyImageToClipboard;
using share::saveImagePng;
using share::suggestFileName;

int main(int argc, char **argv) {
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QGuiApplication app(argc, argv);

    // suggestFileName: thuần, ghép "<prefix>-<seq>.png".
    {
        ok(suggestFileName("cap", 3) == QStringLiteral("cap-3.png"), "suggestFileName cap-3.png");
    }

    // saveImagePng: lưu ảnh 4x4 ra temp, file tồn tại, load lại đúng kích thước.
    {
        QImage img(4, 4, QImage::Format_ARGB32);
        img.fill(Qt::red);
        QString path = QDir(QDir::tempPath()).filePath("ezsnag_test_share.png");
        QFile::remove(path); // dọn sạch lần chạy trước
        bool saved = saveImagePng(img, path);
        ok(saved, "saveImagePng tra true");
        ok(QFile::exists(path), "file PNG ton tai sau khi luu");
        QImage back(path);
        ok(back.size() == QSize(4, 4), "load lai dung kich thuoc 4x4");
        QFile::remove(path); // dọn dẹp
    }

    // saveImagePng: path rỗng -> false.
    {
        QImage img(4, 4, QImage::Format_ARGB32);
        img.fill(Qt::blue);
        ok(!saveImagePng(img, QString()), "saveImagePng path rong -> false");
    }

    // copyImageToClipboard: ảnh null -> false.
    {
        ok(!copyImageToClipboard(QImage()), "copyImageToClipboard anh null -> false");
    }

    // copyImageToClipboard: ảnh hợp lệ. Offscreen có thể không có clipboard thật ->
    // chỉ assert true nếu clipboard khả dụng, không thì bỏ qua (test không treo).
    {
        QImage img(4, 4, QImage::Format_ARGB32);
        img.fill(Qt::green);
        if (QGuiApplication::clipboard()) {
            ok(copyImageToClipboard(img), "copyImageToClipboard anh hop le -> true");
        } else {
            std::printf("SKIP copyImageToClipboard anh hop le (khong co clipboard offscreen)\n");
        }
    }

    std::printf("test_share: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
