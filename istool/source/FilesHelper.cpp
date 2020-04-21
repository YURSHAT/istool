// FilesHelper.cpp: implementation of the CFilesHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "istool.h"
#include "FilesHelper.h"
#include "MainFrm.h"
#include "MyDoc.h"
#include "DlgAddDirectory.h"
#include "FileInfo.h"
#include "SplitPath.h"
#include "StringToken.h"
#include "DlgODBC.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesHelper::CFilesHelper(CMyDoc* pDoc) : m_pDoc(pDoc) {}

CFilesHelper::~CFilesHelper() {}

void CFilesHelper::OnDropFiles(HWND hWnd,HDROP hDropInfo,LPCTSTR lpszCurrentFolder) {
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

	for(UINT iFile = 0; iFile < nFiles; iFile++) {
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);

		DWORD dwAttr = ::GetFileAttributes(szFileName);
		if(dwAttr==0xFFFFFFFF) {
			CString txt = _L("Error|GetFileAttributes","Error getting information about %1.");
			txt.Replace("%1",szFileName);
			AtlMessageBox(hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		} else if(dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
			// Add directory
			CDlgAddDirectory dlg(szFileName);
			if(dlg.DoModal(hWnd)==IDOK) {
				CString strWildCard;
				if(dlg.m_bAddFiles && !dlg.m_strWildCard.IsEmpty())
					strWildCard = dlg.m_strWildCard;
				else
					strWildCard = "*.*";
				CFileInfoArray fia;
				CWaitCursor wait;

					CString strAddedDir(szFileName);
					int pos = strAddedDir.ReverseFind('\\');
					if(pos<0) pos = strAddedDir.ReverseFind('/');
					if(pos>=0) strAddedDir = strAddedDir.Mid(pos+1);

				CMyUtils::EndWith(szFileName,'\\');
				fia.AddDir(
					CString(szFileName),strWildCard,
					dlg.m_bIncludeSubDirectories,
					dlg.m_bAddDirectories
				);
				if(fia.GetSize()>0) {
					// Finn katalog filene skal registreres under
					CString strCurrentFolder(lpszCurrentFolder);
					CMyUtils::EndWith(strCurrentFolder,'\\');
					strCurrentFolder += strAddedDir;

					{ // Add the main directory entry
						CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_DIRS);
						pLine->SetParameter("Name",strCurrentFolder);
						m_pDoc->GetScript().AddLine(pLine);
					}
					CMyUtils::EndWith(strCurrentFolder,'\\');
					
					// Loop gjennom og add alle filer funnet
					for(int i=0;i<fia.GetSize();i++) {
						if(dlg.m_bAddFiles && !fia[i].IsDirectory()) {
							// Get path between szFileName and the files name
							CString strSubPath(fia[i].GetFilePath());
							strSubPath = strSubPath.Mid(_tcslen(szFileName)/*+1*/);
							pos = strSubPath.ReverseFind('\\');
							if(pos<0) strSubPath.Empty();
							else strSubPath = /*"\\"+*/strSubPath.Left(pos);

							CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_FILES);
							CString strSource(fia[i].GetFilePath());
							CString strSourceDir;
							if(!m_pDoc->GetUseAbsolutePaths() && m_pDoc->GetSourceDir(strSourceDir)) {
								strSource = CFunc::ExtractRelativePath(strSourceDir,strSource);
								if(strSource.IsEmpty())
									strSource = fia[i].GetFilePath();
							}
							if(dlg.m_bExternal && !dlg.m_strRoot.IsEmpty()) {
								if(!_tcsnicmp(dlg.m_strRoot,strSource,dlg.m_strRoot.GetLength()))
									strSource = "{src}\\" + strSource.Mid(dlg.m_strRoot.GetLength());
							}

							pLine->SetParameter("Source",strSource);
							pLine->SetParameter("DestDir",strCurrentFolder+strSubPath);

							if(fia[i].IsReadOnly()) pLine->SetParameterFlag("Attribs","readonly",true);
							if(fia[i].IsHidden()) pLine->SetParameterFlag("Attribs","hidden",true);
							if(fia[i].IsSystem()) pLine->SetParameterFlag("Attribs","system",true);
							if(dlg.m_bExternal) pLine->SetParameterFlag("Flags","external",true);
							m_pDoc->GetScript().AddLine(pLine);
						} else if(dlg.m_bAddDirectories && fia[i].IsDirectory()) {
							// Get path between szFileName and the files name
							CString strSubPath(fia[i].GetFilePath());
							strSubPath = strSubPath.Mid(_tcslen(szFileName)/*+1*/);

							CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_DIRS);
							pLine->SetParameter("Name",strCurrentFolder+strSubPath);
							m_pDoc->GetScript().AddLine(pLine);
						}
					}
					m_pDoc->SetModifiedFlag();
				}
			}
		} else {
			// Add file
			InsertFileName(szFileName,lpszCurrentFolder);
			m_pDoc->SetModifiedFlag();
		}
	}
	::DragFinish(hDropInfo);
}

