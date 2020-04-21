// ViewFiles.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewRegistryT.h"
#include "MyDoc.h"
#include "DlgFileAssociation.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRegistryT

CViewRegistryT::CViewRegistryT() : CMyTreeView(CInnoScript::SEC_REGISTRY) {
	SetSubMenu(2);
	m_dwFlags |= VFL_DRAGACCEPTFILES;
}

/////////////////////////////////////////////////////////////////////////////
// CViewRegistryT message handlers

void CViewRegistryT::Populate() {
	CWaitCursor wait;
	CTreeViewCtrl& ctrl = GetTreeCtrl();
	ctrl.SetRedraw(FALSE);
	ctrl.DeleteAllItems();

	CScriptList	list;
	GetDocument()->GetScript().GetList(m_sec,list);

	for(int nPos=0;nPos<list.GetSize();nPos++)
		InsertItem(list[nPos]);

	ctrl.SetRedraw(TRUE);
	ctrl.UpdateWindow();
}

HTREEITEM CViewRegistryT::InsertItem(CScriptLine* pLine) {
	if(pLine->GetComment()) return NULL;

	CString tmp(pLine->GetParameter("ValueName"));
	if(tmp.IsEmpty()) tmp = "(Default)";

	CString strFolder;
	LPCTSTR psz = pLine->GetParameter("Root");
	if(psz) strFolder += psz;
	strFolder += "\\";
	psz = pLine->GetParameter("Subkey");
	if(psz) strFolder += psz;

	HTREEITEM hParent = CMyApp::FindParentItem(GetTreeCtrl(),strFolder);
	HTREEITEM hItem = GetTreeCtrl().InsertItem(tmp,7,7,hParent,TVI_SORT);
	SetItemData(hItem,pLine);
//	SortChildren(hParent);
	CMyApp::MyExpand(GetTreeCtrl(),hParent);
	return hItem;
}

void CViewRegistryT::SetItemData(HTREEITEM hItem,CScriptLine* pLine) {
	CString strName(pLine->GetParameter("ValueName"));
	if(strName.IsEmpty()) strName = "(Default)";
	GetTreeCtrl().SetItemText(hItem,strName);
	GetTreeCtrl().SetItemData(hItem,reinterpret_cast<DWORD>(pLine));
}

// Find full path to an automatic item
void CViewRegistryT::GetItemDirectory(HTREEITEM hItem,CString& ref){
	ATLASSERT(GetTreeCtrl().GetItemData(hItem)==NULL);
	ref.Empty();

	//ref = GetItemText(hItem);
	GetItemText(hItem,ref.GetBuffer(256),256);
	while(hItem = GetTreeCtrl().GetParentItem(hItem)) {
		CString strTmp;
		GetItemText(hItem,strTmp.GetBuffer(256),256);
		ref = strTmp + "\\" + ref;
	}
}

LRESULT CViewRegistryT::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	HDROP hDropInfo = (HDROP)wParam;
	SetActiveWindow();      // activate us first !
	CFilesHelper helper(GetDocument());
	helper.OnDropFilesRegistry(m_hWnd,hDropInfo);
	Populate();
	return 0;
}
