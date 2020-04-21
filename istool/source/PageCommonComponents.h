// WTL
#pragma once

#include "StringToken.h"
#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPageCommonComponents dialog

class CPageCommonComponents : 
	public CPropertyPageImpl<CPageCommonComponents>,
	public CMyPropertyPageBase<CPageCommonComponents>,
	public CWinDataExchange<CPageCommonComponents>,
	public CDialogResize<CPageCommonComponents>,
	public Henden::CToolTipDialog<CPageCommonComponents>
{
public:
	enum { IDD = IDD_COMMON_COMPONENTS };

	BEGIN_MSG_MAP(CPageCommonComponents)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageCommonComponents>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageCommonComponents>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageCommonComponents>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	//	REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDialogResize<CPageCommonComponents>)
		MESSAGE_HANDLER(WM_USER, OnUser)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CPageCommonLanguages)
		DLGRESIZE_CONTROL(IDC_COMMON_COMPONENTLIST, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT, DLSZ_SIZE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COMMON_COMPONENTSTEXT, DLSZ_SIZE_X|DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(CPageCommonComponents)
		DDX_TEXT(IDC_EDIT, m_strEdit)
	END_DDX_MAP()

	CMyCheckListViewCtrl	m_wndComponentList;
	bool					m_bIsBooleanExp;
	CString					m_strEdit;

	LRESULT OnUser(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		BuildEdit();
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		DlgResize_Init(false,false,0);

		_L(m_hWnd,"CommonComponents");
		m_wndComponentList.SubclassWindow(GetDlgItem(IDC_COMMON_COMPONENTLIST));

		m_wndComponentList.InsertColumn(0,"Description",LVCFMT_LEFT,10,0);
		m_wndComponentList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

		CInnoScriptEx& script = m_pDoc->GetScript();
		CScriptList	components;
		script.GetList(CInnoScript::SEC_COMPONENTS,components);
		m_bIsBooleanExp = false;
		if(components.GetSize()) {
			int nItem = 0;
			for(int nPos=0;nPos<components.GetSize();nPos++) {
				CScriptLine* pScriptComponent = components[nPos];
				if(pScriptComponent->GetComment()) continue;
				LPCTSTR pszComponent = pScriptComponent->GetParameter("Name");
				m_wndComponentList.InsertItem(nItem,pScriptComponent->GetParameter("Description"));
				m_wndComponentList.SetItemData(nItem,(DWORD)pszComponent);

				// Set correct state
				int nState = -1;
				for(int nPos2=0;nPos2<m_listBase.GetSize();nPos2++) {
					CInnoScript::CLine* pBase = m_listBase[nPos2];
					m_bIsBooleanExp = m_bIsBooleanExp || CMyApp::IsBooleanExp(pBase->GetParameter("Components"));
					if(pBase->GetParameterFlag("Components",pszComponent)) {
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

			if(m_strEdit.CompareNoCase(SAFESTR(pItem->GetParameter("Components"))))
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
				if(bForce || !m_strEdit.IsEmpty()) pBase->SetParameter("Components",m_strEdit);
			} else {
				UINT nCount = m_wndComponentList.GetItemCount();
				while(nCount--) {
					LPCTSTR pszComponent = (LPCTSTR)m_wndComponentList.GetItemData(nCount);
					int nCheckState = m_wndComponentList.GetCheckState(nCount);
					if(nCheckState>0)
						pBase->SetParameterFlag("Components",pszComponent,true);
					else if(nCheckState==0)
						pBase->SetParameterFlag("Components",pszComponent,false);
				}
			}
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

#if 0
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
	CPageCommonComponents::CPageCommonComponents(CScriptList& listBase,CMyDoc* pDoc,const bool bNew) : 
		m_listBase(listBase), m_pDoc(pDoc), m_bNew(bNew) , m_bInitialized(false)
	{
		m_strTitle = _L("DialogTitles|Component","Component");
		SetTitle((LPCTSTR)m_strTitle);

		for(int nPos=0;nPos<m_listBase.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_listBase[nPos];

			m_strEdit = pItem->GetParameter("Components");
			break;
		}
	}

	CScriptList&	m_listBase;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
	bool		m_bInitialized;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_COMMON_COMPONENTLIST,_L("Help|CommonComponents|ComponentList","List of components that can be chosen for this item(s)."))
		TOOLTIP_HANDLER(IDC_EDIT,_L("Help|CommonComponents|Edit","A space separated list of component names, telling Setup to which components the entry belongs. If the end user selects a component from this list, the entry is processed (for example: the file is installed)."))
	END_TOOLTIP_MAP()
};
