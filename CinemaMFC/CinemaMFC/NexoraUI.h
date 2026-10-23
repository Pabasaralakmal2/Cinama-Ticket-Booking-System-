#pragma once
#include <gdiplus.h>
using namespace Gdiplus;
namespace NexoraUI {
    static const COLORREF CLR_BG_DARK     = RGB(12, 11, 10);
    static const COLORREF CLR_BG_PANEL    = RGB(22, 19, 16);
    static const COLORREF CLR_GOLD        = RGB(198, 163, 106);
    static const COLORREF CLR_GOLD_BRIGHT = RGB(228, 193, 136);
    static const COLORREF CLR_TEAL        = RGB(61, 120, 180);
    static const COLORREF CLR_RED         = RGB(182, 69, 69);
    static const COLORREF CLR_TEXT        = RGB(243, 236, 223);
    static const COLORREF CLR_TEXT_DIM    = RGB(163, 152, 136);
    static const Color GCLR_BG         = Color(255, 12, 11, 10);
    static const Color GCLR_BG2        = Color(255, 22, 19, 16);
    static const Color GCLR_PANEL      = Color(255, 33, 28, 23);
    static const Color GCLR_GOLD       = Color(255, 198, 163, 106);
    static const Color GCLR_GOLD_DIM   = Color(180, 198, 163, 106);
    static const Color GCLR_TEAL       = Color(255, 61, 120, 180);
    static const Color GCLR_RED        = Color(255, 182, 69, 69);
    static const Color GCLR_WHITE      = Color(255, 243, 236, 223);
    static const Color GCLR_GLASS      = Color(60, 255, 255, 255);
    static const Color GCLR_TRANSPARENT = Color(0, 0, 0, 0);
    inline void DrawBackground(Graphics& g, int W, int H) {
        SolidBrush bgBrush(GCLR_BG);
        g.FillRectangle(&bgBrush, 0, 0, W, H);
    }

    inline Bitmap* LoadTheaterBg() {
        WCHAR exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        std::wstring basePath = exePath;
        size_t lastSlash = basePath.find_last_of(L"\\");
        if (lastSlash != std::wstring::npos)
            basePath = basePath.substr(0, lastSlash + 1);

        Bitmap* bmp = Bitmap::FromFile((basePath + L"nexora_main_bg.png").c_str());
        if (!bmp || bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = Bitmap::FromFile((basePath + L"assets\\nexora_main_bg.png").c_str());
        }
        if (!bmp || bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = Bitmap::FromFile((basePath + L"nexora_main_bg.jpg").c_str());
        }
        if (!bmp || bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = Bitmap::FromFile((basePath + L"assets\\nexora_main_bg.jpg").c_str());
        }
        if (bmp && bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = nullptr;
        }
        return bmp;
    }

        inline Bitmap* LoadMovieManagerBg() {
        WCHAR exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        std::wstring basePath = exePath;
        size_t lastSlash = basePath.find_last_of(L"\\");
        if (lastSlash != std::wstring::npos)
            basePath = basePath.substr(0, lastSlash + 1);

        Bitmap* bmp = Bitmap::FromFile((basePath + L"admin_movie_bg.png").c_str());
        if (!bmp || bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = Bitmap::FromFile((basePath + L"assets\\admin_movie_bg.png").c_str());
        }
        if (!bmp || bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = Bitmap::FromFile((basePath + L"admin_movie_bg.jpg").c_str());
        }
        if (!bmp || bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = Bitmap::FromFile((basePath + L"assets\\admin_movie_bg.jpg").c_str());
        }
        if (!bmp || bmp->GetLastStatus() != Ok) {
            delete bmp;
            bmp = LoadTheaterBg();
        }
        return bmp;
    }

    inline void DrawTheaterBackground(Graphics& g, Bitmap* pBg, int W, int H) {
        if (!pBg || pBg->GetLastStatus() != Ok) {
            DrawBackground(g, W, H);
            return;
        }
        g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        REAL imgW = (REAL)pBg->GetWidth();
        REAL imgH = (REAL)pBg->GetHeight();
        REAL scale = max((REAL)W / imgW, (REAL)H / imgH);
        REAL dw = imgW * scale;
        REAL dh = imgH * scale;
        REAL dx = ((REAL)W - dw) / 2.0f;
        REAL dy = ((REAL)H - dh) / 2.0f;
        g.DrawImage(pBg, RectF(dx, dy, dw, dh));
    }

