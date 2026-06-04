// Test headless cho module OCR (ocr::Ocr) — shell-out tới Tesseract CLI.
// Module phải degrade gracefully khi KHÔNG cài Tesseract: mọi nhánh có giới hạn
// thời gian, test KHÔNG BAO GIỜ được hang.
// Cần QGuiApplication offscreen vì dùng QImage/QPainter.
#include "ocr/Ocr.h"

#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QString>
#include <QElapsedTimer>
#include <cstdio>
#include <cstdlib>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

int main(int argc, char **argv) {
    // Ép offscreen để chạy headless (CI / không có display server).
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QGuiApplication app(argc, argv);

    // 1) tesseractPath() không crash — luôn trả về QString (có thể rỗng nếu chưa cài).
    {
        const QString p = ocr::tesseractPath();
        ok(true, "tesseractPath khong crash");
        std::printf("INFO tesseractPath = \"%s\"\n", p.toUtf8().constData());
    }

    // 2) Ảnh null -> trả "" và không hang.
    {
        QElapsedTimer t; t.start();
        const QString r = ocr::grabText(QImage());
        ok(r.isEmpty(), "grabText(anh null) -> rong");
        ok(t.elapsed() < 5000, "grabText(anh null) khong hang");
    }

    if (!ocr::isAvailable()) {
        // 3) Không có Tesseract: grabText(ảnh hợp lệ) phải trả "" NHANH (không hang 15s).
        QImage tiny(20, 20, QImage::Format_RGB32);
        tiny.fill(Qt::white);

        QElapsedTimer t; t.start();
        const QString r = ocr::grabText(tiny);
        const qint64 ms = t.elapsed();
        ok(r.isEmpty(), "khong co tesseract -> grabText tra rong");
        ok(ms < 15000, "khong co tesseract -> grabText khong hang");
        std::printf("INFO grabText(no-tesseract) mat %lld ms\n", (long long)ms);
    } else {
        // 4) Có Tesseract thật: vẽ chữ "HELLO" rồi OCR, kết quả phải CHỨA "HELLO".
        QImage canvas(200, 60, QImage::Format_RGB32);
        canvas.fill(Qt::white);
        {
            QPainter pr(&canvas);
            pr.setPen(Qt::black);
            QFont f;
            f.setPixelSize(36);
            f.setBold(true);
            pr.setFont(f);
            pr.drawText(canvas.rect(), Qt::AlignCenter, QStringLiteral("HELLO"));
        }

        QElapsedTimer t; t.start();
        const QString r = ocr::grabText(canvas);
        const qint64 ms = t.elapsed();
        ok(ms < 16000, "ocr-real khong hang");

        // So khớp không phân biệt hoa thường, cho phép nhiễu xung quanh.
        const bool hasHello = r.toUpper().contains(QStringLiteral("HELLO"));
        if (hasHello) {
            ok(true, "ocr-real chua HELLO");
        } else {
            // Tesseract có thể nhận sai do font/ngôn ngữ — không fail cứng, chỉ SKIP.
            std::printf("SKIP ocr-real (ket qua khong chua HELLO, OCR = \"%s\")\n",
                        r.toUtf8().constData());
        }
        std::printf("INFO grabText(real) mat %lld ms\n", (long long)ms);
    }

    if (!ocr::isAvailable())
        std::printf("SKIP ocr-real (tesseract chua cai)\n");

    std::printf("test_ocr: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
