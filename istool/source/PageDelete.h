#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageDelete dialog

class CPageDelete : 
	public CPropertyPageImpl<CPageDelete>,
	public CMyPropertyPageBase<CPageDelete>,
	public CWinDataExchange<CPageDelete>,
	public Henden::CToolTipDialog<CPageDelete>
{
public:
	enum { IDD = IDD_DELETE };

	BEGIN_MSG_MAP(CPageDelete)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageDelete>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageDelete>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageDelete>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_TYPE, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_TYPE, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_DELETE_NAME, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_DELETE_NAME, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_DELETE_NAME, CBN_DROPDOWN, OnDropdownDeleteName)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageDelete)
		DDX_TEXT(IDC_DELETE_NAME, m_strName)
		DDX_CBINDEX(IDC_TYPE, m_nType)
	END_DDX_MAP()

	CComboBox2	m_wndName;
	CString		m_strName;
	int			m_nType;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Delete");
		m_wndName.SubclassWindow(GetDlgItem(IDC_DELETE_NAME));

		CComboBox box(GetDlgItem(IDC_TYPE));
		box.AddString(_L("Delete|Type|Files","Files"));
		box.AddString(_L("Delete|Type|FilesDirs","Files and/or directories"));
		box.AddString(_L("Delete|Type|DirIfEmpty","Directory if empty"));

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];

			if(m_strName.CompareNoCase(SAFESTR(pItem->GetParameter("Name"))))
				m_strName.Empty();
			if(m_nType!=CInnoScriptEx::GetDeleteType(pItem->GetParameter("Type")))
				m_nType = -1;
		}
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnDropdownDeleteName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_wndName.GetCount()<=0) {
			CWaitCursor wait;
			m_pDoc->AddFiles(m_wndName);
			m_pDoc->AddDirs(m_wndName);
		}
		return 0;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_list.GetSize()==1;

		if(bForce) {
			if(m_strName.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("You must enter a name."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}

			if(m_nType==-1) {
				AtlMessageBox(m_hWnd,_L("You must select a type."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
		}

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];
			if(bForce || !m_strName.IsEmpty()) pItem->SetParameter("Name",m_strName);
			if(bForce || m_nType!=-1) pItem->SetParameter("Type",CInnoScriptEx::GetDeleteType(m_nType));
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	CString m_strTitle;
	CPageDelete(CScriptList& list,CMyDoc* pDoc,const bool bNew) : 
		m_list(list), m_pDoc(pDoc), m_bNew(bNew), m_wndName(pDoc)
	{
		m_strTitle = _L("DialogTitles|Delete","Delete");
		SetTitle((LPCTSTR)m_strTitle);

		m_nType = -1;
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];

			m_strName = pItem->GetParameter("Name");
			m_nType = CInnoScriptEx::GetDeleteType(pItem->GetParameter("Type"));
			break;
		}
	}

	CScriptList&	m_list;
	CMyDoc*			m_pDoc;
	const bool		m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_DELETE_NAME,_L("Help|Delete|Name","Name of the file or directory to delete."))
		TOOLTIP_HANDLER(IDC_TYPE,_L("Help|Delete|Type","Specifies what is to be deleted by the uninstaller."))
	END_TOOLTIP_MAP()
};