    inline void DrawGlassBackground(Graphics& g, Bitmap* pBg, int W, int H, BYTE tintAlpha = 210) {
        DrawTheaterBackground(g, pBg, W, H);

        SolidBrush glassBrush(Color(tintAlpha, 14, 13, 18));
        g.FillRectangle(&glassBrush, 0, 0, W, H);

        LinearGradientBrush glossBrush(
            PointF(0, 0), PointF(0, (REAL)min(H, 180)),
            Color(35, 255, 255, 255),
            Color(0, 255, 255, 255));
        g.FillRectangle(&glossBrush, 0, 0, W, min(H, 180));
    }

    inline void DrawGlassPanel(Graphics& g, RectF rect, BYTE alpha = 60) {
        SolidBrush panelBrush(Color(alpha + 30, 30, 28, 55));
        g.FillRectangle(&panelBrush, rect);
        Pen borderPen(Color(100, 218, 165, 32), 1.0f);
        g.DrawRectangle(&borderPen, rect.X, rect.Y, rect.Width - 1, rect.Height - 1);
    }
    inline void DrawGoldSeparator(Graphics& g, int x1, int y, int x2) {
        LinearGradientBrush sepBrush(
            PointF((REAL)x1, (REAL)y), PointF((REAL)x2, (REAL)y),
            Color(0, 218, 165, 32),
            Color(0, 218, 165, 32));
        Color sepColors[] = {
            Color(0, 218, 165, 32),
            Color(200, 218, 165, 32),
            Color(255, 255, 200, 80),
            Color(200, 218, 165, 32),
            Color(0, 218, 165, 32)
        };
        REAL positions[] = { 0.0f, 0.2f, 0.5f, 0.8f, 1.0f };
        sepBrush.SetInterpolationColors(sepColors, positions, 5);
        g.FillRectangle(&sepBrush, (REAL)x1, (REAL)y, (REAL)(x2 - x1), 1.5f);
    }
    inline void DrawPrimaryButton(Graphics& g, RectF rect, const WCHAR* text, int state = 0) {
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        Color topColor, bottomColor, glowColor;
        if (state == 2) {
            topColor   = Color(255, 30, 130, 110);
            bottomColor= Color(255, 0,  90,  75);
            glowColor  = Color(0, 0, 0, 0);
        } else if (state == 1) {
            topColor   = Color(255, 25, 195, 170);
            bottomColor= Color(255, 0,  160, 140);
            glowColor  = Color(80, 0, 200, 180);
        } else {
            topColor   = Color(255, 15, 155, 135);
            bottomColor= Color(255, 0,  120, 100);
            glowColor  = Color(40, 0, 180, 160);
        }
        if (state > 0) {
            for (int g_i = 4; g_i >= 1; g_i--) {
                Color glowC(80 / g_i, 0, 200, 180);
                Pen glowPen(glowC, (REAL)(g_i * 2));
                g.DrawRectangle(&glowPen,
                    rect.X - g_i, rect.Y - g_i,
                    rect.Width + g_i*2 - 1, rect.Height + g_i*2 - 1);
            }
        }
        LinearGradientBrush fillBrush(
            PointF(rect.X, rect.Y),
            PointF(rect.X, rect.Y + rect.Height),
            topColor, bottomColor);
        g.FillRectangle(&fillBrush, rect);
        LinearGradientBrush shineBrush(
            PointF(rect.X, rect.Y),
            PointF(rect.X, rect.Y + rect.Height * 0.5f),
            Color(60, 255, 255, 255),
            Color(0, 255, 255, 255));
        g.FillRectangle(&shineBrush, rect.X + 1, rect.Y + 1, rect.Width - 2, rect.Height * 0.5f);
        Pen borderPen(Color(180, 0, 220, 200), 1.5f);
        g.DrawRectangle(&borderPen, rect.X, rect.Y, rect.Width - 1, rect.Height - 1);
        FontFamily ff(L"Segoe UI");
        Gdiplus::Font btnFont(&ff, 11, FontStyleBold, UnitPixel);
        SolidBrush txtBrush(Color(255, 255, 255, 255));
        StringFormat sf;
        sf.SetAlignment(StringAlignmentCenter);
        sf.SetLineAlignment(StringAlignmentCenter);
        g.DrawString(text, -1, &btnFont, rect, &sf, &txtBrush);
    }
    inline void DrawGoldButton(Graphics& g, RectF rect, const WCHAR* text, int state = 0) {
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        Color topColor, bottomColor;
        if (state == 2) {
            topColor   = Color(255, 160, 120, 20);
            bottomColor= Color(255, 110,  80,  5);
        } else if (state == 1) {
            topColor   = Color(255, 255, 220, 80);
            bottomColor= Color(255, 218, 165, 32);
        } else {
            topColor   = Color(255, 230, 185, 50);
            bottomColor= Color(255, 170, 120, 15);
        }
        if (state > 0) {
            for (int g_i = 3; g_i >= 1; g_i--) {
                Color glowC(70 / g_i, 218, 165, 32);
                Pen glowPen(glowC, (REAL)(g_i * 2));
                g.DrawRectangle(&glowPen, rect.X - g_i, rect.Y - g_i,
                    rect.Width + g_i*2-1, rect.Height + g_i*2-1);
            }
        }
        LinearGradientBrush fillBrush(
            PointF(rect.X, rect.Y),
            PointF(rect.X, rect.Y + rect.Height),
            topColor, bottomColor);
        g.FillRectangle(&fillBrush, rect);
        LinearGradientBrush shineBrush(
            PointF(rect.X, rect.Y),
            PointF(rect.X, rect.Y + rect.Height * 0.5f),
            Color(80, 255, 255, 255),
            Color(0, 255, 255, 255));
        g.FillRectangle(&shineBrush, rect.X+1, rect.Y+1, rect.Width-2, rect.Height*0.45f);
        Pen borderPen(Color(255, 255, 200, 60), 1.5f);
        g.DrawRectangle(&borderPen, rect.X, rect.Y, rect.Width - 1, rect.Height - 1);
        FontFamily ff(L"Segoe UI");
        Gdiplus::Font btnFont(&ff, 11, FontStyleBold, UnitPixel);
        SolidBrush txtBrush(Color(255, 20, 12, 2));
        StringFormat sf;
        sf.SetAlignment(StringAlignmentCenter);
        sf.SetLineAlignment(StringAlignmentCenter);
        g.DrawString(text, -1, &btnFont, rect, &sf, &txtBrush);
    }
    inline void DrawDangerButton(Graphics& g, RectF rect, const WCHAR* text, int state = 0) {
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        Color topColor    = (state == 1) ? Color(255, 230, 70, 70)  : Color(255, 180, 40, 40);
        Color bottomColor = (state == 1) ? Color(255, 180, 30, 30)  : Color(255, 130, 20, 20);
        LinearGradientBrush fillBrush(
            PointF(rect.X, rect.Y),
            PointF(rect.X, rect.Y + rect.Height),
            topColor, bottomColor);
        g.FillRectangle(&fillBrush, rect);
        LinearGradientBrush shineBrush(
            PointF(rect.X, rect.Y),
            PointF(rect.X, rect.Y + rect.Height * 0.5f),
            Color(60, 255, 255, 255),
            Color(0, 255, 255, 255));
        g.FillRectangle(&shineBrush, rect.X+1, rect.Y+1, rect.Width-2, rect.Height*0.45f);
        Pen borderPen(Color(200, 255, 100, 100), 1.0f);
        g.DrawRectangle(&borderPen, rect.X, rect.Y, rect.Width - 1, rect.Height - 1);
        FontFamily ff(L"Segoe UI");
        Gdiplus::Font btnFont(&ff, 10, FontStyleBold, UnitPixel);
        SolidBrush txtBrush(Color(255, 255, 235, 235));
        StringFormat sf;
        sf.SetAlignment(StringAlignmentCenter);
        sf.SetLineAlignment(StringAlignmentCenter);
        g.DrawString(text, -1, &btnFont, rect, &sf, &txtBrush);
    }
    inline void DrawHeading(Graphics& g, const WCHAR* text, RectF rect,
                            REAL size = 20, bool bold = true) {
        FontFamily ff(L"Segoe UI");
        int style = bold ? FontStyleBold : FontStyleRegular;
        Gdiplus::Font font(&ff, size, style, UnitPixel);
        StringFormat sf;
        sf.SetAlignment(StringAlignmentCenter);
        sf.SetLineAlignment(StringAlignmentCenter);
        SolidBrush shadowBrush(Color(100, 0, 0, 0));
        RectF shadowRect(rect.X+2, rect.Y+2, rect.Width, rect.Height);
        g.DrawString(text, -1, &font, shadowRect, &sf, &shadowBrush);
        SolidBrush goldBrush(GCLR_GOLD);
        g.DrawString(text, -1, &font, rect, &sf, &goldBrush);
    }
    inline void DrawLabel(Graphics& g, const WCHAR* text, RectF rect,
                          Color color, REAL size = 10, bool bold = false) {
        FontFamily ff(L"Segoe UI");
        int style = bold ? FontStyleBold : FontStyleRegular;
        Gdiplus::Font font(&ff, size, style, UnitPixel);
        StringFormat sf;
        sf.SetLineAlignment(StringAlignmentCenter);
        SolidBrush brush(color);
        g.DrawString(text, -1, &font, rect, &sf, &brush);
    }

