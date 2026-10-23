#include "pch.h"
#include "NexoraCinemaDlg.h"
#include "NexoraCinemaApp.h"
#include "DatabaseManager.h"
#include "AdminMovieManagerDlg.h"
#include "AdminLoginDlg.h"
#include "NexoraUI.h"
#include <gdiplus.h>
using namespace Gdiplus;
BEGIN_MESSAGE_MAP(CNexoraCinemaDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_WM_DRAWITEM()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BTN_NAV_ADMIN,  &CNexoraCinemaDlg::OnBnClickedBtnNavAdmin)
    ON_BN_CLICKED(IDC_BTN_NAV_EXIT,   &CNexoraCinemaDlg::OnBnClickedBtnNavExit)
END_MESSAGE_MAP()
CNexoraCinemaDlg::CNexoraCinemaDlg(CWnd* pParent)
    : CDialogEx(IDD_NEXORACINEMA_DIALOG, pParent)
    , m_pLogoBitmap(nullptr)
    , m_pTheaterBgBitmap(nullptr)
    , m_pAdminBtnBitmap(nullptr)
    , m_pExitBtnBitmap(nullptr)
    , m_HoveredBtn(-1) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CNexoraCinemaDlg::~CNexoraCinemaDlg() {
    delete m_pLogoBitmap;
    delete m_pTheaterBgBitmap;
    delete m_pAdminBtnBitmap;
    delete m_pExitBtnBitmap;
}
void CNexoraCinemaDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
}
void CNexoraCinemaDlg::LoadLogo() {
    WCHAR exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring basePath = exePath;
    size_t lastSlash = basePath.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos)
        basePath = basePath.substr(0, lastSlash + 1);

    std::wstring logoPath = basePath + L"nexora_logo.png";
    m_pLogoBitmap = Bitmap::FromFile(logoPath.c_str());
    if (!m_pLogoBitmap || m_pLogoBitmap->GetLastStatus() != Ok) {
        delete m_pLogoBitmap;
        m_pLogoBitmap = Bitmap::FromFile((basePath + L"assets\\nexora_logo.png").c_str());
    }

    std::wstring btnPath = basePath + L"admin_dashboard_btn.png";
    m_pAdminBtnBitmap = Bitmap::FromFile(btnPath.c_str());
    if (!m_pAdminBtnBitmap || m_pAdminBtnBitmap->GetLastStatus() != Ok) {
        delete m_pAdminBtnBitmap;
        m_pAdminBtnBitmap = Bitmap::FromFile((basePath + L"assets\\admin_dashboard_btn.png").c_str());
    }

    std::wstring exitPath = basePath + L"btn_exit.png";
    m_pExitBtnBitmap = Bitmap::FromFile(exitPath.c_str());
    if (!m_pExitBtnBitmap || m_pExitBtnBitmap->GetLastStatus() != Ok) {
        delete m_pExitBtnBitmap;
        m_pExitBtnBitmap = Bitmap::FromFile((basePath + L"assets\\btn_exit.png").c_str());
    }
}
BOOL CNexoraCinemaDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    DatabaseManager::GetInstance().Connect();
    m_pTheaterBgBitmap = NexoraUI::LoadTheaterBg();
    LoadLogo();
    NexoraUI::SetupCustomButton(this, IDC_BTN_NAV_ADMIN);
    NexoraUI::SetupCustomButton(this, IDC_BTN_NAV_EXIT);
    TRACKMOUSEEVENT tme = { sizeof(tme), TME_HOVER | TME_LEAVE, GetSafeHwnd(), 1 };
    TrackMouseEvent(&tme);
    ShowWindow(SW_SHOWMAXIMIZED);
    return TRUE;
}
BOOL CNexoraCinemaDlg::OnEraseBkgnd(CDC* ) {
    return TRUE;
}
void CNexoraCinemaDlg::OnPaint() {
    CPaintDC dc(this);
    if (IsIconic()) {
        SendMessage(WM_ICONERASEBKGND,
            reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect; GetClientRect(&rect);
        dc.DrawIcon((rect.Width() - cxIcon + 1) / 2,
                    (rect.Height() - cyIcon + 1) / 2, m_hIcon);
        return;
    }
    CRect rc;
    GetClientRect(&rc);
    int W = rc.Width(), H = rc.Height();
    Graphics g(dc.GetSafeHdc());
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    g.SetTextRenderingHint(TextRenderingHintAntiAlias);
    if (m_pTheaterBgBitmap && m_pTheaterBgBitmap->GetLastStatus() == Ok) {
        NexoraUI::DrawTheaterBackground(g, m_pTheaterBgBitmap, W, H);
        int vigTop = (int)(H * 0.76f);
        LinearGradientBrush seatShade(
            PointF(0, (REAL)vigTop), PointF(0, (REAL)H),
            Color(0, 8, 8, 12),
            Color(180, 8, 8, 12));
        g.FillRectangle(&seatShade, 0, vigTop, W, H - vigTop);
    } else {
        NexoraUI::DrawBackground(g, W, H);
        int logoAreaH = (int)(H * 0.58f);
        int logoAreaW = W - 80;
        if (m_pLogoBitmap) {
            REAL iw = (REAL)m_pLogoBitmap->GetWidth();
            REAL ih = (REAL)m_pLogoBitmap->GetHeight();
            REAL scale = min((REAL)logoAreaW / iw, (REAL)logoAreaH / ih);
            REAL dw = iw * scale, dh = ih * scale;
            REAL ox = ((REAL)W - dw) / 2.0f;
            REAL oy = ((REAL)logoAreaH - dh) / 2.0f + 10.0f;
            g.DrawImage(m_pLogoBitmap, RectF(ox, oy, dw, dh));
        }
    }
    NexoraUI::DrawGoldSeparator(g, 0, 0, W);
}
void CNexoraCinemaDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) {
    Graphics g(lpDIS->hDC);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    g.SetTextRenderingHint(TextRenderingHintAntiAlias);
    CRect r = lpDIS->rcItem;

    RectF fullRect((REAL)(r.left - 1), (REAL)(r.top - 1),
                   (REAL)(r.Width() + 2), (REAL)(r.Height() + 2));
    RectF rect((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height());
    bool pressed = (lpDIS->itemState & ODS_SELECTED) != 0;
    bool hovered = (nIDCtl == m_HoveredBtn);
    int state = pressed ? 2 : (hovered ? 1 : 0);
    if (nIDCtl == IDC_BTN_NAV_ADMIN) {
        if (m_pAdminBtnBitmap && m_pAdminBtnBitmap->GetLastStatus() == Ok) {
            SolidBrush bgBrush(Color(255, 8, 8, 20));
            g.FillRectangle(&bgBrush, fullRect);

            RectF drawRect = rect;
            if (state == 1) {
                SolidBrush glow(Color(25, 200, 160, 255));
                g.FillEllipse(&glow, drawRect);
            } else if (state == 2) {
                drawRect.Y += 2.0f;
            }

            REAL imgW = (REAL)m_pAdminBtnBitmap->GetWidth();
            REAL imgH = (REAL)m_pAdminBtnBitmap->GetHeight();
            REAL scale = min(drawRect.Width / imgW, drawRect.Height / imgH);
            REAL dw = imgW * scale, dh = imgH * scale;
            REAL dx = drawRect.X + (drawRect.Width  - dw) / 2.0f;
            REAL dy = drawRect.Y + (drawRect.Height - dh) / 2.0f;
            g.DrawImage(m_pAdminBtnBitmap, RectF(dx, dy, dw, dh));
        } else {
            NexoraUI::DrawPrimaryButton(g, rect, L"\u2699   Admin Dashboard", state);
        }
    } else if (nIDCtl == IDC_BTN_NAV_EXIT) {
        if (m_pExitBtnBitmap && m_pExitBtnBitmap->GetLastStatus() == Ok) {
            SolidBrush bgBrush(Color(255, 8, 8, 20));
            g.FillRectangle(&bgBrush, fullRect);

            RectF drawRect = rect;
            if (state == 1) {
                SolidBrush glow(Color(25, 255, 80, 80));
                g.FillEllipse(&glow, drawRect);
            } else if (state == 2) {
                drawRect.Y += 2.0f;
            }

            REAL imgW = (REAL)m_pExitBtnBitmap->GetWidth();
            REAL imgH = (REAL)m_pExitBtnBitmap->GetHeight();
            REAL scale = min(drawRect.Width / imgW, drawRect.Height / imgH);
            REAL dw = imgW * scale, dh = imgH * scale;
            REAL dx = drawRect.X + (drawRect.Width  - dw) / 2.0f;
            REAL dy = drawRect.Y + (drawRect.Height - dh) / 2.0f;
            g.DrawImage(m_pExitBtnBitmap, RectF(dx, dy, dw, dh));
        } else {
            NexoraUI::DrawDangerButton(g, rect, L"\u2715   Exit", state);
        }
    } else {
        CDialogEx::OnDrawItem(nIDCtl, lpDIS);
    }
}
void CNexoraCinemaDlg::OnMouseMove(UINT nFlags, CPoint point) {
    int btnIds[] = { IDC_BTN_NAV_ADMIN, IDC_BTN_NAV_EXIT };
    int newHover = -1;
    for (int id : btnIds) {
        CWnd* pBtn = GetDlgItem(id);
        if (pBtn) {
            CRect r; pBtn->GetWindowRect(&r); ScreenToClient(&r);
            CPoint clientPt = point;
            if (r.PtInRect(clientPt)) { newHover = id; break; }
        }
    }
    if (newHover != m_HoveredBtn) {
        m_HoveredBtn = newHover;
        for (int id : btnIds) {
            CWnd* p = GetDlgItem(id);
            if (p) p->Invalidate(FALSE);
        }
    }
    CDialogEx::OnMouseMove(nFlags, point);
}
HCURSOR CNexoraCinemaDlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}
HBRUSH CNexoraCinemaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    static CBrush s_darkBg(NexoraUI::CLR_BG_DARK);
    if (nCtlColor == CTLCOLOR_DLG) return s_darkBg;
    if (nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkMode(TRANSPARENT);
        return s_darkBg;
    }
    return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}
