#include "ui/RecentTray.h"

#include <QLabel>

RecentTray::RecentTray(QWidget *parent) : QWidget(parent) {
    // Khay cao cố định ~72px, đủ chỗ cho thumbnail ~56px + margin.
    setFixedHeight(72);

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(6, 6, 6, 6);   // margin chặt
    m_layout->setSpacing(6);
    m_layout->setAlignment(Qt::AlignLeft);       // ảnh mới dồn về trái

    // Nhãn gợi ý mờ khi chưa có ảnh nào (optional).
    m_hint = new QLabel(tr("Ảnh chụp gần đây sẽ hiện ở đây"), this);
    m_hint->setStyleSheet("color: rgba(0,0,0,96);");
    m_layout->addWidget(m_hint);
}

void RecentTray::addCapture(const QImage &img) {
    if (img.isNull())
        return;

    // Tạo thumbnail thu nhỏ theo chiều cao, giữ tỉ lệ.
    QImage thumb = img.scaledToHeight(kThumbH, Qt::SmoothTransformation);

    auto *btn = new QToolButton(this);
    btn->setIcon(QIcon(QPixmap::fromImage(thumb)));
    btn->setIconSize(thumb.size());
    btn->setAutoRaise(true);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setToolTip(tr("%1 x %2").arg(img.width()).arg(img.height()));

    // Lưu ảnh full-res; bấm thumbnail thì phát ảnh đó ra ngoài.
    QImage full = img;
    connect(btn, &QToolButton::clicked, this, [this, full]() {
        emit captureChosen(full);
    });

    // Prepend: ảnh mới nhất nằm bên trái (sau nhãn gợi ý ở index 0).
    m_layout->insertWidget(1, btn);
    m_images.prepend(full);
    m_buttons.prepend(btn);

    // Vượt giới hạn thì bỏ ảnh cũ nhất (cuối danh sách).
    while (m_buttons.size() > kMaxItems) {
        QToolButton *old = m_buttons.takeLast();
        m_images.removeLast();
        m_layout->removeWidget(old);
        old->deleteLater();   // xoá nút, tránh leak
    }

    updateHint();
}

int RecentTray::count() const {
    return m_buttons.size();
}

void RecentTray::updateHint() {
    // Chỉ hiện gợi ý khi khay rỗng.
    if (m_hint)
        m_hint->setVisible(m_buttons.isEmpty());
}
