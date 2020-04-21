// WTL
#pragma once

#include "StringToken.h"
#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPageCommonLanguages dialog

class CPageCommonLanguages : 
	public CPropertyPageImpl<CPageCommonLanguages>,
	public CMyPropertyPageBase<CPageCommonLanguages>,
	public CWinDataExchange<CPageCommonLanguages>,
	public CDialogResize<CPageCommonLanguages>,
	public Henden::CToolTipDialog<CPageCommonLanguages>
{
public:
	enum { IDD = IDD_COMMON_LANGUAGES };

	BEGIN_MSG_MAP(CPageCommonLanguages)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageCommonLanguages>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageCommonLanguages>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageCommonLanguages>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(LVN_ITEMCHANGED, IDC_TASKS_LIST, OnItemChangedCommonComponentList)
	//	REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDialogResize<CPageCommonLanguages>)
		MESSAGE_HANDLER(WM_USER, OnUser)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CPageCommonLanguages)
		DLGRESIZE_CONTROL(IDC_LANGUAGES_LIST, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT, DLSZ_SIZE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_LANGUAGES_NOTE, DLSZ_SIZE_X|DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(CPageCommonLanguages)
		DDX_TEXT(IDC_EDIT, m_strEdit)
	END_DDX_MAP()

	//CListViewCtrl	m_wndComponentList;
	CMyCheckListViewCtrl	m_wndList;
	bool					m_bIsBooleanExp;
	CString					m_strEdit;

#define m_wndComponentList m_wndList

	LRESULT OnUser(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		BuildEdit();
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		DlgResize_Init(false,false,0);
		m_wndList.SubclassWindow(GetDlgItem(IDC_LANGUAGES_LIST));

		_L(m_hWnd,"CommonLanguages");
		m_wndList.InsertColumn(0,"Description",LVCFMT_LEFT,10,0);

		CInnoScriptEx& script = m_pDoc->GetScript();
		CScriptList	list;
		script.GetList(CInnoScript::SEC_LANGUAGES,list);
		m_bIsBooleanExp = false;
		if(list.GetSize()) {
			for(int i=0;i<list.GetSize();i++) {
				CInnoScript::CLine* pLine = list[i];
				if(pLine->GetComment()) continue;
				LPCTSTR pszTask = pLine->GetParameter("Name");
				int iItem = m_wndList.InsertItem(m_wndList.GetItemCount(),pLine->GetParameter("Name"));
				m_wndList.SetItemData(iItem,(DWORD)pszTask);

				// Set correct state
				bool bCheckState = false;
				for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
					CScriptLine* pBase = m_listBase[nPos];
					m_bIsBooleanExp = m_bIsBooleanExp || CMyApp::IsBooleanExp(pBase->GetParameter("Languages"));
					if(pBase->GetParameterFlag("Languages",pszTask))
						bCheckState = true;
				}
				m_wndList.SetCheckState(iItem,bCheckState);
			}
		}

		CRect rc;
		m_wndList.GetClientRect(rc);
		m_wndList.SetColumnWidth(0,rc.Width());
		m_wndList.EnableWindow(!m_bIsBooleanExp);
		((CEdit)GetDlgItem(IDC_EDIT)).SetReadOnly(!m_bIsBooleanExp);
	
		for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_listBase[nPos];

			if(m_strEdit.CompareNoCase(SAFESTR(pItem->GetParameter("Languages"))))
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
				if(bForce || !m_strEdit.IsEmpty()) pBase->SetParameter("Languages",m_strEdit);
			} else {
				UINT nCount = m_wndComponentList.GetItemCount();
				while(nCount--) {
					LPCTSTR pszTask = (LPCTSTR)m_wndComponentList.GetItemData(nCount);
					if(m_wndList.GetCheckState(nCount))
						pBase->SetParameterFlag("Languages",pszTask,true);
					else
						pBase->SetParameterFlag("Languages",pszTask,false);
				}
			}
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnItemChangedCommonComponentList(int,NMHDR* pNMHDR,BOOL&) {
		NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

		if((pNMListView->uOldState & LVIS_STATEIMAGEMASK) == (pNMListView->uNewState & LVIS_STATEIMAGEMASK))
			return 0;	// No change
		
		if(!m_bNew) SetModified();
		return 0;
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

	CString m_strTitle;
	CPageCommonLanguages::CPageCommonLanguages(CScriptList& listBase,CMyDoc* pDoc,const bool bNew) : 
		m_listBase(listBase), m_pDoc(pDoc), m_bNew(bNew), m_bInitialized(false)
	{
		m_strTitle = _L("DialogTitles|Language","Language");
		SetTitle((LPCTSTR)m_strTitle);

		for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_listBase[nPos];

			m_strEdit = pItem->GetParameter("Languages");
			break;
		}
	}

	CScriptList&	m_listBase;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
	bool		m_bInitialized;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_LANGUAGES_LIST,_L("Help|CommonLanguages|List","List of languages that can be chosen for this item(s)."))
	END_TOOLTIP_MAP()
};
