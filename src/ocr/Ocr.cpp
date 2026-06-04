#include "ocr/Ocr.h"

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>
#include <QStringList>
#include <QTemporaryFile>

namespace ocr {

QString tesseractPath() {
    // 1) Ưu tiên tìm trong PATH.
    const QString inPath = QStandardPaths::findExecutable(QStringLiteral("tesseract"));
    if (!inPath.isEmpty())
        return inPath;

    // 2) Các đường dẫn cài đặt phổ biến trên Windows.
    QStringList candidates;
    candidates << QStringLiteral("C:/Program Files/Tesseract-OCR/tesseract.exe")
               << QStringLiteral("C:/Program Files (x86)/Tesseract-OCR/tesseract.exe");

    const QString localAppData = qEnvironmentVariable("LOCALAPPDATA");
    if (!localAppData.isEmpty())
        candidates << (localAppData + QStringLiteral("/Programs/Tesseract-OCR/tesseract.exe"));

    for (const QString &c : candidates) {
        if (QFile::exists(c))
            return c;
    }

    // Không tìm thấy.
    return QString();
}

bool isAvailable() {
    return !tesseractPath().isEmpty();
}

QString grabText(const QImage &img, const QString &lang) {
    // Không có Tesseract hoặc ảnh rỗng -> trả ngay "".
    const QString exe = tesseractPath();
    if (exe.isEmpty() || img.isNull())
        return QString();

    // Lưu ảnh ra file PNG tạm (tên duy nhất) trong thư mục temp hệ thống.
    QTemporaryFile tmp(QDir::tempPath() + QStringLiteral("/ezsnag_ocr_XXXXXX.png"));
    tmp.setAutoRemove(true); // tự xoá khi huỷ object
    if (!tmp.open())
        return QString();
    const QString tmpPng = tmp.fileName();
    tmp.close(); // đóng handle để tesseract đọc được file
    if (!img.save(tmpPng, "PNG"))
        return QString();

    // Chạy tesseract: <tmpPng> stdout -l <lang>  -> in text ra stdout.
    QProcess proc;
    proc.setProgram(exe);
    proc.setArguments(QStringList()
                      << tmpPng
                      << QStringLiteral("stdout")
                      << QStringLiteral("-l") << lang);
    proc.start();

    // Chờ tối đa 15s; timeout hoặc exit != 0 -> trả "".
    QString text;
    if (proc.waitForFinished(15000)
        && proc.exitStatus() == QProcess::NormalExit
        && proc.exitCode() == 0) {
        text = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
    } else {
        // Timeout hoặc lỗi: đảm bảo tiến trình bị kill, không để treo.
        proc.kill();
        proc.waitForFinished(2000);
        text = QString();
    }

    // tmp tự xoá khi destruct (autoRemove). Trả text.
    return text;
}

} // namespace ocr
