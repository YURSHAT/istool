// ViewIcons.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewIconsT.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewIconsT

CViewIconsT::CViewIconsT() : CMyTreeView(CInnoScript::SEC_ICONS) {
}

/////////////////////////////////////////////////////////////////////////////
// CViewIconsT message handlers

void CViewIconsT::Populate() {
	GetTreeCtrl().DeleteAllItems();

	CScriptList	list;
	GetDocument()->GetScript().GetList(m_sec,list);

	for(int nPos=0;nPos<list.GetSize();nPos++)
		InsertItem(list[nPos]);
}

HTREEITEM CViewIconsT::InsertItem(CScriptLine* pLine) {
	if(pLine->GetComment()) return NULL;

	CString strName(pLine->GetParameter("Name"));
	CString strFolder;
	int pos = strName.ReverseFind('\\');
	if(pos<=0) pos = strName.ReverseFind(':');
	if(pos>=0) {
		strFolder = strName.Left(pos);
		strName = strName.Mid(pos+1);
	}

	HTREEITEM hParent = CMyApp::FindParentItem(GetTreeCtrl(),strFolder);
	HTREEITEM hItem = GetTreeCtrl().InsertItem(strName,0,0,hParent,TVI_SORT);
	GetTreeCtrl().SetItemData(hItem,(DWORD)pLine);
	GetTreeCtrl().SortChildren(hParent);
	CMyApp::MyExpand(GetTreeCtrl(),hParent);
	return hItem;
}

void CViewIconsT::OnPreNewItem(CScriptLine* pLine) {
	CString strName;
	GetCurrentFolder(strName);
	strName += "\\";
	pLine->SetParameter("Name",strName);
}

void CViewIconsT::GetCurrentFolder(CString& strFolder) {
	CTreeViewCtrl& ctrl = GetTreeCtrl();
	strFolder.Empty();
	HTREEITEM hItem = ctrl.GetSelectedItem();
	while(hItem) {
		CScriptLine* pBase = reinterpret_cast<CScriptLine*>(ctrl.GetItemData(hItem));
		if(!pBase) {
			// Folder not specifically created by user
			while(hItem) {
				if(!strFolder.IsEmpty()) strFolder = "\\" + strFolder;
				CString strTmp;
				ctrl.GetItemText(hItem,strTmp);
				strFolder = (LPCTSTR)strTmp + strFolder;
				hItem = ctrl.GetParentItem(hItem);
			}
		}
		hItem = ctrl.GetParentItem(hItem);
	}
	if(strFolder.IsEmpty()) strFolder = "{group}";
}
