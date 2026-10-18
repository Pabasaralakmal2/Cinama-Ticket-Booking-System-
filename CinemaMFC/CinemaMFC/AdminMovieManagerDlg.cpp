#include "pch.h"
#include "AdminMovieManagerDlg.h"
#include "ShowtimeManagerDlg.h"
#include "NexoraUI.h"
#include <string>

BEGIN_MESSAGE_MAP(CAdminMovieManagerDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_DRAWITEM()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BTN_ADD_MOVIE,    &CAdminMovieManagerDlg::OnBnClickedBtnAdd)
    ON_BN_CLICKED(IDC_BTN_EDIT_MOVIE,   &CAdminMovieManagerDlg::OnBnClickedBtnUpdate)
    ON_BN_CLICKED(IDC_BTN_DELETE_MOVIE, &CAdminMovieManagerDlg::OnBnClickedBtnDelete)
    ON_BN_CLICKED(IDC_BTN_MGR_SHOWTIMES,&CAdminMovieManagerDlg::OnBnClickedBtnManageShowtimes)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_MOVIE_LIST, &CAdminMovieManagerDlg::OnLvnItemchangedMovieList)
END_MESSAGE_MAP()

// Constructor / Destructor
CAdminMovieManagerDlg::CAdminMovieManagerDlg(CWnd* pParent)
    : CDialogEx(IDD_ADMIN_MOVIE_MGR_DIALOG, pParent)
    , m_pTheaterBgBitmap(nullptr)
    , m_HoveredBtn(-1)
{
    for (int i = 0; i < BTN_COUNT; ++i)
        m_pBtnBitmap[i] = nullptr;
}

CAdminMovieManagerDlg::~CAdminMovieManagerDlg()
{
    for (int i = 0; i < BTN_COUNT; ++i)
        delete m_pBtnBitmap[i];
    delete m_pTheaterBgBitmap;
}

// Helpers
std::wstring CAdminMovieManagerDlg::GetExeDir()
{
    WCHAR exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path = exePath;
    size_t pos = path.find_last_of(L"\\");
    return (pos != std::wstring::npos) ? path.substr(0, pos + 1) : L"";
}

Gdiplus::Bitmap* CAdminMovieManagerDlg::LoadAssetBitmap(const std::wstring& fileName)
{
    std::wstring base = GetExeDir();
    Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromFile((base + fileName).c_str());
    if (!bmp || bmp->GetLastStatus() != Gdiplus::Ok) {
        delete bmp;
        bmp = Gdiplus::Bitmap::FromFile((base + L"assets\\" + fileName).c_str());
    }
    if (bmp && bmp->GetLastStatus() != Gdiplus::Ok) {
        delete bmp; bmp = nullptr;
    }
    return bmp;
}

// Data Exchange
void CAdminMovieManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MOVIE_LIST,       m_MovieList);
    DDX_Control(pDX, IDC_TXT_TITLE,        m_EditTitle);
    DDX_Control(pDX, IDC_TXT_GENRE,        m_EditGenre);
    DDX_Control(pDX, IDC_TXT_DURATION,     m_EditDuration);
    DDX_Control(pDX, IDC_TXT_TRAILER_URL,  m_EditTrailerUrl);
    DDX_Control(pDX, IDC_TXT_POSTER_URL,   m_EditPosterUrl);
    DDX_Control(pDX, IDC_TXT_DESCRIPTION,  m_EditDescription);
    DDX_Control(pDX, IDC_CMB_MOVIE_FORMAT, m_ComboFormat);
    DDX_Control(pDX, IDC_CMB_MOVIE_STATUS, m_ComboStatus);
}

