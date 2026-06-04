#include "EzsnagxFile.h"
#include <QFile>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

namespace canvas {

bool saveEzsnagx(const CanvasDocument &doc, const QString &path) {
    if (path.isEmpty())
        return false;

    // Serialize doc -> JSON (định dạng đẹp, dễ đọc/diff).
    const QByteArray data = QJsonDocument(doc.toJson()).toJson();

    // Ghi NGUYÊN TỬ: QSaveFile chỉ thay file gốc khi commit() thành công.
    QSaveFile f(path);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    if (f.write(data) != data.size())
        return false;          // dở dang -> không commit, file gốc giữ nguyên
    return f.commit();
}

bool loadEzsnagx(CanvasDocument &out, const QString &path) {
    if (path.isEmpty())
        return false;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    const QByteArray data = f.readAll();
    f.close();

    QJsonParseError err{};
    const QJsonDocument jdoc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !jdoc.isObject())
        return false;

    out = CanvasDocument::fromJson(jdoc.object());
    return true;
}

bool exportPng(const CanvasDocument &doc, const QString &path) {
    const QImage img = doc.renderFlattened();
    if (img.isNull())
        return false;
    return img.save(path, "PNG");
}

QString defaultExtension() {
    return QStringLiteral(".ezsnagx");
}

} // namespace canvas
