#include "PenWidthPicker.h"

#include <QVariant>

PenWidthPicker::PenWidthPicker(QWidget *parent)
    : QWidget(parent)
    , m_combo(new QComboBox(this)) {
    // Hàng nhỏ gọn: nhãn + combobox, lề sát.
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    auto *label = new QLabel(QStringLiteral("Nét:"), this);

    // Danh sách độ dày nét có sẵn (px).
    const QVector<int> widths = {1, 2, 3, 5, 8, 12};
    for (int w : widths) {
        // Hiển thị "N px", lưu giá trị int vào itemData.
        m_combo->addItem(QStringLiteral("%1 px").arg(w), QVariant(w));
    }

    // Mặc định chọn 3 px.
    setWidth(3);

    layout->addWidget(label);
    layout->addWidget(m_combo);

    // Khi đổi lựa chọn, phát tín hiệu với giá trị độ dày tương ứng.
    connect(m_combo, &QComboBox::currentIndexChanged, this, [this](int) {
        emit widthChanged(width());
    });
}

int PenWidthPicker::width() const {
    return m_combo->currentData().toInt();
}

void PenWidthPicker::setWidth(int w) {
    // Tìm mục có độ dày gần nhất với w.
    int bestIndex = 0;
    int bestDiff = -1;
    for (int i = 0; i < m_combo->count(); ++i) {
        const int candidate = m_combo->itemData(i).toInt();
        const int diff = qAbs(candidate - w);
        if (bestDiff < 0 || diff < bestDiff) {
            bestDiff = diff;
            bestIndex = i;
        }
    }
    m_combo->setCurrentIndex(bestIndex);
}
