#include "core/Version.h"
#include <QStringList>

namespace core {

Version Version::parse(const QString &s) {
    QString t = s.trimmed();
    if (t.startsWith('v') || t.startsWith('V'))
        t = t.mid(1);
    const QStringList parts = t.split('.');
    auto at = [&](int i) -> int {
        if (i >= parts.size()) return 0;
        bool ok = false;
        const int v = parts.at(i).trimmed().toInt(&ok);
        return ok ? v : 0;
    };
    return Version{ at(0), at(1), at(2) };
}

QString Version::toString() const {
    return QString("%1.%2.%3").arg(major).arg(minor).arg(patch);
}

int compare(const Version &a, const Version &b) {
    if (a.major != b.major) return a.major < b.major ? -1 : 1;
    if (a.minor != b.minor) return a.minor < b.minor ? -1 : 1;
    if (a.patch != b.patch) return a.patch < b.patch ? -1 : 1;
    return 0;
}

QString appVersion() {
#ifdef EZSNAGIT_VERSION
    return QString(EZSNAGIT_VERSION);
#else
    return QStringLiteral("0.0.0");
#endif
}

} // namespace core
