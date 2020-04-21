// WTL
#pragma once

#include "MyDoc.h"
#include "Registry.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgODBC dialog

class CDlgODBC : 
	public CDialogImpl<CDlgODBC>, 
	public CMyDialogBase<CDlgODBC>
{
public:
	enum { IDD = IDD_ODBC };

	enum {
		DATA_SYSTEM,
		DATA_USER,
	};

	BEGIN_MSG_MAP(CDlgODBC)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgODBC>)
	END_MSG_MAP()

	CListViewCtrl	m_wndList;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			UINT nCount = m_wndList.GetItemCount();
			for(UINT nRow=0;nRow<nCount;nRow++) {
				if(m_wndList.GetItemState(nRow,LVIS_SELECTED)) {
					CString strKey;
					CString strValue;
					m_wndList.GetItemText(nRow,0,strKey);
					m_wndList.GetItemText(nRow,1,strValue);
					DWORD dwData = m_wndList.GetItemData(nRow);

					CString strLine;
					strLine.Format("Root: %s; SubKey: Software\\ODBC\\ODBC.INI\\ODBC Data Sources; "
						"ValueType: string; ValueName: %s; ValueData: %s; "
						"Flags: createvalueifdoesntexist uninsdeletevalue",
						
						dwData==DATA_USER ? "HKCU" : "HKLM",
						strKey, strValue);
					m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_REGISTRY,strLine));

					CString strPath;
					strPath.Format("Software\\ODBC\\ODBC.INI\\%s",strKey);
					WriteKey(strPath,dwData);
				}
			}
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"ODBC");
		CenterWindow(GetParent());

		CWaitCursor wait;
		m_wndList.Attach(GetDlgItem(IDC_LIST));
		m_wndList.InsertColumn(0,_L("ODBC|Name","Name"),LVCFMT_LEFT,10,0);
		m_wndList.InsertColumn(1,_L("ODBC|Driver","Driver"),LVCFMT_LEFT,10,0);
		m_wndList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

		ReadRegistry(HKEY_CURRENT_USER);
		ReadRegistry(HKEY_LOCAL_MACHINE);

		CRect rc;
		m_wndList.GetClientRect(rc);
		m_wndList.SetColumnWidth(0,rc.Width()/2);
		m_wndList.SetColumnWidth(1,rc.Width() - (rc.Width()/2));
		return TRUE;
	}

	void ReadRegistry(HKEY hKey) {
		CRegistryEx		reg;
		CStringArray	scArray;
		CString			strData;
		BOOL			bRet;
		LONG			nCount = 0;

		bRet = reg.Open(hKey,"Software\\ODBC\\ODBC.INI\\ODBC Data Sources");
		bRet = reg.ListValues("",scArray);

		for(nCount=0;nCount<scArray.GetSize();nCount++) {
			UINT nItem = m_wndList.InsertItem(nCount,scArray[nCount]);
			reg.Read(scArray[nCount],strData);
			m_wndList.SetItemText(nItem,1,strData);
			if(hKey==HKEY_LOCAL_MACHINE)
				m_wndList.SetItemData(nItem,DATA_SYSTEM);
			else
				m_wndList.SetItemData(nItem,DATA_USER);
		}

		reg.Close();
	}

	void WriteKey(const CString& strPath,const DWORD dwData) {
		CStringArray	scArray;
		CRegistryEx reg;
		reg.Open(dwData==DATA_USER ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,strPath);
		reg.ListValues("",scArray);

		for(LONG n=0;n<scArray.GetSize();n++) {
			CString strLine;
			strLine.Format("Root: %s; SubKey: %s; Flags: createvalueifdoesntexist uninsdeletevalue; "
				"ValueName: %s",
				dwData==DATA_USER ? "HKCU" : "HKLM",
				strPath, scArray[n]);

			CString strData;
			DWORD dwData;

			if(reg.Read(scArray[n],strData) && reg.m_Info.dwType==REG_SZ) {
				strLine += "; ValueType: string; ValueData: "+strData;
			} else if(reg.Read(scArray[n],dwData) && reg.m_Info.dwType==REG_DWORD) {
				CString strTmp;
				strTmp.Format("%d",dwData);
				strLine += "; ValueType: dword; ValueData: "+strTmp;
			} else {
				CString txt = _L("Unknown type for value '%1'.");
				txt.Replace("%1",scArray[n]);
				AtlMessageBox(m_hWnd,(LPCTSTR)txt,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			}
			
			m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_REGISTRY,strLine));
			m_pDoc->SetModifiedFlag();
		}

		// Subkeys
		reg.ListKey("",scArray);
		for(int n=0;n<scArray.GetSize();n++) WriteKey(strPath+"\\"+scArray[n],dwData);
	}

	CDlgODBC(CMyDoc* pDoc) : m_pDoc(pDoc) {}

	CMyDoc*	m_pDoc;
};
