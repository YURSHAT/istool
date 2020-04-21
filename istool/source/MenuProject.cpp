/*==============================================================================

  $Id: MenuProject.cpp,v 1.1 2002/03/21 17:55:38 net Exp $
  
  Implements the Project menu in the CMainFrame class

==============================================================================*/

#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

#include "DlgSelectFonts.h"
#include "DlgCompile.h"
#include "DlgFileAssociation.h"
#include "FilesHelper.h"
#include "DlgBdeAlias.h"
#include "DlgIEShortcut.h"
#include "DlgLangOptions.h"
#include "DlgLogFile.h"
// Setup pages
#include "SetupCompiler.h"
#include "SetupDiskSpanning.h"
#include "SetupApplication.h"
#include "SetupUserInfo.h"
#include "SetupAppDir.h"
#include "SetupProgramGroup.h"
#include "SetupPages.h"
#include "SetupAppearance.h"
#include "SetupUninstall.h"
#include "Sheets.h"

LRESULT CMainFrame::OnProjectUseAbsolutePaths(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CUpdate::UpdateAll(CUpdate::HINT_APPLYCHANGES);
	bool bAbs = true;
	try {
		bAbs = !m_document.GetScript().GetPropertyBool("UseAbsolutePaths",CInnoScript::PRJ_ISTOOL);
	}
	catch(...) {
		bAbs ^= 1;
	}
	m_document.GetScript().SetPropertyBool("UseAbsolutePaths",bAbs,CInnoScript::PRJ_ISTOOL);
	m_document.SetModifiedFlag(true);

	CScriptList files;
	m_document.GetScript().GetList(CInnoScript::SEC_FILES,files);

	CString strSourceDir;
	m_document.GetSourceDir(strSourceDir);

	if(files.GetSize()>0) {
		if(bAbs) {
			int nResult = AtlMessageBox(m_hWnd,_L("ConvertAbsPaths","Do you want to convert existing paths to absolute paths?"),IDR_MAINFRAME,MB_YESNO|MB_ICONQUESTION);
			if(nResult==IDYES) {
				for(int nPos=0;nPos<files.GetSize();nPos++) {
					CScriptLine* pLine = files[nPos];
					CString strSource = pLine->GetParameter("Source");
					if(CFunc::GetDriveLength(strSource)==0 && (strSource.GetLength()==0 || strSource[0]!='{')) {
						// This is a relative path
						CString strNewSource(strSourceDir);
						if(strSource.GetLength()>0) {
							if(strSource.Left(3)=="..\\" || strSource.Left(3)=="../") {
								int nPos = strNewSource.ReverseFind('\\');
								if(nPos<0)
									nPos = strNewSource.ReverseFind('/');
								if(nPos>=0) {
									strNewSource = strNewSource.Left(nPos);
									strSource = strSource.Mid(3);
								}
							} else if(strSource.Left(2)==".\\" || strSource.Left(2)=="./") {
								strSource = strSource.Mid(2);
							}

							if(strSource[0]!='\\' && strSource[0]!='/')
								CMyUtils::EndWith(strNewSource,'\\');

							strNewSource += strSource;
							pLine->SetParameter("Source",strNewSource);
						}
					}
				}
			}
		} else {
			int nResult = AtlMessageBox(m_hWnd,_L("ConvertRelPaths","Do you want to convert existing paths to relative paths?"),IDR_MAINFRAME,MB_YESNO|MB_ICONQUESTION);
			if(nResult==IDYES) {
				for(int nPos=0;nPos<files.GetSize();nPos++) {
					CScriptLine* pLine = files[nPos];
					CString strSource = pLine->GetParameter("Source");
					strSource = CFunc::ExtractRelativePath(strSourceDir,strSource);
					if(!strSource.IsEmpty() && strSource[0]!='{')
						pLine->SetParameter("Source",strSource);
				}
			}
		}
	}
	
	CUpdate::UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectRunSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;
	CString strOutputExe;
	if(m_document.GetOutputExe(m_document.m_pScript,m_hWnd,strOutputExe))
		CMyApp::MyExec(strOutputExe,NULL);
	return 0;
}

