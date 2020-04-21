// SetupCompiler.h : header file

#pragma once

#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupCompiler dialog
class CSetupCompiler : 
	public CPropertyPageImpl<CSetupCompiler>,
	public CMyPropertyPageBase<CSetupCompiler>,
	public CWinDataExchange<CSetupCompiler>,
	public Henden::CToolTipDialog<CSetupCompiler>
{
public:
	enum { IDD = IDD_SETUP_COMPILER };

	BEGIN_MSG_MAP(CSetupCompiler)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupCompiler>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupCompiler>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupCompiler>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_CHECKUSESETUPLDR, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPRESSION, CBN_SELCHANGE, OnSelChangeCompression)
		COMMAND_HANDLER(IDC_MERGEDUPLICATEFILES, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPRESSLEVEL, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_INTERNALCOMPRESSLEVEL, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_OUTPUTDIR, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SOURCEDIR, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_OUTPUTBASEFILENAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSTALLLOGMODE, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_SOLIDCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_VERSIONINFOVERSION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_VERSIONINFOCOMPANY, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_VERSIONINFODESCRIPTION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_VERSIONINFOTEXTVERSION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_ENCRYPTION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_OUTPUTMANIFESTFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_ARCHITECTURESINSTALLIN64BITMODE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_ARCHITECTURESALLOWED, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_VERSIONINFOCOPYRIGHT, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SETUPLOGGING, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPRESSIONTHREADS, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_VERSIONINFOPRODUCTNAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_VERSIONINFOPRODUCTVERSION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SIGNTOOL, EN_CHANGE, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupCompiler)
		DDX_CONTROL(IDC_BTN_SOURCE_DIR, m_btnSourceDir)
		DDX_CONTROL(IDC_BTN_OUTPUT_DIR, m_btnOutputDir)
		DDX_CONTROL(IDC_OUTPUTMANIFESTFILE_BUTTON, m_wndOutputManifestFile)
		DDX_CHECK(IDC_CHECKUSESETUPLDR, m_bUseSetupLdr)
		if(m_nCompression!=2) {
			DDX_INT(IDC_COMPRESSLEVEL, m_nCompressLevel)
		} else {
			DDX_CBINDEX(IDC_COMPRESSLEVELCOMBO, m_nCompressLevel)
		}
		DDX_TEXT(IDC_OUTPUTDIR, m_strOutputDir)
		DDX_TEXT(IDC_SOURCEDIR, m_strSourceDir)
		DDX_TEXT(IDC_OUTPUTBASEFILENAME, m_strOutputBaseFilename)
		DDX_CHECK(IDC_MERGEDUPLICATEFILES, m_bMergeDuplicateFiles)
		DDX_CBINDEX(IDC_COMPRESSION, m_nCompression)
		DDX_CBINDEX(IDC_INTERNALCOMPRESSLEVEL, m_nInternalCompressLevel)
		DDX_CHECK(IDC_SOLIDCOMPRESSION, m_bSolidCompression)
		DDX_TEXT(IDC_VERSIONINFOVERSION, m_strVersionInfoVersion)
		DDX_TEXT(IDC_VERSIONINFOCOMPANY, m_strVersionInfoCompany)
		DDX_TEXT(IDC_VERSIONINFODESCRIPTION, m_strVersionInfoDescription)
		DDX_TEXT(IDC_VERSIONINFOTEXTVERSION, m_strVersionInfoTextVersion)
		DDX_CHECK(IDC_ENCRYPTION, m_bEncryption)
		DDX_TEXT(IDC_OUTPUTMANIFESTFILE, m_strOutputManifestFile)
		DDX_TEXT(IDC_ARCHITECTURESINSTALLIN64BITMODE, m_strArchitecturesInstallIn64BitMode)
		DDX_TEXT(IDC_ARCHITECTURESALLOWED, m_strArchitecturesAllowed)
		DDX_TEXT(IDC_VERSIONINFOCOPYRIGHT, m_strVersionInfoCopyright)
		DDX_CHECK(IDC_SETUPLOGGING, m_bSetupLogging)
		DDX_TEXT(IDC_COMPRESSIONTHREADS, m_strCompressionThreads)
		DDX_TEXT(IDC_VERSIONINFOPRODUCTNAME, m_strVersionInfoProductName)
		DDX_TEXT(IDC_VERSIONINFOPRODUCTVERSION, m_strVersionInfoProductVersion)
		DDX_TEXT(IDC_SIGNTOOL, m_strSignTool)
	END_DDX_MAP()

	CEdit			m_wndCompressLevel;
	CComboBox		m_wndCompression, m_wndCompressLevelCombo, m_wndInternalCompressLevel;
	Henden::CButtonFolder	m_btnSourceDir;
	Henden::CButtonFolder	m_btnOutputDir;
	CUpDownCtrl		m_wndSpinCompressLevel;
	BOOL			m_bUseSetupLdr;
	UINT			m_nCompressLevel;
	CString			m_strOutputDir;
	CString			m_strSourceDir;
	CString			m_strOutputBaseFilename;
	BOOL			m_bMergeDuplicateFiles, m_bEncryption, m_bSetupLogging;
	CString			m_strMessagesFile;
	int				m_nCompression;
	int				m_nInternalCompressLevel;
	BOOL			m_bSolidCompression;
	CString			m_strVersionInfoVersion, m_strVersionInfoCompany, m_strVersionInfoDescription, m_strVersionInfoTextVersion;
	CString			m_strOutputManifestFile;
	Henden::CButtonFile	m_wndOutputManifestFile;
	CString			m_strArchitecturesInstallIn64BitMode, m_strArchitecturesAllowed, m_strVersionInfoCopyright;
	CString			m_strCompressionThreads, m_strVersionInfoProductName, m_strVersionInfoProductVersion, m_strSignTool;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Compiler");
		m_wndCompressLevel.Attach(GetDlgItem(IDC_COMPRESSLEVEL));
		m_wndCompressLevelCombo.Attach(GetDlgItem(IDC_COMPRESSLEVELCOMBO));
		m_wndInternalCompressLevel.Attach(GetDlgItem(IDC_INTERNALCOMPRESSLEVEL));
		m_wndCompression.Attach(GetDlgItem(IDC_COMPRESSION));
		m_wndSpinCompressLevel.Attach(GetDlgItem(IDC_SPINCOMPRESSLEVEL));

		m_wndCompression.AddString("zip");
		m_wndCompression.AddString("bzip");
		m_wndCompression.AddString("lzma");
		m_wndCompression.AddString("none");

		m_wndCompressLevelCombo.AddString("fast");
		m_wndCompressLevelCombo.AddString("normal");
		m_wndCompressLevelCombo.AddString("max");
		m_wndCompressLevelCombo.AddString("ultra");
		m_wndCompressLevelCombo.AddString("ultra64");

		m_wndSpinCompressLevel.SetRange(1,9);

		m_wndInternalCompressLevel.AddString("none");
		m_wndInternalCompressLevel.AddString("fast");
		m_wndInternalCompressLevel.AddString("normal");
		m_wndInternalCompressLevel.AddString("max");
		m_wndInternalCompressLevel.AddString("ultra");
		m_wndInternalCompressLevel.AddString("ultra64");

		m_wndCompressLevel.EnableWindow(m_nCompression<2);
		m_wndSpinCompressLevel.EnableWindow(m_nCompression<2);
		GetDlgItem(IDC_STATIC1).EnableWindow(m_nCompression<3);
		GetDlgItem(IDC_SOLIDCOMPRESSION).EnableWindow(m_nCompression<3);

		m_wndCompressLevel.ShowWindow(m_nCompression==2 ? SW_HIDE : SW_SHOW);
		m_wndSpinCompressLevel.ShowWindow(m_nCompression==2 ? SW_HIDE : SW_SHOW);
		m_wndCompressLevelCombo.ShowWindow(m_nCompression!=2 ? SW_HIDE : SW_SHOW);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		script.SetPropertyBool("UseSetupLdr",m_bUseSetupLdr ? true : false);
		script.SetPropertyString("InternalCompressLevel",CInnoScriptEx::GetInternalCompressLevel(m_nInternalCompressLevel));
		script.SetPropertyString("OutputDir",m_strOutputDir);
		script.SetPropertyString("SourceDir",m_strSourceDir);
		script.SetPropertyString("OutputBaseFilename",m_strOutputBaseFilename);
		script.SetPropertyBool("MergeDuplicateFiles",m_bMergeDuplicateFiles ? true : false);
		script.SetPropertyString("MessagesFile",m_strMessagesFile);
		script.SetPropertyBool("SolidCompression",m_bSolidCompression ? true : false);
		script.SetPropertyString("VersionInfoVersion",m_strVersionInfoVersion);
		script.SetPropertyString("VersionInfoCompany",m_strVersionInfoCompany);
		script.SetPropertyString("VersionInfoDescription",m_strVersionInfoDescription);
		script.SetPropertyString("VersionInfoTextVersion",m_strVersionInfoTextVersion);
		script.SetPropertyBool("Encryption",m_bEncryption ? true : false);
		script.SetPropertyString("OutputManifestFile",m_strOutputManifestFile);
		script.SetPropertyString("ArchitecturesInstallIn64BitMode",m_strArchitecturesInstallIn64BitMode);
		script.SetPropertyString("ArchitecturesAllowed",m_strArchitecturesAllowed);
		script.SetPropertyString("VersionInfoCopyright",m_strVersionInfoCopyright);
		script.SetPropertyBool("SetupLogging",m_bSetupLogging ? true : false);
		script.SetPropertyString("CompressionThreads",m_strCompressionThreads);

		CString str;
		if(m_nCompression==0 && m_nCompressLevel==7)
			str = "zip";
		else if(m_nCompression==0)
			str.Format("zip/%d",m_nCompressLevel);
		else if(m_nCompression==1 && m_nCompressLevel==9)
			str = "bzip";
		else if(m_nCompression==1)
			str.Format("bzip/%d",m_nCompressLevel);
		else if(m_nCompression==2) {
			str = "lzma";
			switch(m_nCompressLevel) {
				case 0:
					str += "/fast";
					break;
				case 1:
					str += "/normal";
					break;
				case 3:
					str += "/ultra";
					break;
				case 4:
					str += "/ultra64";
					break;
			}
		} else
			str = "none";
		script.SetPropertyString("Compression",str);
		script.SetPropertyString("VersionInfoProductName",m_strVersionInfoProductName);
		script.SetPropertyString("VersionInfoProductVersion",m_strVersionInfoProductVersion);
		script.SetPropertyString("SignTool",m_strSignTool);

		return PSNRET_NOERROR;
	}

	CSetupCompiler(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew),
		m_btnSourceDir(_T("Select source directory")),
		m_btnOutputDir(_T("Select output directory")),
		m_wndOutputManifestFile(false),
		CPropertyPageImpl<CSetupCompiler>(pszTitle)
	{
		m_nCompressLevel = 0;
		m_nCompression = -1;

		CInnoScriptEx& script = m_pDoc->GetScript();

		m_bUseSetupLdr				= script.GetPropertyBool("UseSetupLdr");
		m_nInternalCompressLevel	= CInnoScriptEx::GetInternalCompressLevel(script.GetPropertyString("InternalCompressLevel"));
		m_strOutputDir				= script.GetPropertyString("OutputDir");
		m_strSourceDir				= script.GetPropertyString("SourceDir");
		m_strOutputBaseFilename		= script.GetPropertyString("OutputBaseFilename");
		m_bMergeDuplicateFiles		= script.GetPropertyBool("MergeDuplicateFiles");
		m_strMessagesFile			= script.GetPropertyString("MessagesFile");
		m_bSolidCompression			= script.GetPropertyBool("SolidCompression");
		m_strVersionInfoVersion		= script.GetPropertyString("VersionInfoVersion");
		m_strVersionInfoCompany		= script.GetPropertyString("VersionInfoCompany");
		m_strVersionInfoDescription	= script.GetPropertyString("VersionInfoDescription");
		m_strVersionInfoTextVersion	= script.GetPropertyString("VersionInfoTextVersion");
		m_bEncryption				= script.GetPropertyBool("Encryption");
		m_strOutputManifestFile		= script.GetPropertyString("OutputManifestFile");
		m_strArchitecturesInstallIn64BitMode	= script.GetPropertyString("ArchitecturesInstallIn64BitMode");
		m_strArchitecturesAllowed	= script.GetPropertyString("ArchitecturesAllowed");
		m_strVersionInfoCopyright	= script.GetPropertyString("VersionInfoCopyright");
		m_bSetupLogging				= script.GetPropertyBool("SetupLogging");
		m_strCompressionThreads		= script.GetPropertyString("CompressionThreads");

		CString str;
		str = script.GetPropertyString("Compression");
		long nPos = str.Find('/');
		if(!_tcsnicmp(str,"lzma",4)) {
			m_nCompressLevel = 2;
			if(nPos>0) {
				CString tmp = str.Mid(nPos+1);
				if(!_stricmp(tmp,"fast"))
					m_nCompressLevel = 0;
				else if(!_stricmp(tmp,"normal"))
					m_nCompressLevel = 1;
				else if(!_stricmp(tmp,"ultra"))
					m_nCompressLevel = 3;
				else if(!_stricmp(tmp,"ultra64"))
					m_nCompressLevel = 4;
				str = str.Left(nPos);
			}
		} else if(nPos!=-1) {
			m_nCompressLevel = atol(str.Mid(nPos+1));
			str = str.Left(nPos);
		} else if(!_stricmp(str,"zip")) {
			m_nCompressLevel = 7;
		} else {
			m_nCompressLevel = 9;
		}

		if(!_stricmp(str,"zip"))
			m_nCompression = 0;
		else if(!_stricmp(str,"bzip"))
			m_nCompression = 1;
		else if(!_stricmp(str,"lzma"))
			m_nCompression = 2;
		else
			m_nCompression = 3;
	
		m_strVersionInfoProductName		= script.GetPropertyString("VersionInfoProductName");
		m_strVersionInfoProductVersion	= script.GetPropertyString("VersionInfoProductVersion");
		m_strSignTool					= script.GetPropertyString("SignTool");
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnSelChangeCompression(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		if(m_nCompression==0)
			m_nCompressLevel = 7;
		else if(m_nCompression==1)
			m_nCompressLevel = 9;
		else if(m_nCompression==2)
			m_nCompressLevel = 2;

		m_wndCompressLevel.EnableWindow(m_nCompression<2);
		m_wndSpinCompressLevel.EnableWindow(m_nCompression<2);
		GetDlgItem(IDC_STATIC1).EnableWindow(m_nCompression<3);
		GetDlgItem(IDC_SOLIDCOMPRESSION).EnableWindow(m_nCompression<3);

		m_wndCompressLevel.ShowWindow(m_nCompression==2 ? SW_HIDE : SW_SHOW);
		m_wndSpinCompressLevel.ShowWindow(m_nCompression==2 ? SW_HIDE : SW_SHOW);
		m_wndCompressLevelCombo.ShowWindow(m_nCompression!=2 ? SW_HIDE : SW_SHOW);

		DoDataExchange(DDX_LOAD);
		return 0;
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_BTN_SOURCE_DIR, _L("Help|Compiler|SourceDirButton","Click to select source directory."))
		TOOLTIP_HANDLER(IDC_BTN_OUTPUT_DIR, _L("Help|Compiler|OutputDirButton","Click to select output directory."))
		TOOLTIP_HANDLER(IDC_CHECKUSESETUPLDR, _L("Help|Compiler|UseSetupLdr","This tells the Setup Compiler which type of Setup to create. If this is yes, it compiles all setup data into a single EXE. If this is no, it compiles the setup data into at least three files: SETUP.EXE, SETUP.0, and SETUP-1.BIN. The only reason you would probably want to use no is for debugging purposes."))
		TOOLTIP_HANDLER(IDC_COMPRESSLEVEL, _L("Help|Compiler|CompressLevel","What level of compression to use."))
		TOOLTIP_HANDLER(IDC_COMPRESSLEVELCOMBO, _L("Help|Compiler|CompressLevelCombo","What level of compression to use."))
		TOOLTIP_HANDLER(IDC_OUTPUTDIR, _L("Help|Compiler|OutputDir","Specifies the \"output\" directory for the script, which is where the Setup Compiler will place the resulting SETUP.* files. By default, it creates a directory named \"Output\" under the directory containing the script for this."))
		TOOLTIP_HANDLER(IDC_SOURCEDIR, _L("Help|Compiler|SourceDir","Specifies a new source directory for the script."))
		TOOLTIP_HANDLER(IDC_OUTPUTBASEFILENAME, _L("Help|Compiler|OutputBaseFileName","This directive allows you to assign a different name for the resulting Setup file(s), so you don't have to manually rename them after running the Setup Compiler."))
		TOOLTIP_HANDLER(IDC_MERGEDUPLICATEFILES, _L("Help|Compiler|MergeDuplicateFiles","Normally two file entries referring to the same source file will be compressed and stored only once. If you have a bunch of identical files in your installation, make them point to the same source file in the script, and the size of your installation can drop significantly. If you wish to disable this feature for some reason, set this directive to no."))
		TOOLTIP_HANDLER(IDC_COMPRESSION, _L("Help|Compiler|Compression","This specifies the method of compression to use on the files."))
		TOOLTIP_HANDLER(IDC_INTERNALCOMPRESSLEVEL, _L("Help|Compiler|InternalCompressLevel","This specifies the level of LZMA compression to use on Setup's internal structures."))
		TOOLTIP_HANDLER(IDC_SOLIDCOMPRESSION, _L("Help|Compiler|SolidCompression","If yes, solid compression will be enabled. This causes all files to be compressed at once instead of separately."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOVERSION, _L("Help|Compiler|VersionInfoVersion","Specifies the binary file version value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOCOMPANY, _L("Help|Compiler|VersionInfoCompany","Specifies the company name value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_VERSIONINFODESCRIPTION, _L("Help|Compiler|VersionInfoDescription","Specifies the file description value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOTEXTVERSION, _L("Help|Compiler|VersionInfoTextVersion","Specifies the textual file version value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_ENCRYPTION, _L("Help|Compiler|Encryption","If checked, files that are compiled into the installation (via [Files] section entries) will be encrypted using ARCFOUR encryption, with a 128-bit key derived from the value of the Password [Setup] section directive."))
		TOOLTIP_HANDLER(IDC_OUTPUTMANIFESTFILE, _L("Help|Compiler|OutputManifestFile","When this directive is set, the compiler will create a manifest file detailing information about the files compiled into Setup. The file will be created in the output directory unless a path is included."))
		TOOLTIP_HANDLER(IDC_ARCHITECTURESINSTALLIN64BITMODE, _L("Help|Compiler|ArchitecturesInstallIn64BitMode","Specifies the 64-bit processor architecture(s) on which Setup should install in 64-bit mode. If this directive is not specified or is blank, Setup will always install in 32-bit mode.\r\n\r\nNormally, you should not change this directive from its default value unless your application contains native 64-bit binaries."))
		TOOLTIP_HANDLER(IDC_ARCHITECTURESALLOWED, _L("Help|Compiler|ArchitecturesAllowed","Specifies which processor architecture(s) Setup is allowed to run on. If this directive is not specified or is blank, Setup will be allowed to run on all processor architectures capable of executing its 32-bit code (including ones it doesn't recognize). Otherwise, if a user's processor architecture is not one of those specified in this directive, Setup will display an error message and exit."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOCOPYRIGHT, _L("Help|Compiler|VersionInfoCopyright","Specifies the copyright value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_SETUPLOGGING, _L("Help|Compiler|SetupLogging","If set to yes, Setup will always create a log file. Equivalent to passing /LOG on the command line."))
		TOOLTIP_HANDLER(IDC_COMPRESSIONTHREADS, _L("Help|Compiler|CompressionThreads","Controls whether multi-threading mode is enabled on the LZMA compressor. Multi-threading can speed up the compression process by 50% or more on systems with multiple processor cores, and 20% or more on systems with single-core Intel processors featuring Hyper-Threading Technology.\n\nValid values are auto, 1, 2, or more."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOPRODUCTNAME, _L("Help|Compiler|ProductName","Specifies the product name value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOPRODUCTVERSION, _L("Help|Compiler|ProductVersion","Specifies the product version value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_SIGNTOOL, _L("Help|Compiler|SignTool","Specifies the name and parameters of the Sign Tool to be used to digitally sign Setup and Uninstall."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*				m_pDoc;
	const bool			m_bNew;
};
