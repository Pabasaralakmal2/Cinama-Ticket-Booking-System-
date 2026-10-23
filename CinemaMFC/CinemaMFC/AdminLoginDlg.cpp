#include "pch.h"
#include "AdminLoginDlg.h"
#include "NexoraUI.h"
using namespace Gdiplus;

BEGIN_MESSAGE_MAP(CAdminLoginDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_DRAWITEM()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BTN_ADMIN_LOGIN, &CAdminLoginDlg::OnBnClickedBtnLogin)
END_MESSAGE_MAP()

CAdminLoginDlg::CAdminLoginDlg(CWnd* pParent)
    : CDialogEx(IDD_ADMIN_LOGIN_DIALOG, pParent)
    , m_pTheaterBgBitmap(nullptr)
    , m_pLoginBtnBitmap(nullptr)
    , m_pCancelBtnBitmap(nullptr)
    , m_HoveredBtn(-1) {
}

CAdminLoginDlg::~CAdminLoginDlg() {
    delete m_pLoginBtnBitmap;
    delete m_pCancelBtnBitmap;
    delete m_pTheaterBgBitmap;
}

void CAdminLoginDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TXT_ADMIN_EMAIL, m_EditEmail);
    DDX_Control(pDX, IDC_TXT_ADMIN_PASS, m_EditPassword);
}

BOOL CAdminLoginDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    SetWindowText(_T("Admin Login - Nexora Dolby Cinema"));

    WCHAR exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring basePath = exePath;
    size_t lastSlash = basePath.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos)
        basePath = basePath.substr(0, lastSlash + 1);

    m_pTheaterBgBitmap = NexoraUI::LoadTheaterBg();

    m_pLoginBtnBitmap = Bitmap::FromFile((basePath + L"btn_login.png").c_str());
    if (!m_pLoginBtnBitmap || m_pLoginBtnBitmap->GetLastStatus() != Ok) {
        delete m_pLoginBtnBitmap;
        m_pLoginBtnBitmap = Bitmap::FromFile((basePath + L"assets\\btn_login.png").c_str());
    }

    m_pCancelBtnBitmap = Bitmap::FromFile((basePath + L"btn_cancel.png").c_str());
    if (!m_pCancelBtnBitmap || m_pCancelBtnBitmap->GetLastStatus() != Ok) {
        delete m_pCancelBtnBitmap;
        m_pCancelBtnBitmap = Bitmap::FromFile((basePath + L"assets\\btn_cancel.png").c_str());
    }

    NexoraUI::SetupCustomButton(this, IDC_BTN_ADMIN_LOGIN);
    NexoraUI::SetupCustomButton(this, IDCANCEL);

    TRACKMOUSEEVENT tme = { sizeof(tme), TME_HOVER | TME_LEAVE, GetSafeHwnd(), 1 };
    TrackMouseEvent(&tme);

    CenterWindow();
    return TRUE;
}

BOOL CAdminLoginDlg::OnEraseBkgnd(CDC* ) {
    return TRUE;
}

void CAdminLoginDlg::OnPaint() {
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);
    int W = rect.Width(), H = rect.Height();
    Graphics g(dc.GetSafeHdc());

    NexoraUI::DrawGlassBackground(g, m_pTheaterBgBitmap, W, H, 200);

    NexoraUI::DrawGoldSeparator(g, 0, 0, W);

    Pen goldTrim(Color(100, 218, 165, 32), 1.0f);
    g.DrawRectangle(&goldTrim, 5.0f, 5.0f, (REAL)(W - 10), (REAL)(H - 10));
}

HBRUSH CAdminLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_DLG) {
        pDC->SetTextColor(RGB(228, 193, 136));
        pDC->SetBkColor(RGB(18, 18, 20));
        pDC->SetBkMode(TRANSPARENT);
        static CBrush s_brush(RGB(18, 18, 20));
        return s_brush;
    }
    return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAdminLoginDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) {
    bool hovered = (nIDCtl == m_HoveredBtn);
    Bitmap* pBmp = (nIDCtl == IDC_BTN_ADMIN_LOGIN) ? m_pLoginBtnBitmap
                 : (nIDCtl == IDCANCEL)            ? m_pCancelBtnBitmap
                 : nullptr;

    if (pBmp && pBmp->GetLastStatus() == Ok) {
        NexoraUI::DrawImageButton(lpDIS, pBmp, hovered);
    } else {
        CDialogEx::OnDrawItem(nIDCtl, lpDIS);
    }
}

void CAdminLoginDlg::OnMouseMove(UINT nFlags, CPoint point) {
    int btnIds[] = { IDC_BTN_ADMIN_LOGIN, IDCANCEL };
    int newHover = -1;
    for (int id : btnIds) {
        CWnd* pBtn = GetDlgItem(id);
        if (pBtn) {
            CRect r; pBtn->GetWindowRect(&r); ScreenToClient(&r);
            if (r.PtInRect(point)) { newHover = id; break; }
        }
    }
    if (newHover != m_HoveredBtn) {
        int oldHover = m_HoveredBtn;
        m_HoveredBtn = newHover;
        for (int id : btnIds) {
            CWnd* p = GetDlgItem(id);
            if (p) p->Invalidate(FALSE);
        }
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

void CAdminLoginDlg::OnBnClickedBtnLogin() {
    CString email, password;
    m_EditEmail.GetWindowText(email);
    m_EditPassword.GetWindowText(password);
    if (email == _T("pabasaralakmal275@gmail.com") && password == _T("2004")) {
        EndDialog(IDOK);
    } else {
        AfxMessageBox(_T("Access Denied!\n\nYou do not have admin rights.\nPlease enter valid admin credentials."),
                      MB_OK | MB_ICONERROR);
    }
}
