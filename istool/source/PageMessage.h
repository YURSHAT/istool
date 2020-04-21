#pragma once

#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPageMessage dialog

class CPageMessage : 
	public CPropertyPageImpl<CPageMessage>,
	public CMyPropertyPageBase<CPageMessage>,
	public CWinDataExchange<CPageMessage>,
	public Henden::CToolTipDialog<CPageMessage>
{
public:
	enum { IDD = IDD_MESSAGE };

	BEGIN_MSG_MAP(CPageMessage)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageMessage>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageMessage>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageMessage>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_MESSAGE_KEY, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_MESSAGE_TEXT, EN_CHANGE, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageMessage)
		DDX_TEXT(IDC_MESSAGE_TEXT, m_strText)
		DDX_CBINDEX(IDC_MESSAGE_KEY, m_nKeyIndex)
	END_DDX_MAP()

	CComboBox	m_wndKey;
	CString	m_strText;
	int				m_nKeyIndex;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Message");
		m_wndKey.Attach(GetDlgItem(IDC_MESSAGE_KEY));

		m_pDoc->AddValidMessages(m_hWnd,m_wndKey);
		for(int nPos0=0;nPos0<m_listMessages.GetSize();nPos0++) {
			CScriptLine* pMessage = m_listMessages[nPos0];
			int nPos = m_wndKey.FindStringExact(0,pMessage->GetKey());
			if(nPos==CB_ERR && pMessage->GetKey()) {
				m_wndKey.AddString(pMessage->GetKey());
				nPos = m_wndKey.FindStringExact(0,pMessage->GetKey());
			}
			m_wndKey.SetCurSel(nPos);
			break;
		}
		UpdateDefaultMessage();
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listMessages.GetSize()==1;

		if(bForce) {
			if(m_nKeyIndex<0) {
				AtlMessageBox(m_hWnd,_L("You must enter an ID."),IDR_MAINFRAME,MB_OK|MB_ICONSTOP);
				GetDlgItem(IDC_MESSAGE_KEY).SetFocus();
				return PSNRET_INVALID;
			}
		}
		for(int nPos=0;nPos<m_listMessages.GetSize();nPos++) {
			CInnoScript::CLineSetup* pLine = (CInnoScript::CLineSetup*)m_listMessages[nPos];
			CString strID;
			m_wndKey.GetLBText(m_nKeyIndex,strID);

			if(bForce || !strID.IsEmpty()) pLine->SetKey(strID);
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
		UpdateDefaultMessage();
		return 0;
	}

	void UpdateDefaultMessage() {
		DoDataExchange(DDX_SAVE,IDC_MESSAGE_KEY);
		if(m_nKeyIndex<0) {
			GetDlgItem(IDC_MESSAGE_DEFAULT).SetWindowText("");
		} else {
			CString strID, strValue;
			m_wndKey.GetLBText(m_nKeyIndex,strID);
			strValue = strID;

			CInnoScriptEx* pMessages = m_pDoc->GetMessages();
			if(pMessages) {
				CScriptList	list;
				pMessages->GetList(CInnoScript::SEC_MESSAGES,list);
				for(int nPos=0;nPos<list.GetSize();nPos++) {
					CScriptLine* pLine = list[nPos];
					CString str = pLine->GetKey();
					if(!str.CompareNoCase(strID)) {
						strValue = pLine->GetValue();
						break;
					}
				}
			}
			strValue.Replace("%n","\r\n");
			GetDlgItem(IDC_MESSAGE_DEFAULT).SetWindowText(strValue);
		}
	}

	CString m_strTitle;
	CPageMessage(CScriptList& listMessages,CMyDoc* pDoc,const bool bNew) : 
		m_listMessages(listMessages), m_pDoc(pDoc), m_bNew(bNew)
	{
		m_strTitle = _L("DialogTitles|Message","Message");
		SetTitle((LPCTSTR)m_strTitle);

		m_nKeyIndex = -1;
		for(int nPos=0;nPos<m_listMessages.GetSize();nPos++) {
			CScriptLine* pMessage = m_listMessages[nPos];
	//		m_strKey = pMessage->GetName();
			m_strText = pMessage->GetValue();
			m_strText.Replace("%n","\r\n");
			break;
		}
	}

	CScriptList&	m_listMessages;
	CMyDoc*			m_pDoc;
	const bool		m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_MESSAGE_TEXT,_L("Help|Message|Text","The message text."))
		TOOLTIP_HANDLER(IDC_MESSAGE_KEY,_L("Help|Message|Key","The message name."))
		TOOLTIP_HANDLER(IDC_MESSAGE_DEFAULT,_L("Help|Message|Default","The default message text."))
	END_TOOLTIP_MAP()
};