LRESULT CMainFrame::OnProjectCompileAndRunSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CDlgCompile dlg(&m_document,true);
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);
	if(dlg.LoadCompiler()) {
		if(dlg.DoModal(m_hWnd)==IDC_GOTOERROR) {
			SetSection(ID_VIEW_SCRIPT);
			m_wndView.SendMessage(SCI_GOTOLINE,(WPARAM)dlg.GetErrorLine(),(LPARAM)0);
			m_wndView.SetFocus();
		} else if(!CMyApp::m_prefs.m_bNoOutputExeFilename)
			m_document.UpdateAll();
	}

	return 0;
}

LRESULT CMainFrame::OnProjectOpenCompiler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.OpenInnoSetup(m_hWnd);
	return 0;
}

LRESULT CMainFrame::OnProjectCompileScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CDlgCompile dlg(&m_document,false);
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);
	if(dlg.LoadCompiler()) {
		if(dlg.DoModal(m_hWnd)==IDC_GOTOERROR) {
			SetSection(ID_VIEW_SCRIPT);
			m_wndView.SendMessage(SCI_GOTOLINE,(WPARAM)dlg.GetErrorLine(),(LPARAM)0);
			m_wndView.SetFocus();
			return 1;
		} else if(!CMyApp::m_prefs.m_bNoOutputExeFilename)
			m_document.UpdateAll();
	}
	return 0;
}

LRESULT CMainFrame::OnProjectCreateAssociation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgFileAssociation	dlg(&m_document);
	if(dlg.DoModal(AfxGetMainWnd())==IDOK) {
		m_document.UpdateAll();
	}
	return 0;
}

LRESULT CMainFrame::OnProjectInsertFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CFilesHelper helper(&m_document);
	CString strFolder;
	if(m_wndFilesT.GetView()) {
		m_wndFilesT.GetView()->GetCurrentFolder(strFolder);
	} else if(m_wndFilesL.GetView() && m_wndFilesL.GetView()->GetSelectedCount()==1) {
		UINT nCount = m_wndFilesL.GetView()->GetItemCount();
		while(nCount--) {
			if(m_wndFilesL.GetView()->GetItemState(nCount,LVIS_SELECTED)==LVIS_SELECTED) {
				CScriptLine* pLine = (CScriptLine*)m_wndFilesL.GetView()->GetItemData(nCount);
				if(pLine && pLine->GetSection()==CInnoScript::SEC_DIRS) {
					strFolder = pLine->GetParameter("Name");
				}
				break;
			}
		}
	}
	if(strFolder.IsEmpty())
		strFolder = "{app}";

	helper.OnFileAddFiles(strFolder);
	m_document.UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectImportRegistry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CFileDialog dlg(TRUE,".reg",NULL,0,"Registry Files (*.reg)\0*.reg\0All Files (*.*)\0*.*\0",0);
	if(dlg.DoModal()!=IDOK) return 0;

	CFilesHelper helper(&m_document);
	if(helper.ImportRegistry(m_hWnd,dlg.m_szFileName))	
		m_document.UpdateAll();
	else
		m_document.UpdateAll();	// for reg on W2000?!?
	return 0;
}

LRESULT CMainFrame::OnProjectImportIni(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CFileDialog dlg(TRUE,".ini",NULL,0,"INI Files (*.ini)\0*.ini\0All Files (*.*)\0*.*\0",0);
	if(dlg.DoModal()!=IDOK) return 0;
	CFilesHelper helper(&m_document);
	helper.ImportIni(m_hWnd,dlg.m_szFileName);	
	return 0;
}

LRESULT CMainFrame::OnProjectInstallFonts(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;

	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgSelectFonts dlg(&m_document);
	if(dlg.DoModal(AfxGetMainWnd())==IDOK) {
		wait.Restore();
		m_document.UpdateAll();
	}
	return 0;
}

