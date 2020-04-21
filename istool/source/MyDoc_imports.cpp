#include "stdafx.h"
#include "MyDoc.h"
#include "Translate.h"
#include "resource.h"
#include "mywtl.h"

// copy mode constants (see: convert options)
#define CM_SKIPSAMEOROLDER	0x01	// sets copy mode to "AlwaysSkipIfSameOrOlder" instead of "Normal"
// ConvertIWZ return values
#define IWZE_ALLOK          0x00	// conversion succeeded
#define IWZE_WARNINGS       0x01	// conversion succeeded but there are warnings
#define IWZE_UNKNOWN_ERROR  0x10	// some error occured during conversion
#define IWZE_NOTFOUND       0x11	// source was not found, or couldn't be open
#define IWZE_NOTSUPPORTED   0x12	// source is in unsupported format
#define IWZE_ALLOC_ERROR    0x13	// some problem with host's pAllocProc

bool CMyDoc::iwz_IsIWZFile(LPCTSTR pszPathName) {
	return !CString(pszPathName).Right(4).CompareNoCase(_T(".iwz"));
}

LPCSTR __stdcall iwz_AllocProc(int iBufLen,LPSTR* ppStr) {
	*ppStr = new CHAR[iBufLen];
	return *ppStr;
}

bool CMyDoc::iwz_LoadFile(LPCTSTR lpszPathName) {
	CWaitCursor wait;
	LPSTR pszData = NULL;
	HMODULE hLib = LoadLibrary(_T("iwz2issl.dll"));
	if(!hLib) {
		CString txt = _L("Failed to open '%1'.");
		txt.Replace(_T("%1"),_T("iwz2issl.dll"));
		AtlMessageBox(AfxGetMainHWnd(),(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return false;
	}

	typedef int(__stdcall *tConvertIWZ)(LPCTSTR,DWORD,LPVOID,LPSTR*);

	tConvertIWZ ConvertIWZ = (tConvertIWZ) GetProcAddress(hLib,"ConvertIWZ");
	int nRet = ConvertIWZ(lpszPathName,CM_SKIPSAMEOROLDER,iwz_AllocProc,&pszData);

	switch(nRet) {
	case IWZE_WARNINGS:
		{
			AtlMessageBox(AfxGetMainHWnd(),_L("Conversion was successful, but with warnings."),IDR_MAINFRAME,MB_OK|MB_ICONWARNING);
		}
		break;
	case IWZE_ALLOK:
		{
			AtlMessageBox(AfxGetMainHWnd(),_L("Conversion was successful."),IDR_MAINFRAME);
		}
		break;
	default:
		{
			CString txt = _L("Conversion failed with code '%1'.");
			CString tmp;
			tmp.Format(_T("%d"),nRet);
			txt.Replace(_T("%1"),tmp);
			AtlMessageBox(AfxGetMainHWnd(),(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			if(pszData) delete []pszData;
			return false;
		}
		break;
	}

	try {
		GetScript().Clear();
		GetScript().LoadScriptBuffer(pszData);
	}
	catch(...) {
		AtlMessageBox(AfxGetMainHWnd(),_L("ScriptParseError","An error occured while parsing the script."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		if(pszData) delete []pszData;
		return false;
	}

	FreeLibrary(hLib);
	if(pszData) delete []pszData;
	return true;
}

bool CMyDoc::VBIsVBFile(LPCTSTR pszPathName) {
	return !CString(pszPathName).Right(4).CompareNoCase(_T(".lst"));
}

bool CMyDoc::VBReplaceFileConstant(CString& str,bool bPrivate) {
	str.Replace(_T("$(AppPath)"),_T("{app}"));
	str.Replace(_T("$(WinSysPath)"),_T("{sys}"));
	str.Replace(_T("$(MSDAOPath)"),_T("{dao}"));
	str.Replace(_T("$(Font)"),_T("{fonts}"));
	str.Replace(_T("$(ProgramFiles)"),_T("{pf}"));
	str.Replace(_T("$(CommonFiles)"),_T("{cf}"));
	str.Replace(_T("$(CommonFilesSys)"),_T("{cf}\\System"));
	if(!bPrivate) {
		str.Replace(_T("$(Programs)"),_T("{commonprograms}"));	// icons
		str.Replace(_T("$(Start Menu)"),_T("{commonstartmenu}"));
	} else {
		str.Replace(_T("$(Programs)"),_T("{usersprograms}"));	// icons
		str.Replace(_T("$(Start Menu)"),_T("{userstartmenu}"));
	}
	return str.Replace(_T("$(WinSysPathSysFile)"),_T("{sys}"))!=0;
	// $(WinPath) $(CommonFilesSys)
}

void CMyDoc::VBImportIconSection(CIniFile& file,UINT nGroup) {
	CString strSection, strKey, strValue;

	// GetScript().SetPropertyString("DefaultGroupName",strValue);
	CString	strPrivateGroup, strParent;

	strKey.Format(_T("%d"),nGroup);
	file.GetString(_T("IconGroups"),_T("Group")+strKey,strSection);
	file.GetString(_T("IconGroups"),_T("PrivateGroup")+strKey,strPrivateGroup);
	file.GetString(_T("IconGroups"),_T("Parent")+strKey,strParent);

	if(!nGroup) {
		GetScript().SetPropertyString(_T("DefaultGroupName"),strSection);
		strParent = "{group}\\";
	} else {
		VBReplaceFileConstant(strParent,_tstol(strPrivateGroup)!=0);
		strParent += _T("\\") + strSection + _T("\\");
	}

	CScriptList files;
	GetScript().GetList(CInnoScript::SEC_FILES,files);

	UINT nCount = 1;
	do {
		strKey.Format(_T("%d"),nCount);

		CString	strIcon, strTitle, strStartIn;
		
		file.GetString(strSection,_T("Icon")+strKey,strIcon);
		file.GetString(strSection,_T("Title")+strKey,strTitle);
		file.GetString(strSection,_T("StartIn")+strKey,strStartIn);
		if(strIcon.IsEmpty() || strTitle.IsEmpty()) break;

		// Set icon name
		CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_ICONS);
		pLine->SetParameter(_T("Name"),strParent+strTitle);

		// Strip of '"' from beginning and end
		if(!strIcon.IsEmpty() && strIcon[0]==_T('"') && strIcon[strIcon.GetLength()-1]==_T('"'))
			strIcon = strIcon.Mid(1,strIcon.GetLength()-2);

		// Find file this icon points to
		bool bFoundFile = false;
		for(int nPos=0;nPos<files.GetSize();nPos++) {
			CScriptLine* pFile = files[nPos];
			CString strSource(pFile->GetParameter(_T("Source")));
			if(!strSource.Right(strIcon.GetLength()).CompareNoCase(strIcon)) {
				CString strFileName;
				CInnoScriptEx::GetDestName(pFile,strFileName);
				pLine->SetParameter(_T("Filename"),strFileName);
				bFoundFile = true;
				break;
			}
		}
		if(!bFoundFile) pLine->SetParameter(_T("Filename"),_T("{app}\\")+strIcon);

		// Find working dir, if specified
		if(!strStartIn.IsEmpty()) {
			VBReplaceFileConstant(strStartIn,false);
			pLine->SetParameter(_T("WorkingDir"),strStartIn);
		}
		GetScript().AddLine(pLine);
		nCount++;
	} while(true);
}

void CMyDoc::VBImportFileSection(CIniFile& file,LPCTSTR pszSection) {
	CString str;
	str.Format(_T("; [%s]"),pszSection);
	GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_FILES,str));

	UINT nFile = 1;
	do {
		CString	strKey, strValue;
		strKey.Format(_T("File%d"),nFile++);

		file.GetString(pszSection,strKey,strValue);
		if(strValue.IsEmpty()) break;
		if(!strValue.Left(13).CompareNoCase(_T("@VB6STKIT.DLL"))) continue;

		GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_FILES,_T("; ")+strValue));

		CString	strFileName(_tcstok(strValue.GetBuffer(0),_T(",")));
		CString strDest(_tcstok(NULL,_T(",")));
		CString strFlags1(_tcstok(NULL,_T(",")));
		CString strFlags2(_tcstok(NULL,_T(",")));

		CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_FILES);
#if 1
		pLine->SetParameter(_T("Source"),strFileName.Mid(1));
#else
		CString strPath;
		CProject::GetFilePath(strFileName.Mid(1),strPath);
		pLine->SetParameter("Source",strPath + strFileName.Mid(1));
#endif

		const bool bSysFile = VBReplaceFileConstant(strDest,false);
		pLine->SetParameter(_T("DestDir"),strDest);

		if(bSysFile) {
			pLine->SetParameterFlag(_T("Flags"),_T("restartreplace"),true); 
			pLine->SetParameterFlag(_T("Flags"),_T("uninsneveruninstall"),true); 
			pLine->SetParameterFlag(_T("Flags"),_T("sharedfile"),true); 
		} else {
			pLine->SetParameterFlag(_T("Flags"),_T("promptifolder"),true);
		}

		if(!strFlags1.CompareNoCase(_T("$(DLLSelfRegister)")))
			pLine->SetParameterFlag(_T("Flags"),_T("regserver"),true);
		else if(!strFlags1.CompareNoCase(_T("$(Shared)")))
			pLine->SetParameterFlag(_T("Flags"),_T("sharedfile"),true);
		else if(!strFlags1.CompareNoCase(_T("$(TLBRegister)")))
			pLine->SetParameterFlag(_T("Flags"),_T("regtypelib"),true);
		if(!strFlags2.CompareNoCase(_T("$(DLLSelfRegister)")))
			pLine->SetParameterFlag(_T("Flags"),_T("regserver"),true);
		else if(!strFlags2.CompareNoCase(_T("$(Shared)")))
			pLine->SetParameterFlag(_T("Flags"),_T("sharedfile"),true);
		else if(!strFlags2.CompareNoCase(_T("$(TLBRegister)")))
			pLine->SetParameterFlag(_T("Flags"),_T("regtypelib"),true);

		GetScript().AddLine(pLine);
	} while(true);
	GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_FILES,_T("")));
}

