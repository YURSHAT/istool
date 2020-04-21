#pragma once

#include "MyDoc.h"
#include "FilesHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CPageComponent dialog

class CPageComponent : 
	public CPropertyPageImpl<CPageComponent>,
	public CMyPropertyPageBase<CPageComponent>,
	public CWinDataExchange<CPageComponent>,
	public Henden::CToolTipDialog<CPageComponent>
{
public:
	enum { IDD = IDD_COMPONENT };

	BEGIN_MSG_MAP(CPageComponent)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageComponent>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageComponent>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageComponent>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_COMPONENT_NAME, EN_CHANGE, OnModified)
		NOTIFY_HANDLER(LVN_ITEMCHANGED, IDC_COMPONENT_TYPES, OnItemChangedComponentTypes)
		COMMAND_HANDLER(IDC_COMPONENT_DESCRIPTION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_COMPONENT_TYPES, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_COMPONENT_FIXED, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPONENT_RESTART, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPONENT_DISABLENOUNINSTALLWARNING, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPONENT_EXTRADISKSPACEREQUIRED, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_COMPONENTSEXCLUSIVE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DONTINHERITCHECK, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CHECKABLEALONE, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageComponent)
		DDX_TEXT(IDC_COMPONENT_DESCRIPTION, m_strDescription)
		DDX_CHECK(IDC_COMPONENT_FIXED, m_bFixed)
		DDX_TEXT(IDC_COMPONENT_NAME, m_strName)
		DDX_CHECK(IDC_COMPONENT_RESTART, m_bRestart)
		DDX_CHECK(IDC_COMPONENT_DISABLENOUNINSTALLWARNING, m_nDisableNoUninstallWarning)
		DDX_TEXT(IDC_COMPONENT_EXTRADISKSPACEREQUIRED, m_strExtraDiskSpaceRequired)
		DDX_CHECK(IDC_COMPONENTSEXCLUSIVE, m_nExclusive)
		DDX_CHECK(IDC_DONTINHERITCHECK, m_nDontInheritCheck)
		DDX_CHECK(IDC_CHECKABLEALONE, m_nCheckableAlone)
	END_DDX_MAP()

	CCheckListViewCtrl	m_wndTypeList;
	CString			m_strDescription;
	BOOL			m_bFixed;
	CString			m_strName;
	BOOL			m_bRestart;
	int				m_nDisableNoUninstallWarning;
	CString			m_strExtraDiskSpaceRequired;
	int				m_nExclusive, m_nDontInheritCheck, m_nCheckableAlone;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Component");
		m_wndTypeList.SubclassWindow(GetDlgItem(IDC_COMPONENT_TYPES));

		m_wndTypeList.InsertColumn(0,"Description",LVCFMT_LEFT,10,0);
		m_wndTypeList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

		CScriptList	listTypes;
		m_script.GetList(CInnoScript::SEC_TYPES,listTypes);

		if(!listTypes.GetSize()) {
			m_wndTypeList.InsertItem(0,"Full installation");
			m_wndTypeList.SetItemData(0,(DWORD)"full");
			m_wndTypeList.InsertItem(1,"Compact installation");
			m_wndTypeList.SetItemData(1,(DWORD)"compact");
			m_wndTypeList.InsertItem(2,"Custom installation");
			m_wndTypeList.SetItemData(2,(DWORD)"custom");
		} else {
			int nItem = 0;
			for(int nPos=0;nPos<listTypes.GetSize();nPos++) {
				CScriptLine* pLine = listTypes[nPos];
				if(pLine->GetComment()) continue;
				LPCTSTR pszType = pLine->GetParameter("Name");
				m_wndTypeList.InsertItem(nItem,pLine->GetParameter("Description"));
				m_wndTypeList.SetItemData(nItem,(DWORD)pszType);
				nItem++;
			}
		}
		UINT nCount = m_wndTypeList.GetItemCount();
		while(nCount--) {
			LPCTSTR pszType = (LPCTSTR)m_wndTypeList.GetItemData(nCount);

			// Set correct state
			int nState = -1;
			for(int nPos=0;nPos<m_list.GetSize();nPos++) {
				CScriptLine* pLine = m_list[nPos];
				if(pLine->GetParameterFlag("Types",pszType)) {
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
			//SetState(nCount,nState);
			m_wndTypeList.SetCheckState(nCount,nState);
		}

		CRect rc;
		m_wndTypeList.GetClientRect(rc);
		m_wndTypeList.SetColumnWidth(0,rc.Width());



		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pItem = m_list[nPos];

			if(m_strName.CompareNoCase(SAFESTR(pItem->GetParameter("Name")))) m_strName.Empty();
			if(m_strDescription.CompareNoCase(SAFESTR(pItem->GetParameter("Description")))) m_strDescription.Empty();
			if(m_strExtraDiskSpaceRequired.CompareNoCase(SAFESTR(pItem->GetParameter("ExtraDiskSpaceRequired")))) m_strExtraDiskSpaceRequired.Empty();

			//Flags
			if(m_bFixed != (pItem->GetParameterFlag("Flags","fixed") ? TRUE : FALSE)) {
				GetDlgItem(IDC_COMPONENT_FIXED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bFixed = 2;
			}
			if(m_bRestart != (pItem->GetParameterFlag("Flags","restart") ? TRUE : FALSE)) {
				GetDlgItem(IDC_COMPONENT_RESTART).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRestart = 2;
			}
			if(m_nDisableNoUninstallWarning != (pItem->GetParameterFlag("Flags","disablenouninstallwarning") ? TRUE : FALSE)) {
				GetDlgItem(IDC_COMPONENT_DISABLENOUNINSTALLWARNING).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nDisableNoUninstallWarning = 2;
			}
			if(m_nExclusive != (pItem->GetParameterFlag("Flags","exclusive") ? TRUE : FALSE)) {
				GetDlgItem(IDC_COMPONENTSEXCLUSIVE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nExclusive = 2;
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
#if 0
	LRESULT OnItemChangingComponentTypes(int,NMHDR* pNMHDR,BOOL&) {
		NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

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
	LRESULT OnItemChangedComponentTypes(int,NMHDR* pNMHDR,BOOL&) {
		NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

		if((pNMListView->uOldState & LVIS_STATEIMAGEMASK) == (pNMListView->uNewState & LVIS_STATEIMAGEMASK))
			return 0;	// No change
		
		if(!m_bNew) SetModified();
		return 0;
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
					CFilesHelper(m_pDoc).RenameComponent(strOldName,m_strName);
				}
			}

			CInnoScriptEx::SetString(pItem,bForce,"Name",m_strName);
			CInnoScriptEx::SetString(pItem,bForce,"Description",m_strDescription);
			CInnoScriptEx::SetString(pItem,bForce,"ExtraDiskSpaceRequired",m_strExtraDiskSpaceRequired);

			// Flags
			CInnoScriptEx::SetFlag(pItem,"Flags","fixed",m_bFixed);
			CInnoScriptEx::SetFlag(pItem,"Flags","restart",m_bRestart);
			CInnoScriptEx::SetFlag(pItem,"Flags","disablenouninstallwarning",m_nDisableNoUninstallWarning);
			CInnoScriptEx::SetFlag(pItem,"Flags","exclusive",m_nExclusive);
			CInnoScriptEx::SetFlag(pItem,"Flags","dontinheritcheck",m_nDontInheritCheck);
			CInnoScriptEx::SetFlag(pItem,"Flags","checkablealone",m_nCheckableAlone);

			UINT nCount = m_wndTypeList.GetItemCount();
			while(nCount--) {
				LPCTSTR pszType = (LPCTSTR)m_wndTypeList.GetItemData(nCount);
				if(m_wndTypeList.GetCheckState(nCount))
					pItem->SetParameterFlag("Types",pszType,true);
					//pItem->SetType(pszType,false);
				else
					pItem->SetParameterFlag("Types",pszType,false);
					//pItem->SetType(pszType,true);
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
		ListView_SetItemState(m_wndTypeList.m_hWnd, nItem, 
			UINT((nState==2 ? 1 : nState+2) << 12), LVIS_STATEIMAGEMASK);
	}

	int GetState(int nItem) {
		UINT nState = ListView_GetItemState(m_wndTypeList.m_hWnd,nItem,LVIS_STATEIMAGEMASK);
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
	CPageComponent(CMyDoc* pDoc,CScriptList& list,const bool bNew,CInnoScriptEx& script) : 
		m_pDoc(pDoc), m_list(list), m_bNew(bNew), m_script(script), m_bInitialized(false)
	{
		m_strTitle = _L("DialogTitles|Component","Component");
		SetTitle((LPCTSTR)m_strTitle);

		m_bFixed = FALSE;
		m_bRestart = FALSE;
		m_nDisableNoUninstallWarning = FALSE;
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];

			m_strName = pItem->GetParameter("Name");
			m_strDescription = pItem->GetParameter("Description");
			m_strExtraDiskSpaceRequired = pItem->GetParameter("ExtraDiskSpaceRequired");
			//Flags
			m_bFixed = pItem->GetParameterFlag("Flags","fixed");
			m_bRestart = pItem->GetParameterFlag("Flags","restart");
			m_nDisableNoUninstallWarning = pItem->GetParameterFlag("Flags","disablenouninstallwarning");
			m_nExclusive = pItem->GetParameterFlag("Flags","exclusive");
			m_nDontInheritCheck = pItem->GetParameterFlag("Flags","dontinheritcheck");
			m_nCheckableAlone = pItem->GetParameterFlag("Flags","checkablealone");
			break;
		}
	}

	CScriptList&	m_list;
	const bool				m_bNew;
	CInnoScriptEx&				m_script;
	bool					m_bInitialized;
	CMyDoc*					m_pDoc;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_COMPONENT_DESCRIPTION,_L("Help|Component|Description","The description of the component, which can include constants. This description is shown to the end user during installation."))
		TOOLTIP_HANDLER(IDC_COMPONENT_FIXED,_L("Help|Component|Fixed","Instructs Setup that this component can not be manually selected or unselected by the end user during installation."))
		TOOLTIP_HANDLER(IDC_COMPONENT_NAME,_L("Help|Component|Name","The internal name of the component.\r\n\r\nThe total number of \\ or / characters in the name of the component is called the level of the component. Any component with a level of 1 or more is a child component. The component listed before the child component with a level of 1 less than the child component, is the parent component. Other components with the same parent component as the child component are sibling components."))
		TOOLTIP_HANDLER(IDC_COMPONENT_RESTART,_L("Help|Component|Restart","Instructs Setup to ask the user to restart the system if this component is installed, regardless of whether this is necessary (for example because of [Files] section entries with the restartreplace flag). Like AlwaysRestart but per component."))
		TOOLTIP_HANDLER(IDC_COMPONENT_DISABLENOUNINSTALLWARNING,_L("Help|Component|DisableNoUninstallWarning","Instructs Setup not to warn the user that this component will not be uninstalled after he/she deselected this component when it's already installed on his/her machine.\r\n\r\nDepending on the complexity of your components, you can try to use the [InstallDelete] section and this flag to automatically 'uninstall' deselected components."))
		TOOLTIP_HANDLER(IDC_COMPONENT_EXTRADISKSPACEREQUIRED,_L("Help|Component|ExtraDiskSpaceRequired","The extra disk space required by this component, similar to the ExtraDiskSpaceRequired directive for the [Setup] section."))
		TOOLTIP_HANDLER(IDC_COMPONENTSEXCLUSIVE,_L("Help|Component|Exclusive","Instructs Setup that this component is mutually exclusive with sibling components that also have the exclusive flag."))
		TOOLTIP_HANDLER(IDC_COMPONENT_TYPES,_L("Help|Component|Types","A space separated list of types this component belongs to. If the end user selects a type from this list, this component will be installed.\r\n\r\nIf the fixed flag isn't used (see below), any custom types (types using the iscustom flag) in this list are ignored by Setup."))
		TOOLTIP_HANDLER(IDC_DONTINHERITCHECK, _L("Help|Component|DontInheritCheck","Specifies that the component should not automatically become checked when its parent is checked. Has no effect on top-level components, and cannot be combined with the exclusive flag."))
		TOOLTIP_HANDLER(IDC_CHECKABLEALONE, _L("Help|Component|CheckableAlone","This flag specifies that the component can be checked when none of its children are. By default, if no Components parameter directly references the component, unchecking all of the component's children will cause the component to become unchecked."))
	END_TOOLTIP_MAP()
};
