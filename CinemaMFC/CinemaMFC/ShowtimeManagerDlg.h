#pragma once
#include <afxwin.h>
#include <afxcmn.h>
#include <afxdialogex.h>
#include "resource.h"
#include "ShowtimeRepository.h"
#include "MovieRepository.h"
#include <gdiplus.h>
class CShowtimeManagerDlg : public CDialogEx {
public:
    CShowtimeManagerDlg(CWnd* pParent = nullptr);
    virtual ~CShowtimeManagerDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SHOWTIME_MGR_DIALOG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
protected:
    CListCtrl m_ShowtimeList;
    ShowtimeRepository m_ShowRepo;
    MovieRepository m_MovieRepo;
    CComboBox m_ComboMovie;
    CComboBox m_ComboHall;
    CComboBox m_ComboFormat;
    CEdit m_EditDate;
    CEdit m_EditTime;
    CEdit m_EditPrice;
    Gdiplus::Bitmap* m_pAddBtnBitmap;
    Gdiplus::Bitmap* m_pDeleteBtnBitmap;
    Gdiplus::Bitmap* m_pTheaterBgBitmap;
    int m_HoveredBtn;
    void DrawCustomButton(LPDRAWITEMSTRUCT lpDIS, Gdiplus::Bitmap* pBmp, int nCtlId);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedBtnAddShowtime();
    afx_msg void OnBnClickedBtnDeleteShowtime();
    afx_msg void OnCbnSelchangeComboHall(); 
    void RefreshShowtimeList();
    void LoadComboBoxes();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelchangeCmbFormat();
};