bool CMyDoc::VBLoadFile(LPCTSTR lpszPathName) {
	CString		strValue;
	CIniFile	file(lpszPathName);

	strValue.Format(_T("; '%s' imported by ISTool version %d.%d.%d"),lpszPathName,VER_MAJOR,VER_MINOR,VER_REVISION);
	GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_NONE,strValue));
	GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_NONE,_T("")));

	file.GetString(_T("Setup"),_T("Title"),strValue);
	if(!strValue.IsEmpty()) {
		GetScript().SetPropertyString(_T("AppName"),strValue);
		GetScript().SetPropertyString(_T("AppVerName"),strValue);
	}
	GetScript().SetPropertyString(_T("PrivilegesRequired"),_T("admin"));

	file.GetString(_T("Setup"),_T("DefaultDir"),strValue);
	if(!strValue.IsEmpty()) {
		strValue.Replace(_T("$(ProgramFiles)"),_T("{pf}"));
		GetScript().SetPropertyString(_T("DefaultDirName"),strValue);
	}

	// Files
	VBImportFileSection(file,_T("Bootstrap Files"));
	VBImportFileSection(file,_T("Setup1 Files"));

	// Icons
	UINT nGroup = 0;
	do {
		CString strGroup;
		strGroup.Format(_T("Group%d"),nGroup);

		file.GetString(_T("IconGroups"),strGroup,strValue);
		if(strValue.IsEmpty()) break;	// No more icons

		VBImportIconSection(file,nGroup);
//		if(!strValue.IsEmpty()) {
//			GetScript().SetPropertyString("DefaultGroupName",strValue);
//			VBImportIconSection(file,strValue);
//		}
		nGroup++;
	} while(true);

	SetModifiedFlag(true);
	return true;
}