    inline LRESULT CALLBACK CleanButtonSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
        if (uMsg == WM_NCPAINT) {
            return 0;
        }
        if (uMsg == WM_ERASEBKGND) {
            return TRUE;
        }
        return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    inline void SetupCustomButton(CWnd* pParent, int nBtnId) {
        CButton* pBtn = (CButton*)pParent->GetDlgItem(nBtnId);
        if (pBtn && pBtn->GetSafeHwnd()) {
            HWND hWnd = pBtn->GetSafeHwnd();
            LONG style = GetWindowLong(hWnd, GWL_STYLE);
            style &= ~(BS_DEFPUSHBUTTON | WS_BORDER);
            SetWindowLong(hWnd, GWL_STYLE, style | BS_OWNERDRAW);

            LONG exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
            exStyle &= ~(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME);
            SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);

            SetWindowSubclass(hWnd, CleanButtonSubclassProc, (UINT_PTR)nBtnId, 0);

            CRect r;
            pBtn->GetClientRect(&r);
            if (r.Width() > 0 && r.Height() > 0) {
                HRGN hRgn = CreateRoundRectRgn(0, 0, r.Width() + 1, r.Height() + 1, r.Height(), r.Height());
                pBtn->SetWindowRgn(hRgn, TRUE);
            }
            SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            pBtn->Invalidate(FALSE);
        }
    }

    inline void UpdateCustomButtonRegion(CWnd* pParent, int nBtnId) {
        CButton* pBtn = (CButton*)pParent->GetDlgItem(nBtnId);
        if (pBtn && pBtn->GetSafeHwnd()) {
            CRect r;
            pBtn->GetClientRect(&r);
            if (r.Width() > 0 && r.Height() > 0) {
                HRGN hRgn = CreateRoundRectRgn(0, 0, r.Width() + 1, r.Height() + 1, r.Height(), r.Height());
                pBtn->SetWindowRgn(hRgn, TRUE);
            }
        }
    }

    inline void DrawImageButton(LPDRAWITEMSTRUCT lpDIS, Gdiplus::Bitmap* pBmp, bool isHovered) {
        Gdiplus::Graphics g(lpDIS->hDC);
        g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
        g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

        CRect r = lpDIS->rcItem;
        Gdiplus::RectF rect((Gdiplus::REAL)r.left, (Gdiplus::REAL)r.top, (Gdiplus::REAL)r.Width(), (Gdiplus::REAL)r.Height());
        bool pressed = (lpDIS->itemState & ODS_SELECTED) != 0;

        Gdiplus::RectF drawRect = rect;
        if (pressed) {
            drawRect.Y += 2.0f;
            drawRect.Height -= 2.0f;
        }

        if (isHovered && !pressed) {
            Gdiplus::SolidBrush glow(Gdiplus::Color(35, 255, 255, 255));
            g.FillEllipse(&glow, drawRect);
        }

        if (pBmp && pBmp->GetLastStatus() == Gdiplus::Ok) {
            Gdiplus::REAL imgW = (Gdiplus::REAL)pBmp->GetWidth();
            Gdiplus::REAL imgH = (Gdiplus::REAL)pBmp->GetHeight();
            Gdiplus::REAL scale = min(drawRect.Width / imgW, drawRect.Height / imgH);
            Gdiplus::REAL dw = imgW * scale;
            Gdiplus::REAL dh = imgH * scale;
            Gdiplus::REAL dx = drawRect.X + (drawRect.Width - dw) / 2.0f;
            Gdiplus::REAL dy = drawRect.Y + (drawRect.Height - dh) / 2.0f;
            g.DrawImage(pBmp, Gdiplus::RectF(dx, dy, dw, dh));
        }
    }
}
