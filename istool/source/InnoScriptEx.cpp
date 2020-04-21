// InnoScriptEx.cpp: implementation of the CInnoScriptEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InnoScriptEx.h"
#include "mywtl.h"
#include "resource.h"

struct {
	LPCTSTR	m_pszName;
	LPCTSTR	m_pszValue;
} m_defaults2[] = {
	// [LangOptions]
	
	"LanguageName",					"English",
	"LanguageID",					"$0409",
	"DialogFontName",				"MS Shell Dlg",
	"DialogFontSize",				"8",
	"DialogFontStandardHeight",		"13",
	"TitleFontName",				"Arial",
	"TitleFontSize",				"29",
	"WelcomeFontName",				"Arial",
	"WelcomeFontSize",				"12",
	"CopyrightFontName",			"Arial",
	"CopyrightFontSize",			"8",
	"RightToLeft",					"false",
		
	// [Setup]
	// Inno Setup 5.2.4
	"SignTool",						"",
	// Inno Setup 5.2.3
	"VersionInfoProductName",		"",
	"VersionInfoproductVersion",	"",
	// Inno Setup 5.2.1
	"SignedUninstaller",			"false",
	"SignedUninstallerDir",			"",
	// Inno Setup 5.1.14
	"CompressionThreads",			"auto",
	// Inno Setup 5.1.13
	"SetupLogging",					"false",
	"AppSupportPhone",				"",
	// Inno Setup 5.1.7
	"TerminalServicesAware",		"true",
	"ShowUndisplayableLanguages",	"false",
	// Inno Setup 5.1.5
	"VersionInfoCopyright",			"",
	// Inno Setup 5.1.0
	"ArchitecturesInstallIn64BitMode",	"",
	"ArchitecturesAllowed",			"",
	// Inno Setup 5.0.5
	"OutputManifestFile",			"",
	// Inno Setup 5.0.4
	"ChangesEnvironment",			"false",
	// Inno Setup 4.2.4
	"AppModifyPath",				"",
	"AppComments",					"",
	"AppContact",					"",
	"AppReadmeFile",				"",
	// Inno Setup 4.2.2
	"Encryption",					"false",
	// Inno Setup 4.2.0
	"TouchDate",					"current",
	"TouchTime",					"current",
	// Inno Setup 4.1.8
	"AppendDefaultDirName",			"true",
	"AppendDefaultGroupName",		"true",
	// Inno Setup 4.1.3
	"WizardImageStretch",			"true",
	// Inno Setup 4.1.0
	"SetupIconFile",				"",
	"VersionInfoVersion",			"",
	"VersionInfoCompany",			"",
	"VersionInfoDescription",		"",
	"VersionInfoTextVersion",		"",
	"TimeStampRounding",			"2",
	// Inno Setup 4.0.10
	"TimeStampsInUTC",				"false",
	"LanguageDetectionMethod",		"uilanguage",
	// Inno Setup 4.0.9
	"AllowCancelDuringInstall",		"true",
	// Inno Setup 4.0.1
	"SolidCompression",				"false",
	"DetectLanguageUsingLocale",	"false",
	// Inno Setup 4.0.0
	"ShowLanguageDialog",			"yes",
	"DiskSliceSize",				"1457664",
	"SlicesPerDisk",				"1",

	// My Inno Setup Extensions 3.0.6.1
	"DefaultUserInfoSerial",		"",

	// My Inno Setup Extensions 3.0.3
	"ShowTasksTreeLines",			"false",

	// Inno Setup 3.0.4, Inno Setup 5.1.9
	"PrivilegesRequired",			"admin",

	// Inno Setup 3.0.3
	"AlwaysRestart",				"false",
	"RestartIfNeededByRun",			"true",

	// Inno Setup 3.0.1
	"UninstallRestartComputer",		"false",

	// Inno Setup 3.0.0
	"UserInfoPage",					"false",
	"DefaultUserInfoName",			"{sysuserinfoname}",
	"DefaultUserInfoOrg",			"{sysuserinfoorg}",
	"UsePreviousUserInfo",			"true",

	// Inno Setup 2.0.18
	"AllowUNCPath",					"true",

	// Inno Seup 2.0.17
	"Compression",					"lzma",
	"InternalCompressLevel",		"normal",

	// Inno Setup 2.0.7
	"AlwaysShowDirOnReadyPage",		"false",
	"AlwaysShowGroupOnReadyPage",	"false",

	// Inno Setup 2.0.0
	"DisableReadyMemo",				"false",
	"AlwaysShowComponentsList",		"true",
	"ShowComponentSizes",			"true",
	"FlatComponentsList",			"true",
	"DisableReadyPage",				"false",
	"UsePreviousSetupType",			"true",
	"UsePreviousTasks",				"true",
	"WindowVisible",				"false",
	"WizardImageBackColor",			"$400000",
	"WizardImageFile",				"compiler:wizmodernimage.bmp",
	"WizardSmallImageFile",			"compiler:wizmodernsmallimage.bmp",

	// Inno Setup
	"UpdateUninstallLogAppName",	"true",		// IS 1.3.20
	"AdminPrivilegesRequired",		"false",
	"AllowNoIcons",					"false",
	"AllowRootDirectory",			"false",
	"AlwaysUsePersonalGroup",		"false",
	"AppCopyright",					"",
	"AppName",						"",
	"AppVerName",					"",
	"BackColor",					"clBlue",
	"BackSolid",					"false",
	"ChangesAssociations",			"false",
	"CreateAppDir",					"true",
	"DefaultDirName",				"{pf}\\",
	"DefaultGroupName",				"",
	"DisableDirPage",				"false",
	"DisableFinishedPage",			"false",
	"DisableProgramGroupPage",		"false",
	"DisableStartupPrompt",			"true",
	"DiskClusterSize",				"512",
	"DiskSpanning",					"false",
	"MergeDuplicateFiles",			"true",
	"EnableDirDoesntExistWarning",	"false",
	"ExtraDiskSpaceRequired",		"0",
	"InfoAfterFile",				"",
	"InfoBeforeFile",				"",
	"LicenseFile",					"",
	"MessagesFile",					"compiler:default.isl",
	"MinVersion",					"",
	"OnlyBelowVersion",				"",
	"OutputBaseFilename",			"setup",
	"OutputDir",					"",
	"OverwriteUninstRegEntries",	"true",
	"Password",						"",
	"ReserveBytes",					"0",
	"SourceDir",					"",
	"Uninstallable",				"true",
	"UseSetupLdr",					"true",
	"WindowShowCaption",			"true",
	"WindowStartMaximized",			"true",
	"WindowResizable",				"true",

	// Inno Setup 1.3.0
	"AppID",						"",
	"AppPublisher",					"",
	"AppPublisherURL",				"",
	"AppSupportURL",				"",
	"AppUpdatesURL",				"",
	"AppVersion",					"",
	"CreateUninstallRegKey",		"true",
	"UninstallLogMode",				"append",
	
	// Inno Setup 1.3.1
	"UsePreviousAppDir",			"true",
	
	// Inno Setup 1.3.3
	"BackColor2",					"clBlack",
	"BackColorDirection",			"toptobottom",
	"UninstallFilesDir",			"{app}",
	
	// Inno Setup 1.3.6
	"DirExistsWarning",				"auto",
	"UninstallDisplayIcon",			"",
	"UninstallDisplayName",			"",

	// Inno Setup 1.3.10 (My suggestion:)
	"UsePreviousGroup",				"true",

	// Inno Setup 1.3.14
	"AppMutex",						"",
	NULL
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInnoScriptEx::CInnoScriptEx() {}

CInnoScriptEx::~CInnoScriptEx() {}

void CInnoScriptEx::GetList(SECTION sec,CScriptList& list) {
	for(long i=0;i<GetSize();i++) {
		CLine* pLine = m_lines[i];
		if((sec==SEC_CODE || !pLine->GetComment()) && pLine->GetSection()==sec) {
			list.Add(pLine);
		}
	}
}

void CInnoScriptEx::GetCompleteList(CScriptList& list) {
	for(long i=0;i<GetSize();i++)
		list.Add(m_lines[i]);
}

void CInnoScriptEx::AddLine(CLine* pLine) {
	bool bFound = false;
	for(long i=0;i<GetSize();i++) {
		if(bFound && m_lines[i]->GetSection()!=pLine->GetSection()) {
#if 1
			// Rewind
			while(i>0 && m_lines[i-1]->GetSection()==pLine->GetSection()) {
				CString strLine;
				m_lines[i-1]->Write(strLine.GetBufferSetLength(10000),10000);
				strLine.ReleaseBuffer();
				if(strLine.Trim().IsEmpty())
					i--;
				else
					break;
			}
#endif
			m_lines.InsertAt(i,pLine);
			return;
		}
		if(m_lines[i]->GetSection()==pLine->GetSection())
			bFound = true;
	}
	m_lines.Add(pLine);
}

void CInnoScriptEx::DeleteLine(CLine* pLine) {
	for(long i=0;i<GetSize();i++) {
		if(m_lines[i]==pLine) {
			m_lines.RemoveAt(i);
			delete pLine;
			break;
		}
	}
}

UINT CInnoScriptEx::CountLines(SECTION sec) {
	UINT nCount = 0;
	for(long i=0;i<GetSize();i++) {
		CLine* pLine = m_lines[i];
		if(pLine->GetSection()==sec && !pLine->GetComment())
			nCount++;
	}
	return nCount;
}

bool CInnoScriptEx::GetBoolean(LPCTSTR pszValue) {
	if(!_stricmp(pszValue,"yes"))
		return true;
	if(!_stricmp(pszValue,"true"))
		return true;
	if(atol(pszValue))
		return true;
	return false;
}

int CInnoScriptEx::Convert(const CConverter* pData,LPCTSTR pszValue) {
	if(!pszValue) return -1;
	for(UINT n=0;pData[n].m_pszString;n++) {
		if(!_stricmp(pszValue,pData[n].m_pszString))
			return pData[n].m_nValue;
	}
	return -1;
}

LPCTSTR CInnoScriptEx::Convert(const CConverter* pData,int nValue) {
	for(UINT n=0;pData[n].m_pszString;n++) {
		if(nValue==pData[n].m_nValue)
			return pData[n].m_pszString;
	}
	return NULL;
}

void CInnoScriptEx::GetDestDir(CLine* pLine,CString& ref) {
	ref.Empty();
	
	if(pLine->GetSection()!=SEC_DIRS) return;

	CString str(pLine->GetParameter("Name"));
	int pos = str.ReverseFind('\\');
	if(pos>=0) ref = str.Left(pos);
	else ref.Empty();
}

void CInnoScriptEx::GetDestName(CLine* pLine,CString& ref) {
	ref.Empty();
	if(pLine->GetSection()!=SEC_FILES) return;

	ref = pLine->GetParameter("DestDir");
	CMyUtils::EndWith(ref,'\\');
	LPCTSTR pszDestName = pLine->GetParameter("DestName");
	if(!pszDestName) {
		CString tmp(pLine->GetParameter("Source"));
		int pos = tmp.ReverseFind('\\');
		if(pos<=0) pos = tmp.ReverseFind(':');
		if(pos>=0) ref += tmp.Mid(pos+1);
		else ref += pLine->GetParameter("Source");
	} else
		ref += pszDestName;
}

void CInnoScriptEx::GetDestTitle(CLine* pLine,CString& ref) {
	ref.Empty();
	if(pLine->GetSection()==SEC_FILES) {
		LPCTSTR pszDestName = pLine->GetParameter("DestName");
		if(!pszDestName) {
			CString tmp(pLine->GetParameter("Source"));
			int pos = tmp.ReverseFind('\\');
			if(pos<=0) pos = tmp.ReverseFind(':');
			if(pos>=0) ref = tmp.Mid(pos+1);
			else ref = pLine->GetParameter("Source");
		} else {
			ref = pszDestName;
		}
	} else if(pLine->GetSection()==SEC_DIRS) {
		CString str(pLine->GetParameter("Name"));
		int pos = str.ReverseFind('\\');
		if(pos>=0) ref = str.Mid(pos+1);
		else ref = pLine->GetParameter("Name");
	}
}

void CInnoScriptEx::SetString(CLine* pLine,bool bForce,LPCTSTR pszName,LPCTSTR pszValue) {
//	if(bForce || !m_strName.IsEmpty()) pLine->m_strName = m_strName;

	if(bForce || (pszValue && *pszValue)) {
		if(pszValue && *pszValue)
			pLine->SetParameter(pszName,pszValue);
		else
			pLine->DeleteParameter(pszName);
	}
}

void CInnoScriptEx::SetLong(CLine* pLine,bool bForce,LPCTSTR pszName,LONG nDefault,LONG nValue) {
//	if(bForce || m_nIconIndex!=-1) pLine->m_nIconIndex = m_nIconIndex;
	if(bForce || nValue!=nDefault) {
		if(nValue!=nDefault) {
			CString strValue;
			strValue.Format("%d",nValue);
			pLine->SetParameter(pszName,strValue);
		} else
			pLine->DeleteParameter(pszName);
	}
}

void CInnoScriptEx::SetFlag(CLine* pLine,LPCTSTR pszName,LPCTSTR pszFlag,int nFlag) {
//	if(m_bReadOnly==TRUE) pFile->SetParameterFlag("Attribs","readonly",true);
//	else if(m_bReadOnly==FALSE) pFile->SetParameterFlag("Attribs","readonly",false);

	if(nFlag==1)
		pLine->SetParameterFlag(pszName,pszFlag,true);
	else if(nFlag==0)
		pLine->SetParameterFlag(pszName,pszFlag,false);
}

void CInnoScriptEx::AddComment(SECTION sec,LPCTSTR pszComment) {
	AddLine(new CLineComment(sec,pszComment));
}

void CInnoScriptEx::SetPropertyBool(LPCTSTR pszName,bool bValue,SECTION sec/*=SEC_SETUP*/) {
	SetPropertyString(pszName,bValue ? "true" : "false",sec);
}

void CInnoScriptEx::SetPropertyNumber(LPCTSTR pszName,LONG nValue,SECTION sec/*=SEC_SETUP*/) {
	CString str;
	str.Format("%d",nValue);
	SetPropertyString(pszName,str,sec);
}

LPCTSTR CInnoScriptEx::GetDefault(LPCTSTR pszName,SECTION sec/*=SEC_SETUP*/) {
	if(sec!=SEC_SETUP && sec!=SEC_LANGOPTIONS) return NULL;

	// If it equals default don't add it
	for(UINT n=0;m_defaults2[n].m_pszName;n++) {
		if(!_stricmp(pszName,m_defaults2[n].m_pszName))
			return m_defaults2[n].m_pszValue;
	}
#ifdef _DEBUG
	CString strError;
	strError.Format("Failed to find default value for setup directive %s!",pszName);
	AtlMessageBox(AfxGetMainHWnd(),(LPCTSTR)strError,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
#endif
	return NULL;
}


void CInnoScriptEx::SetPropertyString(LPCTSTR pszName,LPCTSTR pszValue,SECTION sec/*=SEC_SETUP*/) {
//	if(pszValue && !*pszValue) pszValue = NULL;

	for(long i=0;i<GetSize();i++) {
		CLine* pLine = m_lines[i];
		if(pLine->GetSection()==sec && !pLine->GetComment() && pLine->GetKey() && !_stricmp(pLine->GetKey(),pszName)) {
			if(pszValue)
				pLine->SetParameter(pszName,pszValue);
			else
				pLine->DeleteParameter(pszName);
			return;
		}
	}
	// Not found so far
	if(!pszValue) return;

	// If it equals default don't add it
	LPCTSTR pszDefault = GetDefault(pszName,sec);
	if((sec==SEC_SETUP || sec==SEC_LANGOPTIONS) && pszDefault && !_stricmp(pszDefault,pszValue))
		return;

	CString str;
	str.Format("%s=%s",pszName,pszValue);
	CLine* pLine = new CLineSetup(sec,str);
	pLine->SetParameter(pszName,pszValue);
	AddLine(pLine);
}

bool CInnoScriptEx::GetPropertyBool(LPCTSTR pszName,SECTION sec/*=SEC_SETUP*/) {
	LPCTSTR pszValue = GetPropertyString(pszName,sec);
	return pszValue && GetBoolean(pszValue);
}

LONG CInnoScriptEx::GetPropertyNumber(LPCTSTR pszName,SECTION sec/*=SEC_SETUP*/) {
	return atol(GetPropertyString(pszName,sec));
}

LPCTSTR CInnoScriptEx::GetPropertyString(LPCTSTR pszName,SECTION sec/*=SEC_SETUP*/) {
	for(long i=0;i<GetSize();i++) {
		CLine* pLine = m_lines[i];
		if(pLine->GetSection()==sec && !pLine->GetComment() && pLine->GetKey() && !_stricmp(pLine->GetKey(),pszName)) {
			return pLine->GetValue();
		}
	}
	return GetDefault(pszName,sec);
}

static const CInnoScriptEx::CConverter m_valueType[] = {
	"none",		0,
	"string",	1,
	"expandsz",	2,
	"multisz",	3,
	"dword",	4,
	"binary",	5,
	"qword",	6,
	NULL
};

LPCTSTR CInnoScriptEx::GetRegistryValueType(int nRoot) {
	return Convert(m_valueType,nRoot);
}

int CInnoScriptEx::GetRegistryValueType(LPCTSTR pszValue) {
	return Convert(m_valueType,pszValue);
}

static const CInnoScriptEx::CConverter m_dirExistsWarning[] = {
	"auto",		0,
	"yes",		1,
	"true",		1,
	"1",		1,
	"no",		2,
	"false",	2,
	"0",		2,
	NULL
};

int CInnoScriptEx::GetDirExistsWarning(LPCTSTR pszValue) {
	return Convert(m_dirExistsWarning,pszValue);
}

LPCTSTR CInnoScriptEx::GetDirExistsWarning(int nMode) {
	return Convert(m_dirExistsWarning,nMode);
}

static const CInnoScriptEx::CConverter m_showLanguageDialog[] = {
	"auto",		0,
	"yes",		1,
	"true",		1,
	"1",		1,
	"no",		2,
	"false",	2,
	"0",		2,
	NULL
};

int CInnoScriptEx::GetShowLanguageDialog(LPCTSTR pszValue) {
	return Convert(m_showLanguageDialog,pszValue);
}

LPCTSTR CInnoScriptEx::GetShowLanguageDialog(int nMode) {
	return Convert(m_showLanguageDialog,nMode);
}

static const CInnoScriptEx::CConverter m_privilegesRequired[] = {
	"none",			0,
	"poweruser",	1,
	"admin",		2,
	NULL
};

LPCTSTR CInnoScriptEx::GetPrivilegesRequired(int nMode) {
	return Convert(m_privilegesRequired,nMode);
}

int CInnoScriptEx::GetPrivilegesRequired(LPCTSTR pszValue) {
	return Convert(m_privilegesRequired,pszValue);
}

static const CInnoScriptEx::CConverter m_restartComputer[] = {
	"auto",		0,
	"yes",		1,
	"true",		1,
	"1",		1,
	"no",		2,
	"false",	2,
	"0",		2,
	NULL
};

int CInnoScriptEx::GetRestartComputer(LPCTSTR pszValue) {
	return Convert(m_restartComputer,pszValue);
}

LPCTSTR CInnoScriptEx::GetRestartComputer(int nMode) {
	return Convert(m_restartComputer,nMode);
}

static const CInnoScriptEx::CConverter m_deleteType[] = {
	"files",			0,
	"filesandordirs",	1,
	"dirifempty",		2,
	NULL
};

int CInnoScriptEx::GetDeleteType(LPCTSTR pszType) {
	return Convert(m_deleteType,pszType);
}

LPCTSTR CInnoScriptEx::GetDeleteType(int nType) {
	return Convert(m_deleteType,nType);
}

static const CInnoScriptEx::CConverter m_backColorDirection[] = {
	"toptobottom",	0,
	"lefttoright",	1,
	NULL
};

int CInnoScriptEx::GetBackColorDirection(LPCTSTR pszValue) {
	return Convert(m_backColorDirection,pszValue);
}

LPCTSTR CInnoScriptEx::GetBackColorDirection(int nMode) {
	return Convert(m_backColorDirection,nMode);
}

static const CInnoScriptEx::CConverter m_langaugeDetectionMethod[] = {
	"uilanguage",	0,
	"locale",		1,
	"none",			2,
	NULL
};

int CInnoScriptEx::GetLanguageDetectionMethod(LPCTSTR pszValue) {
	return Convert(m_langaugeDetectionMethod,pszValue);
}

LPCTSTR CInnoScriptEx::GetLanguageDetectionMethod(int nMode) {
	return Convert(m_langaugeDetectionMethod,nMode);
}

static const CInnoScriptEx::CConverter m_internalCompressLevel[] = {
	"none",		0,
	"fast",		1,
	"normal",	2,
	"max",		3,
	"ultra",	4,
	"ultra64",	5,
	NULL
};

int CInnoScriptEx::GetInternalCompressLevel(LPCTSTR pszValue) {
	return Convert(m_internalCompressLevel,pszValue);
}

LPCTSTR CInnoScriptEx::GetInternalCompressLevel(int nMode) {
	return Convert(m_internalCompressLevel,nMode);
}

static const CInnoScriptEx::CConverter m_uninstallLogMode[] = {
	"append",		0,
	"new",			1,
	"overwrite",	2,
	NULL
};

int CInnoScriptEx::GetUninstallLogMode(LPCTSTR pszValue) {
	return Convert(m_uninstallLogMode,pszValue);
}

LPCTSTR CInnoScriptEx::GetUninstallLogMode(int nMode) {
	return Convert(m_uninstallLogMode,nMode);
}

static const CInnoScriptEx::CConverter m_root[] = {
	"HKCR",	0,
	"HKCU",	1,
	"HKLM",	2,
	"HKU",	3,
	"HKCC",	4,
	NULL
};

int CInnoScriptEx::GetRegistryRoot(LPCTSTR pszValue) {
	return Convert(m_root,pszValue);
}

LPCTSTR CInnoScriptEx::GetRegistryRoot(int nRoot) {
	return Convert(m_root,nRoot);
}



// File system additions
bool CInnoScriptEx::GetFolderName(CString& ref,CScriptLine* p) {
	ref.Empty();
	if(p->GetSection()==CInnoScript::SEC_ICONS) {
		CString strDir(p->GetParameter("Name"));
		int nPos = strDir.ReverseFind('\\');
		if(nPos>=0) 
			ref = strDir.Left(nPos);
	} else if(p->GetSection()==CInnoScript::SEC_FILES) {
		ref = p->GetParameter("DestDir");
	} else if(p->GetSection()==CInnoScript::SEC_DIRS) {
		ref = p->GetParameter("Name");
	} else if(p->GetSection()==CInnoScript::SEC_REGISTRY) {
		LPCTSTR pszRoot = p->GetParameter("Root");
		LPCTSTR pszSubkey = p->GetParameter("Subkey");
		if(pszRoot) pszRoot = GetRootName(pszRoot);
		if(!pszRoot) pszRoot = "";
		if(!pszSubkey) pszSubkey = "";
		ref.Format("%s\\%s",pszRoot,pszSubkey);
	} else {
#ifdef _DEBUG
		AtlMessageBox(AfxGetMainHWnd(),"Unhandled script section specified in GetFolderName().",IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		ATLASSERT(FALSE);
#endif
	}

	
	if(!ref.IsEmpty() && ref[ref.GetLength()-1]=='\\') 
		ref.ReleaseBuffer(ref.GetLength()-1);

	return ref.IsEmpty() ? false : true;
}

const CInnoScriptEx::CConstantInfo CInnoScriptEx::m_constants[]= {
	// Directory Constants 
	"{app}",			"The application directory", true,
	"{win}",			"The system's Windows directory", true,
	"{sys}",			"The system's Windows System directory", true,
	"{src}",			"The directory in which the Setup files are located", true,
	"{sd}",				"System Drive", true,
	"{pf}",				"Program Files", true,
	"{cf}",				"Common Files", true,
	"{tmp}",			"Temporary directory", true,
	"{fonts}",			"Fonts directory", true,
	"{dao}",			"DAO directory", true,
	"{pf32}",			"Program Files", true,
	"{pf64}",			"Program Files", true,
	"{cf32}",			"Common Files", true,
	"{cf64}",			"Common Files", true,

	// Shell Folder Constants 
	"{group}",			"The path to the program group", true,
	"{sendto}",			"The path to the current user's Send To folder", true,
	
	"{userappdata}",	"The path to the Application Data folder", true,
	"{commonappdata}",	"The path to the Application Data folder", true,
	"{userdesktop}",	"The path to the desktop folder", true,
	"{commondesktop}",	"The path to the desktop folder", true,
	"{userdocs}",		"The path to the My Documents folder", true,
	"{commondocs}",		"The path to the My Documents folder", true,
	"{userfavorites}",	"The path to the Favorites folder", true,
	"{commonfavorites}","The path to the Favorites folder", true,
	"{localappdata}",	"The path to the Application Data folder", true,

	"{userprograms}",	"The path to the Programs folder on the Start Menu", true,
	"{commonprograms}", "The path to the Programs folder on the Start Menu", true,
	"{userstartmenu}",	"The path to the top level of the Start Menu", true,
	"{commonstartmenu}","The path to the top level of the Start Menu", true,
	"{userstartup}",	"The path to the Startup folder on the Start Menu", true,
	"{commonstartup}",	"The path to the Startup folder on the Start Menu", true,
	"{usertemplates}",	"The path to the Templates folder", true,
	"{commontemplates}","The path to the Templates folder", true,

	// Other Constants 
	"{\\}",												"A backslash character", false,
	"{%NAME,DefaultValue}",								"Embeds an environment variable", false,
	"{computername}",									"The name of the computer", false,
	"{groupname}",										"Group selected on Select Program Group wizard page", false,
	"{hwnd}",											"Handle of the Setup program's background window", false,
	"{reg:HKxx\\SubkeyName,ValueName|DefaultValue}",	"Embeds a registry value", false,
	"{ini:filename,section,key|defaultvalue}",			"Embeds a ini value", false,
	"{param:parametername|defaultvalue}",				"Embeds a command line parameter value", false,
	"{code:FunctionName|DefaultValue}",					"Embeds a code value", false,
	"{srcexe}",											"Full pathname of the Setup program file", false,
	"{username}",										"Name of user who is running Setup program", false,
	"{wizardhwnd}",										"Handle to wizard window", false,
	"{userinfoname}",									"Name entered by the user", false,
	"{userinfoorg}",									"Organization entered by the user", false,
	"{sysuserinfoname}",								"Name that Windows is registered to", false,
	"{sysuserinfoorg}",									"Organization that Windows is registered to", false,
	"{uninstallexe}",									"Full path and filename of the uninstall program", false,
	"{cmd}",											"System's command interpreter", false,
	"{language}",										"The current setup language", false,
	"{drive:...}",										"Extracts drive from path", false,
	"{syswow64}",										"?", false,
	"{log}",											"The log file name, or an empty string if logging is not enabled.", false,
	"{regasmexe}",										"?", false,
	"{regasmexe32}",									"?", false,
	"{regasmexe64}",									"?", false,

	NULL,NULL,false
};

bool CInnoScriptEx::IsFolderConstant(LPCTSTR pszConstant) {
	UINT nPos = 0;
	while(m_constants[nPos].m_pszConstant) {
		if(!_stricmp(pszConstant,m_constants[nPos].m_pszConstant))
			return true;
		nPos++;
	}
	return false;
}

LPCTSTR CInnoScriptEx::GetConstantName(LPCTSTR pszConstant) {
	UINT nPos = 0;
	while(m_constants[nPos].m_pszConstant) {
		if(!_stricmp(pszConstant,m_constants[nPos].m_pszConstant))
			return m_constants[nPos].m_pszName;
		nPos++;
	}
	return pszConstant;
}

LPCTSTR CInnoScriptEx::GetRootName(LPCTSTR pszRoot) {
	if(!pszRoot) return NULL;
	if(!_stricmp(pszRoot,"HKCR"))
		return "HKEY_CLASSES_ROOT";
	else if(!_stricmp(pszRoot,"HKCU"))
		return "HKEY_CURRENT_USER";
	else if(!_stricmp(pszRoot,"HKLM"))
		return "HKEY_LOCAL_MACHINE";
	else if(!_stricmp(pszRoot,"HKU"))
		return "HKEY_USERS";
	else if(!_stricmp(pszRoot,"HKCC"))
		return "HKEY_CURRENT_CONFIG";
	return NULL;
}

LPCTSTR CInnoScriptEx::GetDisplayName(CScriptLine* p) {
	if(p->GetSection()==CInnoScript::SEC_DIRS) {
		LPCTSTR pszName = p->GetParameter("Name");
		LPCTSTR psz = _tcsrchr(pszName,'\\');
		if(psz)
			return psz+1;
		return pszName;
	} else if(p->GetSection()==CInnoScript::SEC_FILES) {
		LPCTSTR pszName = p->GetParameter("DestName");
		if(pszName) return pszName;

		pszName = p->GetParameter("Source");
		LPCTSTR psz = _tcsrchr(pszName,'\\');
		if(psz) 
			return psz+1;
		return pszName;
	} else if(p->GetSection()==CInnoScript::SEC_ICONS) {
		LPCTSTR pszName = p->GetParameter("Name");
		LPCTSTR psz = _tcsrchr(pszName,'\\');
		if(psz) 
			return psz+1;
		return pszName;
	} else if(p->GetSection()==CInnoScript::SEC_REGISTRY) {
		return p->GetParameter("ValueName");
	} else {
#ifdef _DEBUG
		AtlMessageBox(AfxGetMainHWnd(),"Unhandled script section specified in GetDisplayName().",IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		ATLASSERT(FALSE);
#endif
	}

	return NULL;
}

void CInnoScriptEx::Split(const CString& Str,TSetupVersionDataVersion& Ver,WORD& ServicePack) {
//void Split(const CString& Str,DWORD& ver,WORD& ServicePack) {
	int I,J;
	CString Z, B;
    bool HasBuild;

	//Ver = 0;
	memset(&Ver,0,sizeof Ver);
    ServicePack = 0;
	Z = Str; 
	Z.MakeLower();

	I = Z.Find("sp");
	if(I>=0) {
		J = atol(Z.Mid(I+2));
		if(J<0 || J>255) throw 0;
		ServicePack = J;
		Z.SetAt(I,0);
		Z.ReleaseBuffer();
	}

	I = Z.Find(".");
	if(I==Z.GetLength()-1) throw 0;
	if(I>=0) {
		J = atol(Z.Left(I));
		if(J<0 || J>255) throw 0;
		Ver.Major = J;
		Z = Z.Mid(I+1);

		I = Z.Find(".");
		HasBuild = I>=0;
		if(!HasBuild) {
			I = Z.GetLength();
			B.Empty();
		} else {
			B = Z.Mid(I+1);
			Z = Z.Left(I);
		}
		J = atol(Z);
		if(J<0 || J>99) throw 0;
		if(J<10 && Z[0]!='0') J *= 10;
		Ver.Minor = J;
		if(HasBuild) {
			J = atol(B);
			if(J<0 || J>65535) throw 0;
			Ver.Build = J;
		}
	} else { // no minor version specified }
		J = atol(Str);
		if(J<0 || J>255) throw 0;
		Ver.Major = J;
	}
}

bool CInnoScriptEx::StrToVersionNumbers(const CString& S,TSetupVersionData& VerData) {
	int I;
	WORD SP;

	try {
		I = S.Find(",");
		if(I<0) throw 0;

		CString tmp;
		tmp = S.Left(I);
		tmp.TrimLeft(), tmp.TrimRight();
		Split(tmp,VerData.WinVersion,SP);
		if(SP) throw 0; // only NT has service packs
		tmp = S.Mid(I+1);
		tmp.TrimLeft(), tmp.TrimRight();
		Split(tmp,VerData.NTVersion, VerData.NTServicePack);
		return true;
	}
	catch(...) {
		return false;
	}
}

CString CInnoScriptEx::GetMessage(const CString& strName,CString strHomeDir) {
	CMyUtils::EndWith(strHomeDir,'\\');
	CString strMessagesFile;
	CScriptList lines;
	GetList(CInnoScript::SEC_LANGUAGES,lines);
	if(lines.GetSize()==0)
		strMessagesFile = "compiler:default.isl";
	else
		strMessagesFile = lines[0]->GetParameter("MessagesFile");

	long nPos = strMessagesFile.Trim().ReverseFind(',');
	do {
		CString strFile;
		if(nPos<0) {
			strFile = strMessagesFile;
			strMessagesFile.Empty();
		} else {
			strFile = strMessagesFile.Mid(nPos+1);
			strMessagesFile.ReleaseBufferSetLength(nPos);
		}

		strFile.Trim().Replace("compiler:",strHomeDir);

		CString strMessage;
		DWORD dwChars = GetPrivateProfileString("Messages",strName,NULL,strMessage.GetBuffer(1024),1024,strFile);
		strMessage.ReleaseBufferSetLength(dwChars);
		if(!strMessage.IsEmpty())
			return strMessage;

		nPos = strMessagesFile.Trim().ReverseFind(',');
	} while(!strMessagesFile.IsEmpty());


	return "strMessagesFile";
}
