#include "ui/MainWindow.h"
#include "core/Version.h"
#include <QLabel>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Ezsnagit"));
    resize(900, 600);

    // P0: chỉ là khung. Một label giữa màn báo app đã chạy + version.
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    auto *label = new QLabel(
        QStringLiteral("Ezsnagit %1\nClone Snagit — khung P0").arg(core::appVersion()),
        central);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    setCentralWidget(central);

    statusBar()->showMessage(QStringLiteral("Sẵn sàng — khung P0"));
}