static LPCTSTR GetFullRoot(LPCTSTR pszRoot) {
	if(!_stricmp(pszRoot,"HKCR"))
		return "HKEY_CLASSES_ROOT";
	if(!_stricmp(pszRoot,"HKCU"))
		return "HKEY_CURRENT_USER";
	if(!_stricmp(pszRoot,"HKLM"))
		return "HKEY_LOCAL_MACHINE";
	if(!_stricmp(pszRoot,"HKU"))
		return "HKEY_USERS";
	if(!_stricmp(pszRoot,"HKCC"))
		return "HKEY_CURRENT_CONFIG";

	return pszRoot;
}

LRESULT CMainFrame::OnProjectExportRegistry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CScriptList	list;
	m_document.GetScript().GetList(CInnoScript::SEC_REGISTRY,list);
	if(list.GetSize()==0) return 0;

	CFileDialog dlg(FALSE,".reg",NULL,0,"Registry Files (*.reg)\0*.reg\0All Files (*.*)\0*.*\0",0);
	if(dlg.DoModal()!=IDOK) return 0;

	CWaitCursor wait;
	FILE* fp = fopen(dlg.m_szFileName,"w");	
	if(!fp) {
		CString txt = _L("Error|CreateFile","Failed to create '%1'.");
		txt.Replace("%1",dlg.m_szFileName);
		AtlMessageBox(m_hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return 0;
	}


	fprintf(fp,"REGEDIT4\n\n");
	for(int nPos=0;nPos<list.GetSize();nPos++) {
		CScriptLine *pLine = list[nPos];

		fprintf(fp,"[%s\\%s]\n",
			GetFullRoot(pLine->GetParameter("Root")),
			pLine->GetParameter("Subkey")
		);

		CString strValueName(pLine->GetParameter("ValueName"));
		if(strValueName.IsEmpty()) strValueName = "@";

		CString strValueData(pLine->GetParameter("ValueData"));
		if(!strValueData.IsEmpty()) {
			// Fix for different stuff
			LPCTSTR pszValueType = pLine->GetParameter("ValueType");
			if(!_stricmp(pszValueType,"binary")) {
				strValueData = "hex:" + strValueData;
				strValueData.Replace(" ",",");
			} else if(!_stricmp(pszValueType,"dword")) {
				if(strValueData[0]=='$') {
					strValueData = "dword:" + strValueData.Mid(1);
				} else {
					DWORD dwValueData = atol(strValueData);
					strValueData.Format("dword:%08X",dwValueData);
				}
			} else {
				strValueData.Replace("{{","{");
				strValueData = "\"" + strValueData;
				strValueData += "\"";
			}
		}

		fprintf(fp,"\"%s\"=%s\n",strValueName,strValueData);

		fprintf(fp,"\n");
	}
	
	fclose(fp);
	return 0;
}

