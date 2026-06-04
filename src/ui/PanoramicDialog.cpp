#include "ui/PanoramicDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

PanoramicDialog::PanoramicDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("Chụp ghép dọc"));
    setModal(false);

    auto *lay = new QVBoxLayout(this);
    m_info = new QLabel(this);
    lay->addWidget(m_info);

    auto *row = new QHBoxLayout;
    auto *addBtn = new QPushButton(QStringLiteral("Chụp thêm khung"), this);
    m_done = new QPushButton(QStringLiteral("Ghép xong"), this);
    auto *cancelBtn = new QPushButton(QStringLiteral("Hủy"), this);
    row->addWidget(addBtn);
    row->addWidget(m_done);
    row->addWidget(cancelBtn);
    lay->addLayout(row);

    connect(addBtn, &QPushButton::clicked, this, &PanoramicDialog::captureRequested);
    connect(m_done, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    updateInfo();
}

void PanoramicDialog::addFrame(const QImage &img)
{
    if (!img.isNull()) m_frames.append(img);
    updateInfo();
}

void PanoramicDialog::updateInfo()
{
    m_info->setText(QStringLiteral("Đã chụp %1 khung. Cuộn nội dung rồi \"Chụp thêm khung\";\n"
                                   "xong thì bấm \"Ghép xong\" để nối lại thành ảnh dài.")
                        .arg(m_frames.size()));
    if (m_done) m_done->setEnabled(m_frames.size() >= 2);   // cần ít nhất 2 khung để ghép
}