// OnInitDialog
BOOL CAdminMovieManagerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetWindowText(_T("Nexora Dolby Cinema - Movie Management"));

    m_MovieList.InsertColumn(0, _T("ID"),          LVCFMT_LEFT, 40);
    m_MovieList.InsertColumn(1, _T("Title"),       LVCFMT_LEFT, 130);
    m_MovieList.InsertColumn(2, _T("Genre"),       LVCFMT_LEFT, 100);
    m_MovieList.InsertColumn(3, _T("Duration"),    LVCFMT_LEFT, 60);
    m_MovieList.InsertColumn(4, _T("Format"),      LVCFMT_LEFT, 55);
    m_MovieList.InsertColumn(5, _T("Status"),      LVCFMT_LEFT, 90);
    m_MovieList.InsertColumn(6, _T("Trailer URL"), LVCFMT_LEFT, 170);
    m_MovieList.InsertColumn(7, _T("Poster URL"),  LVCFMT_LEFT, 160);
    m_MovieList.InsertColumn(8, _T("Description"), LVCFMT_LEFT, 190);
    m_MovieList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
    m_MovieList.SetBkColor(RGB(8, 18, 45));
    m_MovieList.SetTextBkColor(RGB(8, 18, 45));
    m_MovieList.SetTextColor(RGB(220, 235, 255));

    m_ComboFormat.AddString(_T("2D"));
    m_ComboFormat.AddString(_T("3D"));
    m_ComboFormat.AddString(_T("IMAX"));
    m_ComboFormat.SetCurSel(0);

    m_ComboStatus.AddString(_T("Now Showing"));
    m_ComboStatus.AddString(_T("Coming Soon"));
    m_ComboStatus.AddString(_T("Ended"));
    m_ComboStatus.SetCurSel(0);

    m_pTheaterBgBitmap = NexoraUI::LoadMovieManagerBg();

    m_pBtnBitmap[BTN_ADD]      = LoadAssetBitmap(L"btn_add_movie.png");
    m_pBtnBitmap[BTN_UPDATE]   = LoadAssetBitmap(L"btn_update_movie.png");
    m_pBtnBitmap[BTN_DELETE]   = LoadAssetBitmap(L"btn_delete_movie.png");
    m_pBtnBitmap[BTN_SHOWTIME] = LoadAssetBitmap(L"btn_manage_showtime.png");

    NexoraUI::SetupCustomButton(this, IDC_BTN_ADD_MOVIE);
    NexoraUI::SetupCustomButton(this, IDC_BTN_EDIT_MOVIE);
    NexoraUI::SetupCustomButton(this, IDC_BTN_DELETE_MOVIE);
    NexoraUI::SetupCustomButton(this, IDC_BTN_MGR_SHOWTIMES);

    TRACKMOUSEEVENT tme = { sizeof(tme), TME_HOVER | TME_LEAVE, GetSafeHwnd(), 1 };
    TrackMouseEvent(&tme);

    RefreshMovieList();
    CenterWindow();
    return TRUE;
}

BOOL CAdminMovieManagerDlg::OnEraseBkgnd(CDC*) { return TRUE; }

// RefreshMovieList
void CAdminMovieManagerDlg::RefreshMovieList()
{
    m_MovieList.DeleteAllItems();
    std::vector<Movie> movies = m_Repo.GetAllMovies();
    int i = 0;
    for (const auto& movie : movies) {
        CString idStr;
        idStr.Format(_T("%d"), movie.GetMovieId());
        m_MovieList.InsertItem(i, idStr);
        m_MovieList.SetItemText(i, 1, CString(movie.GetTitle().c_str()));
        m_MovieList.SetItemText(i, 2, CString(movie.GetGenre().c_str()));
        m_MovieList.SetItemText(i, 3, CString(movie.GetDuration().c_str()));
        m_MovieList.SetItemText(i, 4, CString(movie.GetFormat().c_str()));
        m_MovieList.SetItemText(i, 5, CString(movie.GetStatus().c_str()));
        m_MovieList.SetItemText(i, 6, CString(movie.GetTrailerUrl().c_str()));
        m_MovieList.SetItemText(i, 7, CString(movie.GetPosterUrl().c_str()));
        m_MovieList.SetItemText(i, 8, CString(movie.GetDescription().c_str()));
        i++;
    }
}

// Button Handlers
void CAdminMovieManagerDlg::OnBnClickedBtnAdd()
{
    CString title, genre, duration, trailerUrl, posterUrl, description;
    m_EditTitle.GetWindowText(title);
    m_EditGenre.GetWindowText(genre);
    m_EditDuration.GetWindowText(duration);
    m_EditTrailerUrl.GetWindowText(trailerUrl);
    m_EditPosterUrl.GetWindowText(posterUrl);
    m_EditDescription.GetWindowText(description);
    if (title.IsEmpty()) { AfxMessageBox(_T("Title cannot be empty.")); return; }
    CString format, status;
    int selFmt = m_ComboFormat.GetCurSel();
    if (selFmt != CB_ERR) m_ComboFormat.GetLBText(selFmt, format); else format = _T("2D");
    int selStatus = m_ComboStatus.GetCurSel();
    if (selStatus != CB_ERR) m_ComboStatus.GetLBText(selStatus, status); else status = _T("Now Showing");
    Movie newMovie;
    newMovie.SetTitle(std::string(CT2CA(title)));
    newMovie.SetGenre(std::string(CT2CA(genre)));
    newMovie.SetDuration(std::string(CT2CA(duration)));
    newMovie.SetTrailerUrl(std::string(CT2CA(trailerUrl)));
    newMovie.SetPosterUrl(std::string(CT2CA(posterUrl)));
    newMovie.SetDescription(std::string(CT2CA(description)));
    newMovie.SetFormat(std::string(CT2CA(format)));
    newMovie.SetStatus(std::string(CT2CA(status)));
    if (m_Repo.AddMovie(newMovie)) {
        AfxMessageBox(_T("Movie Added Successfully!")); RefreshMovieList();
    } else { AfxMessageBox(_T("Failed to add movie.")); }
}