LRESULT CMainFrame::OnProjectImportMessages(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CFileDialog dlg(TRUE,NULL,NULL,0,"Message Files (*.isl;*.iss)\0*.isl;*.iss\0All Files (*.*)\0*.*\0",0);

	if(dlg.DoModal()!=IDOK) return 0;

	CWaitCursor wait;

	CString strFilename(dlg.m_szFileName);
	CString strSection;
	FILE* file = fopen(strFilename,"r");
	if(!file) {
		CString txt = _L("Failed to open '%1'.");
		txt.Replace("%1",strFilename);
		AtlMessageBox(m_hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return 0;
	}
	DWORD dwFlags = 0;
	CString str;
	UINT nLine = 0;
	bool bInSection = false;
	bool bAdded = false;
	while(fgets(str.GetBuffer(1024),1024,file)) {
		nLine++;
		str.ReleaseBuffer();
		str.TrimLeft(); str.TrimRight();
		if(str.IsEmpty() || str[0]==';') continue;

		if(str[0]=='[') {
			if(!str.CompareNoCase("[Messages]")) {
				bInSection = true;
			} else {
				bInSection = false;
			}
		} else if(bInSection) {
			CScriptLine* pLine = NULL;
			try {
				pLine = new CInnoScript::CLineSetup(CInnoScript::SEC_MESSAGES,str);
				bAdded = true;
				if(CMyApp::m_prefs.m_bOverwriteMessages)
					m_document.GetScript().SetPropertyString(pLine->GetKey(),pLine->GetValue(),pLine->GetSection());
				else
					m_document.GetScript().AddLine(pLine);
				m_document.SetModifiedFlag();
			} catch(LPCTSTR ptr) {
				if(pLine) delete pLine;
				CString txt = _L("Error|ParseMessages","Error parsing messages.\n\n%1");
				txt.Replace("%1",ptr);
				AtlMessageBox(m_hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			}
		}
	}
	fclose(file);

	if(bAdded) m_document.UpdateAll();

	return 0;
}

#define GetCurrentFolder(a) a = "{app}"

LRESULT CMainFrame::OnProjectCreateDirectory(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CString strFolder;
	GetCurrentFolder(strFolder);
	strFolder += "\\";

	CScriptList	list;

	CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_DIRS);
	pLine->SetParameter("Name",strFolder);
	list.Add(pLine);

	if(!CSheets::ShowSheet(m_hWnd,list,true)) {
		delete pLine;
		return 0;
	}

	m_document.SetModifiedFlag();
	m_document.GetScript().AddLine(pLine);
	m_document.UpdateAll();
	
	return 0;
}

LRESULT CMainFrame::OnProjectCreateOdbcProfile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CFilesHelper helper(&m_document);
	if(helper.CreateODBCProfile())
		m_document.UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectCreateBdeAlias(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgBdeAlias dlg;
	if(dlg.DoModal(AfxGetMainWnd())==IDOK) {
		CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_FILES);
		pLine->SetParameter("Source","AddAlias.exe");
		pLine->SetParameter("DestDir","{tmp}");
		m_document.GetScript().AddLine(pLine);

		m_document.GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_RUN,"; Alias "+dlg.m_strAliasName));

		CString strParams = "\"";
		if(dlg.m_bOverwrite) strParams += "-";
		strParams += dlg.m_strAliasName;
		strParams += "\" \"";
		strParams += dlg.m_strDataDirectory;
		strParams += "\" \"";
		strParams += dlg.m_strDriverName;
		strParams += "\"";

		CScriptLine* pRun = new CScriptLine(CInnoScript::SEC_RUN);
		pRun->SetParameter("Filename","{tmp}\\AddAlias.exe");
		pRun->SetParameter("Parameters",strParams);
		m_document.GetScript().AddLine(pRun);
		m_document.UpdateAll();
		m_document.SetModifiedFlag();
	}
	return 0;
}

LRESULT CMainFrame::OnProjectCreateInternetShortcut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgIEShortcut dlg;
	dlg.m_strFileName = "{app}\\";
	dlg.m_strFileName += m_document.GetScript().GetPropertyString("AppName");
	dlg.m_strFileName += ".url";

	dlg.m_strURL = "http://www.";
	dlg.m_strURL += m_document.GetScript().GetPropertyString("AppName");
	dlg.m_strURL += ".com/";
	dlg.m_strURL.MakeLower();

	if(dlg.DoModal(AfxGetMainWnd())==IDOK) {
		CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_INI);
		pLine->SetParameter("Filename",dlg.m_strFileName);
		pLine->SetParameter("Section","InternetShortcut");
		pLine->SetParameter("Key","URL");
		pLine->SetParameter("String",dlg.m_strURL);
		m_document.GetScript().AddLine(pLine);

		pLine = new CScriptLine(CInnoScript::SEC_UNINSTALLDELETE);
		pLine->SetParameter("Type","files");
		pLine->SetParameter("Name",dlg.m_strFileName);
		m_document.GetScript().AddLine(pLine);
		
		m_document.UpdateAll();
		m_document.SetModifiedFlag();
	}
	return 0;
}

