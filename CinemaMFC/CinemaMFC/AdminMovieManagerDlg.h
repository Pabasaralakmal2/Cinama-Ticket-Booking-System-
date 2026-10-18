#pragma once
#include <afxwin.h>
#include <afxcmn.h>
#include <afxdialogex.h>
#include "resource.h"
#include "MovieRepository.h"
#include <gdiplus.h>

class CAdminMovieManagerDlg : public CDialogEx {
public:
    CAdminMovieManagerDlg(CWnd* pParent = nullptr);
    virtual ~CAdminMovieManagerDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ADMIN_MOVIE_MGR_DIALOG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    CListCtrl    m_MovieList;
    MovieRepository m_Repo;
    CEdit        m_EditTitle;
    CEdit        m_EditGenre;
    CEdit        m_EditDuration;
    CEdit        m_EditTrailerUrl;
    CEdit        m_EditPosterUrl;
    CEdit        m_EditDescription;
    CComboBox    m_ComboFormat;
    CComboBox    m_ComboStatus;

    // One bitmap per custom button
    enum { BTN_ADD = 0, BTN_UPDATE, BTN_DELETE, BTN_SHOWTIME, BTN_COUNT };
    Gdiplus::Bitmap* m_pBtnBitmap[BTN_COUNT];
    Gdiplus::Bitmap* m_pTheaterBgBitmap;
    int          m_HoveredBtn;   // stores the control ID of hovered btn, or -1

    std::wstring GetExeDir();
    Gdiplus::Bitmap* LoadAssetBitmap(const std::wstring& fileName);
    void DrawCustomButton(LPDRAWITEMSTRUCT lpDIS, int bitmapIndex);

    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedBtnAdd();
    afx_msg void OnBnClickedBtnUpdate();
    afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedBtnManageShowtimes();
    afx_msg void OnLvnItemchangedMovieList(NMHDR *pNMHDR, LRESULT *pResult);
    void RefreshMovieList();
    DECLARE_MESSAGE_MAP()
};