void CFilesHelper::InsertFileName(LPCTSTR lpszFileName,LPCTSTR lpszCurrentFolder) {
	CString strCurrentFolder(lpszCurrentFolder);

	CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_FILES);
	CString strSourceDir;
	if(!m_pDoc->GetUseAbsolutePaths() && m_pDoc->GetSourceDir(strSourceDir)) {
		CString strSource = CFunc::ExtractRelativePath(strSourceDir,lpszFileName);
		if(strSource.IsEmpty()) strSource = lpszFileName;
		pLine->SetParameter("Source",strSource);
	} else
		pLine->SetParameter("Source",lpszFileName);
	pLine->SetParameter("DestDir",strCurrentFolder);

	m_pDoc->GetScript().AddLine(pLine);
	AutoComponentSelect(pLine);
}

void CFilesHelper::CreateIcon(HWND hWnd,CScriptLine* pItem) {
	ATLASSERT(pItem);

	// Find sensible information for the icon
	CString strDestName, strName, strFilename;
	CSplitPath path;
	if(pItem->GetParameter("DestName")) {
		strDestName = pItem->GetParameter("DestName");
		path.Split(strDestName);
	} else {
		path.Split(pItem->GetParameter("Source"));
	}
	strName = CString("{group}\\")+path.GetFilename();
	path.SetDrive("");
	path.SetDirectory(SAFESTR(pItem->GetParameter("DestDir")));
	path.Make(strFilename.GetBuffer(MAX_PATH));

	// Enumerate icons to see if this file got an icon already
	bool bAddIcon = true;
	CScriptList icons;
	m_pDoc->GetScript().GetList(CInnoScript::SEC_ICONS,icons);
	for(int nPos=0;nPos<icons.GetSize();nPos++) {
		CScriptLine* pIconCheck = icons[nPos];
		if(strFilename == SAFESTR(pIconCheck->GetParameter("Filename"))) {
			CString txt = _L("IconAlreadyExists","An icon already exists for %1.\n\nCreate icon anyway?");
			txt.Replace("%1",strFilename);
			int nRet = AtlMessageBox(hWnd,(LPCTSTR)txt,MB_YESNOCANCEL|MB_DEFBUTTON2|MB_ICONQUESTION);
			if(nRet==IDCANCEL)
				return;
			else if(nRet==IDNO)
				bAddIcon = false;
			break;
		}
	}

	// Create the icon
	if(bAddIcon) {
		CScriptLine* pIcon = new CScriptLine(CInnoScript::SEC_ICONS);
		pIcon->SetParameter("Name",strName);
		pIcon->SetParameter("Filename",strFilename);
		pIcon->SetParameter("WorkingDir",pItem->GetParameter("DestDir"));
		pIcon->SetParameter("Comment",path.GetFilename());
		pIcon->SetParameterFlag("Flags","createonlyiffileexists",true);
		m_pDoc->GetScript().AddLine(pIcon);
		m_pDoc->SetModifiedFlag();
	}
}

void CFilesHelper::OnFileAddFiles(LPCTSTR lpszCurrentFolder) {
	const int nSize = 65535;
	CFileDialog dlg(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLESIZING,NULL,0);
	LPSTR lpstrFile = new CHAR[nSize];
	dlg.m_ofn.lpstrFile = lpstrFile;
	dlg.m_ofn.lpstrFile[0] = 0;
	dlg.m_ofn.nMaxFile = nSize;
	if(dlg.DoModal()==IDOK) {
		if(dlg.m_ofn.nFileExtension==0) {
			LPCTSTR lpszFile = dlg.m_ofn.lpstrFile;
			CString strFolder(lpszFile);
			lpszFile += dlg.m_ofn.nFileOffset;
			while(*lpszFile) {
				CString strFile(lpszFile);
				while(*lpszFile++);
				InsertFileName(strFolder+"\\"+strFile,lpszCurrentFolder);
			}
		} else {
			InsertFileName(dlg.m_ofn.lpstrFile,lpszCurrentFolder);
		}
		m_pDoc->SetModifiedFlag();
	}
	delete []lpstrFile;
}

