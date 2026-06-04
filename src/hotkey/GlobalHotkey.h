#pragma once
#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QHash>

// Hotkey toàn cục (system-wide): hoạt động kể cả khi app ở nền/không focus.
// Đăng ký qua RegisterHotKey của Windows, bắt WM_HOTKEY trong native event filter
// rồi phát signal activated(id). Dùng để bấm Print Screen (hoặc tổ hợp khác) chụp
// màn hình từ bất cứ đâu. Nền tảng khác: stub no-op, registerHotkey() trả false.
namespace hotkey {

class GlobalHotkey : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    // Tự cài mình làm native event filter của app (qApp->installNativeEventFilter).
    explicit GlobalHotkey(QObject *parent = nullptr);
    ~GlobalHotkey() override;

    // Đăng ký 1 hotkey với id tự chọn (duy nhất). mods = Ctrl/Alt/Shift/Meta,
    // key = phím chính (A-Z, F1-F12, Print...). Trả true nếu OS chấp nhận.
    // Nếu id đã đăng ký, sẽ hủy cái cũ trước khi đăng ký lại.
    bool registerHotkey(int id, Qt::KeyboardModifiers mods, Qt::Key key);

    // Hủy 1 hotkey theo id (no-op nếu chưa đăng ký).
    void unregisterHotkey(int id);

    // Hủy toàn bộ hotkey đang đăng ký.
    void unregisterAll();

    // Bắt WM_HOTKEY (eventType == "windows_generic_MSG") -> phát activated(id).
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

signals:
    void activated(int id);

private:
    QHash<int, bool> m_registered; // id -> đã RegisterHotKey thành công
};

} // namespace hotkey
