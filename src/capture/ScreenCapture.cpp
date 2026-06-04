#include "capture/ScreenCapture.h"
#include "capture/ScreenGeometry.h"

#ifdef _WIN32
#include <windows.h>
#include <dwmapi.h>
#include <QString>

namespace capture {

QRect virtualDesktopRect() {
    const int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    const int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    const int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    const int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    return QRect(x, y, w, h);
}

// Chụp một vùng pixel vật lý [src] ra QImage (top-down RGB32) qua GDI BitBlt.
static QImage grab(const QRect &src) {
    if (src.width() <= 0 || src.height() <= 0) return QImage();
    const int w = src.width(), h = src.height();

    HDC screenDC = GetDC(nullptr);
    if (!screenDC) return QImage();

    HDC memDC = CreateCompatibleDC(screenDC);
    HBITMAP bmp = memDC ? CreateCompatibleBitmap(screenDC, w, h) : nullptr;
    if (!memDC || !bmp) {                       // hết tài nguyên / desktop quá lớn
        if (bmp) DeleteObject(bmp);
        if (memDC) DeleteDC(memDC);
        ReleaseDC(nullptr, screenDC);
        return QImage();
    }

    HGDIOBJ oldBmp = SelectObject(memDC, bmp);

    QImage img(w, h, QImage::Format_RGB32);     // bộ nhớ BGRA little-endian = khớp BI_RGB 32bit
    bool okGrab = !img.isNull();
    if (okGrab) {
        // CAPTUREBLT để bắt cả lớp layered (vd menu trong suốt).
        okGrab = BitBlt(memDC, 0, 0, w, h, screenDC, src.x(), src.y(), SRCCOPY | CAPTUREBLT);
    }
    if (okGrab) {
        BITMAPINFO bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = -h;             // âm = top-down
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        const int scan = GetDIBits(memDC, bmp, 0, h, img.bits(), &bi, DIB_RGB_COLORS);
        okGrab = (scan == h);                   // thiếu scanline -> coi như lỗi
    }

    SelectObject(memDC, oldBmp);
    DeleteObject(bmp);
    DeleteDC(memDC);
    ReleaseDC(nullptr, screenDC);
    return okGrab ? img : QImage();
}

QImage captureVirtualDesktop() {
    return grab(virtualDesktopRect());
}

QImage captureRegion(const QRect &regionPx) {
    const QRect clamped = clampToBounds(regionPx, virtualDesktopRect());
    return clamped.isEmpty() ? QImage() : grab(clamped);
}

// Rect "thật" của cửa sổ: ưu tiên DWM extended frame bounds (sát viền, bỏ shadow);
// fallback GetWindowRect. Trả pixel vật lý.
static QRect windowFrameRect(HWND hwnd) {
    RECT r;
    if (SUCCEEDED(DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &r, sizeof(r)))
        && r.right > r.left && r.bottom > r.top)
        return QRect(QPoint(r.left, r.top), QPoint(r.right - 1, r.bottom - 1));
    if (GetWindowRect(hwnd, &r) && r.right > r.left && r.bottom > r.top)   // fallback
        return QRect(QPoint(r.left, r.top), QPoint(r.right - 1, r.bottom - 1));
    return QRect();
}

// Cửa sổ shell (desktop/taskbar) — không cho chọn để chụp.
static bool isShellWindow(HWND hwnd) {
    if (hwnd == GetShellWindow()) return true;
    wchar_t cls[64] = {0};
    GetClassNameW(hwnd, cls, 64);
    const QString cn = QString::fromWCharArray(cls);
    return cn == QLatin1String("Progman") || cn == QLatin1String("WorkerW")
        || cn == QLatin1String("Shell_TrayWnd") || cn == QLatin1String("Shell_SecondaryTrayWnd")
        || cn == QLatin1String("NotifyIconOverflowWindow");
}

static BOOL CALLBACK enumProc(HWND hwnd, LPARAM lparam) {
    auto *out = reinterpret_cast<QVector<QRect> *>(lparam);
    if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) return TRUE;        // ẩn/minimize
    const LONG_PTR ex = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    if (ex & WS_EX_TOOLWINDOW) return TRUE;                           // palette nổi
    if (isShellWindow(hwnd)) return TRUE;                             // desktop/taskbar
    // Bỏ layered window trong suốt hoàn toàn (helper vô hình alpha=0).
    if (ex & WS_EX_LAYERED) {
        BYTE alpha = 255; DWORD flags = 0; COLORREF key = 0;
        if (GetLayeredWindowAttributes(hwnd, &key, &alpha, &flags) && (flags & LWA_ALPHA) && alpha == 0)
            return TRUE;
    }
    // Bỏ cửa sổ bị DWM cloak (vd app UWP ở desktop ảo khác).
    BOOL cloaked = FALSE;
    if (SUCCEEDED(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &cloaked, sizeof(cloaked))) && cloaked)
        return TRUE;
    const QRect fr = windowFrameRect(hwnd);
    if (fr.width() > 1 && fr.height() > 1) out->append(fr);
    return TRUE;
}

QVector<QRect> enumerateWindowRects() {
    QVector<QRect> rects;                 // EnumWindows trả theo Z-order trước→sau
    EnumWindows(enumProc, reinterpret_cast<LPARAM>(&rects));
    return rects;
}

} // namespace capture

#else  // ----- nền tảng khác: stub (bổ sung sau) -----

namespace capture {
QRect  virtualDesktopRect()                       { return QRect(); }
QImage captureVirtualDesktop()                     { return QImage(); }
QImage captureRegion(const QRect &)                { return QImage(); }
QVector<QRect> enumerateWindowRects()              { return {}; }
}

#endif