class CTextImport {
public:
	static void GetTextFromFile(LPCTSTR,CString&);
protected:
	static inline BOOL IsBOM(PBYTE pb) {
		return (*pb == 0xFF) & (*(pb+1) == 0xFE);
	}
	static inline BOOL IsRBOM(PBYTE pb) {
		return (*pb == 0xFE) & (*(pb+1) == 0xFF);
	}
};

void CTextImport::GetTextFromFile(LPCTSTR lpszFileName,CString& str) {
	str.Empty();

	FILE* fp = fopen(lpszFileName,"rb");
	fseek(fp,0,SEEK_END);
	int nLength = ftell(fp);
	fseek(fp,0,SEEK_SET);
	BYTE* ptr = new BYTE[nLength+2];
	memset(ptr,0,nLength+2);
	fread(ptr,nLength,1,fp);
	fclose(fp);

	if(IsBOM(ptr) || IsRBOM(ptr)) {
		USES_CONVERSION;
		str = W2A(((LPCWSTR)ptr)+1);
	} else {
		int nPos = nLength;
		while(nPos--) {
			if(ptr[nPos]==0 && ptr[nPos+1]=='"') {
				memmove(&ptr[nPos],&ptr[nPos+1],nLength-nPos);
			}
		}
		str = ptr;
	}

	delete []ptr;
}

