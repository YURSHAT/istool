// WTL complete

#include "stdafx.h"
#include "istool.h"
#include "ViewFilesL.h"
#include "MyDoc.h"
#include "FilesHelper.h"
#include "StringToken.h"
#include "MainFrm.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewFilesL

CViewFilesL::CViewFilesL() : CMyListView(CInnoScript::SEC_FILES) {
	SetSubMenu(1);
	m_dwFlags |= VFL_DRAGACCEPTFILES;
}

/////////////////////////////////////////////////////////////////////////////
// CViewFilesL message handlers

void CViewFilesL::RefreshText() {
	LONG nCount = GetItemCount();
	while(nCount--) SetItemTexts(nCount);
}

void CViewFilesL::Populate() {
	CWaitCursor wait;
	SetRedraw(FALSE);
	DeleteAllItems();

	CScriptList	files, dirs;
	AfxGetDocument()->GetScript().GetList(CInnoScript::SEC_FILES,files);
	AfxGetDocument()->GetScript().GetList(CInnoScript::SEC_DIRS,dirs);

	for(int nPos=0;nPos<dirs.GetSize();nPos++)
		InsertItem(dirs[nPos]);

	for(int nPos=0;nPos<files.GetSize();nPos++)
		InsertItem(files[nPos]);

	SetRedraw(TRUE);
}

const CListInfo CViewFilesL::m_colInfo[] = {
	"Name",				LVCFMT_LEFT,	180, 0,
	"Source",			LVCFMT_LEFT,	180, CLI_HIDE,
	"DestDir",			LVCFMT_LEFT,	180, 0,
	"FontInstall",		LVCFMT_LEFT,	150, CLI_HIDE,
	"Flags",			LVCFMT_LEFT,	150, 0,
	"Attribs",			LVCFMT_LEFT,	150, CLI_HIDE,
	"MinVersion",		LVCFMT_LEFT,	 80, CLI_HIDE,
	"OnlyBelowVersion",	LVCFMT_LEFT,	 80, CLI_HIDE,
	"Components",		LVCFMT_LEFT,	100, CLI_HIDE,
	"Tasks",			LVCFMT_LEFT,	100, CLI_HIDE,
	"Languages",		LVCFMT_LEFT,	 80, CLI_HIDE,
	"Permissions",		LVCFMT_LEFT,	100, CLI_HIDE,
	"Excludes",			LVCFMT_LEFT,	100, CLI_HIDE,
	NULL
};

void CViewFilesL::SetItemTexts(UINT nItem) {
	CScriptLine* pLine = (CScriptLine*)GetItemData(nItem);

	LPCTSTR pszComment = pLine->GetComment();
	if(pszComment) {
		SetItemText(nItem,0,pszComment);
		return;
	}

	if(pLine->GetSection()==CInnoScript::SEC_FILES) {
		const CListInfo* pInfo = GetListInfo();

		UINT nColumn = 0;
		CString strDestName;
		CInnoScriptEx::GetDestTitle(pLine,strDestName);
		while(pInfo && pInfo->m_pszParameter) {
			if(nColumn==0)
				SetItemText(nItem,nColumn++,strDestName);
			else
				SetItemText(nItem,nColumn++,pLine->GetParameter(pInfo->m_pszParameter));
			pInfo++;
		}
	} else if(pLine->GetSection()==CInnoScript::SEC_DIRS) {
		CString strDestDir, strTitle;
		CInnoScriptEx::GetDestDir(pLine,strDestDir);
		CInnoScriptEx::GetDestTitle(pLine,strTitle);

		const CListInfo* pInfo = GetListInfo();
		UINT nColumn = 0;
		CString strDestName;
		CInnoScriptEx::GetDestTitle(pLine,strDestName);
		while(pInfo && pInfo->m_pszParameter) {
			if(nColumn==0)
				SetItemText(nItem,nColumn++,strTitle);
			else if(nColumn==2)
				SetItemText(nItem,nColumn++,strDestDir);
			else
				SetItemText(nItem,nColumn++,pLine->GetParameter(pInfo->m_pszParameter));
			pInfo++;
		}
	}
}

UINT CViewFilesL::InsertItem(CScriptLine* pLine) {
	int nItem = GetItemCount();

	if(pLine->GetSection()==CInnoScript::SEC_FILES)
		nItem = CListViewCtrl::InsertItem(nItem,NULL,pLine->GetComment() ? 11 : 0);
	else
		nItem = CListViewCtrl::InsertItem(nItem,NULL,pLine->GetComment() ? 11 : 1);

	SetItemData(nItem,(DWORD)pLine);
	SetItemTexts(nItem);
	return nItem;
}

LRESULT CViewFilesL::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	HDROP hDropInfo = (HDROP)wParam;
	SetActiveWindow();      // activate us first !
	CString strFolder;
	CFilesHelper helper(AfxGetDocument());
	helper.OnDropFiles(m_hWnd,hDropInfo,"{app}");
	Populate();
	return 0;
}

LRESULT CViewFilesL::OnCreateIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CFilesHelper helper(AfxGetDocument());

	UINT nItem = GetItemCount();
	while(nItem--)
		if(GetItemState(nItem,LVIS_SELECTED)==LVIS_SELECTED) {
			CScriptLine* pItem = reinterpret_cast<CScriptLine*>(GetItemData(nItem));

			if(!pItem || pItem->GetComment() || pItem->GetSection()!=CInnoScript::SEC_FILES) continue;
			helper.CreateIcon(m_hWnd,pItem);
		}
	return 0;
}

LRESULT CViewFilesL::OnUpdateUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	if(wParam==ID_POPUP_CREATEICON)
		return GetSelectedCount()>0;
	else
		return CMyListView::OnUpdateUI(uMsg,wParam,lParam,bHandled);
}