void CAdminMovieManagerDlg::OnBnClickedBtnUpdate()
{
    POSITION pos = m_MovieList.GetFirstSelectedItemPosition();
    if (pos == NULL) { AfxMessageBox(_T("Please select a movie to update.")); return; }
    int nItem   = m_MovieList.GetNextSelectedItem(pos);
    int movieId = _ttoi(m_MovieList.GetItemText(nItem, 0));
    CString title, genre, duration, trailerUrl, posterUrl, description;
    m_EditTitle.GetWindowText(title);
    m_EditGenre.GetWindowText(genre);
    m_EditDuration.GetWindowText(duration);
    m_EditTrailerUrl.GetWindowText(trailerUrl);
    m_EditPosterUrl.GetWindowText(posterUrl);
    m_EditDescription.GetWindowText(description);
    CString format, status;
    int selFmt = m_ComboFormat.GetCurSel();
    if (selFmt != CB_ERR) m_ComboFormat.GetLBText(selFmt, format); else format = _T("2D");
    int selStatus = m_ComboStatus.GetCurSel();
    if (selStatus != CB_ERR) m_ComboStatus.GetLBText(selStatus, status); else status = _T("Now Showing");
    auto moviePtr = m_Repo.GetMovieById(movieId);
    if (moviePtr) {
        moviePtr->SetTitle(std::string(CT2CA(title)));
        moviePtr->SetGenre(std::string(CT2CA(genre)));
        moviePtr->SetDuration(std::string(CT2CA(duration)));
        moviePtr->SetTrailerUrl(std::string(CT2CA(trailerUrl)));
        moviePtr->SetPosterUrl(std::string(CT2CA(posterUrl)));
        moviePtr->SetDescription(std::string(CT2CA(description)));
        moviePtr->SetFormat(std::string(CT2CA(format)));
        moviePtr->SetStatus(std::string(CT2CA(status)));
        if (m_Repo.UpdateMovie(*moviePtr)) {
            AfxMessageBox(_T("Movie Updated Successfully!")); RefreshMovieList();
        } else { AfxMessageBox(_T("Failed to update movie.")); }
    }
}

void CAdminMovieManagerDlg::OnBnClickedBtnDelete()
{
    POSITION pos = m_MovieList.GetFirstSelectedItemPosition();
    if (pos == NULL) return;
    int nItem   = m_MovieList.GetNextSelectedItem(pos);
    int movieId = _ttoi(m_MovieList.GetItemText(nItem, 0));
    if (MessageBox(_T("Are you sure you want to delete this movie?"),
        _T("Confirm Delete"), MB_YESNO | MB_ICONWARNING) == IDYES) {
        if (m_Repo.DeleteMovie(movieId)) RefreshMovieList();
    }
}

void CAdminMovieManagerDlg::OnBnClickedBtnManageShowtimes()
{
    CShowtimeManagerDlg dlg(this);
    this->ShowWindow(SW_HIDE);
    dlg.DoModal();
    this->ShowWindow(SW_SHOW);
}

// List selection -> fill edit fields
void CAdminMovieManagerDlg::OnLvnItemchangedMovieList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED)) {
        int movieId = _ttoi(m_MovieList.GetItemText(pNMLV->iItem, 0));
        auto moviePtr = m_Repo.GetMovieById(movieId);
        if (moviePtr) {
            m_EditTitle.SetWindowText(CString(moviePtr->GetTitle().c_str()));
            m_EditGenre.SetWindowText(CString(moviePtr->GetGenre().c_str()));
            m_EditDuration.SetWindowText(CString(moviePtr->GetDuration().c_str()));
            m_EditTrailerUrl.SetWindowText(CString(moviePtr->GetTrailerUrl().c_str()));
            m_EditPosterUrl.SetWindowText(CString(moviePtr->GetPosterUrl().c_str()));
            m_EditDescription.SetWindowText(CString(moviePtr->GetDescription().c_str()));
            int idxFmt = m_ComboFormat.FindStringExact(-1, CString(moviePtr->GetFormat().c_str()));
            if (idxFmt != CB_ERR) m_ComboFormat.SetCurSel(idxFmt);
            int idxStatus = m_ComboStatus.FindStringExact(-1, CString(moviePtr->GetStatus().c_str()));
            if (idxStatus != CB_ERR) m_ComboStatus.SetCurSel(idxStatus);
        }
    }
    *pResult = 0;
}

