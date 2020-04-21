#pragma once

#include "MyDoc.h"
#include "FilesHelper.h"
#include "StringToken.h"

/////////////////////////////////////////////////////////////////////////////
// CPageType dialog

class CPageType : 
	public CPropertyPageImpl<CPageType>,
	public CMyPropertyPageBase<CPageType>,
	public CWinDataExchange<CPageType>,
	public Henden::CToolTipDialog<CPageType>
{
public:
	enum { IDD = IDD_TYPE };

	BEGIN_MSG_MAP(CPageType)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageType>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageType>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageType>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_TYPE_NAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_TYPE_DESCRIPTION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_TYPE_ISCUSTOM, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageType)
		DDX_TEXT(IDC_TYPE_DESCRIPTION, m_strDescription)
		DDX_TEXT(IDC_TYPE_NAME, m_strName)
		DDX_CHECK(IDC_TYPE_ISCUSTOM, m_bIsCustom)
	END_DDX_MAP()

	CString	m_strDescription;
	CString	m_strName;
	BOOL			m_bIsCustom;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Type");
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pLine = m_list[nPos];

			if(m_strName.CompareNoCase(SAFESTR(pLine->GetParameter("Name")))) m_strName.Empty();
			if(m_strDescription.CompareNoCase(SAFESTR(pLine->GetParameter("Description")))) m_strDescription.Empty();

			//Flags
			if(m_bIsCustom != (pLine->GetParameterFlag("Flags","iscustom") ? TRUE : FALSE)) {
				GetDlgItem(IDC_TYPE_ISCUSTOM).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bIsCustom = 2;
			}
		}
		DoDataExchange(DDX_LOAD);
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
			CScriptLine* pLine = m_list[nPos];

			m_strName.Replace(' ','_');

			if(bForce) {
				CString strOldName(pLine->GetParameter("Name"));
				if(strOldName.CompareNoCase(m_strName)) {
					CFilesHelper(m_pDoc).RenameType(strOldName,m_strName);
				}
			}

			CInnoScriptEx::SetString(pLine,bForce,"Name",m_strName);
			CInnoScriptEx::SetString(pLine,bForce,"Description",m_strDescription);

			// Flags
			CInnoScriptEx::SetFlag(pLine,"Flags","iscustom",m_bIsCustom);
		}

		/*
		** Sjekk at alle typer som er brukt av komponenter er gyldige.
		*/
		CScriptList	list, listTypes;
		CString		strMessage;

		m_pDoc->GetScript().GetList(CInnoScript::SEC_COMPONENTS,list);
		m_pDoc->GetScript().GetList(CInnoScript::SEC_TYPES,listTypes);
		for(int nPos=0;nPos<list.GetSize();nPos++) {
			CScriptLine* pLine = list[nPos];
			CString strTypes = pLine->GetParameter("Types");
			
			CStringToken token(strTypes," ");
			while(LPCTSTR pszType = token.GetNext()) {
				bool bFound = false;
				for(int nPos2=0;nPos2<listTypes.GetSize();nPos2++) {
					CScriptLine* pType = listTypes[nPos2];
					CString strTypeName = pType->GetParameter("Name");
					if(!_stricmp(strTypeName,pszType)) {
						bFound = true;
						break;
					}
				}
				if(!bFound) {
					if(strMessage.GetLength()>0) strMessage += "\n";
					CString txt = _L("FixedComponentErrorType","Fixed component '%1' which used non-existing type '%2'.");
					txt.Replace("%1",pLine->GetParameter("Description"));
					txt.Replace("%2",pszType);
					strMessage += (LPCTSTR)txt;
					pLine->SetParameterFlag("Types",pszType,false);
				}
			}
		}
		if(strMessage.GetLength()>0) AtlMessageBox(m_hWnd,(LPCTSTR)strMessage,IDR_MAINFRAME);
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	CString m_strTitle;
	CPageType(CScriptList& list,CMyDoc* pDoc,const bool bNew) : 
		m_pDoc(pDoc), m_bNew(bNew), m_list(list)
	{
		m_strTitle = _L("DialogTitles|Type","Type");
		SetTitle((LPCTSTR)m_strTitle);

		m_bIsCustom = FALSE;
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pLine = m_list[nPos];

			m_strName = pLine->GetParameter("Name");
			m_strDescription = pLine->GetParameter("Description");
			//Flags
			m_bIsCustom = pLine->GetParameterFlag("Flags","iscustom");
			break;
		}
	}

	CScriptList&	m_list;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_TYPE_DESCRIPTION, _L("Help|Type|Description","The description of the type, which can include constants. This description is shown during installation."))
		TOOLTIP_HANDLER(IDC_TYPE_NAME, _L("Help|Type|Name","The internal name of the type. Used as parameter for components in the [Components] section to instruct Setup to which types a component belongs."))
		TOOLTIP_HANDLER(IDC_TYPE_ISCUSTOM, _L("Help|Type|IsCustom","Instructs Setup that the type is a custom type. Whenever the end user manually changes the components selection during installation, Setup will set the setup type to the custom type. Note that if you don't define a custom type, Setup will only allow the user to choose a setup type and he/she can no longer manually select/unselect components."))
	END_TOOLTIP_MAP()
};
