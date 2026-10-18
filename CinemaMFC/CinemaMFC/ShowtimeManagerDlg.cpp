#include "pch.h"
#include "ShowtimeManagerDlg.h"
#include "NexoraUI.h"

BEGIN_MESSAGE_MAP(CShowtimeManagerDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_DRAWITEM()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BTN_ADD_SHOWTIME, &CShowtimeManagerDlg::OnBnClickedBtnAddShowtime)
    ON_BN_CLICKED(IDC_BTN_DELETE_SHOWTIME, &CShowtimeManagerDlg::OnBnClickedBtnDeleteShowtime)
    ON_CBN_SELCHANGE(IDC_CMB_HALL, &CShowtimeManagerDlg::OnCbnSelchangeComboHall)
    ON_CBN_SELCHANGE(IDC_CMB_FORMAT, &CShowtimeManagerDlg::OnCbnSelchangeCmbFormat)
END_MESSAGE_MAP()

CShowtimeManagerDlg::CShowtimeManagerDlg(CWnd* pParent)
    : CDialogEx(IDD_SHOWTIME_MGR_DIALOG, pParent)
    , m_pAddBtnBitmap(nullptr)
    , m_pDeleteBtnBitmap(nullptr)
    , m_pTheaterBgBitmap(nullptr)
    , m_HoveredBtn(-1) {
}

CShowtimeManagerDlg::~CShowtimeManagerDlg() {
    delete m_pAddBtnBitmap;
    delete m_pDeleteBtnBitmap;
    delete m_pTheaterBgBitmap;
}

void CShowtimeManagerDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SHOWTIME_LIST, m_ShowtimeList);
    DDX_Control(pDX, IDC_CMB_MOVIE, m_ComboMovie);
    DDX_Control(pDX, IDC_CMB_HALL, m_ComboHall);
    DDX_Control(pDX, IDC_CMB_FORMAT, m_ComboFormat);
    DDX_Control(pDX, IDC_TXT_DATE, m_EditDate);
    DDX_Control(pDX, IDC_TXT_TIME, m_EditTime);
    DDX_Control(pDX, IDC_TXT_PRICE, m_EditPrice);
}

BOOL CShowtimeManagerDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    SetWindowText(_T("Nexora Dolby Cinema - Showtime Management"));

    m_ShowtimeList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 60);
    m_ShowtimeList.InsertColumn(1, _T("Movie ID"), LVCFMT_LEFT, 90);
    m_ShowtimeList.InsertColumn(2, _T("Hall"), LVCFMT_LEFT, 80);
    m_ShowtimeList.InsertColumn(3, _T("Date"), LVCFMT_LEFT, 120);
    m_ShowtimeList.InsertColumn(4, _T("Time"), LVCFMT_LEFT, 100);
    m_ShowtimeList.InsertColumn(5, _T("Format"), LVCFMT_LEFT, 80);
    m_ShowtimeList.InsertColumn(6, _T("Price (LKR)"), LVCFMT_LEFT, 110);
    m_ShowtimeList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    WCHAR exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring basePath = exePath;
    size_t lastSlash = basePath.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos)
        basePath = basePath.substr(0, lastSlash + 1);

    m_pTheaterBgBitmap = NexoraUI::LoadTheaterBg();

    m_pAddBtnBitmap = Gdiplus::Bitmap::FromFile((basePath + L"btn_add_showtime.png").c_str());
    if (!m_pAddBtnBitmap || m_pAddBtnBitmap->GetLastStatus() != Gdiplus::Ok) {
        delete m_pAddBtnBitmap;
        m_pAddBtnBitmap = Gdiplus::Bitmap::FromFile((basePath + L"assets\\btn_add_showtime.png").c_str());
    }

    m_pDeleteBtnBitmap = Gdiplus::Bitmap::FromFile((basePath + L"btn_delete_showtime.png").c_str());
    if (!m_pDeleteBtnBitmap || m_pDeleteBtnBitmap->GetLastStatus() != Gdiplus::Ok) {
        delete m_pDeleteBtnBitmap;
        m_pDeleteBtnBitmap = Gdiplus::Bitmap::FromFile((basePath + L"assets\\btn_delete_showtime.png").c_str());
    }

    // Configure buttons to eliminate white borders completely
    NexoraUI::SetupCustomButton(this, IDC_BTN_ADD_SHOWTIME);
    NexoraUI::SetupCustomButton(this, IDC_BTN_DELETE_SHOWTIME);

    TRACKMOUSEEVENT tme = { sizeof(tme), TME_HOVER | TME_LEAVE, GetSafeHwnd(), 1 };
    TrackMouseEvent(&tme);

    LoadComboBoxes();
    RefreshShowtimeList();

    CenterWindow();
    return TRUE;
}

