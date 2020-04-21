// WTL
#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageFiles dialog

class CPageFiles : 
	public CPropertyPageImpl<CPageFiles>,
	public CMyPropertyPageBase<CPageFiles>,
	public CWinDataExchange<CPageFiles>,
	public Henden::CToolTipDialog<CPageFiles>
{
public:
	enum { IDD = IDD_FILES };

	BEGIN_MSG_MAP(CPageFiles)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageFiles>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageFiles>)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageFiles>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_FONTINSTALL, EN_CHANGE, OnChangeFontInstall)
		COMMAND_HANDLER(IDC_READONLY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_EXTERNAL, BN_CLICKED, OnExternal)
		COMMAND_HANDLER(IDC_SOURCE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_HIDDEN, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SYSTEM, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPARETIMESTAMP, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CONFIRMOVERWRITE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DELETEAFTERINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_FONTISNTTRUETYPE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ISREADME, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGSERVER, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RESTARTREPLACE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SHAREDFILE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSNEVERUNINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGTYPELIB, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SKIPIFSOURCEDOESNTEXIST, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DESTNAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_OVERWRITEREADONLY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DESTDIR, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_DESTDIR, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_FILES_ONLYIFDESTFILEEXISTS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RECURSESUBDIRS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_FILES_NOREGERROR, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSRESTARTDELETE, BN_CLICKED, OnModified)
		// IS 3.0.5
		COMMAND_HANDLER(IDC_PROMPTIFOLDER, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ONLYIFDOESNTEXIST, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_IGNOREVERSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DONTCOPY, BN_CLICKED, OnModified)
		// IS 4.0.5
		COMMAND_HANDLER(IDC_UNINSREMOVEREADONLY, BN_CLICKED, OnModified)
		// IS 4.1.0
		COMMAND_HANDLER(IDC_PERMISSIONS, EN_CHANGE, OnModified)
		// IS 4.1.6
		COMMAND_HANDLER(IDC_EXCLUDES, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SORTFILESBYEXTENSION, BN_CLICKED, OnModified)
		// IS 4.2.0
		COMMAND_HANDLER(IDC_TOUCH, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REPLACESAMEVERSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_NOENCRYPTION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_NOCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DONTVERIFYCHECKSUM, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSNOSHAREDFILEPROMPT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CREATEALLSUBDIRS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_32BIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_64BIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_FILES_SOLIDBREAK, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_EXTERNALSIZE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SETNTFSCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNSETNTFSCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SORTFILESBYNAME, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GACINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_STRONGASSEMBLYNAME, EN_CHANGE, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageFiles)
		DDX_TEXT(IDC_SOURCE, m_strSource)
		DDX_TEXT(IDC_DESTDIR, m_strDestDir)
		DDX_TEXT(IDC_DESTNAME, m_strDestName)
		DDX_TEXT(IDC_FONTINSTALL, m_strFontInstall)
		DDX_CHECK(IDC_HIDDEN, m_bHidden)
		DDX_CHECK(IDC_READONLY, m_bReadOnly)
		DDX_CHECK(IDC_SYSTEM, m_bSystem)
		DDX_CHECK(IDC_COMPARETIMESTAMP, m_bCompareTimeStamp)
		DDX_CHECK(IDC_CONFIRMOVERWRITE, m_bConfirmOverwrite)
		DDX_CHECK(IDC_DELETEAFTERINSTALL, m_bDeleteAfterInstall)
		DDX_CHECK(IDC_FONTISNTTRUETYPE, m_bFontIsntTrueType)
		DDX_CHECK(IDC_ISREADME, m_bIsReadMe)
		DDX_CHECK(IDC_REGSERVER, m_bRegServer)
		DDX_CHECK(IDC_RESTARTREPLACE, m_bRestartReplace)
		DDX_CHECK(IDC_SHAREDFILE, m_bSharedFile)
		DDX_CHECK(IDC_UNINSNEVERUNINSTALL, m_bUninsNeverUninstall)
		DDX_CHECK(IDC_EXTERNAL, m_bExternal)
		DDX_CHECK(IDC_REGTYPELIB, m_bRegTypeLib)
		DDX_CHECK(IDC_SKIPIFSOURCEDOESNTEXIST, m_bSkipIfSourceDoesntExist)
		DDX_CHECK(IDC_OVERWRITEREADONLY, m_bOverwriteReadOnly)
		DDX_CHECK(IDC_FILES_ONLYIFDESTFILEEXISTS, m_nOnlyIfDestFileExists)
		DDX_CHECK(IDC_RECURSESUBDIRS, m_bRecurseSubDirs)
		DDX_CHECK(IDC_FILES_NOREGERROR, m_bNoRegError)
		DDX_CHECK(IDC_UNINSRESTARTDELETE, m_nUninsRestartDelete)
		// IS 3.0.5
		DDX_CHECK(IDC_PROMPTIFOLDER, m_nPromptIfOlder)
		DDX_CHECK(IDC_ONLYIFDOESNTEXIST, m_nOnlyIfDoesntExist)
		DDX_CHECK(IDC_IGNOREVERSION, m_nIgnoreVersion)
		DDX_CHECK(IDC_DONTCOPY, m_nDontCopy)
		// IS 4.0.5
		DDX_CHECK(IDC_UNINSREMOVEREADONLY, m_nUninsRemoveReadOnly)
		// IS 4.1.0
		DDX_TEXT(IDC_PERMISSIONS, m_strPermissions)
		// IS 4.1.6
		DDX_TEXT(IDC_EXCLUDES, m_strExcludes)
		DDX_CHECK(IDC_SORTFILESBYEXTENSION, m_nSortFilesByExtension)
		// IS 4.2.0
		DDX_CHECK(IDC_TOUCH, m_nTouch)
		DDX_CHECK(IDC_REPLACESAMEVERSION, m_nReplaceSameVersion)
		DDX_CHECK(IDC_NOENCRYPTION, m_nNoEncryption)
		DDX_CHECK(IDC_NOCOMPRESSION, m_nNoCompression)
		DDX_CHECK(IDC_DONTVERIFYCHECKSUM, m_nDontVerifyChecksum)
		DDX_CHECK(IDC_UNINSNOSHAREDFILEPROMPT, m_nUninsNoSharedFilePrompt)
		DDX_CHECK(IDC_CREATEALLSUBDIRS, m_nCreateAllSubdirs)
		DDX_CHECK(IDC_32BIT, m_n32Bit)
		DDX_CHECK(IDC_64BIT, m_n64Bit)
		DDX_CHECK(IDC_FILES_SOLIDBREAK, m_nSolidBreak)
		DDX_TEXT(IDC_EXTERNALSIZE, m_strExternalSize)
		DDX_CHECK(IDC_SETNTFSCOMPRESSION, m_nSetNTFSCompression)
		DDX_CHECK(IDC_UNSETNTFSCOMPRESSION, m_nUnsetNTFSCompression)
		DDX_CHECK(IDC_SORTFILESBYNAME, m_nSortFilesByName)
		DDX_CHECK(IDC_GACINSTALL, m_nGacInstall)
		DDX_TEXT(IDC_STRONGASSEMBLYNAME, m_strStrongAssemblyName)
	END_DDX_MAP()

	CEdit2		m_wndSource;
	CComboBox2	m_wndDestDir;
	Henden::CButtonFile	m_btnSource;
	CButton		m_wndFontIsntTrueType;
	CString		m_strSource;
	CString		m_strDestDir;
	CString		m_strDestName;
	CString		m_strFontInstall;
	BOOL		m_bHidden;
	BOOL		m_bReadOnly;
	BOOL		m_bSystem;
	BOOL		m_bCompareTimeStamp;
	BOOL		m_bConfirmOverwrite;
	BOOL		m_bDeleteAfterInstall;
	BOOL		m_bFontIsntTrueType;
	BOOL		m_bIsReadMe;
	BOOL		m_bRegServer;
	BOOL		m_bRestartReplace;
	BOOL		m_bSharedFile;
	BOOL		m_bUninsNeverUninstall;
	BOOL		m_bExternal;
	BOOL		m_bRegTypeLib;
	BOOL		m_bSkipIfSourceDoesntExist;
	BOOL		m_bOverwriteReadOnly;
	int			m_nOnlyIfDestFileExists;
	BOOL		m_bRecurseSubDirs;
	BOOL		m_bNoRegError;
	int			m_nUninsRestartDelete;
	int			m_nPromptIfOlder, m_nOnlyIfDoesntExist, m_nIgnoreVersion, m_nDontCopy;
	int			m_nUninsRemoveReadOnly;
	CString		m_strPermissions;
	CString		m_strExcludes, m_strExternalSize, m_strStrongAssemblyName;
	int			m_nSortFilesByExtension;
	int			m_nTouch, m_nReplaceSameVersion, m_nNoEncryption;
	int			m_nNoCompression, m_nDontVerifyChecksum;
	int			m_nUninsNoSharedFilePrompt;
	int			m_nCreateAllSubdirs, m_n32Bit, m_n64Bit, m_nSolidBreak, m_nSetNTFSCompression, m_nUnsetNTFSCompression;
	int			m_nSortFilesByName, m_nGacInstall;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
