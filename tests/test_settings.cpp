// Test headless cho core::Settings — round-trip qua QSettings.
// QUAN TRỌNG: dùng IniFormat + đường dẫn tạm để KHÔNG đụng registry/settings thật của user.
#include "core/Settings.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <cstdio>

static int g_pass = 0, g_fail = 0;
static void ok(bool cond, const char *name) {
    if (cond) ++g_pass; else { ++g_fail; std::printf("FAIL %s\n", name); }
}

using core::Settings;

int main(int argc, char **argv) {
    // Cô lập storage: ini ở thư mục tạm, không chạm registry thật.
    const QString tempRoot = QDir::tempPath() + "/ezsnag_test_settings";
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, tempRoot);

    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Ez");
    QCoreApplication::setApplicationName("Ezsnagit");

    // defaultSaveDir round-trip.
    {
        Settings::setDefaultSaveDir("C:/tmp/x");
        ok(Settings::defaultSaveDir() == "C:/tmp/x", "defaultSaveDir round-trip");
    }
    // autoCopy: false rồi true.
    {
        Settings::setAutoCopy(false);
        ok(Settings::autoCopy() == false, "autoCopy = false");
        Settings::setAutoCopy(true);
        ok(Settings::autoCopy() == true, "autoCopy = true");
    }
    // nextQuickSaveSeq tăng dần.
    {
        const int a = Settings::nextQuickSaveSeq();
        const int b = Settings::nextQuickSaveSeq();
        ok(b == a + 1, "nextQuickSaveSeq tang dan (b == a+1)");
    }
    // lastDir round-trip.
    {
        Settings::setLastDir("C:/tmp/last");
        ok(Settings::lastDir() == "C:/tmp/last", "lastDir round-trip");
    }

    std::printf("test_settings: %d passed, %d failed\n", g_pass, g_fail);

    // Dọn ini tạm (best-effort).
    QDir(tempRoot).removeRecursively();

    return g_fail == 0 ? 0 : 1;
}
