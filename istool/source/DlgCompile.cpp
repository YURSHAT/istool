#include "stdafx.h"
#include "ISTool.h"
#include "DlgCompile.h"

CDlgCompile::CDlgCompile(CMyDoc* pDoc,const bool bForceRun) :
	m_hCompiler(NULL), m_pDoc(pDoc), m_nCurrentLine(0),
	m_eAbort(FALSE,TRUE), m_eDone(FALSE,TRUE), m_bForceRun(bForceRun),
	m_hCompilerDLS(NULL)
{
	m_sec = CInnoScript::SEC_NONE;
	m_logFile = NULL;
	m_nErrorLine = 0;
	LPCTSTR pszLogFile = m_pDoc->GetScript().GetPropertyString("LogFile",CInnoScript::PRJ_ISTOOL);
	bool bAppend = m_pDoc->GetScript().GetPropertyBool("LogFileAppend",CInnoScript::PRJ_ISTOOL);
	if(pszLogFile && *pszLogFile) {
		m_logFile = bAppend ? fopen(pszLogFile,"ab") : fopen(pszLogFile,"wb");
	}
}

CDlgCompile::~CDlgCompile() {
	if(m_hCompiler) FreeLibrary(m_hCompiler);
	if(m_hCompilerDLS) FreeLibrary(m_hCompilerDLS);
	if(m_logFile) fclose(m_logFile);
}

LRESULT CDlgCompile::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	_L(m_hWnd,"Compile");
	CenterWindow(GetParent());

	m_wndList.Attach(GetDlgItem(IDC_COMPILE_LIST));
	m_wndList.SetHorizontalExtent(1024);
	m_wndProgress.Attach(GetDlgItem(IDC_PROGRESS));

	HMENU hMenu = GetSystemMenu(FALSE);
	EnableMenuItem(hMenu,SC_CLOSE,MF_BYCOMMAND|MF_GRAYED);
	//GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_GRAYED);

	//m_script.SetLines(m_pDoc->GetScript().CopyScript());
	m_script = m_pDoc->GetScript();
	m_nCurrentLine = 0;

	if(!m_hCompiler) LoadCompiler();
	if(m_hCompiler) {
		DWORD dwThreadID;
		CreateThread(NULL,0,ThreadEntry,this,0,&dwThreadID);
	}

	return TRUE;
}

LRESULT CDlgCompile::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if(wID==IDCANCEL) {
		if(WaitForSingleObject(m_eDone,0)==WAIT_OBJECT_0) {
			EndDialog(wID);
		} else {
			m_eAbort.SetEvent();
			::EnableWindow(GetDlgItem(IDCANCEL),FALSE);
		}
	}
	return 0;
}

LRESULT CDlgCompile::OnGotoError(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	EndDialog(wID);
	return 0;
}

//-------------------------------------------------------------------------------------

