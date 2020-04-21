// WTL
#pragma once

#include "StringToken.h"
#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPageCommonTasks dialog

class CPageCommonTasks : 
	public CPropertyPageImpl<CPageCommonTasks>,
	public CMyPropertyPageBase<CPageCommonTasks>,
	public CWinDataExchange<CPageCommonTasks>,
	public CDialogResize<CPageCommonTasks>,
	public Henden::CToolTipDialog<CPageCommonTasks>
{
public:
	enum { IDD = IDD_COMMON_TASKS };

	BEGIN_MSG_MAP(CPageCommonTasks)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageCommonTasks>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageCommonTasks>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageCommonTasks>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	//	REFLECT_NOTIFICATIONS()
		MESSAGE_HANDLER(WM_USER, OnUser)
		CHAIN_MSG_MAP(CDialogResize<CPageCommonTasks>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CPageCommonLanguages)
		DLGRESIZE_CONTROL(IDC_TASKS_LIST, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT, DLSZ_SIZE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_TASKS_NOTE, DLSZ_SIZE_X|DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()


	BEGIN_DDX_MAP(CPageCommonTasks)
		DDX_TEXT(IDC_EDIT, m_strEdit)
	END_DDX_MAP()

	CMyCheckListViewCtrl	m_wndComponentList;
	bool					m_bIsBooleanExp;
	CString					m_strEdit;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		DlgResize_Init(false,false,0);

		_L(m_hWnd,"CommonTasks");
		m_wndComponentList.SubclassWindow(GetDlgItem(IDC_TASKS_LIST));
		m_wndComponentList.InsertColumn(0,"Description",LVCFMT_LEFT,10,0);
		m_wndComponentList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

		CInnoScriptEx& script = m_pDoc->GetScript();
		CScriptList	tasks;
		script.GetList(CInnoScript::SEC_TASKS,tasks);
		m_bIsBooleanExp = false;
		if(tasks.GetSize()) {
			int nItem = 0;
			for(int nPos2=0;nPos2<tasks.GetSize();nPos2++) {
				CInnoScript::CLine* pScriptTask = tasks[nPos2];
				if(pScriptTask->GetComment()) continue;
				LPCTSTR pszTask = pScriptTask->GetParameter("Name");
				m_wndComponentList.InsertItem(nItem,pScriptTask->GetParameter("Description"));
				m_wndComponentList.SetItemData(nItem,(DWORD)pszTask);

				// Set correct state
				int nState = -1;
				for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
					CScriptLine* pBase = m_listBase[nPos];
					m_bIsBooleanExp = m_bIsBooleanExp || CMyApp::IsBooleanExp(pBase->GetParameter("Tasks"));
					if(pBase->GetParameterFlag("Tasks",pszTask)) {
						if(nState<0) {
							nState = 1;
						} else if(nState!=1) {
							nState = 2;
							break;
						}
					} else {
						if(nState<0) {
							nState = 0;
						} else if(nState!=0) {
							nState = 2;
							break;
						}
					}
				}
				//SetState(nItem,nState);
				if(nState>1)
					m_wndComponentList.SetCheckStateIndeterminate(nItem);
				else
					m_wndComponentList.SetCheckState(nItem,nState);
				nItem++;
			}
		}

		CRect rc;
		m_wndComponentList.GetClientRect(rc);
		m_wndComponentList.SetColumnWidth(0,rc.Width());
		m_wndComponentList.EnableWindow(!m_bIsBooleanExp);
		((CEdit)GetDlgItem(IDC_EDIT)).SetReadOnly(!m_bIsBooleanExp);

		for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_listBase[nPos];

			if(m_strEdit.CompareNoCase(SAFESTR(pItem->GetParameter("Tasks"))))
				m_strEdit.Empty();
		}

		DoDataExchange(DDX_LOAD);
		m_bInitialized = true;
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listBase.GetSize()==1;
		CInnoScriptEx& script = m_pDoc->GetScript();
		for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
			CScriptLine* pBase = m_listBase[nPos];

			if(m_bIsBooleanExp) {
				if(bForce || !m_strEdit.IsEmpty()) pBase->SetParameter("Tasks",m_strEdit);
			} else {
				UINT nCount = m_wndComponentList.GetItemCount();
				while(nCount--) {
					LPCTSTR pszTask = (LPCTSTR)m_wndComponentList.GetItemData(nCount);
					int nCheckState = m_wndComponentList.GetCheckState(nCount);
					if(nCheckState>0)
						pBase->SetParameterFlag("Tasks",pszTask,true);
					else if(nCheckState==0)
						pBase->SetParameterFlag("Tasks",pszTask,false);
				}
			}
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnUser(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		BuildEdit();
		return 0;
	}
	void BuildEdit() {
		SetModified();
		CString str;
		UINT nCount = m_wndComponentList.GetItemCount();
		while(nCount--) {
			LPCTSTR pszComponent = (LPCTSTR)m_wndComponentList.GetItemData(nCount);
			if(m_wndComponentList.GetCheckState(nCount)) {
				if(!str.IsEmpty())
					str += _T(' ');
				str += pszComponent;
			}
		}
		SetDlgItemText(IDC_EDIT,str);
	}

#if 0
	LRESULT OnItemChangingCommonComponentList(int,NMHDR* pNMHDR,BOOL&) {
		NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
		// TODO: Add your control notification handler code here
		
		if(!m_bInitialized) {
			return 0;
		}

		if((pNMListView->uOldState & LVIS_STATEIMAGEMASK) == (pNMListView->uNewState & LVIS_STATEIMAGEMASK)) {
			return 1;	// No change
		}
		if(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)==1) {
			SetState(pNMListView->iItem,0);
			return 1;	// No change
		}

		return 0;
	}

	void SetState(int nItem,int nState) {
		// Same as checkbox... 0 = unchecked, 1 = checked, 2 = indeterminate
		ListView_SetItemState(m_wndComponentList.m_hWnd, nItem, 
			UINT((nState==2 ? 1 : nState+2) << 12), LVIS_STATEIMAGEMASK);
	}

	int GetState(int nItem) {
		UINT nState = ListView_GetItemState(m_wndComponentList.m_hWnd,nItem,LVIS_STATEIMAGEMASK);
		switch(nState>>12) {
		case 2:
			return 0;
		case 3:
			return 1;
		default:
			return 2;
		}
	}
#endif

	CString m_strTitle;
	CPageCommonTasks::CPageCommonTasks(CScriptList& listBase,CMyDoc* pDoc,const bool bNew) : 
		m_listBase(listBase), m_pDoc(pDoc), m_bNew(bNew), m_bInitialized(false)
	{
		m_strTitle = _L("DialogTitles|Task","Task");
		SetTitle((LPCTSTR)m_strTitle);
	
		for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_listBase[nPos];

			m_strEdit = pItem->GetParameter("Tasks");
			break;
		}
	}

	CScriptList&	m_listBase;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
	bool		m_bInitialized;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_TASKS_LIST,_L("Help|CommonTasks|List","List of tasks that can be chosen for this item(s)."))
		TOOLTIP_HANDLER(IDC_EDIT,_L("Help|CommonTasks|Edit","A space separated list of task names, telling Setup to which task the entry belongs. If the end user selects a task from this list, the entry is processed (for example: the file is installed)."))
	END_TOOLTIP_MAP()
};
