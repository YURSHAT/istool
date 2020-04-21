#include "stdafx.h"
#include "DownloadDlg.h"

UINT		CDownloadDlg::m_nDialogID = IDD_DOWNLOAD;
CString		CDownload::m_strOptionTitle;
bool		CDownload::m_bOptionDebug;
CString		CDownload::m_strOptionSimple;
CString		CDownload::m_strOptionLabel;
CString		CDownload::m_strOptionDescription;
CString		CDownload::m_strOptionLanguage;
#ifdef USE_RESUME
bool		CDownload::m_bOptionResume = true;
#endif
CString		CDownload::m_strSmallWizardImage;

void CDownloadDlg::CleanupStatic() {
	ClearFiles();

	if(CDownloadDlg::m_hInternet) {
		inet.InternetCloseHandle(CDownloadDlg::m_hInternet);
		CDownloadDlg::m_hInternet = NULL;
	}
}

CDownloadDlg::CDownloadDlg(HWND hWndParent) : IDD(m_nDialogID), m_hWndParent(hWndParent) {
	m_bBusy = false;
	m_bThreadDone = false;
	if(m_strOptionTitle.IsEmpty())
		GetString(IDS_TITLE,m_strTitle);
	else
		m_strTitle = m_strOptionTitle;

	if(m_strOptionLabel.IsEmpty()) GetString(IDS_LABEL,m_strOptionLabel);
	if(m_strOptionDescription.IsEmpty()) GetString(IDS_DESCRIPTION,m_strOptionDescription);
	m_pSmallWizardImage = NULL;
	if(!m_strSmallWizardImage.IsEmpty()) {
		// "F:\\UTVK\\MISC\\isxdl\\WizModernSmallImage-IS.bmp"
		OleLoadPicturePath(CT2W(m_strSmallWizardImage),NULL,0,0,IID_IPicture,(LPVOID*)&m_pSmallWizardImage);
	}
}

CDownloadDlg::~CDownloadDlg() {
	// Delete incomplete file
#ifdef USE_RESUME
	if(!m_bAllOk && !m_bOptionResume)
		DeleteFile(m_strCurrentFile);
#else
	if(!m_bAllOk)
		DeleteFile(m_strCurrentFile);
#endif
}

#ifdef WIZ
LRESULT CDownloadDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(GetDlgItem(IDC_HEADERLABEL))
        DeleteObject((HGDIOBJ)SendDlgItemMessage(IDC_HEADERLABEL, WM_GETFONT, 0, 0));

	if(m_hWndParent) ::ShowWindow(m_hWndParent,SW_SHOW);

	if(m_pSmallWizardImage) m_pSmallWizardImage->Release();

	return TRUE;
}

LRESULT CDownloadDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(GetDlgItem(IDC_HEADERLABEL)) {
		CRect		rc;
		HFONT		hOldFont;
		CPaintDC	dc(*this);

		// Header rect
		::GetWindowRect(GetDlgItem(IDC_HEADERRECT), &rc);
		::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rc, 2);
		dc.FillRect(&rc, (HBRUSH)(COLOR_WINDOW+1));

		// Header label
		::GetWindowRect(GetDlgItem(IDC_HEADERLABEL), &rc);
		::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rc, 2);
		hOldFont = dc.SelectFont((HFONT)SendDlgItemMessage(IDC_HEADERLABEL, WM_GETFONT, 0, 0));
		dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		dc.SetBkColor(GetSysColor(COLOR_WINDOW));
		dc.DrawText(m_strOptionLabel, -1, &rc, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
		dc.SelectFont(hOldFont);

		// Header description
		::GetWindowRect(GetDlgItem(IDC_HEADERDESCRIPTION), &rc);
		::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rc, 2);
		hOldFont = dc.SelectFont((HFONT)SendDlgItemMessage(IDC_HEADERDESCRIPTION, WM_GETFONT, 0, 0));
		dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		dc.SetBkColor(GetSysColor(COLOR_WINDOW));
		dc.DrawText(m_strOptionDescription, -1, &rc, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
		dc.SelectFont(hOldFont);

#if 0
		// Header icon
		GetWindowRect(GetDlgItem(hwndDlg, IDC_ICON), rc);
		MapWindowPoints(HWND_DESKTOP, hwndDlg, rc, 2);
		Icon := SendDlgItemMessage(hwndDlg, IDC_ICON,STM_GETICON , 0, 0);
		DrawIcon(Dc, rc.Left, rc.Top, Icon);
#endif
#if 1
		if(m_pSmallWizardImage) {
			SIZE sizeInHiMetric,sizeInPix;
			int nPixelsPerInchX = GetDeviceCaps(dc, LOGPIXELSX);
			int nPixelsPerInchY = GetDeviceCaps(dc, LOGPIXELSY);

			// get width and height of picture
			m_pSmallWizardImage->get_Width(&sizeInHiMetric.cx);
			m_pSmallWizardImage->get_Height(&sizeInHiMetric.cy);

			// convert himetric to pixels
			sizeInPix.cx = (nPixelsPerInchX * sizeInHiMetric.cx + HIMETRIC_PER_INCH / 2) / HIMETRIC_PER_INCH;
			sizeInPix.cy = (nPixelsPerInchY * sizeInHiMetric.cy + HIMETRIC_PER_INCH / 2) / HIMETRIC_PER_INCH;
			if(sizeInPix.cx>59 || sizeInPix.cy>59) {
				sizeInPix.cx = 55;
				sizeInPix.cy = 55;
			}

			GetDlgItem(IDC_PICTURE).ShowWindow(SW_HIDE);
			::GetWindowRect(GetDlgItem(IDC_PICTURE), &rc);
			::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rc, 2);
			int xDiff = sizeInPix.cx - rc.Width();
			int yDiff = sizeInPix.cy - rc.Height();
			if(xDiff || yDiff) {
				rc.left -= 1 + xDiff / 2;
				rc.top -= yDiff / 2;
				rc.right = rc.left + sizeInPix.cx;
				rc.bottom = rc.top + sizeInPix.cy;
			}
			//rc.InflateRect(sizeInPix.cx - rc.Width(),sizeInPix.cy - rc.Height());
			m_pSmallWizardImage->Render(dc,rc.left,rc.top,rc.Width(),rc.Height(),0,sizeInHiMetric.cy,sizeInHiMetric.cx,-sizeInHiMetric.cy,&rc);
		}
