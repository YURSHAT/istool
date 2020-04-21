#pragma once

#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPageCustomMessage dialog

class CPageCustomMessage : 
	public CPropertyPageImpl<CPageCustomMessage>,
	public CMyPropertyPageBase<CPageCustomMessage>,
	public CWinDataExchange<CPageCustomMessage>,
	public Henden::CToolTipDialog<CPageCustomMessage>
{
public:
	enum { IDD = IDD_CUSTOMMESSAGE };

	BEGIN_MSG_MAP(CPageCustomMessage)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageCustomMessage>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageCustomMessage>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageCustomMessage>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_MESSAGE_KEY, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_MESSAGE_TEXT, EN_CHANGE, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageCustomMessage)
		DDX_TEXT(IDC_MESSAGE_TEXT, m_strText)
		DDX_TEXT(IDC_MESSAGE_KEY, m_strKey)
	END_DDX_MAP()

	CString	m_strKey, m_strText;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"CustomMessage");

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listMessages.GetSize()==1;

		if(bForce) {
			if(m_strKey.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("You must enter an ID."),IDR_MAINFRAME,MB_OK|MB_ICONSTOP);
				GetDlgItem(IDC_MESSAGE_KEY).SetFocus();
				return PSNRET_INVALID;
			}
			if(m_strText.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("You must enter a message."),IDR_MAINFRAME,MB_OK|MB_ICONSTOP);
				GetDlgItem(IDC_MESSAGE_TEXT).SetFocus();
				return PSNRET_INVALID;
			}
		}
		for(int nPos=0;nPos<m_listMessages.GetSize();nPos++) {
			CInnoScript::CLineSetup* pLine = (CInnoScript::CLineSetup*)m_listMessages[nPos];

			if(bForce || !m_strKey.IsEmpty()) pLine->SetKey(m_strKey);
			if(bForce || !m_strText.IsEmpty()) {
				CString str(m_strText);
				str.Replace("\r\n","%n");
				str.Replace("\n","%n");
				pLine->SetValue(str);
			}
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	CString m_strTitle;
	CPageCustomMessage(CScriptList& listMessages,CMyDoc* pDoc,const bool bNew) : 
		m_listMessages(listMessages), m_pDoc(pDoc), m_bNew(bNew)
	{
		m_strTitle = _L("DialogTitles|CustomMessage","Custom Message");
		SetTitle((LPCTSTR)m_strTitle);

		for(int nPos=0;nPos<m_listMessages.GetSize();nPos++) {
			CScriptLine* pMessage = m_listMessages[nPos];
			m_strKey = pMessage->GetKey();
			m_strText = pMessage->GetValue();
			m_strText.Replace("%n","\r\n");
			break;
		}
	}

	CScriptList&	m_listMessages;
	CMyDoc*			m_pDoc;
	const bool		m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_MESSAGE_TEXT,_L("Help|CustomMessage|Text","The custom message text."))
		TOOLTIP_HANDLER(IDC_MESSAGE_KEY,_L("Help|CustomMessage|Key","The name of the custom message."))
	END_TOOLTIP_MAP()
};
