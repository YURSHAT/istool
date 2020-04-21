// mywtl.h

#pragma once

#include "Translate.h"

class CMyApp;
class CMainFrame;
class CMyDoc;

extern CMyApp* AfxGetApp();
extern CMainFrame& AfxGetMainWnd();
extern HWND AfxGetMainHWnd();
extern void AfxGetFileTitle(LPCTSTR pszPathName, LPSTR pszBuffer, UINT nLength);
extern CMyDoc* AfxGetDocument();

template <class T>
class CMyDialogBase {
public:
	BEGIN_MSG_MAP(CMyDialogBase)
		COMMAND_ID_HANDLER(IDHELP, OnHelpCmd)
	END_MSG_MAP()

	LRESULT OnHelpCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		T* pT = static_cast<T*>(this);
		AfxGetApp()->OpenHtmlHelp(HH_DISPLAY_TOPIC,pT->IDD);
		return TRUE;
	}
};

class CMyPropertySheet : public CPropertySheetImpl<CMyPropertySheet> {
public:
	CMyPropertySheet(_U_STRINGorID title = (LPCTSTR)NULL, UINT uStartPage = 0, HWND hWndParent = NULL)
		: CPropertySheetImpl<CMyPropertySheet>(title, uStartPage, hWndParent)
	{ 
		m_psh.dwFlags |= PSH_NOCONTEXTHELP | PSH_HASHELP;
		m_bCentered = false;
	}

	typedef CPropertySheetImpl<CMyPropertySheet> baseClass;

	BEGIN_MSG_MAP(CMyPropertySheet)
		MESSAGE_HANDLER(WM_COMMAND, CPropertySheetImpl<CMyPropertySheet>::OnCommand)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	END_MSG_MAP()

	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
		if(wParam && !m_bCentered) {
			CenterWindow(AfxGetMainHWnd());
			m_bCentered = true;

			CWindow wnd = GetDlgItem(IDOK);
			if(wnd) wnd.SetWindowText(_L("System|OK","OK"));
			wnd = GetDlgItem(IDCANCEL);
			if(wnd) wnd.SetWindowText(_L("System|Cancel","Cancel"));
			wnd = GetDlgItem(0x3021);
			if(wnd) wnd.SetWindowText(_L("System|Apply","&Apply"));
			wnd = GetDlgItem(IDHELP);
			if(wnd) wnd.SetWindowText(_L("System|Help","Help"));
		}
		bHandled = FALSE;
		return 0;
	}
private:
	bool	m_bCentered;
};

template <class T>
class CMyPropertyPageBase {
public:
	BEGIN_MSG_MAP(CMyPropertyPageBase)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	END_MSG_MAP()

	LRESULT OnNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) {
		NMHDR* pNMHDR = (NMHDR*)lParam;
		if(pNMHDR->code==PSN_HELP) {
			T* pT = static_cast<T*>(this);
			AfxGetApp()->OpenHtmlHelp(HH_DISPLAY_TOPIC,pT->IDD);
		} else
			bHandled = FALSE;
		return 0;
	}

	CMyPropertyPageBase() {
		T* pT = static_cast<T*>(this);
		pT->m_psp.dwFlags |= PSP_HASHELP;
	}

	void MyModifyStyle(UINT nID,DWORD dwRemove,DWORD dwAdd) {
		T* pT = static_cast<T*>(this);
		CWindow wnd(pT->GetDlgItem(nID));
		wnd.ModifyStyle(dwRemove,dwAdd);
	}
};

#define MY_REFLECT_NOTIFICATIONS() \
	{ \
		bHandled = TRUE; \
		lResult = MyReflectNotifications(uMsg, wParam, lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define DDX_CBINDEX(nID, var) \
		if(nCtlID == (UINT)-1 || nCtlID == nID) \
		{ \
			if(bSaveAndValidate) var = ::SendMessage(GetDlgItem(nID),CB_GETCURSEL,0,0);\
			else ::SendMessage(GetDlgItem(nID),CB_SETCURSEL,var,0);\
		}

#define DDX_CBICON(wnd,nID,var) \
	if(nCtlID==(UINT)-1 || nCtlID==nID) { \
		if(bSaveAndValidate) var = wnd.GetCurSelIcon(); \
		else wnd.SetCurSelIcon(var); \
	}

#define DDX_CBSTRING(nID,var) \
	if(nCtlID==(UINT)-1 || nCtlID==nID) { \
		if(bSaveAndValidate) {  \
			int nLength = ::GetWindowTextLength(GetDlgItem(nID)); \
			if(nLength>0) { \
				::GetWindowText(GetDlgItem(nID),var.GetBufferSetLength(nLength),nLength+1); \
			} else { \
				::GetWindowText(GetDlgItem(nID),var.GetBufferSetLength(255),256); \
			} \
			var.ReleaseBuffer(); \
		} else { \
			int i = (int)::SendMessage(GetDlgItem(nID), CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)(LPCTSTR)var); \
			if (i < 0) { \
				::SetWindowText(GetDlgItem(nID),var); \
			} else { \
				::SendMessage(GetDlgItem(nID), CB_SETCURSEL, i, 0L); \
			} \
		} \
	}

namespace WTL {

class CMutex {
public:
	CMutex(BOOL bInitiallyOwn = FALSE,LPCTSTR lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL) {
		m_hMutex = CreateMutex(lpsaAttribute,bInitiallyOwn,lpszName);
	}
	~CMutex() {
		CloseHandle(m_hMutex);
	}
protected:
	HANDLE	m_hMutex;
};

};

class CMyCheckListViewCtrl : public CCheckListViewCtrlImpl<CMyCheckListViewCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(_T("ISTool_CheckListView"), GetWndClassName())

	void CheckSelectedItems(int nCurrItem) {
		CCheckListViewCtrlImpl<CMyCheckListViewCtrl>::CheckSelectedItems(nCurrItem);
		GetParent().PostMessage(WM_USER,0,0);
	}

	BOOL SetCheckStateIndeterminate(int nItem)
	{
		//int nCheck = bCheck ? 2 : 1;   // one based index
		return SetItemState(nItem, INDEXTOSTATEIMAGEMASK(0), LVIS_STATEIMAGEMASK);
	}
};

#ifdef _DEBUG
#define VERIFY(x)			ATLASSERT(x)
#else
#define VERIFY(x)			x
#endif

#define IsNewWindows()		((GetVersion()&0xFF)>4)