BOOL CShowtimeManagerDlg::OnEraseBkgnd(CDC* ) {
    return TRUE;
}

void CShowtimeManagerDlg::LoadComboBoxes() {
    std::vector<Movie> movies = m_MovieRepo.GetAllMovies();
    for (const auto& m : movies) {
        int index = m_ComboMovie.AddString(CString(m.GetTitle().c_str()));
        m_ComboMovie.SetItemData(index, m.GetMovieId());
    }
    m_ComboHall.AddString(_T("C1"));
    m_ComboHall.AddString(_T("C2"));
    m_ComboHall.AddString(_T("C3"));
    m_ComboFormat.AddString(_T("2D"));
    m_ComboFormat.AddString(_T("3D"));
}

void CShowtimeManagerDlg::RefreshShowtimeList() {
    m_ShowtimeList.DeleteAllItems();
    std::vector<Showtime> shows = m_ShowRepo.GetAllShowtimes();
    int i = 0;
    for (const auto& s : shows) {
        CString idStr, mIdStr, priceStr;
        idStr.Format(_T("%d"), s.GetShowtimeId());
        mIdStr.Format(_T("%d"), s.GetMovieId());
        priceStr.Format(_T("%.2f"), s.GetTicketPrice());
        m_ShowtimeList.InsertItem(i, idStr);
        m_ShowtimeList.SetItemText(i, 1, mIdStr);
        m_ShowtimeList.SetItemText(i, 2, CString(s.GetHallId().c_str()));
        m_ShowtimeList.SetItemText(i, 3, CString(s.GetShowDate().c_str()));
        m_ShowtimeList.SetItemText(i, 4, CString(s.GetStartTime().c_str()));
        m_ShowtimeList.SetItemText(i, 5, CString(s.GetFormat().c_str()));
        m_ShowtimeList.SetItemText(i, 6, priceStr);
        i++;
    }
}

void CShowtimeManagerDlg::OnCbnSelchangeComboHall() {
    int selIndex = m_ComboHall.GetCurSel();
    if (selIndex != CB_ERR) {
        CString hall;
        m_ComboHall.GetLBText(selIndex, hall);
        m_ComboFormat.ResetContent();
        m_ComboFormat.AddString(_T("2D"));
        if (hall != _T("C3")) {
            m_ComboFormat.AddString(_T("3D"));
        }
        m_ComboFormat.SetCurSel(0);
    }
}

void CShowtimeManagerDlg::OnBnClickedBtnAddShowtime() {
    int mIndex = m_ComboMovie.GetCurSel();
    int hIndex = m_ComboHall.GetCurSel();
    int fIndex = m_ComboFormat.GetCurSel();
    if (mIndex == CB_ERR || hIndex == CB_ERR || fIndex == CB_ERR) {
        AfxMessageBox(_T("Please select Movie, Hall, and Format."));
        return;
    }
    CString hall, format, date, time, priceStr;
    m_ComboHall.GetLBText(hIndex, hall);
    m_ComboFormat.GetLBText(fIndex, format);
    m_EditDate.GetWindowText(date);
    m_EditTime.GetWindowText(time);
    m_EditPrice.GetWindowText(priceStr);
    int movieId = static_cast<int>(m_ComboMovie.GetItemData(mIndex));
    double price = _ttof(priceStr);
    Showtime s;
    s.SetMovieId(movieId);
    s.SetHallId(std::string(CT2CA(hall)));
    s.SetFormat(std::string(CT2CA(format)));
    s.SetShowDate(std::string(CT2CA(date)));
    s.SetStartTime(std::string(CT2CA(time)));
    s.SetTicketPrice(price);
    if (m_ShowRepo.AddShowtime(s)) {
        AfxMessageBox(_T("Showtime Added Successfully!"));
        RefreshShowtimeList();
    } else {
        AfxMessageBox(_T("Failed to add showtime. Check constraints."));
    }
}

