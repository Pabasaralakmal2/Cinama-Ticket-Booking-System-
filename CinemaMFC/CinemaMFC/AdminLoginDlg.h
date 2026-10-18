#pragma once
#include <afxwin.h>
#include <afxdialogex.h>
#include "resource.h"
#include <gdiplus.h>
#include <string>
class CAdminLoginDlg : public CDialogEx {
public:
    CAdminLoginDlg(CWnd* pParent = nullptr);
    virtual ~CAdminLoginDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ADMIN_LOGIN_DIALOG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedBtnLogin();
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    CEdit m_EditEmail;
    CEdit m_EditPassword;
    Gdiplus::Bitmap* m_pTheaterBgBitmap;
    Gdiplus::Bitmap* m_pLoginBtnBitmap;
    Gdiplus::Bitmap* m_pCancelBtnBitmap;
    int m_HoveredBtn;
    DECLARE_MESSAGE_MAP()
};