void CNexoraCinemaDlg::OnSize(UINT nType, int cx, int cy) {
    CDialogEx::OnSize(nType, cx, cy);
    if (cx <= 0 || cy <= 0) return;

    CWnd* pAdminBtn = GetDlgItem(IDC_BTN_NAV_ADMIN);
    CWnd* pExitBtn = GetDlgItem(IDC_BTN_NAV_EXIT);
    if (!pAdminBtn || !pExitBtn || !pAdminBtn->GetSafeHwnd() || !pExitBtn->GetSafeHwnd())
        return;

    int adminW = (int)(cx * 0.22f);
    if (adminW < 260) adminW = 260;
    if (adminW > 380) adminW = 380;
    int adminH = (int)(adminW * (250.0f / 749.0f));
    if (adminH < 65) adminH = 65;
    if (adminH > 95) adminH = 95;

    int exitW = (int)(adminW * 0.55f);
    if (exitW < 140) exitW = 140;
    if (exitW > 220) exitW = 220;
    int exitH = (int)(exitW * (176.0f / 496.0f));
    if (exitH < 44) exitH = 44;
    if (exitH > 65) exitH = 65;

    int totalButtonsH = adminH + exitH + 16;

    int adminY = (int)(cy * 0.72f);
    if (adminY + totalButtonsH > cy - 20) {
        adminY = cy - totalButtonsH - 25;
    }
    int adminX = (cx - adminW) / 2;

    int exitX = (cx - exitW) / 2;
    int exitY = adminY + adminH + 16;

    pAdminBtn->MoveWindow(adminX, adminY, adminW, adminH, TRUE);
    pExitBtn->MoveWindow(exitX, exitY, exitW, exitH, TRUE);

    Invalidate(FALSE);
}
void CNexoraCinemaDlg::OnBnClickedBtnNavAdmin() {
    CAdminLoginDlg loginDlg(this);
    if (loginDlg.DoModal() != IDOK) return;
    CAdminMovieManagerDlg dlg(this);
    ShowWindow(SW_HIDE);
    dlg.DoModal();
    ShowWindow(SW_SHOWMAXIMIZED);
}
void CNexoraCinemaDlg::OnBnClickedBtnNavExit() {
    EndDialog(IDOK);
}
