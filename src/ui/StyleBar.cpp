#include "StyleBar.h"

StyleBar::StyleBar(QWidget *parent) : QWidget(parent) {
    // Hàng ngang, lề sát để vừa nhét vào toolbar.
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(2, 2, 2, 2);
    m_layout->setSpacing(2);

    // Khởi tạo từ bộ preset mặc định.
    setStyles(canvas::defaultQuickStyles());
}

void StyleBar::clearButtons() {
    // Gỡ và huỷ mọi widget con trong layout.
    while (QLayoutItem *item = m_layout->takeAt(0)) {
        if (QWidget *w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }
}

void StyleBar::setStyles(const QVector<canvas::QuickStyle> &styles) {
    clearButtons();

    for (const canvas::QuickStyle &style : styles) {
        QToolButton *btn = new QToolButton(this);
        btn->setText(style.name);

        // Swatch màu 16x16 làm icon cho nút.
        QPixmap swatch(16, 16);
        swatch.fill(style.color);
        btn->setIcon(QIcon(swatch));

        // Hiển thị cả chữ lẫn icon cạnh nhau.
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        // Capture style by value để lambda giữ được dữ liệu sau khi vòng lặp kết thúc.
        connect(btn, &QToolButton::clicked, this, [this, style]() {
            emit styleChosen(style);
        });

        m_layout->addWidget(btn);
    }

    m_layout->addStretch();   // đẩy các nút về bên trái
}
