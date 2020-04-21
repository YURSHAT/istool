#pragma once

#include "MyDoc.h"
#include "FilesHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CPageTask dialog

class CPageTask : 
	public CPropertyPageImpl<CPageTask>,
	public CMyPropertyPageBase<CPageTask>,
	public CWinDataExchange<CPageTask>,
	public Henden::CToolTipDialog<CPageTask>
{
public:
	enum { IDD = IDD_TASK };

	BEGIN_MSG_MAP(CPageTask)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageTask>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageTask>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageTask>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_TASK_NAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_TASK_UNCHECKED, BN_CLICKED, OnModifiedUnchecked)
		COMMAND_HANDLER(IDC_TASK_DESCRIPTION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_TASK_GROUP_DESCRIPTION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_TASK_EXCLUSIVE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_TASK_RESTART, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_TASK_CHECKED_ONCE, BN_CLICKED, OnModifiedCheckedOnce)
		COMMAND_HANDLER(IDC_DONTINHERITCHECK, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CHECKABLEALONE, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageTask)
		DDX_TEXT(IDC_TASK_GROUP_DESCRIPTION, m_strGroupDescription)
		DDX_TEXT(IDC_TASK_DESCRIPTION, m_strDescription)
		DDX_TEXT(IDC_TASK_NAME, m_strName)
		DDX_CHECK(IDC_TASK_UNCHECKED, m_bUnchecked)
		DDX_CHECK(IDC_TASK_EXCLUSIVE, m_bExclusive)
		DDX_CHECK(IDC_TASK_RESTART, m_bRestart)
		DDX_CHECK(IDC_TASK_CHECKED_ONCE, m_bCheckedOnce)
		DDX_CHECK(IDC_DONTINHERITCHECK, m_nDontInheritCheck)
		DDX_CHECK(IDC_CHECKABLEALONE, m_nCheckableAlone)
	END_DDX_MAP()

	CString	m_strGroupDescription;
	CString	m_strDescription;
	CString	m_strName;
	BOOL	m_bUnchecked;
	BOOL	m_bExclusive;
	BOOL	m_bRestart;
	BOOL	m_bCheckedOnce;
	int		m_nDontInheritCheck, m_nCheckableAlone;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Task");
		CScriptList	listTypes;
		m_script.GetList(CInnoScript::SEC_TYPES,listTypes);

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pItem = m_list[nPos];

			if(m_strName.CompareNoCase(SAFESTR(pItem->GetParameter("Name")))) m_strName.Empty();
			if(m_strDescription.CompareNoCase(SAFESTR(pItem->GetParameter("Description")))) m_strDescription.Empty();
			if(m_strGroupDescription.CompareNoCase(SAFESTR(pItem->GetParameter("GroupDescription")))) m_strGroupDescription.Empty();

			//Flags
			if(m_bExclusive != (pItem->GetParameterFlag("Flags","exclusive") ? TRUE : FALSE)) {
				GetDlgItem(IDC_TASK_EXCLUSIVE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bExclusive = 2;
			}
			if(m_bUnchecked != (pItem->GetParameterFlag("Flags","unchecked") ? TRUE : FALSE)) {
				GetDlgItem(IDC_TASK_UNCHECKED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bUnchecked = 2;
			}
			if(m_bRestart != (pItem->GetParameterFlag("Flags","restart") ? TRUE : FALSE)) {
				GetDlgItem(IDC_TASK_RESTART).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRestart = 2;
			}
			if(m_bCheckedOnce != (pItem->GetParameterFlag("Flags","checkedonce") ? TRUE : FALSE)) {
				GetDlgItem(IDC_TASK_CHECKED_ONCE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bCheckedOnce = 2;
			}
			if(m_nDontInheritCheck != (pItem->GetParameterFlag("Flags","dontinheritcheck") ? TRUE : FALSE)) {
				GetDlgItem(IDC_DONTINHERITCHECK).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nDontInheritCheck = 2;
			}
			if(m_nCheckableAlone != (pItem->GetParameterFlag("Flags","checkablealone") ? TRUE : FALSE)) {
				GetDlgItem(IDC_CHECKABLEALONE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nCheckableAlone = 2;
			}
		}
		DoDataExchange(DDX_LOAD);
		
		m_bInitialized = true;
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_list.GetSize()==1;
		if(bForce && m_strName.IsEmpty()) {
			AtlMessageBox(m_hWnd,_L("You must enter a name."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			return PSNRET_INVALID;
		}
		if(bForce && m_strDescription.IsEmpty()) {
			AtlMessageBox(m_hWnd,_L("You must enter a description."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			return PSNRET_INVALID;
		}

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pItem = m_list[nPos];

			m_strName.Replace(' ','_');

			if(bForce) {
				CString strOldName(pItem->GetParameter("Name"));
				if(strOldName.CompareNoCase(m_strName)) {
					CFilesHelper(m_pDoc).RenameTask(strOldName,m_strName);
				}
			}

			
			CInnoScriptEx::SetString(pItem,bForce,"Name",m_strName);
			CInnoScriptEx::SetString(pItem,bForce,"Description",m_strDescription);
			CInnoScriptEx::SetString(pItem,bForce,"GroupDescription",m_strGroupDescription);

			// Flags
			CInnoScriptEx::SetFlag(pItem,"Flags","exclusive",m_bExclusive);
			CInnoScriptEx::SetFlag(pItem,"Flags","unchecked",m_bUnchecked);
			CInnoScriptEx::SetFlag(pItem,"Flags","restart",m_bRestart);
			CInnoScriptEx::SetFlag(pItem,"Flags","checkedonce",m_bCheckedOnce);
			CInnoScriptEx::SetFlag(pItem,"Flags","dontinheritcheck",m_nDontInheritCheck);
			CInnoScriptEx::SetFlag(pItem,"Flags","checkablealone",m_nCheckableAlone);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnModifiedUnchecked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		if(m_bCheckedOnce && m_bUnchecked) {
			m_bCheckedOnce = FALSE;
			DoDataExchange(DDX_LOAD);
		}
		return 0;
	}

	LRESULT OnModifiedCheckedOnce(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		if(m_bCheckedOnce && m_bUnchecked) {
			m_bUnchecked = FALSE;
			DoDataExchange(DDX_LOAD);
		}
		return 0;
	}

	CString m_strTitle;
	CPageTask(CMyDoc* pDoc,CScriptList& list,const bool bNew,CInnoScriptEx& script) : 
		m_pDoc(pDoc),m_list(list), m_bNew(bNew), m_script(script), m_bInitialized(false)
	{
		m_strTitle = _L("DialogTitles|Task","Task");
		SetTitle((LPCTSTR)m_strTitle);

		m_bRestart = FALSE;
		m_bCheckedOnce = FALSE;
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];

			m_strName = pItem->GetParameter("Name");
			m_strDescription = pItem->GetParameter("Description");
			m_strGroupDescription = pItem->GetParameter("GroupDescription");
			//Flags
			m_bExclusive = pItem->GetParameterFlag("Flags","exclusive");
			m_bUnchecked = pItem->GetParameterFlag("Flags","unchecked");
			m_bRestart = pItem->GetParameterFlag("Flags","restart");
			m_bCheckedOnce = pItem->GetParameterFlag("Flags","checkedonce");
			m_nDontInheritCheck = pItem->GetParameterFlag("Flags","dontinheritcheck");
			m_nCheckableAlone = pItem->GetParameterFlag("Flags","checkablealone");
			break;
		}
	}

	CScriptList&	m_list;
	const bool				m_bNew;
	CInnoScriptEx&			m_script;
	bool					m_bInitialized;
	CMyDoc*					m_pDoc;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_TASK_GROUP_DESCRIPTION, _L("Help|Task|GroupDescription","The group description of a group of tasks, which can include constants. Consecutive tasks with the same group description will be grouped below a text label. The text label shows the group description."))
		TOOLTIP_HANDLER(IDC_TASK_DESCRIPTION, _L("Help|Task|Description","The description of the task, which can include constants. This description is shown to the end user during installation."))
		TOOLTIP_HANDLER(IDC_TASK_NAME, _L("Help|Task|Name","The internal name of the task.\r\n\r\nThe total number of \\ or / characters in the name of the task is called the level of the task. Any task with a level of 1 or more is a child task. The task listed before the child task with a level of 1 less than the child task, is the parent task. Other tasks with the same parent task as the child task are sibling tasks.\r\n\r\nA child task can't be selected if its parent task isn't selected. A parent task can't be selected if none of it child tasks are selected and it doesn't install anything itself.\r\n\r\nIf sibling tasks have the exclusive flag, only one of them can be selected."))
		TOOLTIP_HANDLER(IDC_TASK_UNCHECKED, _L("Help|Task|UnChecked","Instructs Setup that this task should be unchecked initially. This flag cannot be combined with the checkedonce flag."))
		TOOLTIP_HANDLER(IDC_TASK_EXCLUSIVE, _L("Help|Task|Exclusive","Instructs Setup that this task is mutually exclusive with sibling tasks that also have the exclusive flag."))
		TOOLTIP_HANDLER(IDC_TASK_RESTART, _L("Help|Task|Restart","Instructs Setup to ask the user to restart the system at the end of installation if this task is selected, regardless of whether it is necessary (for example because of [Files] section entries with the restartreplace flag). Like AlwaysRestart but per task."))
		TOOLTIP_HANDLER(IDC_TASK_CHECKED_ONCE, _L("Help|Task|CheckedOnce","Instructs Setup that this task should be unchecked initially when Setup finds a previous version of the same application is already installed. This flag cannot be combined with the unchecked flag."))
		TOOLTIP_HANDLER(IDC_DONTINHERITCHECK, _L("Help|Task|DontInheritCheck","Specifies that the task should not automatically become checked when its parent is checked. Has no effect on top-level tasks, and cannot be combined with the exclusive flag."))
		TOOLTIP_HANDLER(IDC_CHECKABLEALONE, _L("Help|Task|CheckableAlone","This flag specifies that the task can be checked when none of its children are. By default, if no Tasks parameter directly references the task, unchecking all of the task's children will cause the task to become unchecked."))
	END_TOOLTIP_MAP()
};
