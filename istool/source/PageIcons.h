// WTL
#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"
#include "IconComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CPageIcons dialog

class CPageIcons : 
	public CPropertyPageImpl<CPageIcons>,
	public CMyPropertyPageBase<CPageIcons>,
	public CWinDataExchange<CPageIcons>,
	public Henden::CToolTipDialog<CPageIcons>
{
public:
	enum { IDD = IDD_ICONS };

	BEGIN_MSG_MAP(CPageIcons)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageIcons>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageIcons>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageIcons>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		COMMAND_HANDLER(IDC_ICONFILENAME, CBN_EDITCHANGE, OnChangeIconFilename)
		COMMAND_HANDLER(IDC_CREATEONLYIFFILEEXISTS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_FILENAME, CBN_KILLFOCUS, OnKillFocusFilename)
		COMMAND_HANDLER(IDC_FILENAME, CBN_DROPDOWN, OnDropdownFilename)
		COMMAND_HANDLER(IDC_ICONFILENAME, CBN_DROPDOWN, OnDropdownIconfilename)
		COMMAND_HANDLER(IDC_FILENAME, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_RUNMINIMIZED, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ICONS_UNINSNEVERUNINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_USEAPPPATHS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ICONS_NAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_PARAMETERS, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_ICONINDEX, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_COMMENT, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_WORKINGDIR, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_WORKINGDIR, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_ICONFILENAME, CBN_SELCHANGE, OnChangeIconFilename)
		COMMAND_HANDLER(IDC_FILENAME, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_ICONINDEX, CBN_KILLFOCUS, OnModified)
		COMMAND_HANDLER(IDC_ICONFILENAME, CBN_KILLFOCUS, OnKillFocusFilename)
		COMMAND_HANDLER(IDC_ICONS_CLOSEONEXIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ICONS_DONTCLOSEONEXIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ICONS_RUNMAXIMIZED, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ICONS_HOTKEY, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_FOLDERSHORTCUT, BN_CLICKED, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageIcons)
		DDX_TEXT(IDC_ICONS_NAME, m_strName)
		DDX_TEXT(IDC_PARAMETERS, m_strParameters)
		DDX_TEXT(IDC_ICONFILENAME, m_strIconFilename)
		DDX_CHECK(IDC_CREATEONLYIFFILEEXISTS, m_bCreateOnlyIfFileExists)
		DDX_CHECK(IDC_RUNMINIMIZED, m_bRunMinimized)
		DDX_CHECK(IDC_ICONS_UNINSNEVERUNINSTALL, m_bUninsNeverUninstall)
		DDX_TEXT(IDC_WORKINGDIR, m_strWorkingDir)
		DDX_CHECK(IDC_USEAPPPATHS, m_nUseAppPaths)
		DDX_TEXT(IDC_COMMENT, m_strComment)
		DDX_TEXT(IDC_FILENAME, m_strFilename)
		DDX_CHECK(IDC_ICONS_CLOSEONEXIT, m_bCloseOnExit)
		DDX_CHECK(IDC_ICONS_DONTCLOSEONEXIT, m_bDontCloseOnExit)
		DDX_CHECK(IDC_ICONS_RUNMAXIMIZED, m_nRunMaximized)
		DDX_TEXT(IDC_ICONS_HOTKEY, m_strHotKey)
		DDX_CBINDEX(IDC_ICONINDEX, m_nIconIndex)
		DDX_CHECK(IDC_FOLDERSHORTCUT, m_nFolderShortcut)
	END_DDX_MAP()

	CEdit2			m_wndParameters;
	CEdit2			m_wndName;
	CComboBox2		m_wndIconFilename;
	CComboBox2		m_wndWorkingDir;
	CComboBox2		m_wndFileName;
	CString			m_strName;
	CString			m_strParameters;
	CString			m_strIconFilename;
	BOOL			m_bCreateOnlyIfFileExists;
	int				m_bRunMinimized;
	int				m_bUninsNeverUninstall;
	CString			m_strWorkingDir;
	int				m_nUseAppPaths;
	CString			m_strComment;
	CString			m_strFilename;
	BOOL			m_bCloseOnExit;
	BOOL			m_bDontCloseOnExit;
	int				m_nRunMaximized;
	CString			m_strHotKey;
	CLargeIconComboBox m_wndIconIndex;
	int				m_nIconIndex, m_nFolderShortcut;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Icons");
		m_wndParameters.SubclassWindow(GetDlgItem(IDC_PARAMETERS));
		m_wndName.SubclassWindow(GetDlgItem(IDC_ICONS_NAME));
		m_wndIconFilename.SubclassWindow(GetDlgItem(IDC_ICONFILENAME));
		m_wndWorkingDir.SubclassWindow(GetDlgItem(IDC_WORKINGDIR));
		m_wndFileName.SubclassWindow(GetDlgItem(IDC_FILENAME));
		m_wndIconIndex.SubclassWindow(GetDlgItem(IDC_ICONINDEX));

		m_pDoc->AddFiles(m_wndFileName);
		m_pDoc->AddDirConstants(m_wndWorkingDir);
		m_pDoc->AddDirs(m_wndWorkingDir);
		m_pDoc->AddFiles(m_wndIconFilename);

		for(int nPos=1;nPos<m_listIcons.GetSize();nPos++) {
			CScriptLine* pLine = m_listIcons[nPos];
			CString strTmp;

			if(m_strName.CompareNoCase(SAFESTR(pLine->GetParameter("Name"))))
				m_strName.Empty();
			if(m_strFilename.CompareNoCase(SAFESTR(pLine->GetParameter("Filename"))))
				m_strFilename.Empty();
			if(m_strParameters.CompareNoCase(SAFESTR(pLine->GetParameter("Parameters"))))
				m_strParameters.Empty();
			if(m_strWorkingDir.CompareNoCase(SAFESTR(pLine->GetParameter("WorkingDir"))))
				m_strWorkingDir.Empty();
			if(m_strIconFilename.CompareNoCase(SAFESTR(pLine->GetParameter("IconFilename"))))
				m_strIconFilename.Empty();
			if(m_strComment.CompareNoCase(SAFESTR(pLine->GetParameter("Comment"))))
				m_strComment.Empty();
			if(m_strHotKey.CompareNoCase(SAFESTR(pLine->GetParameter("HotKey"))))
				m_strHotKey.Empty();

			if(m_nIconIndex!=atol(SAFESTR(pLine->GetParameter("IconIndex"))))
				m_nIconIndex = -1;

			if(m_bCreateOnlyIfFileExists != (pLine->GetParameterFlag("Flags","createonlyiffileexists") ? TRUE : FALSE)) {
				GetDlgItem(IDC_CREATEONLYIFFILEEXISTS).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bCreateOnlyIfFileExists = 2;
			}

			if(m_bRunMinimized != (pLine->GetParameterFlag("Flags","runminimized") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUNMINIMIZED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRunMinimized = 2;
			}

			if(m_bUninsNeverUninstall != (pLine->GetParameterFlag("Flags","uninsneveruninstall") ? TRUE : FALSE)) {
				GetDlgItem(IDC_ICONS_UNINSNEVERUNINSTALL).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bUninsNeverUninstall = 2;
			}

			if(m_nUseAppPaths != (pLine->GetParameterFlag("Flags","useapppaths") ? TRUE : FALSE)) {
				GetDlgItem(IDC_USEAPPPATHS).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUseAppPaths = 2;
			}

			if(m_bCloseOnExit != (pLine->GetParameterFlag("Flags","closeonexit") ? TRUE : FALSE)) {
				GetDlgItem(IDC_ICONS_CLOSEONEXIT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bCloseOnExit = 2;
			}

			if(m_bDontCloseOnExit != (pLine->GetParameterFlag("Flags","dontcloseonexit") ? TRUE : FALSE)) {
				GetDlgItem(IDC_ICONS_DONTCLOSEONEXIT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bDontCloseOnExit = 2;
			}

			if(m_nRunMaximized != (pLine->GetParameterFlag("Flags","runmaximized") ? TRUE : FALSE)) {
				GetDlgItem(IDC_ICONS_RUNMAXIMIZED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nRunMaximized = 2;
			}

			if(m_nFolderShortcut != (pLine->GetParameterFlag("Flags","foldershortcut") ? TRUE : FALSE)) {
				GetDlgItem(IDC_FOLDERSHORTCUT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nFolderShortcut = 2;
			}
		}

		SetIcons();
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listIcons.GetSize()==1;

		if(bForce) {
			if(m_strName.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("You must enter a name."),IDR_MAINFRAME,MB_OK|MB_ICONSTOP);
				return PSNRET_INVALID;
			}
			if(m_strFilename.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("You must enter a file name."),IDR_MAINFRAME,MB_OK|MB_ICONSTOP);
				return PSNRET_INVALID;
			}
		}

		CInnoScriptEx& script = m_pDoc->GetScript();
		for(int nPos=0;nPos<m_listIcons.GetSize();nPos++) {
			CScriptLine* pLine = m_listIcons[nPos];

			script.SetString(pLine,bForce,"Name",m_strName);
			script.SetString(pLine,bForce,"Filename",m_strFilename);
			script.SetString(pLine,bForce,"Parameters",m_strParameters);
			script.SetString(pLine,bForce,"WorkingDir",m_strWorkingDir);
			script.SetString(pLine,bForce,"IconFilename",m_strIconFilename);
			script.SetString(pLine,bForce,"Comment",m_strComment);
			script.SetLong(pLine,bForce,"IconIndex",-1,m_nIconIndex);
			script.SetString(pLine,bForce,"HotKey",m_strHotKey);

			// Flags
			if(m_bCreateOnlyIfFileExists==FALSE)
				pLine->SetParameterFlag("Flags","createonlyiffileexists",false);
			else if(m_bCreateOnlyIfFileExists==TRUE)
				pLine->SetParameterFlag("Flags","createonlyiffileexists",true);
			if(m_bRunMinimized==FALSE)
				pLine->SetParameterFlag("Flags","runminimized",false);
			else if(m_bRunMinimized==TRUE)
				pLine->SetParameterFlag("Flags","runminimized",true);
			if(m_bUninsNeverUninstall==FALSE)
				pLine->SetParameterFlag("Flags","uninsneveruninstall",false);
			else if(m_bUninsNeverUninstall==TRUE)
				pLine->SetParameterFlag("Flags","uninsneveruninstall",true);
			if(m_nUseAppPaths==FALSE)
				pLine->SetParameterFlag("Flags","useapppaths",false);
			else if(m_nUseAppPaths==TRUE)
				pLine->SetParameterFlag("Flags","useapppaths",true);

			CInnoScriptEx::SetFlag(pLine,"Flags","closeonexit",m_bCloseOnExit);
			CInnoScriptEx::SetFlag(pLine,"Flags","dontcloseonexit",m_bDontCloseOnExit);
			CInnoScriptEx::SetFlag(pLine,"Flags","runmaximized", m_nRunMaximized);
			CInnoScriptEx::SetFlag(pLine,"Flags","foldershortcut", m_nFolderShortcut);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnChangeIconFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	void UpdateIconList(LPCTSTR pszFilename) {
		CString strFileName(pszFilename);
		if(!CMyUtils::IsFile(strFileName)) {
			CString strSourceDir;
			if(m_pDoc->GetSourceDir(strSourceDir)) {
				CMyUtils::EndWith(strSourceDir,'\\');
				strFileName = strSourceDir + strFileName;
			}
		}

		m_wndIconIndex.ResetContent();

		if(CMyUtils::IsFile(strFileName)) {
			int nNum = (int)ExtractIcon(_Module.GetResourceInstance(), strFileName, (UINT) -1);
			for(int i=0; i<nNum; i++) {
				HICON hIcon = ExtractIcon(_Module.GetResourceInstance(), strFileName, i);
				m_wndIconIndex.AddIcon(hIcon,i);
			}
			m_wndIconIndex.SetCurSel(m_nIconIndex);
		}
	}

	LRESULT OnKillFocusFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		SetIcons();
		return 0;
	}

	void SetIcons() {
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_wndIconIndex.ResetContent();

		CString strFilename;
		if(m_strIconFilename.IsEmpty())
			strFilename = m_strFilename;
		else
			strFilename = m_strIconFilename;

		if(strFilename.IsEmpty()) {
	//		m_wndIconIndex.EnableWindow(FALSE);
			return;
		}

		// Go through files and find this file
		CScriptList	files;
		script.GetList(CInnoScript::SEC_FILES,files);

		for(int nPos=0;nPos<files.GetSize();nPos++) {
			CScriptLine* pFile = files[nPos];
			CString strName;//(pFile->GetParameter("Source"));
			CInnoScriptEx::GetDestName(pFile,strName);

			if(!strName.CompareNoCase(strFilename)) {
				UpdateIconList(pFile->GetParameter("Source"));
	//			m_wndIconIndex.EnableWindow(TRUE);
				return;
			}
		}
	//	m_wndIconIndex.EnableWindow(FALSE);
	}

	LRESULT OnDropdownFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_wndFileName.GetCount()<=0) {
			CWaitCursor wait;
			m_pDoc->AddFiles(m_wndFileName);
		}
		return 0;
	}

	LRESULT OnDropdownIconfilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_wndIconFilename.GetCount()<=0) {
			CWaitCursor wait;
			m_pDoc->AddFiles(m_wndIconFilename);
		}
		return 0;
	}

	CString m_strTitle;
	CPageIcons(CScriptList& listIcons,CMyDoc* pDoc,const bool bNew) : 
		m_listIcons(listIcons), m_pDoc(pDoc), m_bNew(bNew), m_wndWorkingDir(pDoc), 
		m_wndName(pDoc), m_wndIconFilename(pDoc), m_wndFileName(pDoc), m_wndParameters(pDoc)
	{
		m_strTitle = _L("DialogTitles|Icon","Icon");
		SetTitle((LPCTSTR)m_strTitle);

		//{{AFX_DATA_INIT(CPageIcons)
		m_bCreateOnlyIfFileExists = FALSE;
		m_bRunMinimized = FALSE;
		m_bUninsNeverUninstall = FALSE;
		m_nUseAppPaths = FALSE;
		m_bCloseOnExit = FALSE;
		m_bDontCloseOnExit = FALSE;
		m_nRunMaximized = FALSE;
		//}}AFX_DATA_INIT
		m_nFolderShortcut = 0;

		for(int nPos=0;nPos<m_listIcons.GetSize();nPos++) {
			CScriptLine* pLine = m_listIcons[nPos];

			m_strName = pLine->GetParameter("Name");
			m_strFilename = pLine->GetParameter("Filename");
			m_strParameters = pLine->GetParameter("Parameters");
			m_strWorkingDir = pLine->GetParameter("WorkingDir");
			m_strIconFilename = pLine->GetParameter("IconFilename");
			m_nIconIndex = atol(SAFESTR(pLine->GetParameter("IconIndex")));
			m_strComment = pLine->GetParameter("Comment");
			m_strHotKey = pLine->GetParameter("HotKey");

			if(pLine->GetParameterFlag("Flags","createonlyiffileexists")) m_bCreateOnlyIfFileExists = TRUE;
			if(pLine->GetParameterFlag("Flags","runminimized")) m_bRunMinimized = TRUE;
			if(pLine->GetParameterFlag("Flags","uninsneveruninstall")) m_bUninsNeverUninstall = TRUE;
			if(pLine->GetParameterFlag("Flags","useapppaths")) m_nUseAppPaths = 1;
			if(pLine->GetParameterFlag("Flags","closeonexit")) m_bCloseOnExit = 1;
			if(pLine->GetParameterFlag("Flags","dontcloseonexit")) m_bDontCloseOnExit = 1;
			if(pLine->GetParameterFlag("Flags","runmaximized")) m_nRunMaximized = 1;
			if(pLine->GetParameterFlag("Flags","foldershortcut")) m_nFolderShortcut = 1;
			break;
		}
		if(m_bNew && m_strName.IsEmpty()) m_strName = "{group}\\";
	}

	CScriptList&	m_listIcons;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_ICONS_NAME,_L("Help|Icons|Name","The name and location of the shortcut to create. Any of the shell folder constants or directory constants may be used in this parameter."))
		TOOLTIP_HANDLER(IDC_PARAMETERS,_L("Help|Icons|Parameters","Optional command line parameters for the shortcut, which can include constants."))
		TOOLTIP_HANDLER(IDC_ICONFILENAME,_L("Help|Icons|IconFileName","The filename of a custom icon (located on the user's system) to be displayed. This can be an executable image (.exe, .dll) containing icons or a .ico file. If this parameter is not specified or is blank, Windows will use the file's default icon. This parameter can include constants."))
		TOOLTIP_HANDLER(IDC_CREATEONLYIFFILEEXISTS,_L("Help|Icons|CreateOnlyIfFileExists","When this flag is set, the installer will only try to create the icon if the file specified by the Filename parameter exists."))
		TOOLTIP_HANDLER(IDC_RUNMINIMIZED,_L("Help|Icons|RunMinimized","When this flag is set, Setup sets the \"Run\" setting of the icon to \"Minimized\" so that the program will be initially minimized when it is started."))
		TOOLTIP_HANDLER(IDC_ICONS_UNINSNEVERUNINSTALL,_L("Help|Icons|UninsNeverUninstall","Instructs the uninstaller not to delete the icon."))
		TOOLTIP_HANDLER(IDC_WORKINGDIR,_L("Help|Icons|WorkingDir","The working (or Start In) directory for the shortcut, which is the directory in which the program is started from. If this parameter is not specified or is blank, Windows will use a default path, which varies between the different Windows versions. This parameter can include constants."))
		TOOLTIP_HANDLER(IDC_USEAPPPATHS,_L("Help|Icons|UseAppPaths","When this flag is set, specify just a filename (no path) in the Filename parameter, and Setup will retrieve the pathname from the \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\" registry key and prepend it to the filename automatically."))
		TOOLTIP_HANDLER(IDC_COMMENT,_L("Help|Icons|Comment","Specifies the Comment (or \"description\") field of the shortcut, which determines the popup hint for it in Windows 2000, Me, and later. Earlier Windows versions ignore the comment."))
		TOOLTIP_HANDLER(IDC_FILENAME,_L("Help|Icons|FileName","The command line filename for the shortcut, which normally begins with a directory constant."))
		TOOLTIP_HANDLER(IDC_ICONS_CLOSEONEXIT,_L("Help|Icons|CloseOnExit","When this flag is set, Setup will set the \"Close on Exit\" property of the shortcut. This flag only has an effect if the shortcut points to an MS-DOS application (if it has a .pif extension, to be specific). If neither this flag nor the dontcloseonexit flags are specified, Setup will not attempt to change the \"Close on Exit\" property."))
		TOOLTIP_HANDLER(IDC_ICONS_DONTCLOSEONEXIT,_L("Help|Icons|DontCloseOnExit","Same as closeonexit, except it causes Setup to uncheck the \"Close on Exit\" property."))
		TOOLTIP_HANDLER(IDC_ICONS_RUNMAXIMIZED,_L("Help|Icons|RunMaximized","When this flag is set, Setup sets the \"Run\" setting of the icon to \"Maximized\" so that the program will be initially maximized when it is started."))
		TOOLTIP_HANDLER(IDC_ICONS_HOTKEY,_L("Help|Icons|HotKey","The hot key (or \"shortcut key\") setting for the shortcut, which is a combination of keys with which the program can be started."))
		TOOLTIP_HANDLER(IDC_ICONINDEX,_L("Help|Icons|IconIndex","Zero-based index of the icon to use in the file specified by IconFilename."))
		TOOLTIP_HANDLER(IDC_FOLDERSHORTCUT,_L("Help|Icons|FolderShortcut","Creates a special type of shortcut known as a \"Folder Shortcut\". Normally, when a shortcut to a folder is present on the Start Menu, clicking the item causes a separate Explorer window to open showing the target folder's contents. In contrast, a \"folder shortcut\" will show the contents of the target folder as a submenu instead of opening a separate window."))
	END_TOOLTIP_MAP()
};