#endif
	}
	return TRUE;
}
#endif

LRESULT CDownloadDlg::OnThreadDone(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(!m_bBusy) EndDialog(IDOK);
	m_bThreadDone = true;
	return TRUE;
}

LRESULT CDownloadDlg::OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
	if((wParam & 0xFFF0) == IDM_ABOUT) {
		CString str;
		GetString(IDS_ABOUT,str);
		MessageBox(str,m_strTitle,MB_OK|MB_ICONINFORMATION);
	} else
		bHandled = FALSE;

	return 0;
}

LRESULT CDownloadDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_wndProgressFile.Attach(GetDlgItem(IDC_PROGRESS_FILE));
	m_wndProgressAll.Attach(GetDlgItem(IDC_PROGRESS));

	CDownload::SetServer(m_hWnd);
	HMENU hMenu = GetSystemMenu(FALSE);

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ATLASSERT((IDM_ABOUT & 0xFFF0) == IDM_ABOUT);
	ATLASSERT(IDM_ABOUT < 0xF000);

	EnableMenuItem(hMenu,SC_CLOSE,MF_BYCOMMAND|MF_GRAYED);
	AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
	CString str;
	GetString(IDM_ABOUT,str);
	AppendMenu(hMenu,MF_ENABLED|MF_STRING,IDM_ABOUT,str);

	// center the dialog on the screen
	CenterWindow(GetParent());

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_SETUP), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_SETUP), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	if(m_hWndParent && !IsSimple()) ::ShowWindow(m_hWndParent,SW_HIDE);

	DWORD dwThreadID;
	m_hThread = CreateThread(NULL,0,ThreadProc,(LPVOID)this,0,&dwThreadID);

	SetWindowText(m_strTitle);
	if(IsSimple()) SetDlgItemText(IDC_SIMPLE,m_strOptionSimple);

#ifdef WIZ
	if(GetDlgItem(IDC_HEADERLABEL)) {
		LOGFONT LogFont;
		HFONT OldFont = (HFONT)SendDlgItemMessage(IDC_HEADERLABEL, WM_GETFONT, 0, 0);
		GetObject(OldFont, sizeof LOGFONT, &LogFont);
		LogFont.lfWeight = FW_BOLD;
		HFONT Font = CreateFontIndirect(&LogFont);
		SendDlgItemMessage(IDC_HEADERLABEL, WM_SETFONT, (WPARAM)Font, 0);

		::ShowWindow(GetDlgItem(IDC_HEADERLABEL),SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_HEADERDESCRIPTION),SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_HEADERRECT),SW_HIDE);
//			::ShowWindow(GetDlgItem(IDC_ICON),SW_HIDE);
	}
