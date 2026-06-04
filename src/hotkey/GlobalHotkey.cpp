#include "hotkey/GlobalHotkey.h"
#include <QGuiApplication>

#ifdef _WIN32
#include <windows.h>

namespace hotkey {

// Dịch Qt::KeyboardModifiers -> fsModifiers của RegisterHotKey (MOD_*).
static UINT toWinModifiers(Qt::KeyboardModifiers mods) {
    UINT f = 0;
    if (mods & Qt::ControlModifier) f |= MOD_CONTROL;
    if (mods & Qt::AltModifier)     f |= MOD_ALT;
    if (mods & Qt::ShiftModifier)   f |= MOD_SHIFT;
    if (mods & Qt::MetaModifier)    f |= MOD_WIN;
    return f;
}

// Dịch Qt::Key -> virtual-key code (VK_*). Trả 0 nếu không hỗ trợ.
static UINT toVirtualKey(Qt::Key key) {
    // Chữ cái A-Z: VK trùng mã ASCII chữ HOA ('A'..'Z').
    if (key >= Qt::Key_A && key <= Qt::Key_Z)
        return static_cast<UINT>('A' + (key - Qt::Key_A));
    // F1-F12: VK_F1..VK_F12 liên tiếp.
    if (key >= Qt::Key_F1 && key <= Qt::Key_F12)
        return static_cast<UINT>(VK_F1 + (key - Qt::Key_F1));
    switch (key) {
    case Qt::Key_Print: return VK_SNAPSHOT; // Print Screen
    default:            return 0;
    }
}

GlobalHotkey::GlobalHotkey(QObject *parent) : QObject(parent) {
    if (qApp)
        qApp->installNativeEventFilter(this);
}

GlobalHotkey::~GlobalHotkey() {
    unregisterAll();
    if (qApp)
        qApp->removeNativeEventFilter(this);
}

bool GlobalHotkey::registerHotkey(int id, Qt::KeyboardModifiers mods, Qt::Key key) {
    // Đăng ký lại id cũ -> hủy trước cho sạch.
    if (m_registered.contains(id))
        unregisterHotkey(id);

    const UINT vk = toVirtualKey(key);
    if (vk == 0)
        return false; // phím không hỗ trợ

    const UINT fsModifiers = toWinModifiers(mods) | MOD_NOREPEAT;
    if (!RegisterHotKey(nullptr, id, fsModifiers, vk))
        return false;

    m_registered.insert(id, true);
    return true;
}

void GlobalHotkey::unregisterHotkey(int id) {
    if (!m_registered.contains(id))
        return;
    UnregisterHotKey(nullptr, id);
    m_registered.remove(id);
}

void GlobalHotkey::unregisterAll() {
    for (auto it = m_registered.constBegin(); it != m_registered.constEnd(); ++it)
        UnregisterHotKey(nullptr, it.key());
    m_registered.clear();
}

bool GlobalHotkey::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    Q_UNUSED(result);
    if (eventType == "windows_generic_MSG" && message) {
        MSG *m = static_cast<MSG *>(message);
        if (m->message == WM_HOTKEY) {
            const int id = static_cast<int>(m->wParam);
            emit activated(id);
        }
    }
    return false; // không nuốt event
}

} // namespace hotkey

#else // !_WIN32 — stub no-op cho nền tảng khác (vẫn biên dịch được).

namespace hotkey {

GlobalHotkey::GlobalHotkey(QObject *parent) : QObject(parent) {
    if (qApp)
        qApp->installNativeEventFilter(this);
}

GlobalHotkey::~GlobalHotkey() {
    unregisterAll();
    if (qApp)
        qApp->removeNativeEventFilter(this);
}

bool GlobalHotkey::registerHotkey(int id, Qt::KeyboardModifiers mods, Qt::Key key) {
    Q_UNUSED(id);
    Q_UNUSED(mods);
    Q_UNUSED(key);
    return false; // chưa hỗ trợ ngoài Windows
}

void GlobalHotkey::unregisterHotkey(int id) {
    m_registered.remove(id);
}

void GlobalHotkey::unregisterAll() {
    m_registered.clear();
}

bool GlobalHotkey::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    Q_UNUSED(eventType);
    Q_UNUSED(message);
    Q_UNUSED(result);
    return false;
}

} // namespace hotkey

#endif