/*
** Verifies that all files exists on the local computer.
*/
LRESULT CMainFrame::OnProjectVerifyFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor		wait;
	CScriptList		list;
	CString			strSourceDir;

	bool bValidSourceDir = m_document.GetSourceDir(strSourceDir);
	if(bValidSourceDir) CMyUtils::EndWith(strSourceDir,'\\');

	// Unselect all files if files section
	bool bFilesList = m_uCurrentView==ID_VIEW_FILES && CMyApp::m_prefs.m_bFilesList;
	CListViewCtrl m_wndFilesL(m_wndView);
	if(bFilesList) {
		UINT nCount = m_wndFilesL.GetItemCount();
		while(nCount--) m_wndFilesL.SetItemState(nCount,0,LVIS_SELECTED);
	}
	
	m_document.GetScript().GetList(CInnoScript::SEC_FILES,list);
	CString strMessage;
	UINT nCount = 0;
	for(int nPos=0;nPos<list.GetSize();nPos++) {
		CScriptLine* pLine = list[nPos];
		CString strSource = pLine->GetParameter("Source");

		// Don't check empty entries and anything that uses constants
		if(!strSource.GetLength() || strSource[0]=='{') continue;

		CString strFileName;
		m_document.GetScriptFileName(strFileName,strSource);

		if(!CMyUtils::FileExists(strFileName)) {
		//if(!CMyUtils::FileExists(strSource) && !CMyUtils::FileExists(strSourceDir+strSource)) {
			if(bFilesList) {
				UINT nCount = m_wndFilesL.GetItemCount();
				while(nCount--) {
					CScriptLine* pData = (CScriptLine*)m_wndFilesL.GetItemData(nCount);
					if(pData==pLine) {
						m_wndFilesL.SetItemState(nCount,LVIS_SELECTED,LVIS_SELECTED);
						break;
					}
				}
			}
			if(++nCount==11) {
				strMessage += "\n\t...";
			} else if(nCount<11) {
				if(strMessage.GetLength()>0) strMessage += "\n";
				strMessage += "\t" + strSource;
			}
		}
	}
	if(strMessage.GetLength()>0) {
		strMessage = _L("FilesNotFound","The following files could not be found:\n") + strMessage;
		if(bFilesList) strMessage += _L("FilesSelected","\n\nThese files have been selected.");
		AtlMessageBox(m_hWnd,(LPCTSTR)strMessage,IDR_MAINFRAME,MB_OK|MB_ICONWARNING);
	} else {
		AtlMessageBox(m_hWnd,_L("VerifiedOk","All checked files seem to exist."),IDR_MAINFRAME);
	}
	return 0;
}

LRESULT CMainFrame::OnProjectLangOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);
	CDlgLangOptions dlg(&m_document);
	if(dlg.DoModal(AfxGetMainWnd())==IDOK) {
		m_document.UpdateAll();
		m_document.SetModifiedFlag();
	}
	return 0;
}

LRESULT CMainFrame::OnProjectExportMessages(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CScriptList	list;
	m_document.GetScript().GetList(CInnoScript::SEC_MESSAGES,list);
	if(list.GetSize()==0) return 0;

	CFileDialog dlg(FALSE,".isl",NULL,OFN_OVERWRITEPROMPT,"Message Files (*.isl)\0*.isl\0All Files (*.*)\0*.*\0",0);

	if(dlg.DoModal()!=IDOK) return 0;

	CWaitCursor wait;
	FILE* fp = fopen(dlg.m_szFileName,"w");	
	if(!fp) {
		CString txt = _L("Error|CreateFile","Failed to create '%1'.");
		txt.Replace("%1",dlg.m_szFileName);
		AtlMessageBox(m_hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return 0;
	}


	fprintf(fp,"[Messages]\r\n");
	for(int nPos=0;nPos<list.GetSize();nPos++) {
		CScriptLine *pLine = list[nPos];
		CString strLine;

		pLine->Write(strLine.GetBuffer(8000),8000);
		fprintf(fp,"%s\r\n",strLine);
	}
	
	fclose(fp);
	return 0;
}

LRESULT CMainFrame::OnProjectCreateUninstallIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	// TODO: Sjekk at et slikt icon ikke finnes fra før
	CString strAppName = m_document.GetScript().GetPropertyString("AppName");	
	if(strAppName.IsEmpty()) {
		AtlMessageBox(m_hWnd,_L("NeedAppName","Please enter a value for application name in options first."),IDR_MAINFRAME,MB_OK|MB_ICONWARNING);
		return 0;
	}

	CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_ICONS);
	pLine->SetParameter("Name","{group}\\{cm:UninstallProgram, " + strAppName + "}");
	pLine->SetParameter("Filename","{uninstallexe}");
	m_document.GetScript().AddLine(pLine);
	
	m_document.UpdateAll();
	m_document.SetModifiedFlag();
	AtlMessageBox(m_hWnd,_L("Icon added."),IDR_MAINFRAME);
	return 0;
}

