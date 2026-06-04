#pragma once
#include <QRect>
#include <QPoint>
#include <QVector>

// Ánh xạ toạ độ LOGICAL <-> PIXEL VẬT LÝ cho desktop ảo đa màn hình mixed-DPI.
// Logic THUẦN (chỉ QRect/QPoint/QVector, KHÔNG gọi OS), test headless được.
//
// Bối cảnh (NFR SPEC 12 §12.2): Ezsnagit chụp toàn bộ virtual desktop vào MỘT ảnh
// theo PIXEL VẬT LÝ (gốc ảnh = góc trên-trái virtual desktop vật lý). Overlay phủ
// desktop theo toạ độ LOGICAL. Khi các monitor có scale factor khác nhau, một tỉ lệ
// scale đồng nhất (imageSize/widgetSize) là SAI — mỗi monitor có scale riêng. Module
// này map theo từng monitor để chính xác ở ranh giới DPI.
namespace capture {

// Một monitor mô tả bằng cả 2 hệ toạ độ.
struct Monitor {
    QRect logical;   // hình học monitor theo toạ độ LOGICAL (như QScreen::geometry)
    QRect physical;  // hình học monitor theo PIXEL VẬT LÝ (toạ độ virtual desktop vật lý)
};

// Trả chỉ số monitor chứa điểm logical (logical.contains), hoặc -1 nếu không monitor nào.
// Monitor đầu tiên trong danh sách chứa điểm sẽ thắng.
int monitorAt(const QVector<Monitor> &mons, const QPoint &logicalPt);

// Map một điểm từ toạ độ LOGICAL sang toạ độ ẢNH (pixel vật lý đã trừ gốc virtual desktop).
//   1. Tìm monitor chứa logicalPt.
//   2. Nội suy tuyến tính từ ô logical sang ô physical của monitor đó:
//        physX = m.physical.left + (logicalPt.x - m.logical.left) * (m.physical.width / m.logical.width)
//      tương tự cho Y.
//   3. Trừ physVirtualOrigin để ra toạ độ trong ẢNH.
// Fallback: nếu không monitor nào chứa điểm, dùng monitor GẦN NHẤT (theo khoảng cách từ
// điểm tới tâm vùng logical của monitor) để nội suy (ngoại suy tuyến tính). Nếu danh sách
// rỗng thì trả về (logicalPt - physVirtualOrigin) như mapping 1:1.
QPoint logicalToPhysical(const QVector<Monitor> &mons, const QPoint &logicalPt,
                         const QPoint &physVirtualOrigin);

// Map một QRect từ toạ độ LOGICAL sang toạ độ ẢNH: map 2 góc (topLeft, bottomRight) qua
// logicalToPhysical, dựng QRect rồi normalize. Mỗi góc được map theo monitor chứa nó nên
// rect bắc qua biên DPI vẫn đúng tỉ lệ ở từng phía.
QRect logicalRectToImage(const QVector<Monitor> &mons, const QRect &logicalRect,
                         const QPoint &physVirtualOrigin);

} // namespace capture