#if 1
		_L(m_hWnd,"Files");
#else
		CTransDialog trans(m_hWnd);
		trans.Translate();
#endif
		m_wndSource.SubclassWindow(GetDlgItem(IDC_SOURCE));
		m_wndDestDir.SubclassWindow(GetDlgItem(IDC_DESTDIR));
		m_btnSource.SubclassWindow(GetDlgItem(IDC_BTN_SOURCE));
		m_wndFontIsntTrueType.Attach(GetDlgItem(IDC_FONTISNTTRUETYPE));

		m_wndFontIsntTrueType.EnableWindow(!m_strFontInstall.IsEmpty());
		m_pDoc->AddDirConstants(m_wndDestDir);
		m_pDoc->AddDirs(m_wndDestDir);

		for(int nPos=0;nPos<m_listFiles.GetSize();nPos++) {
			CScriptLine* pFile = m_listFiles[nPos];

			if(m_strSource.CompareNoCase(SAFESTR(pFile->GetParameter("Source"))))
				m_strSource.Empty();

			if(m_strDestDir.CompareNoCase(SAFESTR(pFile->GetParameter("DestDir"))))
				m_strDestDir.Empty();

			if(m_strDestName.CompareNoCase(SAFESTR(pFile->GetParameter("DestName"))))
				m_strDestName.Empty();

			if(m_strFontInstall.CompareNoCase(SAFESTR(pFile->GetParameter("FontInstall"))))
				m_strFontInstall.Empty();

			if(m_strPermissions.CompareNoCase(SAFESTR(pFile->GetParameter("Permissions"))))
				m_strPermissions.Empty();

			if(m_strExcludes.CompareNoCase(SAFESTR(pFile->GetParameter("Excludes"))))
				m_strExcludes.Empty();

			if(m_strExternalSize.CompareNoCase(SAFESTR(pFile->GetParameter("ExternalSize"))))
				m_strExternalSize.Empty();

			if(m_strStrongAssemblyName.CompareNoCase(SAFESTR(pFile->GetParameter("StrongAssemblyName"))))
				m_strStrongAssemblyName.Empty();

			// Attributes
			if(m_bReadOnly != (pFile->GetParameterFlag("Attribs","readonly") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_READONLY, BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bReadOnly = 2;
			}

			if(m_bHidden != (pFile->GetParameterFlag("Attribs","hidden") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_HIDDEN,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bHidden = 2;
			}

			if(m_bSystem != (pFile->GetParameterFlag("Attribs","system") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_SYSTEM,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bSystem = 2;
			}

			// Flags
			if(m_bCompareTimeStamp != (pFile->GetParameterFlag("Flags","comparetimestamp") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_COMPARETIMESTAMP, BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bCompareTimeStamp = 2;
			}

			if(m_bConfirmOverwrite != (pFile->GetParameterFlag("Flags","confirmoverwrite") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_CONFIRMOVERWRITE,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bConfirmOverwrite = 2;
			}

			if(m_bDeleteAfterInstall != (pFile->GetParameterFlag("Flags","deleteafterinstall") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_DELETEAFTERINSTALL,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bDeleteAfterInstall = 2;
			}

			if(m_bFontIsntTrueType != (pFile->GetParameterFlag("Flags","fontisnttruetype") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_FONTISNTTRUETYPE,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bFontIsntTrueType = 2;
			}

			if(m_bIsReadMe != (pFile->GetParameterFlag("Flags","isreadme") ? TRUE : FALSE)) {
				MyModifyStyle(IDC_ISREADME,BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bIsReadMe = 2;
			}

			if(m_bRegServer != (pFile->GetParameterFlag("Flags","regserver") ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGSERVER).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRegServer = 2;
			}

			if(m_bRestartReplace != (pFile->GetParameterFlag("Flags","restartreplace") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RESTARTREPLACE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRestartReplace = 2;
			}

			if(m_bSharedFile != (pFile->GetParameterFlag("Flags","sharedfile") ? TRUE : FALSE)) {
				GetDlgItem(IDC_SHAREDFILE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bSharedFile = 2;
			}

			if(m_bUninsNeverUninstall != (pFile->GetParameterFlag("Flags","uninsneveruninstall") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSNEVERUNINSTALL).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bUninsNeverUninstall = 2;
			}

			if(m_bExternal != (pFile->GetParameterFlag("Flags","external") ? TRUE : FALSE)) {
				GetDlgItem(IDC_EXTERNAL).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bExternal = 2;
			}

			if(m_bRegTypeLib != (pFile->GetParameterFlag("Flags","regtypelib") ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGTYPELIB).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRegTypeLib = 2;
			}

			if(m_bSkipIfSourceDoesntExist != (pFile->GetParameterFlag("Flags","skipifsourcedoesntexist") ? TRUE : FALSE)) {
				GetDlgItem(IDC_SKIPIFSOURCEDOESNTEXIST).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bSkipIfSourceDoesntExist = 2;
			}

			if(m_bOverwriteReadOnly != (pFile->GetParameterFlag("Flags","overwritereadonly") ? TRUE : FALSE)) {
				GetDlgItem(IDC_OVERWRITEREADONLY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bOverwriteReadOnly = 2;
			}

			if(m_nOnlyIfDestFileExists != (pFile->GetParameterFlag("Flags","onlyifdestfileexists") ? TRUE : FALSE)) {
				GetDlgItem(IDC_FILES_ONLYIFDESTFILEEXISTS).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nOnlyIfDestFileExists = 2;
			}

			if(m_bRecurseSubDirs != (pFile->GetParameterFlag("Flags","recursesubdirs") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RECURSESUBDIRS).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRecurseSubDirs = 2;
			}

			if(m_bNoRegError != (pFile->GetParameterFlag("Flags","noregerror") ? TRUE : FALSE)) {
				GetDlgItem(IDC_FILES_NOREGERROR).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bNoRegError = 2;
			}

			if(m_nUninsRestartDelete != (pFile->GetParameterFlag("Flags","uninsrestartdelete") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSRESTARTDELETE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsRestartDelete = 2;
			}
			// IS 3.0.5
			if(m_nPromptIfOlder != (pFile->GetParameterFlag("Flags","promptifolder") ? TRUE : FALSE)) {
				GetDlgItem(IDC_PROMPTIFOLDER).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nPromptIfOlder = 2;
			}
			if(m_nOnlyIfDoesntExist != (pFile->GetParameterFlag("Flags","onlyifdoesntexist") ? TRUE : FALSE)) {
				GetDlgItem(IDC_ONLYIFDOESNTEXIST).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nOnlyIfDoesntExist = 2;
			}
			if(m_nIgnoreVersion != (pFile->GetParameterFlag("Flags","ignoreversion") ? TRUE : FALSE)) {
				GetDlgItem(IDC_IGNOREVERSION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nIgnoreVersion = 2;
			}
			if(m_nDontCopy != (pFile->GetParameterFlag("Flags","dontcopy") ? TRUE : FALSE)) {
				GetDlgItem(IDC_DONTCOPY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nDontCopy = 2;
			}
			// IS 4.0.5
			if(m_nUninsRemoveReadOnly != (pFile->GetParameterFlag("Flags","uninsremovereadonly") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSREMOVEREADONLY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsRemoveReadOnly = 2;
			}
			// IS 4.1.6
			if(m_nSortFilesByExtension != (pFile->GetParameterFlag("Flags","sortfilesbyextension") ? TRUE : FALSE)) {
				GetDlgItem(IDC_SORTFILESBYEXTENSION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSortFilesByExtension = 2;
			}
			// IS 4.2.0
			if(m_nTouch != (pFile->GetParameterFlag("Flags","touch") ? TRUE : FALSE)) {
				GetDlgItem(IDC_TOUCH).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nTouch = 2;
			}
			if(m_nReplaceSameVersion != (pFile->GetParameterFlag("Flags","replacesameversion") ? TRUE : FALSE)) {
				GetDlgItem(IDC_REPLACESAMEVERSION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nReplaceSameVersion = 2;
			}
			if(m_nNoEncryption != (pFile->GetParameterFlag("Flags","noencryption") ? TRUE : FALSE)) {
				GetDlgItem(IDC_NOENCRYPTION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nNoEncryption = 2;
			}
			if(m_nNoCompression != (pFile->GetParameterFlag("Flags","nocompression") ? TRUE : FALSE)) {
				GetDlgItem(IDC_NOCOMPRESSION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nNoCompression = 2;
			}
			if(m_nDontVerifyChecksum != (pFile->GetParameterFlag("Flags","dontverifychecksum") ? TRUE : FALSE)) {
				GetDlgItem(IDC_DONTVERIFYCHECKSUM).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nDontVerifyChecksum = 2;
			}
			if(m_nUninsNoSharedFilePrompt != (pFile->GetParameterFlag("Flags","uninsnosharedfileprompt") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSNOSHAREDFILEPROMPT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsNoSharedFilePrompt = 2;
			}
			if(m_nCreateAllSubdirs != (pFile->GetParameterFlag("Flags","createallsubdirs") ? TRUE : FALSE)) {
				GetDlgItem(IDC_CREATEALLSUBDIRS).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nCreateAllSubdirs = 2;
			}
			if(m_n32Bit != (pFile->GetParameterFlag("Flags","32bit") ? TRUE : FALSE)) {
				GetDlgItem(IDC_32BIT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_n32Bit = 2;
			}
			if(m_n64Bit != (pFile->GetParameterFlag("Flags","64bit") ? TRUE : FALSE)) {
				GetDlgItem(IDC_64BIT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_n64Bit = 2;
			}
			if(m_nSolidBreak != (pFile->GetParameterFlag("Flags","solidbreak") ? TRUE : FALSE)) {
				GetDlgItem(IDC_FILES_SOLIDBREAK).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSolidBreak = 2;
			}
			if(m_nSetNTFSCompression != (pFile->GetParameterFlag("Flags","setntfscompression") ? TRUE : FALSE)) {
				GetDlgItem(IDC_SETNTFSCOMPRESSION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSetNTFSCompression = 2;
			}
			if(m_nUnsetNTFSCompression != (pFile->GetParameterFlag("Flags","unsetntfscompression") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNSETNTFSCOMPRESSION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUnsetNTFSCompression = 2;
			}
			if(m_nSortFilesByName != (pFile->GetParameterFlag("Flags","sortfilesbyname") ? TRUE : FALSE)) {
				GetDlgItem(IDC_SORTFILESBYNAME).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSortFilesByName = 2;
			}
			if(m_nGacInstall != (pFile->GetParameterFlag("Flags","gacinstall") ? TRUE : FALSE)) {
				GetDlgItem(IDC_GACINSTALL).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nGacInstall = 2;
			}
		}
		GetDlgItem(IDC_EXTERNALSIZE).EnableWindow(m_bExternal);
		GetDlgItem(IDC_STATIC_EXTERNALSIZE).EnableWindow(m_bExternal);
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listFiles.GetSize()==1;

		if(bForce) {
			if(m_strSource.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("You must enter a source."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				GetDlgItem(IDC_SOURCE).SetFocus();
				return PSNRET_INVALID;
			}
			if(m_strDestDir.IsEmpty() && !m_nDontCopy) {
				AtlMessageBox(m_hWnd,_L("You must enter a destination directory."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				GetDlgItem(IDC_DESTDIR).SetFocus();
				return PSNRET_INVALID;
			}

			CString strSourceDir;
			if(!m_pDoc->GetUseAbsolutePaths() && m_pDoc->GetSourceDir(strSourceDir)) {
				CString strSource = CFunc::ExtractRelativePath(strSourceDir,m_strSource);
				if(!strSource.IsEmpty()) {
					m_strSource = strSource;
					DoDataExchange(DDX_LOAD,IDC_SOURCE);
				}
			}
		}

	#ifdef TODO
		if((bForce || !m_strSource.IsEmpty()) && m_bExternal!=1) {
			if(!CMyUtils::IsFile(m_strSource))
				if(AfxMessageBox(m_strSource+" doesn't exist. Continue?",MB_YESNO|MB_ICONQUESTION)!=IDYES) 
					return PSNRET_INVALID;
		}
	#endif

		for(int nPos=0;nPos<m_listFiles.GetSize();nPos++) {
			CScriptLine* pFile = m_listFiles[nPos];

			CString strOld, strNew;
			CInnoScriptEx::GetDestName(pFile,strOld);

			CInnoScriptEx::SetString(pFile,bForce,"Source",m_strSource);
			CInnoScriptEx::SetString(pFile,bForce,"DestDir",m_strDestDir);
			CInnoScriptEx::SetString(pFile,bForce,"DestName",m_strDestName);

			CInnoScriptEx::GetDestName(pFile,strNew);
			// If destination file has changed, check if references should be changed also
			if(strNew.CompareNoCase(strOld)) m_pDoc->RenameFile(m_hWnd,strOld,strNew);

			CInnoScriptEx::SetString(pFile,bForce,"FontInstall",m_strFontInstall);
			CInnoScriptEx::SetString(pFile,bForce,"Permissions",m_strPermissions);
			CInnoScriptEx::SetString(pFile,bForce,"Excludes",m_strExcludes);
			CInnoScriptEx::SetString(pFile,bForce,"ExternalSize",m_strExternalSize);
			CInnoScriptEx::SetString(pFile,bForce,"StrongAssemblyName",m_strStrongAssemblyName);

			// Attribs
			CInnoScriptEx::SetFlag(pFile,"Attribs","readonly",m_bReadOnly);
			CInnoScriptEx::SetFlag(pFile,"Attribs","hidden",m_bHidden);
			CInnoScriptEx::SetFlag(pFile,"Attribs","system",m_bSystem);

			// Flags
			CInnoScriptEx::SetFlag(pFile,"Flags","comparetimestamp",m_bCompareTimeStamp);
			CInnoScriptEx::SetFlag(pFile,"Flags","confirmoverwrite",m_bConfirmOverwrite);
			CInnoScriptEx::SetFlag(pFile,"Flags","deleteafterinstall",m_bDeleteAfterInstall);
			CInnoScriptEx::SetFlag(pFile,"Flags","fontisnttruetype",m_bFontIsntTrueType);
			CInnoScriptEx::SetFlag(pFile,"Flags","isreadme",m_bIsReadMe);
			CInnoScriptEx::SetFlag(pFile,"Flags","regserver",m_bRegServer);
			CInnoScriptEx::SetFlag(pFile,"Flags","restartreplace",m_bRestartReplace);
			CInnoScriptEx::SetFlag(pFile,"Flags","sharedfile",m_bSharedFile);
			CInnoScriptEx::SetFlag(pFile,"Flags","uninsneveruninstall",m_bUninsNeverUninstall);
			CInnoScriptEx::SetFlag(pFile,"Flags","external",m_bExternal);
			CInnoScriptEx::SetFlag(pFile,"Flags","regtypelib",m_bRegTypeLib);
			CInnoScriptEx::SetFlag(pFile,"Flags","skipifsourcedoesntexist",m_bSkipIfSourceDoesntExist);
			CInnoScriptEx::SetFlag(pFile,"Flags","overwritereadonly",m_bOverwriteReadOnly);
			CInnoScriptEx::SetFlag(pFile,"Flags","onlyifdestfileexists",m_nOnlyIfDestFileExists);
			CInnoScriptEx::SetFlag(pFile,"Flags","recursesubdirs",m_bRecurseSubDirs);
			CInnoScriptEx::SetFlag(pFile,"Flags","noregerror",m_bNoRegError);
			CInnoScriptEx::SetFlag(pFile,"Flags","uninsrestartdelete",m_nUninsRestartDelete);
			CInnoScriptEx::SetFlag(pFile,"Flags","promptifolder",m_nPromptIfOlder);
			CInnoScriptEx::SetFlag(pFile,"Flags","onlyifdoesntexist",m_nOnlyIfDoesntExist);
			CInnoScriptEx::SetFlag(pFile,"Flags","ignoreversion",m_nIgnoreVersion);
			CInnoScriptEx::SetFlag(pFile,"Flags","dontcopy",m_nDontCopy);
			CInnoScriptEx::SetFlag(pFile,"Flags","uninsremovereadonly",m_nUninsRemoveReadOnly);
			CInnoScriptEx::SetFlag(pFile,"Flags","sortfilesbyextension",m_nSortFilesByExtension);
			CInnoScriptEx::SetFlag(pFile,"Flags","touch",m_nTouch);
			CInnoScriptEx::SetFlag(pFile,"Flags","replacesameversion",m_nReplaceSameVersion);
			CInnoScriptEx::SetFlag(pFile,"Flags","noencryption",m_nNoEncryption);
			CInnoScriptEx::SetFlag(pFile,"Flags","nocompression",m_nNoCompression);
			CInnoScriptEx::SetFlag(pFile,"Flags","dontverifychecksum",m_nDontVerifyChecksum);
			CInnoScriptEx::SetFlag(pFile,"Flags","uninsnosharedfileprompt",m_nUninsNoSharedFilePrompt);
			CInnoScriptEx::SetFlag(pFile,"Flags","createallsubdirs",m_nCreateAllSubdirs);
			CInnoScriptEx::SetFlag(pFile,"Flags","32bit",m_n32Bit);
			CInnoScriptEx::SetFlag(pFile,"Flags","64bit",m_n64Bit);
			CInnoScriptEx::SetFlag(pFile,"Flags","solidbreak",m_nSolidBreak);
			CInnoScriptEx::SetFlag(pFile,"Flags","setntfscompression",m_nSetNTFSCompression);
			CInnoScriptEx::SetFlag(pFile,"Flags","unsetntfscompression",m_nUnsetNTFSCompression);
			CInnoScriptEx::SetFlag(pFile,"Flags","sortfilesbyname",m_nSortFilesByName);
			CInnoScriptEx::SetFlag(pFile,"Flags","gacinstall",m_nGacInstall);
		}

		if(m_bNew)
			CFilesHelper(m_pDoc).AutoComponentSelect(m_listFiles);
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnChangeFontInstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_FONTINSTALL);
		m_wndFontIsntTrueType.EnableWindow(!m_strFontInstall.IsEmpty());
		return 0;
	}

	LRESULT OnExternal(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		GetDlgItem(IDC_EXTERNALSIZE).EnableWindow(m_bExternal);
		GetDlgItem(IDC_STATIC_EXTERNALSIZE).EnableWindow(m_bExternal);
		return 0;
	}

	CString m_strTitle;
	CPageFiles::CPageFiles(CScriptList& listFiles,CMyDoc* pDoc,const bool bNew) : 
		m_listFiles(listFiles), m_pDoc(pDoc), m_bNew(bNew), m_wndDestDir(pDoc), m_wndSource(pDoc)
	{
		m_strTitle = _L("DialogTitles|File","File");
		SetTitle((LPCTSTR)m_strTitle);

		m_bHidden = FALSE;
		m_bReadOnly = FALSE;
		m_bSystem = FALSE;
		m_bCompareTimeStamp = FALSE;
		m_bConfirmOverwrite = FALSE;
		m_bDeleteAfterInstall = FALSE;
		m_bFontIsntTrueType = FALSE;
		m_bIsReadMe = FALSE;
		m_bRegServer = FALSE;
		m_bRestartReplace = FALSE;
		m_bSharedFile = FALSE;
		m_bUninsNeverUninstall = FALSE;
		m_bExternal = FALSE;
		m_bRegTypeLib = FALSE;
		m_bSkipIfSourceDoesntExist = FALSE;
		m_bOverwriteReadOnly = FALSE;
		m_nOnlyIfDestFileExists = 0;
		m_bRecurseSubDirs = FALSE;
		m_bNoRegError = FALSE;
		m_nUninsRestartDelete = FALSE;
		m_nPromptIfOlder = 0;
		m_nOnlyIfDoesntExist = 0;
		m_nIgnoreVersion = 0;
		m_nDontCopy = 0;
		m_nUninsRemoveReadOnly = 0;
		m_nSortFilesByExtension = 0;
		m_nTouch = 0;
		m_nReplaceSameVersion = 0;
		m_nNoEncryption = 0;
		m_nNoCompression = 0;
		m_nDontVerifyChecksum = 0;
		m_nUninsNoSharedFilePrompt = 0;
		m_nCreateAllSubdirs = 0;
		m_n32Bit = 0;
		m_n64Bit = 0;
		m_nSolidBreak = 0;
		m_nSetNTFSCompression = 0;
		m_nUnsetNTFSCompression = 0;
		m_nSortFilesByName = 0;
		m_nGacInstall = 0;

		for(int nPos=0;nPos<m_listFiles.GetSize();nPos++) {
			CScriptLine* pFile = m_listFiles[nPos];
			m_strSource = pFile->GetParameter("Source");
			m_strDestDir = pFile->GetParameter("DestDir");
			m_strDestName = pFile->GetParameter("DestName");
			m_strFontInstall = pFile->GetParameter("FontInstall");
			m_strPermissions = pFile->GetParameter("Permissions");
			m_strExcludes = pFile->GetParameter("Excludes");
			m_strExternalSize = pFile->GetParameter("ExternalSize");
			m_strStrongAssemblyName = pFile->GetParameter("StrongAssemblyName");

			if(pFile->GetParameterFlag("Attribs","readonly")) m_bReadOnly = TRUE;
			if(pFile->GetParameterFlag("Attribs","hidden")) m_bHidden = TRUE;
			if(pFile->GetParameterFlag("Attribs","system")) m_bSystem = TRUE;

			if(pFile->GetParameterFlag("Flags","comparetimestamp")) m_bCompareTimeStamp = TRUE;
			if(pFile->GetParameterFlag("Flags","confirmoverwrite")) m_bConfirmOverwrite = TRUE;
			if(pFile->GetParameterFlag("Flags","deleteafterinstall")) m_bDeleteAfterInstall = TRUE;
			if(pFile->GetParameterFlag("Flags","fontisnttruetype")) m_bFontIsntTrueType = TRUE;
			if(pFile->GetParameterFlag("Flags","isreadme")) m_bIsReadMe = TRUE;
			if(pFile->GetParameterFlag("Flags","regserver")) m_bRegServer = TRUE;
			if(pFile->GetParameterFlag("Flags","restartreplace")) m_bRestartReplace = TRUE;
			if(pFile->GetParameterFlag("Flags","sharedfile")) m_bSharedFile = TRUE;
			if(pFile->GetParameterFlag("Flags","uninsneveruninstall")) m_bUninsNeverUninstall = TRUE;
			if(pFile->GetParameterFlag("Flags","external")) m_bExternal = TRUE;
			if(pFile->GetParameterFlag("Flags","regtypelib")) m_bRegTypeLib = TRUE;
			if(pFile->GetParameterFlag("Flags","skipifsourcedoesntexist")) m_bSkipIfSourceDoesntExist = TRUE;
			if(pFile->GetParameterFlag("Flags","overwritereadonly")) m_bOverwriteReadOnly = TRUE;
			if(pFile->GetParameterFlag("Flags","onlyifdestfileexists")) m_nOnlyIfDestFileExists = 1;
			if(pFile->GetParameterFlag("Flags","recursesubdirs")) m_bRecurseSubDirs = 1;
			if(pFile->GetParameterFlag("Flags","noregerror")) m_bNoRegError = TRUE;
			if(pFile->GetParameterFlag("Flags","uninsrestartdelete")) m_nUninsRestartDelete = 1;
			if(pFile->GetParameterFlag("Flags","promptifolder")) m_nPromptIfOlder = 1;
			if(pFile->GetParameterFlag("Flags","onlyifdoesntexist")) m_nOnlyIfDoesntExist = 1;
			if(pFile->GetParameterFlag("Flags","ignoreversion")) m_nIgnoreVersion = 1;
			if(pFile->GetParameterFlag("Flags","dontcopy")) m_nDontCopy = 1;
			if(pFile->GetParameterFlag("Flags","uninsremovereadonly")) m_nUninsRemoveReadOnly = 1;
			if(pFile->GetParameterFlag("Flags","sortfilesbyextension")) m_nSortFilesByExtension = 1;
			if(pFile->GetParameterFlag("Flags","touch")) m_nTouch = 1;
			if(pFile->GetParameterFlag("Flags","replacesameversion")) m_nReplaceSameVersion = 1;
			if(pFile->GetParameterFlag("Flags","noencryption")) m_nNoEncryption = 1;
			if(pFile->GetParameterFlag("Flags","nocompression")) m_nNoCompression = 1;
			if(pFile->GetParameterFlag("Flags","dontverifychecksum")) m_nDontVerifyChecksum = 1;
			if(pFile->GetParameterFlag("Flags","uninsnosharedfileprompt")) m_nUninsNoSharedFilePrompt = 1;
			if(pFile->GetParameterFlag("Flags","createallsubdirs")) m_nCreateAllSubdirs = 1;
			if(pFile->GetParameterFlag("Flags","32bit")) m_n32Bit = 1;
			if(pFile->GetParameterFlag("Flags","64bit")) m_n64Bit = 1;
			if(pFile->GetParameterFlag("Flags","solidbreak")) m_nSolidBreak = 1;
			if(pFile->GetParameterFlag("Flags","setntfscompression")) m_nSetNTFSCompression = 1;
			if(pFile->GetParameterFlag("Flags","unsetntfscompression")) m_nUnsetNTFSCompression = 1;
			if(pFile->GetParameterFlag("Flags","sortfilesbyname")) m_nSortFilesByName = 1;
			if(pFile->GetParameterFlag("Flags","gacinstall")) m_nGacInstall = 1;
			break;
		}
	}

	CScriptList&	m_listFiles;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_SOURCE,_L("Help|Files|Source","The name of the source file. The compiler will prepend the path of your installation's source directory if you do not specify a fully qualified pathname.\r\n\r\nThis can be a wildcard to specify a group of files in a single entry. When a wildcard is used, all files matching it use the same options.\r\n\r\nWhen the flag external is specified, Source must be the full pathname of an existing file (or wildcard) on the distribution media or the user's system (e.g. \"{src}\\license.ini\").\r\n\r\nConstants may only be used when the external flag is specified, because the compiler does not do any constant translating itself."))
		TOOLTIP_HANDLER(IDC_BTN_SOURCE,_L("Help|Files|Source","Click this button to select source file."))
		TOOLTIP_HANDLER(IDC_DESTDIR,_L("Help|Files|DestDir","The directory where the file is to be installed on the user's system. The will almost always begin with one of the directory constants. If the specified path does not already exist on the user's system, it will be created automatically, and removed automatically during uninstallation if empty."))
		TOOLTIP_HANDLER(IDC_FONTINSTALL,_L("Help|Files|FontInstall","Tells Setup the file is a font that needs to be installed. The value of this parameter is the name of the font as stored in the registry or WIN.INI. This must be exactly the same name as you see when you double-click the font file in Explorer. Note that Setup will automatically append \" (TrueType)\" to the end of the name.\r\n\r\nIf the file is not a TrueType font, you must specify the flag fontisnttruetype in the Flags parameter.\r\n\r\nIt's recommended that you use the flags onlyifdoesntexist and uninsneveruninstall when installing fonts to the {fonts} directory.\r\n\r\nTo successfully install a font on Windows 2000/XP, the user must be a member of the Power Users or Administrators groups. On Windows NT 4.0 and earlier, anyone can install a font."))
		TOOLTIP_HANDLER(IDC_READONLY,_L("Help|Files|ReadOnly","Specifies additional attributes for the file."))
		TOOLTIP_HANDLER(IDC_EXTERNAL,_L("Help|Files|External","This flag instructs Inno Setup not to statically compile the file specified by the Source parameter into the installation files, but instead copy from an existing file on the distribution media or the user's system. See the Source parameter description for more information."))
		TOOLTIP_HANDLER(IDC_HIDDEN,_L("Help|Files|Hidden","Specifies additional attributes for the file."))
		TOOLTIP_HANDLER(IDC_SYSTEM,_L("Help|Files|System","Specifies additional attributes for the file."))
		TOOLTIP_HANDLER(IDC_DESTNAME,_L("Help|Files|DestName","This parameter specifies a new name for the file when it is installed on the user's system. By default, Setup uses the name from the Source parameter, so in most cases it's not necessary to specify this parameter."))
		TOOLTIP_HANDLER(IDC_FONTISNTTRUETYPE,_L("Help|Files|FontIsntTrueType","Specify this flag if the entry is installing a non-TrueType font with the FontInstall parameter."))
		TOOLTIP_HANDLER(IDC_IGNOREVERSION,_L("Help|Files|IgnoreVersion","Don't compare version info at all; replace existing files regardless of their version number.\r\n\r\nThis flag should only be used on files private to your application, never on shared system files."))
		TOOLTIP_HANDLER(IDC_DONTCOPY,_L("Help|Files|DontCopy","Don't copy the file to the user's system. This flag is useful if the file is handled by the [Code] section exclusively."))
		TOOLTIP_HANDLER(IDC_UNINSREMOVEREADONLY,_L("Help|Files|UninsRemoveReadOnly","When uninstalling the file, remove any read-only attribute from the file before attempting to delete it."))
		TOOLTIP_HANDLER(IDC_TOUCH,_L("Help|Files|Touch","This flag causes Setup to set the time/date stamp of the installed file(s) to that which is specified by the TouchDate and TouchTime [Setup] section directives.\r\n\r\nThis flag has no effect if combined with the external flag."))
		TOOLTIP_HANDLER(IDC_REPLACESAMEVERSION,_L("Help|Files|ReplaceSameVersion","When this flag is used and the file already exists on the user's system and it has the same version number as the file being installed, Setup will compare the files and replace the existing file if their contents differ.\r\n\r\nThe default behavior (i.e. when this flag isn't used) is to not replace an existing file with the same version number."))
		TOOLTIP_HANDLER(IDC_PERMISSIONS,_L("Help|Files|Permissions","Specifies additional permissions to grant in the file's ACL (access control list). It is not recommended that you use this parameter if you aren't familiar with ACLs or why you would need to change them, because misusing it could negatively impact system security."))
		TOOLTIP_HANDLER(IDC_EXCLUDES,_L("Help|Files|Excludes","Specifies a list of patterns to exclude, separated by commas. This parameter cannot be combined with the external flag."))
		TOOLTIP_HANDLER(IDC_SORTFILESBYEXTENSION,_L("Help|Files|SortFilesByExtension","This flag instructs the compiler to compress the found files sorted by extension befores it sorts by path name. This potentially decreases the size of Setup if SolidCompression is also used."))
		TOOLTIP_HANDLER(IDC_COMPARETIMESTAMP,_L("Help|Files|CompareTimeStamp","(Not recommended, see Inno Setup help file)\r\nInstructs Setup to proceed to comparing time stamps if the file being installed already exists on the user's system."))
		TOOLTIP_HANDLER(IDC_CONFIRMOVERWRITE,_L("Help|Files|ConfirmOverwrite","Always ask the user to confirm before replacing an existing file."))
		TOOLTIP_HANDLER(IDC_DELETEAFTERINSTALL,_L("Help|Files|DeleteAfterInstall","Instructs Setup to install the file as usual, but then delete it once the installation is completed (or aborted). This can be useful for extracting temporary data needed by a program executed in the script's [Run] section."))
		TOOLTIP_HANDLER(IDC_ISREADME,_L("Help|Files|IsReadme","File is the \"README\" file. Only one file in an installation can have this flag. When a file has this flag, the user will asked if he/she would like to view the README file after the installation has completed. If Yes is chosen, Setup will open the file, using the default program for the file type. For this reason, the README file should always end with an extension like .txt, .wri, or .doc."))
		TOOLTIP_HANDLER(IDC_REGSERVER,_L("Help|Files|RegServer","Register the OLE server (a.k.a. ActiveX control). With this flag set, Setup will locate and execute the DLL/OCX's DllRegisterServer export. The uninstaller calls DllUnregisterServer. When used in combination with sharedfile, the DLL/OCX will only be unregistered when the reference count reaches zero."))
		TOOLTIP_HANDLER(IDC_RESTARTREPLACE,_L("Help|Files|RestartReplace","This flag is generally useful when replacing core system files. If the file existed beforehand and was found to be locked resulting in Setup being unable to replace it, Setup will register the file (either in WININIT.INI or by using MoveFileEx, for Windows and Windows NT respectively) to be replaced the next time the system is restarted. When this happens, the user will be prompted to restart the computer at the end of the installation process."))
		TOOLTIP_HANDLER(IDC_REGTYPELIB,_L("Help|Files|RegTypeLib","Register the type library (.tlb). The uninstaller will unregister the type library (unless the flag uninsneveruninstall is specified). As with the regserver flag, when used in combination with sharedfile, the file will only be unregistered by the uninstaller when the reference count reaches zero."))
		TOOLTIP_HANDLER(IDC_SKIPIFSOURCEDOESNTEXIST,_L("Help|Files|SkipIfSourceDoesntExist","This flag instructs the compiler -- or Setup, if the external flag is also used -- to silently skip over the entry if the source file does not exist, instead of displaying an error message."))
		TOOLTIP_HANDLER(IDC_FILES_NOREGERROR,_L("Help|Files|NoRegError","When combined with either the regserver or regtypelib flags, Setup will not display any error message if the registration fails."))
		TOOLTIP_HANDLER(IDC_UNINSRESTARTDELETE,_L("Help|Files|UninsRestartDelete","When this flag is used and the file is in use at uninstall time, the uninstaller will queue the file to be deleted when the system is restarted, and at the end of the uninstallation process ask the user if he/she wants to restart. This flag can be useful when uninstalling things like shell extensions which cannot be programmatically stopped. Note that administrative privileges are required on Windows NT/2000/XP for this flag to have an effect."))
		TOOLTIP_HANDLER(IDC_SHAREDFILE,_L("Help|Files|SharedFile","Uses Windows' shared file counting feature (located in the registry at HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\SharedDLLs). This enables a file to be shared between applications, without worrying about it being inadvertently removed. Each time the file is installed, the reference count for the file is incremented. When an application using the file is uninstalled, the reference count is decremented. If the count reaches zero, the file is deleted (with the user's confirmation)."))
		TOOLTIP_HANDLER(IDC_UNINSNEVERUNINSTALL,_L("Help|Files|UninsNeverUninstall","Never uninstall this file. This flag should be used sparingly, and is usually used in combination with the onlyifdoesntexist flag. It is meant to be used when installing a very common shared file, such as CTL3DV2.DLL or an MFC DLL, because you wouldn't want the uninstaller to delete the file since other applications make use of it also."))
		TOOLTIP_HANDLER(IDC_OVERWRITEREADONLY,_L("Help|Files|OverwriteReadOnly","Always overwrite a read-only file. Without this flag, Setup will ask the user if an existing read-only file should be overwritten."))
		TOOLTIP_HANDLER(IDC_FILES_ONLYIFDESTFILEEXISTS,_L("Help|Files|OnlyIfDestFileExists","Only install the file if a file of the same name already exists on the user's system. This flag may be useful if your installation is a patch to an existing installation, and you don't want files to be installed that the user didn't already have."))
		TOOLTIP_HANDLER(IDC_RECURSESUBDIRS,_L("Help|Files|RecurseSubDirs","Instructs the compiler or Setup to also search for the Source filename/wildcard in subdirectories under the Source directory."))
		TOOLTIP_HANDLER(IDC_PROMPTIFOLDER,_L("Help|Files|PromptIfOlder","By default, when a file being installed has an older version number (or older time stamp, when the comparetimestamp flag is used) than an existing file, Setup will not replace the existing file. (See the Remarks section at the bottom of this topic for more details.) When this flag is used, Setup will ask the user whether the file should be replaced, with the default answer being to keep the existing file."))
		TOOLTIP_HANDLER(IDC_ONLYIFDOESNTEXIST,_L("Help|Files|OnlyIfDoesntExist","Only install the file if it doesn't already exist on the user's system."))
		TOOLTIP_HANDLER(IDC_NOENCRYPTION,_L("Help|Files|NoEncryption","Prevents the file from being stored encrypted. Use this flag if you have enabled encryption (using the [Setup] section directive Encryption) but want to be able to extract the file using the [Code] section support function ExtractTemporaryFile before the user has entered the correct password."))
		TOOLTIP_HANDLER(IDC_NOCOMPRESSION,_L("Help|Files|NoCompression","Prevents the compiler from attempting to compress the file. Use this flag on file types that you know can't benefit from compression (for example, JPEG images) to speed up the compilation process and save a few bytes in the resulting installation."))
		TOOLTIP_HANDLER(IDC_DONTVERIFYCHECKSUM,_L("Help|Files|DontVerifyChecksum","Prevents Setup from verifying the file checksum after extraction. Use this flag on files you wish to modify while already compiled into Setup.\r\n\r\nMust be combined with nocompression."))
		TOOLTIP_HANDLER(IDC_UNINSNOSHAREDFILEPROMPT,_L("Help|Files|UninsNoSharedFilePrompt","Tells the uninstaller to automatically remove the shared file when its reference count reaches zero instead of asking the user. Must be combined with the sharedfile flag to have an effect."))
		TOOLTIP_HANDLER(IDC_CREATEALLSUBDIRS,_L("Help|Files|CreateAllSubdirs","By default the compiler skips empty directories when it recurses subdirectories searching for the Source filename/wildcard. The createallsubdirs flag causes these directories to be created at install time (just like if you created [Dirs] entries for them). Must be combined with recursesubdirs."))
		TOOLTIP_HANDLER(IDC_32BIT,_L("Help|Files|32Bit","Causes the {sys} constant to map to the 32-bit System directory when used in the Source and DestDir parameters, the regserver and regtypelib flags to treat the file as 32-bit, and the sharedfile flag to update the 32-bit SharedDLLs registry key. This is the default behavior in a 32-bit mode install."))
		TOOLTIP_HANDLER(IDC_64BIT,_L("Help|Files|64Bit","Causes the {sys} constant to map to the 64-bit System directory when used in the Source and DestDir parameters, the regserver and regtypelib flags to treat the file as 64-bit, and the sharedfile flag to update the 64-bit SharedDLLs registry key. This is the default behavior in a 64-bit mode install."))
		TOOLTIP_HANDLER(IDC_FILES_SOLIDBREAK,_L("Help|Files|SolidBreak","When solid compression is enabled, this flag instructs the compiler to finalize the current compression stream and begin a new one before compressing the file(s) matched by Source. This allows Setup to seek to the file instantly without having to decompress any preceding files first. May be useful in a large, multi-component installation if you find too much time is being spent decompressing files belonging to components that weren't selected."))
		TOOLTIP_HANDLER(IDC_EXTERNALSIZE,_L("Help|Files|ExternalSize","This parameter must be combined with the external flag and specifies the size of the external file in bytes. If this parameter is not specified, Setup retrieves the file size at startup. Primarily useful for files that aren't available at startup, for example files located on a second disk when disk spanning is being used."))
		TOOLTIP_HANDLER(IDC_SETNTFSCOMPRESSION,_L("Help|Files|SetNTFSCompression","Instructs Setup to enable NTFS compression on the file (even if it didn't replace the file). If it fails to set the compression state for any reason (for example, if compression is not supported by the file system), no error message will be displayed."))
		TOOLTIP_HANDLER(IDC_UNSETNTFSCOMPRESSION,_L("Help|Files|UnsetNTFSCompression","Instructs Setup to disable NTFS compression on the file (even if it didn't replace the file). If it fails to set the compression state for any reason (for example, if compression is not supported by the file system), no error message will be displayed."))
		TOOLTIP_HANDLER(IDC_SORTFILESBYNAME,_L("Help|Files|SortFilesByName","?"))
		TOOLTIP_HANDLER(IDC_GACINSTALL,_L("Help|Files|GacInstall","?"))
		TOOLTIP_HANDLER(IDC_STRONGASSEMBLYNAME,_L("Help|Files|StrongAssemblyName","?"))
	END_TOOLTIP_MAP()
};
