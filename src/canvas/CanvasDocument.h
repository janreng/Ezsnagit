#pragma once
#include "AnnotationObject.h"
#include <QImage>
#include <QJsonObject>
#include <QVector>

// Model thuần cho editor (P2 backbone): ảnh nền + danh sách annotation (z-order dưới->trên)
// + undo/redo đơn giản (chỉ add/remove là đủ cho P2). Không phụ thuộc UI/QObject.
namespace canvas {

class CanvasDocument {
public:
    // --- Nền ---
    void setBackground(const QImage &img);
    const QImage &background() const { return m_background; }

    // --- Objects (z-order: index nhỏ ở dưới, index lớn ở trên) ---
    void addObject(const AnnotationObject &o); // đẩy thao tác vào undo stack
    const QVector<AnnotationObject> &objects() const { return m_objects; }
    void clear();                              // xoá hết object + lịch sử undo/redo

    // --- Undo / Redo ---
    bool undo();                  // gỡ object cuối, đẩy sang redo; false nếu không có gì
    bool redo();                  // thêm lại object đã undo; false nếu không có gì
    bool canUndo() const { return !m_objects.isEmpty(); } // undo được cả doc nạp từ file
    bool canRedo() const { return !m_redo.isEmpty(); }

    // --- Render / Serialize ---
    // Vẽ background rồi paintObject từng cái theo thứ tự -> QImage MỚI (flatten).
    QImage renderFlattened() const;

    // background = PNG base64; objects = mảng {type,rect,color,penWidth,text}.
    QJsonObject toJson() const;
    static CanvasDocument fromJson(const QJsonObject &obj);

private:
    QImage m_background;
    QVector<AnnotationObject> m_objects;       // chính là undo stack (object cuối = mới nhất)
    QVector<AnnotationObject> m_redo;          // object đã undo, chờ redo
};

} // namespace canvas
