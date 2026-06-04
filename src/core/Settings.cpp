#include "core/Settings.h"
#include <QSettings>
#include <QStandardPaths>

namespace core {

// Khoá lưu trong QSettings.
static const char *kDefaultDir = "save/defaultDir";
static const char *kLastDir    = "save/lastDir";
static const char *kQuickSeq   = "save/quickSeq";
static const char *kAutoCopy   = "capture/autoCopy";

QString Settings::defaultSaveDir() {
    QSettings s("Ez", "Ezsnagit");
    // Chưa đặt -> mặc định là Desktop của user.
    const QString def = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    return s.value(kDefaultDir, def).toString();
}

void Settings::setDefaultSaveDir(const QString &dir) {
    QSettings s("Ez", "Ezsnagit");
    s.setValue(kDefaultDir, dir);
}

QString Settings::lastDir() {
    QSettings s("Ez", "Ezsnagit");
    return s.value(kLastDir).toString();
}

void Settings::setLastDir(const QString &dir) {
    QSettings s("Ez", "Ezsnagit");
    s.setValue(kLastDir, dir);
}

int Settings::nextQuickSaveSeq() {
    QSettings s("Ez", "Ezsnagit");
    // Đọc giá trị hiện tại (mặc định 1), lưu value+1, trả về value vừa đọc.
    const int cur = s.value(kQuickSeq, 1).toInt();
    s.setValue(kQuickSeq, cur + 1);
    return cur;
}

bool Settings::autoCopy() {
    QSettings s("Ez", "Ezsnagit");
    return s.value(kAutoCopy, true).toBool();
}

void Settings::setAutoCopy(bool on) {
    QSettings s("Ez", "Ezsnagit");
    s.setValue(kAutoCopy, on);
}

} // namespace core