bool CDlgCompile::LoadCompiler() {
	CString strLibrary;
	if(!m_pDoc->GetCompiler(strLibrary,true,false))
		return false;

	m_hCompiler = LoadLibrary(strLibrary);
	if(!m_hCompiler) 
	{
		CString txt = _L("Failed to open library '%1'.");
		txt.Replace("%1",strLibrary);
		AtlMessageBox(m_hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return false;
	} else 
	{
		m_fCompileScriptA = (ISDllCompileScriptProcA)GetProcAddress(m_hCompiler,"ISDllCompileScript");
		m_fCompileScriptW = NULL;
		//m_fCompileScriptW = (ISDllCompileScriptProcW)GetProcAddress(m_hCompiler,"ISDllCompileScriptW");
		m_fCompileScriptISPPA = (ISDllCompileScriptISPPProcA)GetProcAddress(m_hCompiler,"ISPPDllCompileScript");
		m_fCompileScriptISPPW = NULL;
		//m_fCompileScriptISPPW = (ISDllCompileScriptISPPProcW)GetProcAddress(m_hCompiler,"ISPPDllCompileScriptW");
		m_fGetVersion = (ISDllGetVersionProc)GetProcAddress(m_hCompiler,"ISDllGetVersion");

		// ISPP is installed and ISTool is doing the
		// pre-processing, so bypass ISPP when compiling
		if((m_fCompileScriptISPPA || m_fCompileScriptISPPW) && CMyApp::m_prefs.m_bPreProcess) 
			if(m_pDoc->GetCompiler(strLibrary,true,true)) 
			{
				if(m_hCompilerDLS = LoadLibrary(strLibrary)) 
				{
					m_fCompileScriptA = (ISDllCompileScriptProcA)GetProcAddress(m_hCompilerDLS,"ISDllCompileScript");
					m_fCompileScriptW = NULL;
					//m_fCompileScriptW = (ISDllCompileScriptProcW)GetProcAddress(m_hCompilerDLS,"ISDllCompileScriptW");
				}
			}

		return true;
	}
}

long CDlgCompile::GetErrorLine() {
	return m_nErrorLine - 1;
}

DWORD WINAPI CDlgCompile::ThreadEntry(LPVOID lpParameter) {
	CDlgCompile* pDlg = (CDlgCompile*)lpParameter;
	DWORD dwRet = 0;

	pDlg->m_pDoc->SetCurrentDir();

	if((pDlg->m_fCompileScriptISPPA || pDlg->m_fCompileScriptISPPW) && CMyApp::m_prefs.m_bPreProcess)
		dwRet = pDlg->PreProcess();

	if(dwRet==0)
		dwRet = pDlg->DoCompile();

	if(!pDlg->m_strIncludeFile.IsEmpty())
		DeleteFile(pDlg->m_strIncludeFile);

	if(pDlg->m_logFile) {
		fclose(pDlg->m_logFile);
		pDlg->m_logFile = NULL;
	}
	if(pDlg->m_pDoc->m_bCompileAndExit) {
		CMyApp::m_nExitCode = dwRet;
		pDlg->PostMessage(WM_CLOSE);
	}

	return dwRet;
}

UINT CDlgCompile::PreProcess() {
	// Find script folder
	CString strScriptPath(m_pDoc->GetPathName());
	int nPos = strScriptPath.ReverseFind('\\');
	if(nPos<0) nPos = strScriptPath.ReverseFind('/');
	if(nPos>=0) strScriptPath = strScriptPath.Left(nPos);

	// Compile script
	if(m_fCompileScriptISPPW)
	{
		TCompileScriptParamsW params;
		params.Size = sizeof params;
		params.CompilerPath = NULL;
		params.ScriptPath = CA2W(strScriptPath);
		params.CallbackProc = CompilerCallbackW;
		params.AppData = (DWORD)this;
		
		TIsppOptionsW options = {
			OPTION_B | OPTION_P,
			OPTION_E,
			0,
			"\2{#",
			"\1}",
			'\\'
		};
		m_fCompileScriptISPPW(&params,&options,NULL,NULL);
	}
	else
	{
		TCompileScriptParamsA params;
		params.Size = sizeof params;
		params.CompilerPath = NULL;
		params.ScriptPath = strScriptPath;
		params.CallbackProc = CompilerCallbackA;
		params.AppData = (DWORD)this;
		
		TIsppOptionsA options = {
			OPTION_B | OPTION_P,
			OPTION_E,
			0,
			"\2{#",
			"\1}",
			'\\'
		};
		m_fCompileScriptISPPA(&params,&options,NULL,NULL);
	}
	if(m_strTranslation.IsEmpty()) {
		AddListString("ISPP preprocessing failed.");
		SetFinished();
		return 2;
	}

	// Empty string
	AddListString("");

	m_script.Clear();
	m_script.LoadScriptBuffer(m_strTranslation.GetBuffer());
	//m_script.WriteScript("c:\\temp\\debug.iss");

	return 0;
}

UINT CDlgCompile::DoCompile() {
	CString tmp;
	__time64_t time;
	_time64(&time);
	tmp.Format("Compilation started: %s",_ctime64(&time));
	tmp.TrimRight();
	AddListString(tmp);

	// Perform pre-compile steps
	AddListString("Running pre-compile steps");
	if(!RunCompileSteps(CInnoScript::PRJ_PRECOMPILESTEPS)) {
		SetFinished();
		return 1;
	}
	AddListString("");

	if(!AddDownloadSection())
		return 1;


	// Print compiler version information
	TCompilerVersionInfo* pVersionInfo = m_fGetVersion();
	CString strVersion;
	strVersion.Format("Compiling script using %s %s",
		pVersionInfo->Title,
		pVersionInfo->Version);
	AddListString(strVersion);

	// Find script folder
	CString strScriptPath(m_pDoc->GetPathName());
	int nPos = strScriptPath.ReverseFind('\\');
	if(nPos<0) nPos = strScriptPath.ReverseFind('/');
	if(nPos>=0) strScriptPath = strScriptPath.Left(nPos);

	// Compile script
	if(m_fCompileScriptW)
	{
		TCompileScriptParamsW params;
		params.Size = sizeof params;
		params.CompilerPath = NULL;
		params.ScriptPath = CA2W(strScriptPath);
		params.CallbackProc = CompilerCallbackW;
		params.AppData = (DWORD)this;
		if(m_fCompileScriptW(&params)!=isceNoError) 
		{
			SetFinished();
			return 2;
		}
	} 
	else
	{
		TCompileScriptParamsA params;
		params.Size = sizeof params;
		params.CompilerPath = NULL;
		params.ScriptPath = strScriptPath;
		params.CallbackProc = CompilerCallbackA;
		params.AppData = (DWORD)this;
		if(m_fCompileScriptA(&params)!=isceNoError) 
		{
			SetFinished();
			return 2;
		}
	}

	// Empty string
	AddListString("");

	// Perform post-compile steps
	if(m_logFile) fflush(m_logFile);
	int nString = AddListString("Running post-compile steps");
	if(nString!=LB_ERR) m_wndList.SetTopIndex(nString);
	if(!RunCompileSteps(CInnoScript::PRJ_POSTCOMPILESTEPS)) {
		SetFinished();
		return 3;
	}

	_time64(&time);
	tmp.Format("Compilation ended: %s",_ctime64(&time));
	tmp.TrimRight();
	AddListString(tmp);

	if(!m_pDoc->m_bCompileAndExit) {
		if(m_bForceRun || (CMyApp::m_prefs.m_bTestCompiledSetup && AtlMessageBox(m_hWnd,_L("TestSetupNow","Setup compiled successfully. Test setup now?"),IDR_MAINFRAME,MB_YESNO|MB_ICONQUESTION)==IDYES)) {
			CMyApp::MyExec(m_strOutputExeFilename,NULL);
		}
		CString strOld = m_pDoc->GetScript().GetPropertyString("OutputExeFilename",CInnoScript::PRJ_ISTOOL);
		if(strOld!=m_strOutputExeFilename && !CMyApp::m_prefs.m_bNoOutputExeFilename) {
			m_pDoc->GetScript().SetPropertyString("OutputExeFilename",m_strOutputExeFilename,CInnoScript::PRJ_ISTOOL);
			m_pDoc->SetModifiedFlag();
		}
	}

	// We're done
	SetFinished();
	if(m_logFile) fflush(m_logFile);

	return 0;
}

LONG __stdcall CDlgCompile::CompilerCallbackA(LONG Code,TCompilerCallbackDataA* Data, DWORD AppData) {
	return reinterpret_cast<CDlgCompile*>(AppData)->CompilerCallbackA(Code,Data);
}

UINT CDlgCompile::CompilerCallbackA(LONG Code,TCompilerCallbackDataA* Data) {
	int nString = LB_ERR;
	CString str;

	if(WaitForSingleObject(m_eAbort,0)==WAIT_OBJECT_0)
		return iscrRequestAbort;

	switch(Code) {
	case iscbReceiveTranslation:
		m_strTranslation = Data->NotifyStatus.StatusMsg;
		break;
	case iscbReadScript:
		if(Data->ReadScript.Reset) {
			m_sec = CInnoScript::SEC_NONE;
			//m_pCurrentLine = m_pScriptLines;
			m_nCurrentLine = 0;
		}

		if(m_nCurrentLine>=m_script.GetSize()) {
			Data->ReadScript.LineRead = NULL;
		} else if(m_script[m_nCurrentLine]->GetSection()!=m_sec && m_script[m_nCurrentLine]->GetSection()!=CInnoScript::SEC_NONE) {
			m_sec = m_script[m_nCurrentLine]->GetSection();
			m_strCurrentLine.Format("[%s]",CInnoScriptEx::GetSectionName(m_sec));
			Data->ReadScript.LineRead = m_strCurrentLine;
		} else {
			m_script[m_nCurrentLine]->Write(m_strCurrentLine.GetBuffer(8192),8192);
			m_strCurrentLine.ReleaseBuffer();
			Data->ReadScript.LineRead = m_strCurrentLine;
			m_nCurrentLine++;
		}
		break;

	case iscbNotifyStatus:
		if(Data->NotifyStatus.StatusMsg)
			nString = AddListString(Data->NotifyStatus.StatusMsg);
		break;

	case iscbNotifyIdle:
		if(Data->NotifyIdle.CompressProgress>0) {
			if(!m_wndProgress.IsWindowVisible())
				m_wndProgress.ShowWindow(SW_SHOW);
			
			m_wndProgress.SetRange32(0,Data->NotifyIdle.CompressProgressMax);
			m_wndProgress.SetPos(Data->NotifyIdle.CompressProgress);
		}
		break;

	case iscbNotifySuccess:
		if(Data->NotifySuccess.OutputExeFilename) {
			m_strOutputExeFilename = Data->NotifySuccess.OutputExeFilename;
			nString = AddListString(Data->NotifySuccess.OutputExeFilename);
		}
		break;

	case iscbNotifyError:
		if(Data->NotifyError.ErrorMsg) {
			CString str;
			if(Data->NotifyError.ErrorFilename) 
				str.Format("%s line %d:",Data->NotifyError.ErrorFilename,Data->NotifyError.ErrorLine);
			else {
				str.Format("Line %d:",Data->NotifyError.ErrorLine);
				m_nErrorLine = Data->NotifyError.ErrorLine;
			}

			AddListString(str);
			nString = AddListString(Data->NotifyError.ErrorMsg);
		}
		break;
	}

	if(nString!=LB_ERR) {
//		m_wndList.SetTopIndex(nString);
//		m_wndList.UpdateWindow();
		//Sleep(500);
	}

	return iscrSuccess;
}

LONG __stdcall CDlgCompile::CompilerCallbackW(LONG Code,TCompilerCallbackDataW* Data, DWORD AppData) {
	return reinterpret_cast<CDlgCompile*>(AppData)->CompilerCallbackW(Code,Data);
}

UINT CDlgCompile::CompilerCallbackW(LONG Code,TCompilerCallbackDataW* Data) {
	int nString = LB_ERR;
	CString str;

	if(WaitForSingleObject(m_eAbort,0)==WAIT_OBJECT_0)
		return iscrRequestAbort;

	switch(Code) {
	case iscbReceiveTranslation:
		m_strTranslation = Data->NotifyStatus.StatusMsg;
		break;
	case iscbReadScript:
		if(Data->ReadScript.Reset) {
			m_sec = CInnoScript::SEC_NONE;
			//m_pCurrentLine = m_pScriptLines;
			m_nCurrentLine = 0;
		}

		if(m_nCurrentLine>=m_script.GetSize()) {
			Data->ReadScript.LineRead = NULL;
		} else if(m_script[m_nCurrentLine]->GetSection()!=m_sec && m_script[m_nCurrentLine]->GetSection()!=CInnoScript::SEC_NONE) {
			m_sec = m_script[m_nCurrentLine]->GetSection();
			m_strCurrentLine.Format("[%s]",CInnoScriptEx::GetSectionName(m_sec));
			Data->ReadScript.LineRead = CA2W(m_strCurrentLine);
		} else {
			m_script[m_nCurrentLine]->Write(m_strCurrentLine.GetBuffer(8192),8192);
			m_strCurrentLine.ReleaseBuffer();
			Data->ReadScript.LineRead = CA2W(m_strCurrentLine);
			m_nCurrentLine++;
		}
		break;

	case iscbNotifyStatus:
		if(Data->NotifyStatus.StatusMsg)
			nString = AddListString(CW2A(Data->NotifyStatus.StatusMsg));
		break;

	case iscbNotifyIdle:
		if(Data->NotifyIdle.CompressProgress>0) {
			if(!m_wndProgress.IsWindowVisible())
				m_wndProgress.ShowWindow(SW_SHOW);
			
			m_wndProgress.SetRange32(0,Data->NotifyIdle.CompressProgressMax);
			m_wndProgress.SetPos(Data->NotifyIdle.CompressProgress);
		}
		break;

	case iscbNotifySuccess:
		if(Data->NotifySuccess.OutputExeFilename) {
			m_strOutputExeFilename = Data->NotifySuccess.OutputExeFilename;
			nString = AddListString(CW2A(Data->NotifySuccess.OutputExeFilename));
		}
		break;

	case iscbNotifyError:
		if(Data->NotifyError.ErrorMsg) {
			CString str;
			if(Data->NotifyError.ErrorFilename) 
				str.Format("%s line %d:",Data->NotifyError.ErrorFilename,Data->NotifyError.ErrorLine);
			else {
				str.Format("Line %d:",Data->NotifyError.ErrorLine);
				m_nErrorLine = Data->NotifyError.ErrorLine;
			}

			AddListString(str);
			nString = AddListString(CW2A(Data->NotifyError.ErrorMsg));
		}
		break;
	}

	if(nString!=LB_ERR) {
//		m_wndList.SetTopIndex(nString);
//		m_wndList.UpdateWindow();
		//Sleep(500);
	}

	return iscrSuccess;
}

bool CDlgCompile::AddDownloadSection() {
	CScriptList listDownload;
	m_script.GetList(CInnoScript::PRJ_DOWNLOAD,listDownload);
	if(listDownload.GetSize()>0) {
		CAtlTemporaryFile codefile;
		CString strCode, tmp;
		AddListString("Building download script");

		strCode += "[Code]\r\n";
		//strCode += "function istool_Download(hWnd: Integer; URL, Filename: PChar): Integer; external 'isxdl_Download@files:isxdl.dll stdcall';\r\n";
		strCode += "procedure istool_AddFile(URL, Filename: PChar); external 'isxdl_AddFile@files:isxdl.dll stdcall';\r\n";
		//strCode += "procedure istool_AddFileSize(URL, Filename: PChar; Size: Cardinal); external 'isxdl_AddFileSize@files:isxdl.dll stdcall';\r\n";
		strCode += "function istool_DownloadFiles(hWnd: Integer): Integer; external 'isxdl_DownloadFiles@files:isxdl.dll stdcall';\r\n";
		strCode += "procedure istool_ClearFiles; external 'isxdl_ClearFiles@files:isxdl.dll stdcall';\r\n";
		//strCode += "function istool_IsConnected: Integer; external 'isxdl_IsConnected@files:isxdl.dll stdcall';\r\n";
		//strCode += "function istool_SetOption(Option, Value: PChar): Integer; external 'isxdl_SetOption@files:isxdl.dll stdcall';\r\n";
		//strCode += "function istool_GetFileName(URL: PChar): PChar; external 'isxdl_GetFileName@files:isxdl.dll stdcall';\r\n";
		strCode += "\r\ntype\r\n";
		strCode += "  ISXDL = record\r\n";
		strCode += "    Source:     String;\r\n";
		strCode += "    DestDir:    String;\r\n";
		strCode += "    DestName:   String;\r\n";
		strCode += "    Tasks:      String;\r\n";
		strCode += "    Components: String;\r\n";
		strCode += "    Languages:  String;\r\n";
		strCode += "    Flags:      Cardinal;\r\n";
		strCode += "  end;\r\n";
		strCode += "\r\nvar\r\n";
		strCode += "  istool_files: array of ISXDL;\r\n";
		strCode += "\r\nprocedure istool_download_init();\r\n";
		strCode += "begin\r\n";
		//strCode += "  ExtractTemporaryFile('norwegian.ini');\r\n";
		//strCode += "  isxdl_SetOption('language',ExpandConstant('{tmp}\\norwegian.ini'));\r\n";
		//strCode += "  isxdl_SetOption('title','Setup - Download Demo');\r\n";
		//strCode += "  isxdl_SetOption('label','Some label...');\r\n";
		//strCode += "  isxdl_SetOption('description','Some description...');\r\n";
		tmp.Format("  SetArrayLength(istool_files,%d);\r\n",listDownload.GetSize()); strCode += tmp;
		for(int i=0;i<listDownload.GetSize();i++) {
			CScriptLine* pLine = listDownload[i];
			CString strSource, strDestDir, strDestName, strTasks, strComponents, strLanguages;
			strSource = pLine->GetParameter("Source");
			strDestDir = pLine->GetParameter("DestDir");
			strDestName = pLine->GetParameter("DestName");
			strTasks = pLine->GetParameter("Tasks");
			strComponents = pLine->GetParameter("Components");
			strLanguages = pLine->GetParameter("Languages");

			if(strSource.Find('{')<0)
				tmp.Format("  istool_files[%d].Source     := '%s';\r\n",i,strSource); 
			else
				tmp.Format("  istool_files[%d].Source     := ExpandConstant('%s');\r\n",i,strSource); 
			strCode += tmp;
			if(strDestDir.Find('{')<0)
				tmp.Format("  istool_files[%d].DestDir    := '%s';\r\n",i,strDestDir); 
			else
				tmp.Format("  istool_files[%d].DestDir    := ExpandConstant('%s');\r\n",i,strDestDir); 
			strCode += tmp;
			if(strDestName.Find('{')<0)
				tmp.Format("  istool_files[%d].DestName   := '%s';\r\n",i,strDestName); 
			else
				tmp.Format("  istool_files[%d].DestName   := ExpandConstant('%s');\r\n",i,strDestName); 
			strCode += tmp;
			tmp.Format("  istool_files[%d].Tasks      := '%s';\r\n",i,strTasks);
			strCode += tmp;
			tmp.Format("  istool_files[%d].Components := '%s';\r\n",i,strComponents);
			strCode += tmp;
			tmp.Format("  istool_files[%d].Languages  := '%s';\r\n",i,strLanguages);
			strCode += tmp;
			tmp.Format("  istool_files[%d].Flags      := 0;\r\n",i); 
			strCode += tmp;
		}
		strCode += "end;\r\n";

		strCode += "\r\nfunction istool_checklanguages(Languages: String): Boolean;\r\n";
		strCode += "begin\r\n";
		strCode += "	Result := True;\r\n";
		strCode += "	if Languages = '' then exit;\r\n";
		strCode += "	if Pos(ActiveLanguage,Languages)=0 then Result := False;\r\n";
		strCode += "end;\r\n";

		strCode += "\r\nfunction istool_download(CurPage: Integer): boolean;\r\n";
		strCode += "var\r\n";
		strCode += "  i, NumFiles: Integer;\r\n";
		strCode += "  DestFile, DestDir: String;\r\n";
		strCode += "begin\r\n";
		strCode += "  if CurPage<>wpReady then begin\r\n";
		strCode += "    Result := true;\r\n";
		strCode += "    Exit;\r\n";
		strCode += "  end;\r\n";
		strCode += "  istool_ClearFiles();\r\n";
		strCode += "  istool_download_init();\r\n";
		strCode += "  NumFiles := GetArrayLength(istool_files);\r\n";
		strCode += "  for i:=0 to NumFiles-1 do begin\r\n";
		// 4.0 strCode += "    if srYes <> ShouldProcessEntry(istool_files[i].Components,istool_files[i].Tasks) then continue;\r\n";
		strCode += "	if (IsComponentSelected(istool_files[i].Components) = false) or (IsTaskSelected(istool_files[i].Tasks) = false) then continue;\r\n";
		strCode += "    if not istool_checklanguages(istool_files[i].Languages) then continue;\r\n";
		strCode += "    DestDir := AddBackslash(istool_files[i].DestDir);\r\n";
		strCode += "    if istool_files[i].DestName='' then\r\n";
		strCode += "      DestFile := DestDir + 'dlfile.' + IntToStr(i)\r\n";
		strCode += "    else\r\n";
		strCode += "      DestFile := DestDir + istool_files[i].DestName;\r\n";
//			strCode += "    MsgBox(DestFile,mbInformation,MB_OK);\r\n";
//			strCode += "	MsgBox(DestDir,mbInformation,MB_OK);\r\n";
		strCode += "    istool_AddFile(istool_files[i].Source,DestFile);\r\n";
		strCode += "  end;\r\n";
		strCode += "  Result := 0<>istool_DownloadFiles(StrToInt(ExpandConstant('{wizardhwnd}')));\r\n";
		strCode += "end;\r\n";
		strCode += "\r\n[Files]\r\n";

		tmp.Format("Source: %sisxdl.dll; DestDir: {tmp}; Flags: dontcopy\r\n",theApp.m_strProgramPath);
		strCode += tmp;

		//strCode += "function NextButtonClick(CurPage: Integer): Boolean;\r\n";
		//strCode += "begin\r\n";
		//strCode += "  Result := istool_download(CurPage);\r\n";
		//strCode += "end;\r\n";


		if(FAILED(codefile.Create())) {
			AddListString("Failed to create code file");
			SetFinished();
			return false;
		}
		if(FAILED(codefile.Write((LPCVOID)(LPCTSTR)strCode,strCode.GetLength()))) {
			AddListString("Failed to write code file");
			SetFinished();
			return false;
		}
		CString strTempFileName = codefile.TempFileName();
		codefile.HandsOff();
		AddListString(strTempFileName);
		AddListString("");

		if(m_logFile) AppendLogFile(strTempFileName);

//			AtlMessageBox(m_hWnd,(LPCTSTR)strCode);
//			AtlMessageBox(m_hWnd,(LPCTSTR)strTempFileName);
		m_strIncludeFile = strTempFileName;
		// Add include file to script
		tmp.Format("#include \"%s\"",strTempFileName);
		m_script.AddHead(new CInnoScript::CLineComment(CInnoScript::SEC_NONE,tmp));
	}
	return true;
}

void CDlgCompile::ParseDir(LPCTSTR pszFilter,CAtlTemporaryFile& file,const CString strDestDir,const CString& strRoot) {
	CString tmp;
	CFindFile find;

	if(find.FindFile(pszFilter)) {
		do {
			if(find.IsDots()) {
			} else if(find.IsDirectory()) {
				tmp.Format("%s\\*.*",find.GetFilePath());
				ParseDir(tmp,file,strDestDir,strRoot);
			} else {
				CString strSource = find.GetFilePath();
				strSource = strSource.Mid(strRoot.GetLength());
				CString strSourcePath = find.GetRoot();
				strSourcePath = strSourcePath.Mid(strRoot.GetLength());
				tmp.Format("Source: {src}\\%s; Flags: external; DestDir: %s\r\n",
					find.GetFilePath().Mid(strRoot.GetLength()),
					strDestDir + strSourcePath
				);
				file.Write((LPCTSTR)tmp,tmp.GetLength());
			}
		} while(find.FindNextFile());
	}
}

void CDlgCompile::AppendLogFile(LPCTSTR pszFileName) {
	if(!m_logFile) return;
	FILE* fp = fopen(pszFileName,"rb");
	if(fp) {
		fprintf(m_logFile,"==============================================================================\r\n");
		fprintf(m_logFile,"Contents of \"%s\"\r\n",pszFileName);
		fprintf(m_logFile,"==============================================================================\r\n");

		char szLine[8192];
		while(fgets(szLine,sizeof szLine,fp)) {
			fputs(szLine,m_logFile);
		}
		fclose(fp);

		fprintf(m_logFile,"==============================================================================\r\n");
	}
}

void CDlgCompile::SetFinished() {
	m_eDone.SetEvent();
	::EnableWindow(GetDlgItem(IDCANCEL),TRUE);
	GetDlgItem(IDC_GOTOERROR).EnableWindow(m_nErrorLine>0);
	SetDlgItemText(IDCANCEL,"Close");

	HMENU hMenu = GetSystemMenu(FALSE);
	EnableMenuItem(hMenu,SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
}

int CDlgCompile::AddListString(LPCTSTR pszString) {
	if(m_logFile) fprintf(m_logFile,"%s\r\n",pszString);

	m_wndList.SetRedraw(FALSE);
	int nString = m_wndList.AddString(pszString);
	m_wndList.SetTopIndex(nString);
	m_wndList.SetRedraw(TRUE);
	m_wndList.UpdateWindow();
	return nString;
}

bool CDlgCompile::RunCompileSteps(CInnoScript::SECTION sec) {
	CString strSection;
	if(sec==CInnoScript::PRJ_PRECOMPILESTEPS)
		strSection = "[PRE-COMPILE] ";
	else
		strSection = "[POST-COMPILE] ";
	CScriptList	steps;
	m_script.GetList(sec,steps);

	CString strOutputDir;
	try {
		m_pDoc->GetOutputFolder(strOutputDir);
	}
	catch(...) {
		return false;
	}

	for(int nPos=0;nPos<steps.GetSize();nPos++) {
		CScriptLine* pLine = steps[nPos];
		if(pLine->GetComment()) continue;

		CString strName = pLine->GetParameter("Name");
		CString strParam = pLine->GetParameter("Parameters");

		bool bMinimized = pLine->GetParameterFlag("Flags","runminimized");
		bool bAbortOnError = pLine->GetParameterFlag("Flags","abortonerror");

		strParam.Replace("{script}",m_pDoc->GetPathName());
		strParam.Replace("{outputdir}",strOutputDir);

		AddListString(strSection + strName + " " + strParam);

		if(CMyApp::MyExec(strName,strParam,NULL,true,bMinimized) && bAbortOnError)
			return false;
	}
	return true;
}
