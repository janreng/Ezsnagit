#include "ui/MainWindow.h"
#include <QApplication>

// Điểm vào Ezsnagit. NFR (SPEC 12 §12.2): per-monitor DPI awareness ngay từ đầu để
// capture/overlay đúng hình học trên đa màn hình khác scale.
//
// Qt6 mặc định bật PerMonitorV2 DPI awareness trên Windows (qua manifest nội bộ của Qt),
// và dùng PassThrough high-DPI scaling. Ta đặt rõ chính sách làm tròn để nhất quán.
int main(int argc, char **argv)
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("Ezsnagit"));
    app.setOrganizationName(QStringLiteral("Ez"));

    MainWindow w;
    w.show();
    return app.exec();
}
