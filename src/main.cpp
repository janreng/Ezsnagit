#include "ui/MainWindow.h"
#include <QApplication>
#include <QIcon>
#ifdef _WIN32
#include <windows.h>
#endif

// Điểm vào Ezsnagit. NFR (SPEC 12 §12.2): per-monitor DPI awareness ngay từ đầu để
// capture/overlay đúng hình học trên đa màn hình khác scale.
//
// Qt6 mặc định bật PerMonitorV2 DPI awareness trên Windows (qua manifest nội bộ của Qt),
// và dùng PassThrough high-DPI scaling. Ta đặt rõ chính sách làm tròn để nhất quán.
int main(int argc, char **argv)
{
#ifdef _WIN32
    // KHẲNG ĐỊNH per-monitor DPI v2 NGAY (trước QApplication) — không dựa vào suy đoán
    // manifest của Qt. Đây là điều kiện để capture đúng pixel vật lý trên đa màn hình
    // khác scale (NFR SPEC 12 §12.2). Cần Windows 10 1703+.
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif

    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("Ezsnagit"));
    app.setOrganizationName(QStringLiteral("Ez"));
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icon.png"))); // icon app (tạm mượn từ Ezcel, nhúng qua .qrc)

    MainWindow w;
    w.show();
    return app.exec();
}
