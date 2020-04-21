// WTL
#pragma once

#include "MyDoc.h"
#include "Edit2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageDirs dialog

class CPageDirs : 
	public CPropertyPageImpl<CPageDirs>,
	public CMyPropertyPageBase<CPageDirs>,
	public CWinDataExchange<CPageDirs>,
	public CDialogResize<CPageDirs>,
	public Henden::CToolTipDialog<CPageDirs>
{
public:
	enum { IDD = IDD_DIRS };

	BEGIN_DLGRESIZE_MAP(CPageDirs)
		DLGRESIZE_CONTROL(IDC_NAME, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_PERMISSIONS, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_FLAGS, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_ATTRIBUTES, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_DIRS_DELETEAFTERINSTALL, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_UNINSALWAYSUNINSTALL, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_DIRS_UNINSNEVERUNINSTALL, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_READONLY, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_HIDDEN, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_SYSTEM, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_SETNTFSCOMPRESSION, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_UNSETNTFSCOMPRESSION, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CPageDirs)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageDirs>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageDirs>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageDirs>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_NAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_PERMISSIONS, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSALWAYSUNINSTALL, BN_CLICKED, OnUninsAlwaysUninstall)
		COMMAND_HANDLER(IDC_DIRS_UNINSNEVERUNINSTALL, BN_CLICKED, OnUninsNeverUninstall)
		COMMAND_HANDLER(IDC_DIRS_DELETEAFTERINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_READONLY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_HIDDEN, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SYSTEM, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SETNTFSCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNSETNTFSCOMPRESSION, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDialogResize<CPageDirs>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageDirs)
		DDX_CHECK(IDC_DIRS_DELETEAFTERINSTALL, m_bDeleteAfterInstall)
		DDX_TEXT(IDC_NAME, m_strName)
		DDX_CHECK(IDC_UNINSALWAYSUNINSTALL, m_bUninsAlwaysUninstall)
		DDX_CHECK(IDC_DIRS_UNINSNEVERUNINSTALL, m_bUninsNeverUninstall)
		DDX_CHECK(IDC_READONLY, m_bReadOnly)
		DDX_CHECK(IDC_HIDDEN, m_bHidden)
		DDX_CHECK(IDC_SYSTEM, m_bSystem)
		DDX_TEXT(IDC_PERMISSIONS, m_strPermissions)
		DDX_CHECK(IDC_SETNTFSCOMPRESSION, m_nSetNTFSCompression)
		DDX_CHECK(IDC_UNSETNTFSCOMPRESSION, m_nUnsetNTFSCompression)
	END_DDX_MAP()

	CEdit2	m_wndName;
	CButton	m_wndUninsNeverUninstall;
	CButton	m_wndUninsAlwaysUninstall;
	BOOL	m_bDeleteAfterInstall;
	CString	m_strName;
	BOOL	m_bUninsAlwaysUninstall;
	BOOL	m_bUninsNeverUninstall;
	BOOL	m_bReadOnly;
	BOOL	m_bHidden;
	BOOL	m_bSystem;
	CString	m_strPermissions;
	int		m_nSetNTFSCompression, m_nUnsetNTFSCompression;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		DlgResize_Init(false,false,0);
		_L(m_hWnd,"Dirs");
		m_wndName.SubclassWindow(GetDlgItem(IDC_NAME));
		m_wndUninsNeverUninstall.Attach(GetDlgItem(IDC_DIRS_UNINSNEVERUNINSTALL));
		m_wndUninsAlwaysUninstall.Attach(GetDlgItem(IDC_UNINSALWAYSUNINSTALL));

		for(int nPos=0;nPos<m_listDirs.GetSize();nPos++) {
			CInnoScript::CLine* pDir = m_listDirs[nPos];

			if(m_strName.CompareNoCase(SAFESTR(pDir->GetParameter("Name"))))
				m_strName.Empty();

			if(m_strPermissions.CompareNoCase(SAFESTR(pDir->GetParameter("Permissions"))))
				m_strPermissions.Empty();

			if(m_bDeleteAfterInstall != (pDir->GetParameterFlag("Flags","deleteafterinstall") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_DIRS_DELETEAFTERINSTALL,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bDeleteAfterInstall = 2;
			}

			if(m_bUninsAlwaysUninstall != (pDir->GetParameterFlag("Flags","uninsalwaysuninstall") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_UNINSALWAYSUNINSTALL,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bUninsAlwaysUninstall = 2;
			}

			if(m_bUninsNeverUninstall != (pDir->GetParameterFlag("Flags","uninsneveruninstall") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_DIRS_UNINSNEVERUNINSTALL,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bUninsNeverUninstall = 2;
			}

			if(m_nSetNTFSCompression != (pDir->GetParameterFlag("Flags","setntfscompression") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_SETNTFSCOMPRESSION,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSetNTFSCompression = 2;
			}

			if(m_nUnsetNTFSCompression != (pDir->GetParameterFlag("Flags","unsetntfscompression") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_UNSETNTFSCOMPRESSION,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUnsetNTFSCompression = 2;
			}

			// Attribs
			if(m_bReadOnly != (pDir->GetParameterFlag("attribs","readonly") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_READONLY,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bReadOnly = 2;
			}

			if(m_bHidden != (pDir->GetParameterFlag("attribs","hidden") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_HIDDEN,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bHidden = 2;
			}

			if(m_bSystem != (pDir->GetParameterFlag("attribs","system") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_SYSTEM,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bSystem = 2;
			}
		}
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listDirs.GetSize()==1;

		if(bForce && m_strName.IsEmpty()) {
			AtlMessageBox(m_hWnd,_L("You must enter a name."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			return FALSE;
		}

		for(int nPos=0;nPos<m_listDirs.GetSize();nPos++) {
			CInnoScript::CLine* pDir = m_listDirs[nPos];

			CString strOld, strNew;
			strOld = pDir->GetParameter("Name");
			if(bForce || !m_strName.IsEmpty())
				pDir->SetParameter("Name",m_strName);

			strNew = pDir->GetParameter("Name");
			// If name has changed, check if references should be changed also
			if(strNew.CompareNoCase(strOld) && !m_bNew) m_pDoc->RenameDir(strOld,strNew);

			CInnoScriptEx::SetString(pDir,bForce,"Permissions",m_strPermissions);

			// Flags
			if(m_bDeleteAfterInstall==FALSE) pDir->SetParameterFlag("Flags","deleteafterinstall",false);
			else if(m_bDeleteAfterInstall==TRUE) pDir->SetParameterFlag("Flags","deleteafterinstall",true);
			
			if(m_bUninsAlwaysUninstall==FALSE) pDir->SetParameterFlag("Flags","uninsalwaysuninstall",false);
			else if(m_bUninsAlwaysUninstall==TRUE) pDir->SetParameterFlag("Flags","uninsalwaysuninstall",true);
			
			if(m_bUninsNeverUninstall==FALSE) pDir->SetParameterFlag("Flags","uninsneveruninstall",false);
			else if(m_bUninsNeverUninstall==TRUE) pDir->SetParameterFlag("Flags","uninsneveruninstall",true);

			CInnoScriptEx::SetFlag(pDir,"Flags","setntfscompression",m_nSetNTFSCompression);
			CInnoScriptEx::SetFlag(pDir,"Flags","unsetntfscompression",m_nUnsetNTFSCompression);

			// Attribs
			if(m_bReadOnly==FALSE) pDir->SetParameterFlag("attribs","readonly",false);
			else if(m_bReadOnly==TRUE) pDir->SetParameterFlag("attribs","readonly",true);
			
			if(m_bHidden==FALSE) pDir->SetParameterFlag("attribs","hidden",false);
			else if(m_bHidden==TRUE) pDir->SetParameterFlag("attribs","hidden",true);
			
			if(m_bSystem==FALSE) pDir->SetParameterFlag("attribs","system",false);
			else if(m_bSystem==TRUE) pDir->SetParameterFlag("attribs","system",true);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_bNew) SetModified();
		return 0;
	}

	LRESULT OnUninsAlwaysUninstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		if(m_wndUninsAlwaysUninstall.GetCheck())
			m_wndUninsNeverUninstall.SetCheck(FALSE);
		return 0;
	}

	LRESULT OnUninsNeverUninstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();	
		if(m_wndUninsNeverUninstall.GetCheck())
			m_wndUninsAlwaysUninstall.SetCheck(FALSE);
		return 0;
	}

	CString m_strTitle;
	CPageDirs(CScriptList& listDirs,CMyDoc* pDoc,const bool bNew) : 
		m_listDirs(listDirs), m_pDoc(pDoc), m_bNew(bNew), m_wndName(pDoc)
	{
		m_strTitle = _L("DialogTitles|Dir","Dir");
		SetTitle((LPCTSTR)m_strTitle);

		m_bDeleteAfterInstall = FALSE;
		m_strName = _T("");
		m_bUninsAlwaysUninstall = FALSE;
		m_bUninsNeverUninstall = FALSE;
		m_bReadOnly = FALSE;
		m_bHidden = FALSE;
		m_bSystem = FALSE;
		m_nSetNTFSCompression = 0;
		m_nUnsetNTFSCompression = 0;

		for(int nPos=0;nPos<m_listDirs.GetSize();nPos++) {
			CScriptLine* pDir = m_listDirs[nPos];
			m_strName = pDir->GetParameter("Name");
			m_strPermissions = pDir->GetParameter("Permissions");

			if(pDir->GetParameterFlag("Flags","deleteafterinstall")) m_bDeleteAfterInstall = TRUE;
			if(pDir->GetParameterFlag("Flags","uninsalwaysuninstall")) m_bUninsAlwaysUninstall = TRUE;
			if(pDir->GetParameterFlag("Flags","uninsneveruninstall")) m_bUninsNeverUninstall = TRUE;
			if(pDir->GetParameterFlag("Flags","setntfscompression")) m_nSetNTFSCompression = 1;
			if(pDir->GetParameterFlag("Flags","unsetntfscompression")) m_nUnsetNTFSCompression = 1;
			
			if(pDir->GetParameterFlag("attribs","readonly")) m_bReadOnly = TRUE;
			if(pDir->GetParameterFlag("attribs","hidden")) m_bHidden = TRUE;
			if(pDir->GetParameterFlag("attribs","system")) m_bSystem = TRUE;
			break;
		}
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_NAME,_L("Help|Dirs|Name","The name of the directory to create, which normally will start with one of the directory constants."))
		TOOLTIP_HANDLER(IDC_PERMISSIONS,_L("Help|Dirs|Permissions","Specifies additional permissions to grant in the directory's ACL (access control list). It is not recommended that you use this parameter if you aren't familiar with ACLs or why you would need to change them, because misusing it could negatively impact system security."))
		TOOLTIP_HANDLER(IDC_ATTRIBUTES,_L("Help|Dirs|Attributes",""))
		TOOLTIP_HANDLER(IDC_DIRS_DELETEAFTERINSTALL,_L("Help|Dirs|DeleteAfterInstall","Instructs Setup to create the directory as usual, but then delete it once the installation is completed (or aborted) if it's empty. This can be useful when extracting temporary data needed by a program executed in the script's [Run] section.\r\n\r\nThis flag will not cause directories that already existed before installation to be deleted."))
		TOOLTIP_HANDLER(IDC_UNINSALWAYSUNINSTALL,_L("Help|Dirs|UninsALwaysUninstall","Instructs the uninstaller to always attempt to delete the directory if it's empty. Normally the uninstaller will only try to delete the directory if it didn't already exist prior to installation."))
		TOOLTIP_HANDLER(IDC_DIRS_UNINSNEVERUNINSTALL,_L("Help|Dirs|UninsNeverUninstall","Instructs the uninstaller to not attempt to delete the directory. By default, the uninstaller deletes any directory specified in the [Dirs] section if it is empty."))
		TOOLTIP_HANDLER(IDC_READONLY,_L("Help|Dirs|ReadOnly","Specifies additional attributes for the directory."))
		TOOLTIP_HANDLER(IDC_HIDDEN,_L("Help|Dirs|Hidden","Specifies additional attributes for the directory."))
		TOOLTIP_HANDLER(IDC_SYSTEM,_L("Help|Dirs|System","Specifies additional attributes for the directory."))
		TOOLTIP_HANDLER(IDC_SETNTFSCOMPRESSION,_L("Help|Dirs|SetNTFSCompression","Instructs Setup to enable NTFS compression on the directory. If it fails to set the compression state for any reason (for example, if compression is not supported by the file system), no error message will be displayed.\n\nIf the directory already exists, the compression state of any files present in the directory will not be changed."))
		TOOLTIP_HANDLER(IDC_UNSETNTFSCOMPRESSION,_L("Help|Dirs|UnsetNTFSCompression","Instructs Setup to disable NTFS compression on the directory. If it fails to set the compression state for any reason (for example, if compression is not supported by the file system), no error message will be displayed.\n\nIf the directory already exists, the compression state of any files present in the directory will not be changed."))
	END_TOOLTIP_MAP()

	CScriptList&	m_listDirs;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
};