void CShowtimeManagerDlg::OnBnClickedBtnDeleteShowtime() {
    POSITION pos = m_ShowtimeList.GetFirstSelectedItemPosition();
    if (pos == NULL) return;
    int nItem = m_ShowtimeList.GetNextSelectedItem(pos);
    CString idStr = m_ShowtimeList.GetItemText(nItem, 0);
    int showId = _ttoi(idStr);
    if (MessageBox(_T("Delete this showtime?"), _T("Confirm"), MB_YESNO) == IDYES) {
        m_ShowRepo.DeleteShowtime(showId);
        RefreshShowtimeList();
    }
}

void CShowtimeManagerDlg::OnPaint() {
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);
    int W = rect.Width(), H = rect.Height();
    Gdiplus::Graphics g(dc.GetSafeHdc());

    // 1) Transparent glass background using theater photo
    NexoraUI::DrawGlassBackground(g, m_pTheaterBgBitmap, W, H, 210);

    // 2) Gold top accent bar
    NexoraUI::DrawGoldSeparator(g, 0, 0, W);

    // 3) Glass panel behind the form area
    if (W > 50 && H > 240) {
        CRect listR;
        if (m_ShowtimeList.GetSafeHwnd()) {
            m_ShowtimeList.GetWindowRect(&listR);
            ScreenToClient(&listR);
            Gdiplus::RectF formPanel(15.0f, (Gdiplus::REAL)(listR.bottom + 8),
                                     (Gdiplus::REAL)(W - 30), (Gdiplus::REAL)(H - listR.bottom - 18));
            NexoraUI::DrawGlassPanel(g, formPanel, 50);
        }
    }
}

HBRUSH CShowtimeManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_DLG) {
        pDC->SetTextColor(RGB(228, 193, 136)); // Nexora gold text
        pDC->SetBkColor(RGB(18, 18, 20));
        pDC->SetBkMode(TRANSPARENT);
        static CBrush s_brush(RGB(18, 18, 20));
        return s_brush;
    }
    return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CShowtimeManagerDlg::DrawCustomButton(LPDRAWITEMSTRUCT lpDIS, Gdiplus::Bitmap* pBmp, int nCtlId) {
    bool hovered = (nCtlId == m_HoveredBtn);
    NexoraUI::DrawImageButton(lpDIS, pBmp, hovered);
}

void CShowtimeManagerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) {
    if (nIDCtl == IDC_BTN_ADD_SHOWTIME) {
        DrawCustomButton(lpDIS, m_pAddBtnBitmap, nIDCtl);
    } else if (nIDCtl == IDC_BTN_DELETE_SHOWTIME) {
        DrawCustomButton(lpDIS, m_pDeleteBtnBitmap, nIDCtl);
    } else {
        CDialogEx::OnDrawItem(nIDCtl, lpDIS);
    }
}

void CShowtimeManagerDlg::OnMouseMove(UINT nFlags, CPoint point) {
    int btnIds[] = { IDC_BTN_ADD_SHOWTIME, IDC_BTN_DELETE_SHOWTIME };
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
            if (id == newHover || id == oldHover) {
                CWnd* pBtn = GetDlgItem(id);
                if (pBtn) pBtn->Invalidate(FALSE);
            }
        }
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

void CShowtimeManagerDlg::OnCbnSelchangeCmbFormat() {
}
