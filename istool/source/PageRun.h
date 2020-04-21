#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageRun dialog

class CPageRun : 
	public CPropertyPageImpl<CPageRun>,
	public CMyPropertyPageBase<CPageRun>,
	public CWinDataExchange<CPageRun>,
	public Henden::CToolTipDialog<CPageRun>
{
public:
	enum { IDD = IDD_RUN };

	BEGIN_MSG_MAP(CPageRun)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageRun>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageRun>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageRun>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_RUN_FILENAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_WAITUNTILIDLE, BN_CLICKED, OnWaitUntilIdle)
		COMMAND_HANDLER(IDC_NOWAIT, BN_CLICKED, OnNoWait)
		COMMAND_HANDLER(IDC_RUN_FILENAME, CBN_DROPDOWN, OnDropdownRunFilename)
		COMMAND_HANDLER(IDC_RUN_PARAMETERS, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_RUN_WORKINGDIR, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SHELLEXEC, BN_CLICKED, OnShellExecute)
		COMMAND_HANDLER(IDC_RUN_RUNMINIMIZED, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_SKIPIFDOESNTEXIST, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_RUNONCEID, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_RUN_POSTINSTALL, BN_CLICKED, OnPostInstall)
		COMMAND_HANDLER(IDC_RUN_UNCHECKED, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_DESCRIPTION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_RUN_SKIPIFSILENT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_SKIPIFNOTSILENT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_RUNMAXIMIZED, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_STATUSMSG, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_RUN_HIDEWIZARD, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUNHIDDEN, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_32BIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_64BIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUN_VERB, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_RUNASCURRENTUSER, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RUNASORIGINALUSER, BN_CLICKED, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageRun)
		DDX_CHECK(IDC_NOWAIT, m_nNoWait)
		DDX_TEXT(IDC_RUN_PARAMETERS, m_strParameters)
		DDX_TEXT(IDC_RUN_WORKINGDIR, m_strWorkingDir)
		DDX_CHECK(IDC_SHELLEXEC, m_nShellExec)
		DDX_TEXT(IDC_RUN_FILENAME, m_strFilename)
		DDX_CHECK(IDC_WAITUNTILIDLE, m_nWaitUntilIdle)
		DDX_CHECK(IDC_RUN_RUNMINIMIZED, m_nRunMinimized)
		DDX_CHECK(IDC_RUN_SKIPIFDOESNTEXIST, m_nSkipIfDoesntExist)
		DDX_TEXT(IDC_RUN_RUNONCEID, m_strRunOnceId)
		DDX_CHECK(IDC_RUN_POSTINSTALL, m_nPostInstall)
		DDX_CHECK(IDC_RUN_UNCHECKED, m_nUnchecked)
		DDX_TEXT(IDC_RUN_DESCRIPTION, m_strDescription)
		DDX_CHECK(IDC_RUN_SKIPIFNOTSILENT, m_nSkipIfNotSilent)
		DDX_CHECK(IDC_RUN_SKIPIFSILENT, m_nSkipIfSilent)
		DDX_CHECK(IDC_RUN_RUNMAXIMIZED, m_nRunMaximized)
		DDX_TEXT(IDC_RUN_STATUSMSG, m_strStatusMsg)
		DDX_CHECK(IDC_RUN_HIDEWIZARD, m_nHideWizard)
		DDX_CHECK(IDC_RUNHIDDEN, m_nRunHidden)
		DDX_CHECK(IDC_RUN_32BIT, m_n32Bit)
		DDX_CHECK(IDC_RUN_64BIT, m_n64Bit)
		DDX_TEXT(IDC_RUN_VERB, m_strVerb)
		DDX_CHECK(IDC_RUNASCURRENTUSER, m_nRunAsCurrentUser)
		DDX_CHECK(IDC_RUNASORIGINALUSER, m_nRunAsOriginalUser)
	END_DDX_MAP()

	CEdit			m_wndRunOnceId;
	CEdit2			m_wndParameters;
	CComboBox2		m_wndWorkingDir;
	CComboBox2		m_wndFilename;
	int				m_nNoWait;
	CString			m_strParameters;
	CString			m_strWorkingDir;
	int				m_nShellExec;
	CString			m_strFilename;
	BOOL			m_nWaitUntilIdle;
	int				m_nRunMinimized;
	int				m_nSkipIfDoesntExist;
	CString			m_strRunOnceId;
	int				m_nPostInstall;
	int				m_nUnchecked;
	CString			m_strDescription;
	int				m_nSkipIfNotSilent;
	int				m_nSkipIfSilent;
	int				m_nRunMaximized;
	CString			m_strStatusMsg, m_strVerb;
	int				m_nHideWizard;
	int				m_nRunHidden, m_n32Bit, m_n64Bit, m_nRunAsCurrentUser, m_nRunAsOriginalUser;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Run");
		m_wndRunOnceId.Attach(GetDlgItem(IDC_RUN_RUNONCEID));
		m_wndParameters.SubclassWindow(GetDlgItem(IDC_RUN_PARAMETERS));
		m_wndWorkingDir.SubclassWindow(GetDlgItem(IDC_RUN_WORKINGDIR));
		m_wndFilename.SubclassWindow(GetDlgItem(IDC_RUN_FILENAME));

		//	m_pDoc->AddFiles(m_wndFilename);
		m_pDoc->AddDirConstants(m_wndWorkingDir);
		m_pDoc->AddDirs(m_wndWorkingDir);

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pLine = m_list[nPos];

			if(m_bIsUninst) {
				m_wndRunOnceId.EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC4).EnableWindow(TRUE);
				GetDlgItem(IDC_RUN_POSTINSTALL).EnableWindow(FALSE);
				GetDlgItem(IDC_RUN_UNCHECKED).EnableWindow(FALSE);
				GetDlgItem(IDC_RUN_DESCRIPTION).EnableWindow(FALSE);
				GetDlgItem(IDC_RUN_SKIPIFSILENT).EnableWindow(FALSE);
				GetDlgItem(IDC_RUN_SKIPIFNOTSILENT).EnableWindow(FALSE);
				GetDlgItem(IDC_RUN_HIDEWIZARD).EnableWindow(FALSE);
				GetDlgItem(IDC_RUNASCURRENTUSER).EnableWindow(FALSE);
				GetDlgItem(IDC_RUNASORIGINALUSER).EnableWindow(FALSE);
			} else {
				m_wndRunOnceId.EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC4).EnableWindow(FALSE);
				GetDlgItem(IDC_RUN_STATUSMSG).EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC6).EnableWindow(TRUE);
			}
			
			if(m_strFilename.CompareNoCase(SAFESTR(pLine->GetParameter("Filename")))) m_strFilename.Empty();
			if(m_strParameters.CompareNoCase(SAFESTR(pLine->GetParameter("Parameters")))) m_strParameters.Empty();
			if(m_strWorkingDir.CompareNoCase(SAFESTR(pLine->GetParameter("WorkingDir")))) m_strWorkingDir.Empty();
			if(m_strRunOnceId.CompareNoCase(SAFESTR(pLine->GetParameter("RunOnceId")))) m_strRunOnceId.Empty();
			if(m_strDescription.CompareNoCase(SAFESTR(pLine->GetParameter("Description")))) m_strDescription.Empty();
			if(m_strStatusMsg.CompareNoCase(SAFESTR(pLine->GetParameter("StatusMsg")))) m_strStatusMsg.Empty();
			if(m_strVerb.CompareNoCase(SAFESTR(pLine->GetParameter("Verb")))) m_strVerb.Empty();

			//Flags
			if(m_nNoWait != (pLine->GetParameterFlag("Flags","nowait") ? TRUE : FALSE)) {
				GetDlgItem(IDC_NOWAIT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nNoWait = 2;
			}

			if(m_nShellExec != (pLine->GetParameterFlag("Flags","shellexec") ? TRUE : FALSE)) {
				GetDlgItem(IDC_SHELLEXEC).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nShellExec = 2;
			}

			if(m_nWaitUntilIdle != (pLine->GetParameterFlag("Flags","waituntilidle") ? TRUE : FALSE)) {
				GetDlgItem(IDC_WAITUNTILIDLE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nWaitUntilIdle = 2;
			}

			if(m_nRunMinimized != (pLine->GetParameterFlag("Flags","runminimized") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_RUNMINIMIZED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nRunMinimized = 2;
			}

			if(m_nSkipIfDoesntExist != (pLine->GetParameterFlag("Flags","skipifdoesntexist") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_SKIPIFDOESNTEXIST).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSkipIfDoesntExist = 2;
			}

			if(m_nPostInstall != (pLine->GetParameterFlag("Flags","postinstall") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_POSTINSTALL).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nPostInstall = 2;
			}

			if(m_nUnchecked != (pLine->GetParameterFlag("Flags","unchecked") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_UNCHECKED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUnchecked = 2;
			}

			if(m_nSkipIfSilent != (pLine->GetParameterFlag("Flags","skipifsilent") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_SKIPIFSILENT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSkipIfSilent = 2;
			}

			if(m_nSkipIfNotSilent != (pLine->GetParameterFlag("Flags","skipifnotsilent") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_SKIPIFNOTSILENT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nSkipIfNotSilent = 2;
			}

			if(m_nRunMaximized != (pLine->GetParameterFlag("Flags","runmaximized") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_RUNMAXIMIZED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nRunMaximized = 2;
			}

			if(m_nHideWizard != (pLine->GetParameterFlag("Flags","hidewizard") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_HIDEWIZARD).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nHideWizard = 2;
			}

			if(m_nRunHidden != (pLine->GetParameterFlag("Flags","runhidden") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUNHIDDEN).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nRunHidden = 2;
			}

			if(m_n32Bit != (pLine->GetParameterFlag("Flags","32bit") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_32BIT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_n32Bit = 2;
			}

			if(m_n64Bit != (pLine->GetParameterFlag("Flags","64bit") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUN_64BIT).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_n64Bit = 2;
			}

			if(m_nRunAsCurrentUser != (pLine->GetParameterFlag("Flags","runascurrentuser") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUNASCURRENTUSER).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nRunAsCurrentUser = 2;
			}

			if(m_nRunAsOriginalUser != (pLine->GetParameterFlag("Flags","runasoriginaluser") ? TRUE : FALSE)) {
				GetDlgItem(IDC_RUNASORIGINALUSER).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nRunAsOriginalUser = 2;
			}
		}

		GetDlgItem(IDC_STATIC5).EnableWindow(!m_bIsUninst && m_nPostInstall);
		GetDlgItem(IDC_RUN_DESCRIPTION).EnableWindow(!m_bIsUninst && m_nPostInstall);
		GetDlgItem(IDC_RUN_UNCHECKED).EnableWindow(!m_bIsUninst && m_nPostInstall);
		GetDlgItem(IDC_STATIC_VERB).EnableWindow(m_nShellExec);
		GetDlgItem(IDC_RUN_VERB).EnableWindow(m_nShellExec);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_list.GetSize()==1;
		if(bForce && m_strFilename.IsEmpty()) {
			AtlMessageBox(m_hWnd,_L("Filename is required."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			return PSNRET_INVALID;
		}

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pLine = m_list[nPos];

			CInnoScriptEx::SetString(pLine,bForce,"Filename",m_strFilename);
			CInnoScriptEx::SetString(pLine,bForce,"Parameters",m_strParameters);
			CInnoScriptEx::SetString(pLine,bForce,"WorkingDir",m_strWorkingDir);
			CInnoScriptEx::SetString(pLine,bForce,"Verb",m_strVerb);
			if(m_bIsUninst)
				CInnoScriptEx::SetString(pLine,bForce,"RunOnceId",m_strRunOnceId);
			CInnoScriptEx::SetString(pLine,bForce,"Description",m_strDescription);
			if(!m_bIsUninst)
				CInnoScriptEx::SetString(pLine,bForce,"StatusMsg",m_strStatusMsg);

			// Flags
			CInnoScriptEx::SetFlag(pLine,"Flags","nowait",m_nNoWait);
			CInnoScriptEx::SetFlag(pLine,"Flags","shellexec",m_nShellExec);
			CInnoScriptEx::SetFlag(pLine,"Flags","waituntilidle",m_nWaitUntilIdle);
			CInnoScriptEx::SetFlag(pLine,"Flags","runminimized",m_nRunMinimized);
			CInnoScriptEx::SetFlag(pLine,"Flags","skipifdoesntexist",m_nSkipIfDoesntExist);
			CInnoScriptEx::SetFlag(pLine,"Flags","postinstall",m_nPostInstall);
			CInnoScriptEx::SetFlag(pLine,"Flags","unchecked",m_nUnchecked);
			CInnoScriptEx::SetFlag(pLine,"Flags","skipifsilent",m_nSkipIfSilent);
			CInnoScriptEx::SetFlag(pLine,"Flags","skipifnotsilent",m_nSkipIfNotSilent);
			CInnoScriptEx::SetFlag(pLine,"Flags","runmaximized",m_nRunMaximized);
			CInnoScriptEx::SetFlag(pLine,"Flags","hidewizard",m_nHideWizard);
			CInnoScriptEx::SetFlag(pLine,"Flags","runhidden",m_nRunHidden);
			CInnoScriptEx::SetFlag(pLine,"Flags","32bit",m_n32Bit);
			CInnoScriptEx::SetFlag(pLine,"Flags","64bit",m_n64Bit);
			CInnoScriptEx::SetFlag(pLine,"Flags","runascurrentuser",m_nRunAsCurrentUser);
			CInnoScriptEx::SetFlag(pLine,"Flags","runasoriginaluser",m_nRunAsOriginalUser);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnPostInstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_RUN_POSTINSTALL);
		GetDlgItem(IDC_STATIC5).EnableWindow(!m_bIsUninst && m_nPostInstall);
		GetDlgItem(IDC_RUN_DESCRIPTION).EnableWindow(!m_bIsUninst && m_nPostInstall);
		GetDlgItem(IDC_RUN_UNCHECKED).EnableWindow(!m_bIsUninst && m_nPostInstall);
		return 0;
	}

	LRESULT OnShellExecute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_SHELLEXEC);
		GetDlgItem(IDC_STATIC_VERB).EnableWindow(m_nShellExec);
		GetDlgItem(IDC_RUN_VERB).EnableWindow(m_nShellExec);
		return 0;
	}

	LRESULT OnWaitUntilIdle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		if(m_nWaitUntilIdle==1 && m_nNoWait==1) {
			m_nNoWait = 0;
			DoDataExchange(DDX_LOAD);
		}
		return 0;
	}

	LRESULT OnNoWait(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		if(m_nWaitUntilIdle==1 && m_nNoWait==1) {
			m_nWaitUntilIdle = 0;
			DoDataExchange(DDX_LOAD);
		}
		return 0;
	}

	LRESULT OnDropdownRunFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_wndFilename.GetCount()<=0) {
			CWaitCursor wait;
			m_pDoc->AddFiles(m_wndFilename);
		}
		return 0;
	}

	CString m_strTitle;
	CPageRun(CScriptList& list,CMyDoc* pDoc,const bool bNew,const bool bIsUninst) : 
		m_list(list), m_pDoc(pDoc), m_bNew(bNew), m_wndWorkingDir(pDoc), 
		m_wndFilename(pDoc), m_wndParameters(pDoc), m_bIsUninst(bIsUninst)
	{
		m_strTitle = _L("DialogTitles|Run","Run");
		SetTitle((LPCTSTR)m_strTitle);

		m_nNoWait = FALSE;
		m_nShellExec = FALSE;
		m_nWaitUntilIdle = FALSE;
		m_nRunMinimized = FALSE;
		m_nSkipIfDoesntExist = FALSE;
		m_nPostInstall = FALSE;
		m_nUnchecked = FALSE;
		m_nSkipIfNotSilent = FALSE;
		m_nSkipIfSilent = FALSE;
		m_nRunMaximized = FALSE;
		m_nHideWizard = FALSE;
		m_nRunHidden = FALSE;
		m_n32Bit = FALSE;
		m_n64Bit = FALSE;
		m_nRunAsCurrentUser = FALSE;
		m_nRunAsOriginalUser = FALSE;
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pLine = m_list[nPos];

			m_strFilename		= pLine->GetParameter("Filename");
			m_strParameters		= pLine->GetParameter("Parameters");
			m_strWorkingDir		= pLine->GetParameter("WorkingDir");
			m_strRunOnceId		= pLine->GetParameter("RunOnceId");
			m_strDescription	= pLine->GetParameter("Description");
			m_strStatusMsg		= pLine->GetParameter("StatusMsg");
			m_strVerb			= pLine->GetParameter("Verb");

			//Flags
			if(pLine->GetParameterFlag("Flags","nowait")) m_nNoWait = 1;
			if(pLine->GetParameterFlag("Flags","shellexec")) m_nShellExec = 1;
			if(pLine->GetParameterFlag("Flags","waituntilidle")) m_nWaitUntilIdle = 1;
			if(pLine->GetParameterFlag("Flags","runminimized")) m_nRunMinimized = 1;
			if(pLine->GetParameterFlag("Flags","skipifdoesntexist")) m_nSkipIfDoesntExist = 1;
			if(pLine->GetParameterFlag("Flags","postinstall")) m_nPostInstall = 1;
			if(pLine->GetParameterFlag("Flags","unchecked")) m_nUnchecked = 1;
			if(pLine->GetParameterFlag("Flags","skipifsilent")) m_nSkipIfSilent = 1;
			if(pLine->GetParameterFlag("Flags","skipifnotsilent")) m_nSkipIfNotSilent = 1;
			if(pLine->GetParameterFlag("Flags","runmaximized")) m_nRunMaximized = 1;
			if(pLine->GetParameterFlag("Flags","hidewizard")) m_nHideWizard = 1;
			if(pLine->GetParameterFlag("Flags","runhidden")) m_nRunHidden = 1;
			if(pLine->GetParameterFlag("Flags","32bit")) m_n32Bit = 1;
			if(pLine->GetParameterFlag("Flags","64bit")) m_n64Bit = 1;
			if(pLine->GetParameterFlag("Flags","runascurrentuser")) m_nRunAsCurrentUser = 1;
			if(pLine->GetParameterFlag("Flags","runasoriginaluser")) m_nRunAsOriginalUser = 1;
			break;
		}
	}

	CScriptList&	m_list;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
	const bool	m_bIsUninst;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_NOWAIT, _L("Help|Run|NoWait","If this flag is specified, it will not wait for the process to finish executing before proceeding to the next [Run] entry, or completing Setup. Cannot be combined with waituntilidle."))
		TOOLTIP_HANDLER(IDC_RUN_PARAMETERS, _L("Help|Run|Parameters","Optional command line parameters for the program, which can include constants."))
		TOOLTIP_HANDLER(IDC_RUN_WORKINGDIR, _L("Help|Run|WorkingDir","The directory in which the program is started from. If this parameter is not specified or is blank, it uses the directory from the Filename parameter; if Filename does not include a path, it will use a default directory. This parameter can include constants."))
		TOOLTIP_HANDLER(IDC_SHELLEXEC, _L("Help|Run|ShellExec","This flag is required if Filename is not a directly executable file (an .exe or .com file). When this flag is set, Filename can be a folder or any registered file type -- including .hlp, .doc, and so on. The file will be opened with the application associated with the file type on the user's system, the same way it would be if the user double-clicked the file in Explorer.\r\n\r\nWhen using a folder name in Filename it's recommended that you follow it by a backslash character (e.g. \"{group}\\\"), to ensure that a program of the same name is not executed.\r\n\r\nThere is one drawback to using the shellexec flag: it cannot wait until the process terminates. Therefore, it always works as if the nowait flag was specified."))
		TOOLTIP_HANDLER(IDC_RUN_FILENAME, _L("Help|Run|FileName","The program to execute, or file/folder to open. If Filename is not an executable (.exe or .com) or batch file (.bat or .cmd), you must use the shellexec flag on the entry. This parameter can include constants."))
		TOOLTIP_HANDLER(IDC_WAITUNTILIDLE, _L("Help|Run|WaitUntilIdle","If this flag is specified, it will pause until the process is waiting for user input with no input pending, instead of waiting for the process to terminate. (This calls the WaitForInputIdle Win32 function.) Cannot be combined with nowait."))
		TOOLTIP_HANDLER(IDC_RUN_RUNMINIMIZED, _L("Help|Run|RunMinimized","If this flag is specified, it will launch the program or document in a minimized window."))
		TOOLTIP_HANDLER(IDC_RUN_SKIPIFDOESNTEXIST, _L("Help|Run|SkipOfDoesntExist","If this flag is specified in the [Run] section, Setup won't display an error message if Filename doesn't exist.\r\n\r\nIf this flag is specified in the [UninstallRun] section, the uninstaller won't display the \"some elements could not be removed\" warning if Filename doesn't exist."))
		TOOLTIP_HANDLER(IDC_RUN_RUNONCEID, _L("Help|Run|RunOnceID","Valid only in an [UninstallRun] section. If the same application is installed more than once, \"run\" entries will be duplicated in the uninstall log file. By assigning a string to RunOnceId, you can ensure that a particular [UninstallRun] entry will only be executed once during uninstallation. For example, if two or more \"run\" entries in the uninstall log have a RunOnceId setting of \"DelService\", only the latest entry with a RunOnceId setting of \"DelService\" will be executed; the rest will be ignored. Note that RunOnceId comparisons are case-sensitive."))
		TOOLTIP_HANDLER(IDC_RUN_POSTINSTALL, _L("Help|Run|PostInstall","Valid only in an [Run] section. Instructs Setup to create a checkbox on the Setup Completed wizard page. The user can uncheck or check this checkbox and thereby choose whether this entry should be processed or not. Previously this flag was called showcheckbox."))
		TOOLTIP_HANDLER(IDC_RUN_UNCHECKED, _L("Help|Run|UnChecked","Valid only in an [Run] section. Instructs Setup to initially uncheck the checkbox. The user can still check the checkbox if he/she wishes to process the entry. This flag is ignored if the postinstall flag isn't also specified."))
		TOOLTIP_HANDLER(IDC_RUN_DESCRIPTION, _L("Help|Run|Description","Valid only in a [Run] section. The description of the entry, which can include constants. This description is used for entries with the postinstall flag. If the description is not specified for an entry, Setup will use a default description. This description depends on the type of the entry (normal or shellexec)."))
		TOOLTIP_HANDLER(IDC_RUN_SKIPIFNOTSILENT, _L("Help|Run|SkipIfNotSilent","Valid only in an [Run] section. Instructs Setup to skip this entry if Setup is not running (very) silent."))
		TOOLTIP_HANDLER(IDC_RUN_SKIPIFSILENT, _L("Help|Run|SkipIfSilent","Valid only in an [Run] section. Instructs Setup to skip this entry if Setup is running (very) silent."))
		TOOLTIP_HANDLER(IDC_RUN_RUNMAXIMIZED, _L("Help|Run|RunMaximized","If this flag is specified, it will launch the program or document in a maximized window."))
		TOOLTIP_HANDLER(IDC_RUN_STATUSMSG, _L("Help|Run|StatusMsg","Valid only in a [Run] section. Determines the message displayed on the wizard while the program is executed. If this parameter is not specified or is blank, a default message of \"Finishing installation...\" will be used. This parameter can include constants."))
		TOOLTIP_HANDLER(IDC_RUN_HIDEWIZARD, _L("Help|Run|HideWizard","If this flag is specified, the wizard will be hidden while the program is running."))
		TOOLTIP_HANDLER(IDC_RUNHIDDEN, _L("Help|Run|RunHidden","If this flag is specified, it will launch the program in a hidden window. Never use this flag when executing a program that may prompt for user input."))
		TOOLTIP_HANDLER(IDC_RUN_32BIT, _L("Help|Run|32Bit","Causes the {sys} constant to map to the 32-bit System directory when used in the Filename and WorkingDir parameters. This is the default behavior in a 32-bit mode install."))
		TOOLTIP_HANDLER(IDC_RUN_64BIT, _L("Help|Run|64Bit","Causes the {sys} constant to map to the 64-bit System directory when used in the Filename and WorkingDir parameters. This is the default behavior in a 64-bit mode install."))
		TOOLTIP_HANDLER(IDC_RUN_VERB, _L("Help|Run|Verb","Specifies the action to be performed on the file. Must be combined with the shellexec flag. Commonly available verbs include \"open\" and \"print\". If this parameter is not specified or is blank, the default verb for the file type will be used (typically \"open\")."))
		TOOLTIP_HANDLER(IDC_RUNASCURRENTUSER, _L("Help|Run|RunAsCurrentUser","If this flag is specified, the spawned process will inherit Setup/Uninstall's user credentials (typically, full administrative privileges).\n\nThis is the default behavior when the postinstall flag is not used.\n\nThis flag cannot be combined with the runasoriginaluser flag."))
		TOOLTIP_HANDLER(IDC_RUNASORIGINALUSER, _L("Help|Run|RunAsOriginalUser","Valid only in a [Run] section. If this flag is specified and the system is running Windows Vista or later, the spawned process will execute with the (normally non-elevated) credentials of the user that started Setup initially (i.e., the \"pre-UAC dialog\" credentials).\n\nThis is the default behavior when the postinstall flag is used.\n\nIf a user launches Setup by right-clicking its EXE file and selecting \"Run as administrator\", then this flag, unfortunately, will have no effect, because Setup has no opportunity to run any code with the original user credentials. The same is true if Setup is launched from an already-elevated process. Note, however, that this is not an Inno Setup-specific limitation; Windows Installer-based installers cannot return to the original user credentials either in such cases.\n\nThis flag cannot be combined with the runascurrentuser flag."))
	END_TOOLTIP_MAP()
};
