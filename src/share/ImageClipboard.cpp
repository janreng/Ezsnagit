#include "share/ImageClipboard.h"
#include <QGuiApplication>
#include <QClipboard>

namespace share {

bool copyImageToClipboard(const QImage &img) {
    // Ảnh null -> không có gì để copy.
    if (img.isNull())
        return false;
    // Cần một QGuiApplication mới có clipboard.
    QClipboard *cb = QGuiApplication::clipboard();
    if (!cb)
        return false;
    cb->setImage(img);
    return true;
}

bool saveImagePng(const QImage &img, const QString &path) {
    // Ảnh null hoặc path rỗng -> không lưu được.
    if (img.isNull() || path.isEmpty())
        return false;
    return img.save(path, "PNG");
}

QString suggestFileName(const QString &prefix, int seq) {
    // Tên file thuần dạng "<prefix>-<seq>.png".
    return prefix + QStringLiteral("-") + QString::number(seq) + QStringLiteral(".png");
}

} // namespace share