LRESULT CMainFrame::OnProjectOptions(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	int nPage = 0;

	if(wID!=ID_PROJECT_OPTIONS) nPage = wID - ID_PROJECT_OPTIONS_COMPILER;

	{
		CMyPropertySheet	sheet(_L("DialogTitles|SetupOptions","Setup Options"),nPage);

		CSetupCompiler		pageCompiler(&m_document,false,_L("DialogTitles|Compiler","Compiler"));
		CSetupSpanning		pageSpanning(&m_document,false,_L("DialogTitles|DiskSpanning","Disk Spanning"));
		CSetupApplication	pageApplication(&m_document,false,_L("DialogTitles|Application","Application"));
		CSetupUserInfo		pageUserInfo(&m_document,false,_L("DialogTitles|UserInfo","User Info"));
		CSetupAppDir		pageAppDir(&m_document,false,_L("DialogTitles|AppDirectory","App Directory"));
		CSetupProgramGroup	pageGroup(&m_document,false,_L("DialogTitles|ProgramGroup","Program Group"));
		CSetupPages			pagePages(&m_document,false,_L("DialogTitles|Pages","Pages"));
		CSetupAppearance	pageAppearance(&m_document,false,_L("DialogTitles|Appearance","Appearance"));
		CSetupUninstall		pageUninstall(&m_document,false,_L("DialogTitles|Uninstall","Uninstall"));

		sheet.AddPage(pageCompiler);
		sheet.AddPage(pageSpanning);
		sheet.AddPage(pageApplication);
		sheet.AddPage(pageUserInfo);
		sheet.AddPage(pageAppDir);
		sheet.AddPage(pageGroup);
		sheet.AddPage(pagePages);
		sheet.AddPage(pageAppearance);
		sheet.AddPage(pageUninstall);

		int nRet = sheet.DoModal(AfxGetMainWnd());
		if(nRet==IDOK) m_document.SetModifiedFlag(TRUE);
	}

	m_document.UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectLogFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgLogFile dlg(m_document.GetScript());
	if(dlg.DoModal(m_hWnd)==IDOK) m_document.SetModifiedFlag();

	m_document.UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectUninstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;
	CInnoScriptEx& script = m_document.GetScript();

	if(!script.GetPropertyBool("Uninstallable"))
		return 0;

	LPCTSTR pszKey = script.GetPropertyString("AppID");
	if(!pszKey || !*pszKey) 
		return 0;

	const LPCTSTR pszPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s_is1";
	CString strKey;
	strKey.Format(pszPath,pszKey);

	CRegKey reg;
	if(reg.Open(HKEY_CURRENT_USER,strKey,KEY_READ)!=ERROR_SUCCESS)
		if(reg.Open(HKEY_LOCAL_MACHINE,strKey,KEY_READ)!=ERROR_SUCCESS)
			return 0;

	ULONG nChars = MAX_PATH;
	CString strValue;
	if(reg.QueryStringValue("UninstallString",strValue.GetBuffer(nChars),&nChars)!=ERROR_SUCCESS)
		return 0;

	strValue.ReleaseBuffer(nChars-1);


	STARTUPINFO			si;
	PROCESS_INFORMATION	pi;
	DWORD				dwResult = -1;

	memset(&si,0,sizeof si);
	si.cb = sizeof si;

	if(!CreateProcess(NULL,strValue.GetBuffer(MAX_PATH),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) {
		return 0;
	}

	wait.Restore();
	CloseHandle(pi.hThread);
	WaitForInputIdle(pi.hProcess,INFINITE);
	WaitForSingleObject(pi.hProcess,INFINITE);
	GetExitCodeProcess(pi.hProcess,&dwResult);
    CloseHandle(pi.hProcess);
	
	return 0;
}