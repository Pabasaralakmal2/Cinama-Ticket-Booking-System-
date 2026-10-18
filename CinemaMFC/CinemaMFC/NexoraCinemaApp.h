#pragma once
#include <afxwin.h>
class CNexoraCinemaApp : public CWinApp {
public:
    CNexoraCinemaApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};
extern CNexoraCinemaApp theApp;
