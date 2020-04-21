/**
** @file
** @brief Source code for the CButtonFolder class
*/

#pragma once

namespace Henden {

/**
** @brief Subclass a button and select folder
**
** This class subclasses a button. When you click the button a folder dialog will
** open, and the selected folder will be inserted in the edit control before
** the subclassed button.
** @sa CButtonFile
*/
class CButtonFolder : public CWindowImpl<CButtonFolder,CButton> {
public:
	BEGIN_MSG_MAP(CButtonFolder)
		COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
		REFLECTED_COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
	END_MSG_MAP()

	/// Constructor
	CButtonFolder(LPCTSTR lpszTitle=NULL) : m_lpszTitle(lpszTitle) {}

protected:
	/// The button was clicked - open a browse for folder dialog
	LRESULT OnClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		HWND hWnd = ::GetNextWindow(m_hWnd,GW_HWNDPREV);
		if(hWnd) {
			BROWSEINFO bi = {0};
			TCHAR DirName[MAX_PATH];
			LPITEMIDLIST pidl;
			LPMALLOC pmalloc = NULL;
			CString strText;

			::GetWindowText(hWnd,strText.GetBuffer(MAX_PATH),MAX_PATH);
			//pWnd->GetWindowText(strTe);
			SHGetMalloc(&pmalloc);

			bi.hwndOwner = m_hWnd;
			bi.pszDisplayName = DirName;
			bi.lpszTitle = (LPCTSTR)m_lpszTitle;
			bi.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT/*|BIF_EDITBOX*/;
			bi.lpfn = BrowseCallbackProc;
			bi.lParam = (LPARAM)strText.GetBuffer(MAX_PATH);

			if(pidl = SHBrowseForFolder(&bi)) {
				SHGetPathFromIDList(pidl, strText.GetBuffer(MAX_PATH));
				pmalloc->Free(pidl);
			}

			pmalloc->Release();
			if(pidl) ::SetWindowText(hWnd,strText);
		}
		return 0;
	}

private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData) {
		TCHAR szDir[MAX_PATH];

		switch(uMsg) {
		case BFFM_INITIALIZED:
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)lpData);
			break;
		case BFFM_SELCHANGED:
			// Set the status window to the currently selected path.
			if(SHGetPathFromIDList((LPITEMIDLIST)lParam ,szDir))
				SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
			break;
		default:
			break;
		}
		return 0;
	}

	LPCTSTR m_lpszTitle;
};

} // namespace Henden
