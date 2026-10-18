#include "pch.h"
#include "NexoraCinemaApp.h"
#include "NexoraCinemaDlg.h"
#include "BookingRepository.h"
#include <gdiplus.h>

BEGIN_MESSAGE_MAP(CNexoraCinemaApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CNexoraCinemaApp::CNexoraCinemaApp() {}
CNexoraCinemaApp theApp;

BOOL CNexoraCinemaApp::InitInstance() {
    // Standard MFC initialization
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    AfxEnableControlContainer();

    // Set IE browser emulation for WebBrowser control (trailer dialog)
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER,
        _T("Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION"),
        0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD val = 11000;
        RegSetValueEx(hKey, _T("CinemaMFC.exe"), 0, REG_DWORD, (const BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
    }

    // Initialize GDI+ for custom painting
    Gdiplus::GdiplusStartupInput gdiplusInput;
    ULONG_PTR gdiplusToken = 0;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusInput, NULL);

    // Clean expired bookings from local storage
    BookingRepository::ClearOldBookings();

    SetRegistryKey(_T("Nexora Dolby Cinema App"));

    // Launch the main dialog
    CNexoraCinemaDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return FALSE;
}