#endif

	CString tmp;
	if(!IsSimple()) {
		if(GetString(IDC_STATIC_FILE,tmp)) GetDlgItem(IDC_STATIC_FILE).SetWindowText(tmp);
		if(GetString(IDC_STATIC_SPEED,tmp)) GetDlgItem(IDC_STATIC_SPEED).SetWindowText(tmp);
		if(GetString(IDC_STATIC_STATUS,tmp)) GetDlgItem(IDC_STATIC_STATUS).SetWindowText(tmp);
		if(GetString(IDC_STATIC_ELAPSED,tmp)) GetDlgItem(IDC_STATIC_ELAPSED).SetWindowText(tmp);
		if(GetString(IDC_STATIC_REMAINING,tmp)) GetDlgItem(IDC_STATIC_REMAINING).SetWindowText(tmp);
		if(GetString(IDC_STATIC_PROGRESSFILE,tmp)) GetDlgItem(IDC_STATIC_PROGRESSFILE).SetWindowText(tmp);
		if(GetString(IDC_STATIC_PROGRESSALL,tmp)) GetDlgItem(IDC_STATIC_PROGRESSALL).SetWindowText(tmp);
	}
	if(GetString(167,tmp)) GetDlgItem(IDOK).SetWindowText(tmp);

	return TRUE;
}

bool CDownloadDlg::IsSimple() {
	return m_nDialogID == IDD_DOWNLOAD_SIMPLE;
}

LRESULT CDownloadDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if(m_bThreadDone) {
		EndDialog(IDOK);
		return 0;
	}

	CString strRes;
	GetString(IDS_ABORT,strRes);
	m_bBusy = true;
	long nRet = ::MessageBox(m_hWnd,strRes,m_strTitle,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
	m_bBusy = false;
	if(m_bThreadDone) {
		EndDialog(IDOK);
		return 0;
	}
	if(nRet==IDNO) return 0;

	Henden::CSingleLock lock(m_cs,true);
	m_bAbort = true;
	m_bAskRetry = false;
	if(m_hFile) {
		inet.InternetCloseHandle(m_hFile);
		m_hFile = NULL;
	}
	if(m_hConn) {
		inet.InternetCloseHandle(m_hConn);
		m_hConn = NULL;
	}
	if(m_hInternet) {
		inet.InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
	}
	CWindow wndCancel = GetDlgItem(IDCANCEL);
	if((HWND)wndCancel) wndCancel.EnableWindow(FALSE);

	return 0;
}

LRESULT CDownloadDlg::OnErrorDlg(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	HINTERNET hFile = (HINTERNET)wParam;
	char szBuffer[100];
	if(hFile) {
		DWORD dwSize;
		do {
			inet.InternetReadFile(hFile, (LPVOID)szBuffer, sizeof szBuffer, &dwSize);
		} while(dwSize);
	}

	///AtlMessageBox(m_hWndServer,"dlg");
	m_bErrorDlgResult = inet.InternetErrorDlg(
		m_hWnd, 
		hFile, 
		ERROR_INTERNET_INCORRECT_PASSWORD, 
		FLAGS_ERROR_UI_FILTER_FOR_ERRORS|FLAGS_ERROR_UI_FLAGS_GENERATE_DATA|FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, 
		NULL
	) == ERROR_INTERNET_FORCE_RETRY;
	///AtlMessageBox(m_hWndServer,"dlg2");
	SetEvent(m_hEventWait);
	return 0;
}

LRESULT CDownloadDlg::OnMsgBox(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_nMsgBoxResult = MessageBox(m_strError,m_strTitle,(UINT)wParam);
	SetEvent(m_hEventWait);
	return 0;
}

LRESULT CDownloadDlg::OnText(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	LPTSTR pszStatus = (LPTSTR)lParam;
	if(!IsSimple()) SetDlgItemText((int)wParam,pszStatus);
	delete []pszStatus;
	return 0;
}

LRESULT CDownloadDlg::OnPosFile(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	if(!IsSimple()) {
		m_wndProgressFile.SetRange32(0,(int)lParam);
		m_wndProgressFile.SetPos((int)wParam);
	}
	return 0;
}

LRESULT CDownloadDlg::OnPosAll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	if(!IsSimple()) {
		m_wndProgressAll.SetRange32(0,(int)lParam);
		m_wndProgressAll.SetPos((int)wParam);
	}
	return 0;
}

LRESULT CDownloadDlg::OnUserPass(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	CUserPassDlg dlg((CUserPass*)lParam);
	m_bErrorDlgResult = dlg.DoModal(m_hWnd)==IDOK;
	SetEvent(m_hEventWait);
	return 0;
}
