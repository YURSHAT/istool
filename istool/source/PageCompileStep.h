#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageCompileStep dialog

class CPageCompileStep : 
	public CPropertyPageImpl<CPageCompileStep>,
	public CMyPropertyPageBase<CPageCompileStep>,
	public CWinDataExchange<CPageCompileStep>,
	public Henden::CToolTipDialog<CPageCompileStep>
{
public:
	enum { IDD = IDD_COMPILE_STEP_EDIT };

	BEGIN_MSG_MAP(CPageCompileStep)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageCompileStep>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageCompileStep>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageCompileStep>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_CSE_ABORT_ON_ERROR, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CSE_NAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_CSE_PARAMETERS, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_CSE_RUN_MINIMIZED, BN_CLICKED, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageCompileStep)
		DDX_CHECK(IDC_CSE_ABORT_ON_ERROR, m_bAbortOnError)
		DDX_TEXT(IDC_CSE_NAME, m_strName)
		DDX_TEXT(IDC_CSE_PARAMETERS, m_strParameters)
		DDX_CHECK(IDC_CSE_RUN_MINIMIZED, m_bRunMinimized)
	END_DDX_MAP()

	Henden::CButtonFile	m_wndNameBtn;
	BOOL				m_bAbortOnError;
	CString		m_strName;
	CString		m_strParameters;
	BOOL				m_bRunMinimized;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"CompileStep");
		m_wndNameBtn.SubclassWindow(GetDlgItem(IDC_BTN_CSE_NAME));

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];

			if(m_strName.CompareNoCase(SAFESTR(pItem->GetParameter("Name"))))
				m_strName.Empty();
			if(m_strParameters.CompareNoCase(SAFESTR(pItem->GetParameter("Parameters"))))
				m_strParameters.Empty();

			//Flags
			if(m_bAbortOnError != (pItem->GetParameterFlag("Flags","abortonerror") ? TRUE : FALSE)) {
				GetDlgItem(IDC_CSE_ABORT_ON_ERROR).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bAbortOnError = 2;
			}
			if(m_bRunMinimized != (pItem->GetParameterFlag("Flags","runminimized") ? TRUE : FALSE)) {
				GetDlgItem(IDC_CSE_RUN_MINIMIZED).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bRunMinimized = 2;
			}
		}
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_list.GetSize()==1;

		if(bForce) {
			if(m_strName.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("You must enter a name."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
		}

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];
			if(bForce || !m_strName.IsEmpty()) pItem->SetParameter("Name",m_strName);
			if(bForce || !m_strParameters.IsEmpty()) pItem->SetParameter("Parameters",m_strParameters);

			// Flags
			CInnoScriptEx::SetFlag(pItem,"Flags","abortonerror",m_bAbortOnError);
			CInnoScriptEx::SetFlag(pItem,"Flags","runminimized",m_bRunMinimized);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	CString m_strTitle;
	CPageCompileStep(CScriptList& list,CMyDoc* pDoc,const bool bNew) : 
		m_list(list), m_pDoc(pDoc), m_bNew(bNew), 
		m_wndNameBtn(true,"Executable Files (*.exe;*.bat;*.cmd)|*.exe;*.bat;*.cmd|All Files (*.*)|*.*||")
	{
		m_strTitle = _L("DialogTitles|CompileStep","Compile Step");
		SetTitle((LPCTSTR)m_strTitle);

		m_bAbortOnError = FALSE;
		m_bRunMinimized = FALSE;
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];
			m_strName = pItem->GetParameter("Name");
			m_strParameters = pItem->GetParameter("Parameters");
			m_bAbortOnError = pItem->GetParameterFlag("Flags","abortonerror");
			m_bRunMinimized = pItem->GetParameterFlag("Flags","runminimized");
			break;
		}
	}

	CScriptList&	m_list;
	CMyDoc*			m_pDoc;
	const bool		m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_CSE_ABORT_ON_ERROR,_L("Help|CompileStep|AbortOnError","If this is checked, compilation will be aborted if an error occurs."))
		TOOLTIP_HANDLER(IDC_CSE_NAME,_L("Help|CompileStep|Name","Name and path of the program to run."))
		TOOLTIP_HANDLER(IDC_CSE_PARAMETERS,_L("Help|CompileStep|Parameters","Parameters to the program."))
		TOOLTIP_HANDLER(IDC_CSE_RUN_MINIMIZED,_L("Help|CompileStep|RunMinimized","Runs minimized if this checkbox is checked."))
		TOOLTIP_HANDLER(IDC_BTN_CSE_NAME,_L("Help|CompileStep|NameButton","Click this button to select the program you want to run."))
	END_TOOLTIP_MAP()
};
