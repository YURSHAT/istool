// WTL
#pragma once

#include "ButtonVer.h"
#include "InnoScriptEx.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgVersion dialog

class CDlgVersion : 
	public CDialogImpl<CDlgVersion>, 
	public CWinDataExchange<CDlgVersion>,
	public CMyDialogBase<CDlgVersion>
{
public:
	enum { IDD = IDD_VERSION };

	BEGIN_MSG_MAP(CDlgVersion)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_VER_COMBO,CBN_SELCHANGE,OnSelChangeVerCombo)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgVersion>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgVersion)
		DDX_TEXT(IDC_VER_MAJOR, m_strMajor);
		DDX_TEXT(IDC_VER_BUILD, m_strBuild);
		DDX_TEXT(IDC_VER_MINOR, m_strMinor);
		DDX_TEXT(IDC_VER_SP, m_strSP);
	END_DDX_MAP()

	CComboBox	m_wndCombo;
	CString	m_strMajor;
	CString	m_strBuild;
	CString	m_strMinor;
	CString	m_strSP;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			DoDataExchange(DDX_SAVE);

			UINT nMajor, nMinor, nBuild, nSP;
			nMajor = atol(m_strMajor);
			nMinor = atol(m_strMinor);
			nBuild = atol(m_strBuild);
			nSP = atol(m_strSP);

			m_strVer.Empty();
			CString tmp;
			if(!m_strMajor.IsEmpty()) {
				m_strVer.Format("%d",nMajor);
				if(!m_strMinor.IsEmpty()) {
					if(nBuild==2600 || nBuild==3790)
						tmp.Format(".%02d",nMinor);
					else
						tmp.Format(".%d",nMinor);
					m_strVer += tmp;
					if(!m_strBuild.IsEmpty() && nBuild>0) {
						tmp.Format(".%d",nBuild);
						m_strVer += tmp;
					}
				}
				if(m_type==VER_NT && !m_strSP.IsEmpty() && nSP>0) {
					tmp.Format("sp%d",nSP);
					m_strVer += tmp;
				}
			}
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Version");
		CenterWindow(GetParent());

		//m_wndCombo.SubclassWindow(GetDlgItem(IDC_VER_COMBO));
		m_wndCombo = GetDlgItem(IDC_VER_COMBO);

		::EnableWindow(GetDlgItem(IDC_VER_SP),m_type==VER_NT);

		m_wndCombo.ResetContent();
		if(m_type==VER_NT) {
			m_wndCombo.AddString("Windows NT 4.0");
			m_wndCombo.AddString("Windows NT 4.0 SP1");
			m_wndCombo.AddString("Windows NT 4.0 SP2");
			m_wndCombo.AddString("Windows NT 4.0 SP3");
			m_wndCombo.AddString("Windows NT 4.0 SP4");
			m_wndCombo.AddString("Windows NT 4.0 SP5");
			m_wndCombo.AddString("Windows NT 4.0 SP6");
			m_wndCombo.AddString("Windows 2000");
			m_wndCombo.AddString("Windows 2000 SP1");
			m_wndCombo.AddString("Windows 2000 SP2");
			m_wndCombo.AddString("Windows 2000 SP3");
			m_wndCombo.AddString("Windows XP");
			m_wndCombo.AddString("Windows XP SP1");
			m_wndCombo.AddString("Windows Server 2003");
			m_wndCombo.AddString("Windows Vista");
			m_wndCombo.AddString("Don't Install/No Limit");
		} else {
			m_wndCombo.AddString("Windows 95");
			m_wndCombo.AddString("Windows 95 OSR2");
			m_wndCombo.AddString("Windows 95 OSR2.5");
			m_wndCombo.AddString("Windows 98");
			m_wndCombo.AddString("Windows 98 SE");
			m_wndCombo.AddString("Windows Me");
			m_wndCombo.AddString("Don't Install/No Limit");
		}
		
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	CDlgVersion(VERTYPE type,CString& strVer) : m_strVer(strVer), m_type(type) {
		TSetupVersionData	VerData;
		if(type==VER_NT) {
			CString strVer = m_strVer;
			if(CInnoScriptEx::StrToVersionNumbers("0,"+strVer,VerData)) {
				m_strMajor.Format("%d",VerData.NTVersion.Major);
				m_strMinor.Format("%d",VerData.NTVersion.Minor);
				m_strBuild.Format("%d",VerData.NTVersion.Build);
				m_strSP.Format("%d",VerData.NTServicePack);
			}
		} else {
			CString strVer = m_strVer;
			if(CInnoScriptEx::StrToVersionNumbers(strVer+",0",VerData)) {
				m_strMajor.Format("%d",VerData.WinVersion.Major);
				m_strMinor.Format("%d",VerData.WinVersion.Minor);
				m_strBuild.Format("%d",VerData.WinVersion.Build);
			}
		}
	}

	LRESULT OnSelChangeVerCombo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		int nItem = m_wndCombo.GetCurSel();
		if(nItem<0) return 0;

		if(m_type==VER_NT) {
			switch(nItem) {
			case 0:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1381);
				m_strSP.Empty();
				break;
			case 1:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1381);
				m_strSP.Format("%d",1);
				break;
			case 2:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1381);
				m_strSP.Format("%d",2);
				break;
			case 3:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1381);
				m_strSP.Format("%d",3);
				break;
			case 4:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1381);
				m_strSP.Format("%d",4);
				break;
			case 5:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1381);
				m_strSP.Format("%d",5);
				break;
			case 6:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1381);
				m_strSP.Format("%d",6);
				break;
			case 7:
				m_strMajor.Format("%d",5);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",2195);
				m_strSP.Empty();
				break;
			case 8:
				m_strMajor.Format("%d",5);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",2195);
				m_strSP.Format("%d",1);
				break;
			case 9:
				m_strMajor.Format("%d",5);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",2195);
				m_strSP.Format("%d",2);
				break;
			case 10:
				m_strMajor.Format("%d",5);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",2195);
				m_strSP.Format("%d",3);
				break;
			case 11:
				m_strMajor.Format("%d",5);
				m_strMinor.Format("%d",1);
				m_strBuild.Format("%d",2600);
				m_strSP.Empty();
				break;
			case 12:
				m_strMajor.Format("%d",5);
				m_strMinor.Format("%d",1);
				m_strBuild.Format("%d",2600);
				m_strSP.Format("%d",1);
				break;
			case 13:
				m_strMajor.Format("%d",5);
				m_strMinor.Format("%d",2);
				m_strBuild.Format("%d",3790);
				m_strSP.Empty();
				break;
			case 14:
				m_strMajor.Format("%d",6);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",6000);
				m_strSP.Empty();
				break;
			case 15:
				m_strMajor.Format("%d",0);
				m_strMinor.Empty();
				m_strBuild.Empty();
				m_strSP.Empty();
				break;
			default:
				return 0;
			}
		} else {
			switch(nItem) {
			case 0:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",950);
				break;
			case 1:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1111);
				break;
			case 2:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",0);
				m_strBuild.Format("%d",1212);
				break;
			case 3:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",1);
				m_strBuild.Format("%d",1998);
				break;
			case 4:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",1);
				m_strBuild.Format("%d",2222);
				break;
			case 5:
				m_strMajor.Format("%d",4);
				m_strMinor.Format("%d",90);
				m_strBuild.Format("%d",3000);
				break;
			case 6:
				m_strMajor.Format("%d",0);
				m_strMinor.Empty();
				m_strBuild.Empty();
				break;
			default:
				return 0;
			}
		}
		DoDataExchange(DDX_LOAD);
		return 0;
	}

protected:
	CString&	m_strVer;
	const VERTYPE	m_type;
};
