// WTL
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectLanguageFiles dialog

class CDlgSelectLanguageFiles : 
	public CDialogImpl<CDlgSelectLanguageFiles>, 
	public CWinDataExchange<CDlgSelectLanguageFiles>,
	public CMyDialogBase<CDlgSelectLanguageFiles>,
	public CDialogResize<CDlgSelectLanguageFiles>
{
public:
	enum { IDD = IDD_SELECT_LANGUAGEFILES };

	typedef struct {
		CString		m_strTitle;
		CString		m_strFileName;
		CString		m_strLanguageName;
		CString		m_strLanguageID;
	} Item;

	CAtlArray<Item>		m_items;
	CAtlArray<CString>	m_files;
	CString				m_strResult;

	CDlgSelectLanguageFiles(const CString& strFiles) {
		int iStart = 0;
		int pos = strFiles.Find(",",iStart);
		while(pos>=0) {
			m_files.Add(strFiles.Mid(iStart,pos-iStart).Trim());
			iStart = pos+1;
			pos = strFiles.Find(",",iStart);
		}
		m_files.Add(strFiles.Mid(iStart).Trim());
	}

	CDlgSelectLanguageFiles() {
	}

	BEGIN_DLGRESIZE_MAP(CDlgSelectLanguageFiles)
		DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BROWSE, DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CDlgSelectLanguageFiles)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_BROWSE, OnBrowse)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgSelectLanguageFiles>)
		CHAIN_MSG_MAP(CDialogResize<CDlgSelectLanguageFiles>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgSelectLanguageFiles)
		//DDX_TEXT(IDC_LINENUMBER, m_strLineNumber)
	END_DDX_MAP()

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			DoDataExchange(DDX_SAVE);
			int iCount = m_wndList.GetItemCount();
			for(int i=0;i<iCount;i++) {
				if(m_wndList.GetCheckState(i)) {
					if(!m_strResult.IsEmpty()) m_strResult += ", ";
					UINT iItem = m_wndList.GetItemData(i);
					m_strResult += m_items[iItem].m_strFileName;
				}
			}
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnBrowse(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		BROWSEINFO bi = {0};
		char DirName[MAX_PATH];
		LPITEMIDLIST pidl;
		LPMALLOC pmalloc = NULL;
		CString strText;

		SHGetMalloc(&pmalloc);

		bi.hwndOwner = m_hWnd;
		bi.pszDisplayName = DirName;
		bi.lpszTitle = "Select a folder containing Inno Setup Language files.";
		bi.ulFlags = /*BIF_NEWDIALOGSTYLE|*/BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT/*|BIF_EDITBOX*/;
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = (LPARAM)strText.GetBuffer(MAX_PATH);

		if(pidl = SHBrowseForFolder(&bi)) {
			SHGetPathFromIDList(pidl, strText.GetBuffer(MAX_PATH));
			CMyApp::m_prefs.m_languageDirs.Add(strText);
			FindLanguageFiles();
			pmalloc->Free(pidl);
		}
		pmalloc->Release();

		return 0;
	}

	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData) {
		CHAR szDir[MAX_PATH];

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

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		CWaitCursor wait;

		DlgResize_Init(true,false,0);
		m_wndList.SubclassWindow(GetDlgItem(IDC_LIST));
		m_wndList.InsertColumn(0,"File Name",LVCFMT_LEFT,160,0);
		m_wndList.InsertColumn(1,"Language",LVCFMT_LEFT,160,0);
		m_wndList.InsertColumn(2,"LanguageID",LVCFMT_LEFT,97,0);
		m_wndList.InsertColumn(3,"Path",LVCFMT_LEFT,160,0);
		FindLanguageFiles();

		_L(m_hWnd,"SelectLangFiles");
		CenterWindow(GetParent());
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	void FindLanguageFiles() {
		m_items.RemoveAll();
		WIN32_FIND_DATA wfd;
		CString strInnoFolder = CMyApp::m_prefs.m_strInnoFolder;
		CMyUtils::EndWith(strInnoFolder,'\\');
		for(UINT i=0;i<CMyApp::m_prefs.m_languageDirs.GetCount();i++) {
			CString strPath = CMyApp::m_prefs.m_languageDirs[i];
			CMyUtils::EndWith(strPath,'\\');
			HANDLE hFind = FindFirstFile(strPath + "*.isl",&wfd);
			if(hFind) {
				do {
					TCHAR szName[MAX_PATH];
					TCHAR szID[MAX_PATH];
					GetPrivateProfileString("LangOptions","LanguageName","",szName,sizeof szName,strPath + wfd.cFileName);
					GetPrivateProfileString("LangOptions","LanguageID","",szID,sizeof szName,strPath + wfd.cFileName);

					Item item;
					item.m_strTitle = wfd.cFileName;
					item.m_strFileName = strPath + wfd.cFileName;
					item.m_strLanguageName = szName;
					item.m_strLanguageID = szID;
					
					if(item.m_strFileName.Find(strInnoFolder)==0 /*&& item.m_strFileName.ReverseFind('\\')<strInnoFolder.GetLength()*/) {
						item.m_strFileName.Delete(0,strInnoFolder.GetLength());
						item.m_strFileName.Insert(0,"compiler:");
					}

					m_items.Add(item);

				} while(FindNextFile(hFind,&wfd));
				FindClose(hFind);
			}
		}
		m_wndList.DeleteAllItems();
		for(UINT i=0;i<m_items.GetCount();i++) {
			int iItem = m_wndList.InsertItem(i,m_items[i].m_strTitle);
			m_wndList.SetItemText(iItem,1,m_items[i].m_strLanguageName);
			m_wndList.SetItemText(iItem,2,m_items[i].m_strLanguageID);
			m_wndList.SetItemText(iItem,3,m_items[i].m_strFileName);
			m_wndList.SetItemData(iItem,i);
			for(UINT iFile=0;iFile<m_files.GetCount();iFile++) {
				if(!m_items[i].m_strFileName.CompareNoCase(m_files[iFile])) {
					m_wndList.SetCheckState(iItem,TRUE);
					break;
				}
			}
		}
	}

	CCheckListViewCtrl	m_wndList;
};