// OnPaint: background + glass overlay inside the golden box
// Box region in bg image: left=3%, top=26%, right=98%, bottom=77%
void CAdminMovieManagerDlg::OnPaint()
{
    CPaintDC dc(this);
    CRect clientRect;
    GetClientRect(&clientRect);
    int W = clientRect.Width();
    int H = clientRect.Height();

    Gdiplus::Graphics g(dc.GetSafeHdc());
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

    // 1. Draw full background (cover-fit)
    NexoraUI::DrawTheaterBackground(g, m_pTheaterBgBitmap, W, H);

    // 2. Compute golden box region in client pixels
    Gdiplus::REAL boxLeft   = W * 0.030f;
    Gdiplus::REAL boxTop    = H * 0.260f;
    Gdiplus::REAL boxRight  = W * 0.980f;
    Gdiplus::REAL boxBottom = H * 0.770f;
    Gdiplus::REAL boxW      = boxRight - boxLeft;
    Gdiplus::REAL boxH      = boxBottom - boxTop;
    Gdiplus::RectF boxRect(boxLeft, boxTop, boxW, boxH);

    // 3. Dark navy glass fill inside the box
    Gdiplus::SolidBrush glassBrush(Gdiplus::Color(195, 6, 10, 28));
    g.FillRectangle(&glassBrush, boxRect);

    // 4. Gold separator between list and form (~60% of box height)
    Gdiplus::REAL divY = boxTop + boxH * 0.605f;
    NexoraUI::DrawGoldSeparator(g, (int)boxLeft + 4, (int)divY, (int)boxRight - 4);

    // 5. Subtle vignette outside the box
    Gdiplus::SolidBrush vignette(Gdiplus::Color(100, 2, 4, 12));
    g.FillRectangle(&vignette, 0.0f, 0.0f, (Gdiplus::REAL)W, boxTop);
    g.FillRectangle(&vignette, 0.0f, boxBottom, (Gdiplus::REAL)W, (Gdiplus::REAL)H - boxBottom);
}

HBRUSH CAdminMovieManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetTextColor(RGB(228, 193, 136));
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    if (nCtlColor == CTLCOLOR_EDIT) {
        pDC->SetTextColor(RGB(240, 245, 255));
        pDC->SetBkColor(RGB(8, 14, 38));
        static CBrush s_editBrush(RGB(8, 14, 38));
        return s_editBrush;
    }
    if (nCtlColor == CTLCOLOR_LISTBOX) {
        pDC->SetTextColor(RGB(220, 235, 255));
        pDC->SetBkColor(RGB(8, 14, 38));
        static CBrush s_lbBrush(RGB(8, 14, 38));
        return s_lbBrush;
    }
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_BTN) {
        static CBrush s_dlgBrush(RGB(6, 10, 28));
        return s_dlgBrush;
    }
    return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

// Custom button drawing
void CAdminMovieManagerDlg::DrawCustomButton(LPDRAWITEMSTRUCT lpDIS, int bitmapIndex)
{
    bool hovered = (lpDIS->CtlID == (UINT)m_HoveredBtn);
    NexoraUI::DrawImageButton(lpDIS, m_pBtnBitmap[bitmapIndex], hovered);
}

void CAdminMovieManagerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)
{
    switch (nIDCtl) {
    case IDC_BTN_ADD_MOVIE:     DrawCustomButton(lpDIS, BTN_ADD);      break;
    case IDC_BTN_EDIT_MOVIE:    DrawCustomButton(lpDIS, BTN_UPDATE);   break;
    case IDC_BTN_DELETE_MOVIE:  DrawCustomButton(lpDIS, BTN_DELETE);   break;
    case IDC_BTN_MGR_SHOWTIMES: DrawCustomButton(lpDIS, BTN_SHOWTIME); break;
    default: CDialogEx::OnDrawItem(nIDCtl, lpDIS); break;
    }
}

// Mouse hover tracking
void CAdminMovieManagerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    static const int btnIds[] = {
        IDC_BTN_ADD_MOVIE, IDC_BTN_EDIT_MOVIE,
        IDC_BTN_DELETE_MOVIE, IDC_BTN_MGR_SHOWTIMES
    };
    int newHover = -1;
    for (int id : btnIds) {
        CWnd* pBtn = GetDlgItem(id);
        if (pBtn) {
            CRect r;
            pBtn->GetWindowRect(&r);
            ScreenToClient(&r);
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