bool CFilesHelper::ImportRegistry(HWND hWnd,LPCTSTR lpszRegFile) {
	CWaitCursor wait;
	bool bRet = false;

	CString strFile;
	CTextImport::GetTextFromFile(lpszRegFile,strFile);

	LPCTSTR pszRoot = NULL;
	CString strSubkey;
	CStringToken tok_lines(strFile,"\n\r");
	LPCTSTR lpszFormat = tok_lines.GetNext();

	if(!_stricmp(lpszFormat,"REGEDIT4") || !_stricmp(lpszFormat,"Windows Registry Editor Version 5.00")) {
		while(LPCTSTR lpszLine = tok_lines.GetNext()) {
			CString str(lpszLine);
			CScriptLine* p = NULL;
			str.TrimLeft(); str.TrimRight();
			if(str.IsEmpty() || str[0]==';') continue;

			if(str[0]=='[' && str[str.GetLength()-1]==']') {
				// Find root and key
				CStringToken token(str.Mid(1,str.GetLength()-2),"\\");
				LPCTSTR lpszRoot = token.GetNext();
				strSubkey = token.GetRest();
				strSubkey.Replace("{","{{");

				bool bDeleteKey = false;
				if(*lpszRoot=='-') {
					bDeleteKey = true;
					lpszRoot++;
				}

				if(!_stricmp(lpszRoot,"HKEY_CLASSES_ROOT"))
					pszRoot = "HKCR";
				else if(!_stricmp(lpszRoot,"HKEY_CURRENT_USER"))
					pszRoot = "HKCU";
				else if(!_stricmp(lpszRoot,"HKEY_LOCAL_MACHINE"))
					pszRoot = "HKLM";
				else if(!_stricmp(lpszRoot,"HKEY_USERS"))
					pszRoot = "HKU";
				else if(!_stricmp(lpszRoot,"HKEY_CURRENT_CONFIG"))
					pszRoot = "HKCC";
				else {
					CString txt = _L("Unknown registry root %1.");
					txt.Replace("%1",lpszRoot);
					AtlMessageBox(hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
					pszRoot = NULL;
				}

				//p = new CScriptRegistry(nRoot,strSubkey);
				if(bDeleteKey) {
					p = new CScriptLine(CInnoScript::SEC_REGISTRY);
					p->SetParameter("Root",pszRoot);
					p->SetParameter("SubKey",strSubkey);
					p->SetParameterFlag("Flags","deletekey",true);
				}
			} else {
				// Find value
				while(str.Right(1)=="\\") {
					CString tmp = tok_lines.GetNext();
					tmp.TrimLeft(); tmp.TrimRight();
					if(!tmp.IsEmpty()) {
						str = str.Left(str.GetLength()-1);
						str += tmp;
					}
				}
				CStringToken token(str,"=");
				CString strValueName(token.GetNext());
				CString strValueData(token.GetRest());
				bool bDeleteValue = strValueData == "-";
				if(strValueName[0]=='"' && strValueName[strValueName.GetLength()-1]=='"')
					strValueName = strValueName.Mid(1,strValueName.GetLength()-2);
				if(strValueData[0]=='"' && strValueData[strValueData.GetLength()-1]=='"')
					strValueData = strValueData.Mid(1,strValueData.GetLength()-2);
				
				p = new CScriptLine(CInnoScript::SEC_REGISTRY);
				p->SetParameter("Root",pszRoot);
				p->SetParameter("SubKey",strSubkey);
				int nPos = strValueData.Find(':');
				if(nPos>=0) {
					CStringToken token(strValueData,":");
					LPCTSTR lpszValueType = token.GetNext();
					CString strNewValueData(token.GetRest());

					if(!_stricmp(lpszValueType,"dword")) {
						p->SetParameter("ValueType","dword");
						if(strNewValueData[0]!='$')
							strValueData = "$"+strNewValueData;
						else
							strValueData = strNewValueData;
					} else if(!_stricmp(lpszValueType,"hex")) {
						p->SetParameter("ValueType","binary");
						strValueData = strNewValueData;
						strValueData.Replace(","," ");	// Replace commas with spaces
					} else {
						p->SetParameter("ValueType","string");
					}
				} else
					p->SetParameter("ValueType","string");

				if(strValueName=="@") strValueName.Empty();
				p->SetParameter("ValueName",strValueName);
				// Replace { with {{
				strValueData.Replace("{","{{");
				strValueData.Replace("\\\\","\\");

				p->SetParameter("ValueData",strValueData);
				if(bDeleteValue) {
					p->DeleteParameter("ValueData");
					p->SetParameterFlag("Flags","deletevalue",true);
					p->SetParameter("ValueType","none");
				}
			}
			if(p) {
				m_pDoc->GetScript().AddLine(p);
				m_pDoc->SetModifiedFlag();
//				InsertItem(p);
				bRet = true;
			}
		}
	} else {
		CString txt = _L("Error|UnknownRegistryFormat","Unknown or unimplemented registry format '%1'.");
		txt.Replace("%1",lpszFormat);
		AtlMessageBox(hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}
	return bRet;
}

bool CFilesHelper::CreateODBCProfile() {
	CDlgODBC	dlg(m_pDoc);

	return dlg.DoModal(AfxGetMainWnd())==IDOK;
}

void CFilesHelper::OnDropFilesRegistry(HWND hWnd,HDROP hDropInfo) {
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

	for(UINT iFile = 0; iFile < nFiles; iFile++) {
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
		ImportRegistry(hWnd,szFileName);
	}
	::DragFinish(hDropInfo);
}

bool CFilesHelper::ImportIni(HWND hWnd,LPCTSTR pszPathName) {
	CWaitCursor wait;
	FILE* file = fopen(pszPathName,"r");
	if(!file) {
		CString txt = _L("Failed to open '%1'.");
		txt.Replace("%1",pszPathName);
		AtlMessageBox(hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return false;
	}
	CString strFilename(pszPathName);
	CString strSection;

	CString str;
	bool bAdded = false;
	while(fgets(str.GetBuffer(1024),1024,file)) {
		str.ReleaseBuffer();
		str.TrimLeft(); str.TrimRight();
		if(str.IsEmpty()) continue;

		if(str[0]=='[' && str[str.GetLength()-1]==']') {
			strSection = str.Mid(1,str.GetLength()-2);
		} else if(str[0]!=';') {
			CStringToken token(str,"=");
			CString strKey(token.GetNext());
			CString strString(token.GetRest());

			CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_INI);
			pLine->SetParameter("Filename",strFilename);
			pLine->SetParameter("Section",strSection);
			pLine->SetParameter("Key",strKey);
			pLine->SetParameter("String",strString);
			bAdded = true;
			m_pDoc->GetScript().AddLine(pLine);
			m_pDoc->SetModifiedFlag();
		}
	}
	fclose(file);
	if(bAdded) m_pDoc->UpdateAll();
	return true;
}

void CFilesHelper::AutoComponentSelect(CScriptList& list) {
	// Automatically selects a component if destdir matches a component
	if(CMyApp::m_prefs.m_bAutoComponentSelect) {
		for(int nPos0=0;nPos0<list.GetSize();nPos0++) {
			CScriptLine* pFile = list[nPos0];
			CString str(pFile->GetParameter("DestDir"));

			if(str.IsEmpty()) continue;
			if(str[str.GetLength()-1] == '\\' || str[str.GetLength()-1] == '/')
				str = str.Left(str.GetLength()-1);
			int nPos = str.ReverseFind('\\');
			if(nPos<0) nPos = str.ReverseFind('/');
			if(nPos>=0) {
				str = str.Mid(nPos+1);
				CScriptList list;
				m_pDoc->GetScript().GetList(CInnoScript::SEC_COMPONENTS,list);
				for(int nPos=0;nPos<list.GetSize();nPos++) {
					if(!str.CompareNoCase(list[nPos]->GetParameter("Name"))) {
						pFile->SetParameterFlag("Components",str,true);
						break;
					}
				}
			}
		}
	}
}

void CFilesHelper::AutoComponentSelect(CScriptLine* pLine) {
	CScriptList list;
	list.Add(pLine);
	AutoComponentSelect(list);
}

void CFilesHelper::RenameComponent(LPCTSTR pszFrom,LPCTSTR pszTo) {
	CWaitCursor wait;
	CScriptList	list;
	CInnoScriptEx& script = m_pDoc->GetScript();

	script.GetList(CInnoScript::SEC_FILES,list);
	script.GetList(CInnoScript::SEC_DIRS,list);
	script.GetList(CInnoScript::SEC_ICONS,list);
	script.GetList(CInnoScript::SEC_REGISTRY,list);
	script.GetList(CInnoScript::SEC_INSTALLDELETE,list);
	script.GetList(CInnoScript::SEC_RUN,list);
	script.GetList(CInnoScript::SEC_UNINSTALLDELETE,list);
	script.GetList(CInnoScript::SEC_UNINSTALLRUN,list);
	script.GetList(CInnoScript::SEC_TASKS,list);
	const LPCTSTR pszSection = "Components";

	for(int nPos=0;nPos<list.GetSize();nPos++) {
		CScriptLine* pLine = list[nPos];
		if(pLine->GetParameterFlag(pszSection,pszFrom)) {
			pLine->SetParameterFlag(pszSection,pszFrom,false);
			pLine->SetParameterFlag(pszSection,pszTo,true);
		}
	}
}

void CFilesHelper::RenameType(LPCTSTR pszFrom,LPCTSTR pszTo) {
	CWaitCursor wait;
	CScriptList	list;
	CInnoScriptEx& script = m_pDoc->GetScript();

	script.GetList(CInnoScript::SEC_COMPONENTS,list);
	const LPCTSTR pszSection = "Types";

	for(int nPos=0;nPos<list.GetSize();nPos++) {
		CScriptLine* pLine = list[nPos];
		if(pLine->GetParameterFlag(pszSection,pszFrom)) {
			pLine->SetParameterFlag(pszSection,pszFrom,false);
			pLine->SetParameterFlag(pszSection,pszTo,true);
		}
	}
}

void CFilesHelper::RenameTask(LPCTSTR pszFrom,LPCTSTR pszTo) {
	CWaitCursor wait;
	CScriptList	list;
	CInnoScriptEx& script = m_pDoc->GetScript();

	script.GetList(CInnoScript::SEC_FILES,list);
	script.GetList(CInnoScript::SEC_DIRS,list);
	script.GetList(CInnoScript::SEC_ICONS,list);
	script.GetList(CInnoScript::SEC_REGISTRY,list);
	script.GetList(CInnoScript::SEC_INSTALLDELETE,list);
	script.GetList(CInnoScript::SEC_RUN,list);
	script.GetList(CInnoScript::SEC_UNINSTALLDELETE,list);
	script.GetList(CInnoScript::SEC_UNINSTALLRUN,list);
	const LPCTSTR pszSection = "Tasks";

	for(int nPos=0;nPos<list.GetSize();nPos++) {
		CScriptLine* pLine = list[nPos];
		if(pLine->GetParameterFlag(pszSection,pszFrom)) {
			pLine->SetParameterFlag(pszSection,pszFrom,false);
			pLine->SetParameterFlag(pszSection,pszTo,true);
		}
	}
}
