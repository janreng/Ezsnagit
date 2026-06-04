#include "ui/PreferencesDialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "core/Settings.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(QStringLiteral("Thiết lập"));

    // Ô tích "tự copy" — khởi tạo từ Settings.
    m_autoCopy = new QCheckBox(
        QStringLiteral("Tự copy ảnh vào clipboard sau khi chụp"), this);
    m_autoCopy->setChecked(core::Settings::autoCopy());

    // Hàng chọn thư mục lưu: nhãn + ô chỉ đọc + nút "Chọn…".
    auto *dirLabel =
        new QLabel(QStringLiteral("Thư mục lưu mặc định:"), this);
    m_saveDir = new QLineEdit(core::Settings::defaultSaveDir(), this);
    m_saveDir->setReadOnly(true);
    auto *chooseBtn = new QPushButton(QStringLiteral("Chọn…"), this);
    connect(chooseBtn, &QPushButton::clicked, this,
            &PreferencesDialog::chooseSaveDir);

    auto *dirRow = new QHBoxLayout();
    dirRow->addWidget(dirLabel);
    dirRow->addWidget(m_saveDir, /*stretch=*/1);
    dirRow->addWidget(chooseBtn);

    // Nút chuẩn OK | Cancel.
    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this,
            &PreferencesDialog::onAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Bố cục tổng.
    auto *root = new QVBoxLayout(this);
    root->addWidget(m_autoCopy);
    root->addLayout(dirRow);
    root->addWidget(buttons);
}

void PreferencesDialog::chooseSaveDir() {
    // Mở từ thư mục hiện tại trong ô (nếu có).
    const QString dir = QFileDialog::getExistingDirectory(
        this, QStringLiteral("Chọn thư mục lưu mặc định"),
        m_saveDir->text());
    if (!dir.isEmpty())
        m_saveDir->setText(dir);
}

void PreferencesDialog::onAccept() {
    // Ghi tuỳ chọn về Settings rồi đóng dialog.
    core::Settings::setAutoCopy(m_autoCopy->isChecked());
    core::Settings::setDefaultSaveDir(m_saveDir->text());
    accept();
}
