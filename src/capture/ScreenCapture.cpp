#include "capture/ScreenCapture.h"
#include "capture/ScreenGeometry.h"

#ifdef _WIN32
#include <windows.h>

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

} // namespace capture

#else  // ----- nền tảng khác: stub (bổ sung sau) -----

namespace capture {
QRect  virtualDesktopRect()                       { return QRect(); }
QImage captureVirtualDesktop()                     { return QImage(); }
QImage captureRegion(const QRect &)                { return QImage(); }
}

#endif
