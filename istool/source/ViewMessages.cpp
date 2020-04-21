// ViewMessages.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewMessages.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "MainFrm.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewMessages

CViewMessages::CViewMessages() : CMyListView(CInnoScript::SEC_MESSAGES) {
	SetSubMenu(4);
	m_nItemImage = 17;
	m_dwFlags |= VFL_SETUP;
}

CViewMessages::CViewMessages(CInnoScript::SECTION sec) : CMyListView(sec) {
	SetSubMenu(4);
	m_nItemImage = 17;
	m_dwFlags |= VFL_SETUP;
}

/////////////////////////////////////////////////////////////////////////////
// CViewMessages message handlers

LRESULT CViewMessages::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	CMyListView::OnCreate(uMsg,wParam,lParam,bHandled);
	AddColumn("Name",LVCFMT_LEFT,120,true);
	AddColumn("Message",LVCFMT_LEFT,260,true);
	DoDisplay();

	ModifyStyle(0,LVS_SINGLESEL);

//	bHandled = FALSE;
	return 0;
}

#if 0
const CListInfo CViewMessages::m_colInfo[] = {
	"Name",				DF_NAME,			false,
	"Message",			DF_MESSAGE,			false,
	NULL
};
#endif

void CViewMessages::SetItemTexts(UINT nItem) {
	CScriptLine* pLine = (CScriptLine*)GetItemData(nItem);

	if(pLine->GetComment()) {
		CListViewCtrl::SetItemText(nItem,0,pLine->GetComment());
	} else {
		CListViewCtrl::SetItemText(nItem,DF_NAME,pLine->GetKey());
		CListViewCtrl::SetItemText(nItem,DF_MESSAGE,pLine->GetValue());
	}
}

LRESULT CViewMessages::OnUpdateUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
#if 1
	return CMyListView::OnUpdateUI(uMsg,wParam,lParam,bHandled);
#else
	return 1;
#endif
}
