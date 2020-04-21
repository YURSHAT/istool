// WTL
#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCustomize dialog

class CDlgCustomize : 
	public CDialogImpl<CDlgCustomize>, 
	public CMyDialogBase<CDlgCustomize>
{
public:
	enum { IDD = IDD_CUSTOMIZE };

	BEGIN_MSG_MAP(CDlgCustomize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgCustomize>)
	END_MSG_MAP()

	CListViewCtrl	m_wndColumnList;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			int nItem = 0;
			for(int nPos=0;nPos<m_listView.m_columnList.GetSize();nPos++,nItem++) {
				CMyListView::CColumnInfo* pInfo = m_listView.m_columnList[nPos];

				pInfo->m_bVisible = ListView_GetCheckState(m_wndColumnList.m_hWnd, nItem) ? true : false;
			}
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Customize");
		CenterWindow(GetParent());

		m_wndColumnList.Attach(GetDlgItem(IDC_COLUMNLIST));
		m_wndColumnList.InsertColumn(0,"",LVCFMT_LEFT,10,0);
		m_wndColumnList.SetExtendedListViewStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT);
		for(int nPos=0;nPos<m_listView.m_columnList.GetSize();nPos++) {
			CMyListView::CColumnInfo* pInfo = m_listView.m_columnList[nPos];
			int nItem = m_wndColumnList.InsertItem(m_wndColumnList.GetItemCount(),pInfo->m_szTitle);
			m_wndColumnList.SetItemData(nItem,(DWORD)pInfo);

			ListView_SetItemState(m_wndColumnList.m_hWnd, nItem, 
				UINT(((pInfo->m_bVisible?1:0) + 1) << 12), LVIS_STATEIMAGEMASK);
		}

		CRect rc;
		m_wndColumnList.GetClientRect(rc);
		m_wndColumnList.SetColumnWidth(0,rc.Width());

		return TRUE;
	}

	CDlgCustomize(CMyListView& ref) : m_listView(ref) {
	}

	CMyListView& m_listView;
};
