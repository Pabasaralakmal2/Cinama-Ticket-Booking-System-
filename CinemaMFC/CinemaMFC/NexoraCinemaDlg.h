#pragma once
#include "resource.h"
#include <gdiplus.h>
class CNexoraCinemaDlg : public CDialogEx {
public:
    CNexoraCinemaDlg(CWnd* pParent = nullptr);
    ~CNexoraCinemaDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_NEXORACINEMA_DIALOG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedBtnNavAdmin();
    afx_msg void OnBnClickedBtnNavExit();
    DECLARE_MESSAGE_MAP()
private:
    void LoadLogo();
    HICON              m_hIcon;
    Gdiplus::Bitmap*   m_pLogoBitmap;
    Gdiplus::Bitmap*   m_pTheaterBgBitmap;
    Gdiplus::Bitmap*   m_pAdminBtnBitmap;
    Gdiplus::Bitmap*   m_pExitBtnBitmap;
    int m_HoveredBtn;
};
