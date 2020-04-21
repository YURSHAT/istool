/**
** @file
** @brief Source code for the CButtonFile class
*/

#pragma once

namespace Henden {

/**
** @brief Subclass a button and select file
**
** This class subclasses a button. When you click the button a file dialog will
** open, and the selected file will be inserted in the edit control before
** the subclassed button.
** @sa CButtonFolder
*/
class CButtonFile : public CWindowImpl<CButtonFile,CButton> {
public:
	BEGIN_MSG_MAP(CButtonFile)
		REFLECTED_COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
	END_MSG_MAP()

	/// Constructor 
	CButtonFile(bool bOpen=true,LPCTSTR lpszFilter=NULL,LPCTSTR pszDefExt=NULL) : m_strFilter(lpszFilter), m_pszDefExt(pszDefExt) {
		m_bOpen = bOpen;
		if(m_strFilter.IsEmpty()) m_strFilter = _T("All Files (*.*)|*.*||");
	}

protected:
	/// The button was clicked - do our magic
	LRESULT OnClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		HWND hWnd = ::GetNextWindow(m_hWnd,GW_HWNDPREV);
		if(hWnd) {
			CString strText;
			::GetWindowText(hWnd,strText.GetBuffer(MAX_PATH),MAX_PATH);
			strText.ReleaseBuffer();

			if(strText.Find(_T(':'))>1) strText.Empty();
			LPTSTR pszFilter = m_strFilter.GetBuffer(0);
			while(*pszFilter) {
				if(*pszFilter==_T('|')) *pszFilter = 0;
				pszFilter++;
			}
			CFileDialog dlg(m_bOpen,m_pszDefExt,NULL/*strText*/,0,m_strFilter,hWnd);
			dlg.m_ofn.lpstrFile = strText.GetBuffer(_MAX_PATH);
			if(dlg.DoModal(hWnd)==IDOK) {
				strText.ReleaseBuffer();
				//dlg.GetFilePath(strText.GetBuffer(MAX_PATH),MAX_PATH);
				::SetWindowText(hWnd,strText);
			}
		}
		return 0;
	}

private:
	LPCTSTR	m_pszDefExt;
	CString	m_strFilter;
	BOOL	m_bOpen;
};

} // namespace Henden
