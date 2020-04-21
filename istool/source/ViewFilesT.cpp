// ViewFiles.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewFilesT.h"
#include "MyDoc.h"
#include "FileInfo.h"
#include "DlgAddDirectory.h"
#include "resource.h"
#include "FilesHelper.h"
#include "StringToken.h"
#include "MainFrm.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewFilesT

CViewFilesT::CViewFilesT() : CMyTreeView(CInnoScript::SEC_FILES) {
	SetSubMenu(1);
}

/////////////////////////////////////////////////////////////////////////////
// CViewFilesT message handlers

LRESULT CViewFilesT::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	CMyDropTarget::Register(m_hWnd);

	bHandled = FALSE;
	return 0;
}

void CViewFilesT::Populate() {
	CWaitCursor wait;
	CTreeViewCtrl& ctrl = GetTreeCtrl();
	ctrl.SetRedraw(FALSE);
	ctrl.DeleteAllItems();

	CScriptList	dirs, files;
	GetDocument()->GetScript().GetList(CInnoScript::SEC_FILES,files);
	GetDocument()->GetScript().GetList(CInnoScript::SEC_DIRS,dirs);

	for(int nPos=0;nPos<dirs.GetSize();nPos++)
		InsertItem(dirs[nPos]);

	for(int nPos=0;nPos<files.GetSize();nPos++)
		InsertItem(files[nPos]);

	ctrl.SetRedraw(TRUE);
	ctrl.UpdateWindow();
}

HTREEITEM CViewFilesT::InsertItem(CScriptLine* pLine) {
	if(pLine->GetComment()) return NULL;

	if(pLine->GetSection()==CInnoScript::SEC_DIRS) {
		CString strFolder(pLine->GetParameter("Name"));
		//int pos = strFolder.ReverseFind('\\');
		//if(pos<0) pos = strFolder.ReverseFind(':');
		int pos = myReverseFind(strFolder,'\\');
		if(pos<0) pos = myReverseFind(strFolder,':');
		if(pos>=0) strFolder = strFolder.Left(pos);
		else strFolder.Empty();
		
		HTREEITEM hParent = CMyApp::FindParentItem(GetTreeCtrl(),strFolder);
		HTREEITEM hItem = GetTreeCtrl().InsertItem(NULL,1,2,hParent,TVI_SORT);
		SetItemData(hItem,pLine);
		SortChildren(hParent);
		CMyApp::MyExpand(GetTreeCtrl(),hParent);
		return hItem;
	} else {
		CString tmp(pLine->GetParameter("DestName"));
		if(tmp.IsEmpty()) {
			tmp = pLine->GetParameter("Source");
			int pos = tmp.ReverseFind('\\');
			if(pos<0) pos = tmp.ReverseFind(':');
			if(pos>0) tmp = tmp.Mid(pos+1);
		}
		HTREEITEM hParent = CMyApp::FindParentItem(GetTreeCtrl(),pLine->GetParameter("DestDir"));
		HTREEITEM hItem = GetTreeCtrl().InsertItem(tmp,0,0,hParent,TVI_SORT);
		SetItemData(hItem,pLine);
		SortChildren(hParent);
		CMyApp::MyExpand(GetTreeCtrl(),hParent);
		return hItem;
	}
}

void CViewFilesT::SetItemData(HTREEITEM hItem,CInnoScript::CLine* pLine) {
	if(pLine->GetSection()==CInnoScript::SEC_FILES) {
		CString strName(pLine->GetParameter("DestName"));
		if(strName.IsEmpty()) {
			strName = pLine->GetParameter("Source");
			int pos = strName.ReverseFind('\\');
			if(pos<0) pos = strName.ReverseFind(':');
			if(pos>=0) strName = strName.Mid(pos+1);
		}
		GetTreeCtrl().SetItemText(hItem,strName);
	} else if(pLine->GetSection()==CInnoScript::SEC_DIRS) {
		CString strName = pLine->GetParameter("Name");
		int pos = myReverseFind(strName,'\\');
		if(pos<0) pos = myReverseFind(strName,':');
		if(pos>=0) strName = strName.Mid(pos+1);
		GetTreeCtrl().SetItemText(hItem,strName);
	}
	GetTreeCtrl().SetItemData(hItem,reinterpret_cast<DWORD>(pLine));
}

void CViewFilesT::SortChildren(HTREEITEM hParent) {
	TVSORTCB sort;
	sort.hParent = hParent;
	sort.lpfnCompare = CompareFunc;
	GetTreeCtrl().SortChildrenCB(&sort);
}

int CViewFilesT::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	CScriptLine* pBase1 = reinterpret_cast<CScriptLine*>(lParam1);
	CScriptLine* pBase2 = reinterpret_cast<CScriptLine*>(lParam2);

	if(!pBase1 && !pBase2) return 0;	// Automatic dirs
	if(!pBase1) return -1;
	if(!pBase2) return 1;
#if 1
	bool bDir1 = pBase1->GetSection()==CInnoScript::SEC_DIRS;
	bool bDir2 = pBase2->GetSection()==CInnoScript::SEC_DIRS;

	if(bDir1 && bDir2) {
		//return 0;	// Both are dirs
		CString strName1(pBase1->GetParameter("Name"));
		CString strName2(pBase2->GetParameter("Name"));
		return _stricmp(strName1,strName2);
	}
	if(bDir1) return -1;
	if(bDir2) return 1;
#endif
	return 0;
}

void CViewFilesT::GetCurrentFolder(CString& strFolder,HTREEITEM hItem/*=NULL*/) {
#if 1
	CTreeViewCtrl& ctrl = GetTreeCtrl();
	strFolder.Empty();
	if(!hItem) hItem = ctrl.GetSelectedItem();
	while(hItem) {
		CScriptLine* pBase = reinterpret_cast<CScriptLine*>(ctrl.GetItemData(hItem));
		if(!pBase || pBase->GetSection()==CInnoScript::SEC_DIRS) {
			if(pBase) {
				strFolder = pBase->GetParameter("Name");
			} else {
				// Folder not specifically created by user
				while(hItem) {
					if(!strFolder.IsEmpty()) strFolder = "\\" + strFolder;
					CString strTmp;
					ctrl.GetItemText(hItem,strTmp);
					strFolder = (LPCTSTR)strTmp + strFolder;
					hItem = ctrl.GetParentItem(hItem);
				}
			}
			break;
		}
		hItem = ctrl.GetParentItem(hItem);
	}
	if(strFolder.IsEmpty()) strFolder = "{app}";
#else
	strFolder = "{app}";
#endif
}

void CViewFilesT::InsertFileName(LPCTSTR lpszFileName) {
	CString strCurrentFolder;
	GetCurrentFolder(strCurrentFolder);

	CInnoScript::CLine* pLine = new CInnoScript::CLine(CInnoScript::SEC_FILES);
	pLine->AddParameter("Source",lpszFileName);
	pLine->AddParameter("DestDir",strCurrentFolder);

	InsertItem(pLine);
	GetDocument()->GetScript().AddLine(pLine);
}

void CViewFilesT::OnPreNewItem(CScriptLine* pLine) {
	CString strDestDir;
	GetCurrentFolder(strDestDir);
	pLine->SetParameter("DestDir",strDestDir);
}

// Find full path to an automatic item
void CViewFilesT::GetItemDirectory(HTREEITEM hItem,CString& ref){
	ATLASSERT(GetTreeCtrl().GetItemData(hItem)==NULL);
	ref.Empty();

	GetItemText(hItem,ref);
	while(hItem = GetTreeCtrl().GetParentItem(hItem)) {
		CString strTmp;
		GetItemText(hItem,strTmp);
		ref = strTmp + "\\" + ref;
	}
}

DWORD CViewFilesT::OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
	ScreenToClient(&point);
	HTREEITEM hItem = GetDropItem(point,TVHT_ONITEM);
	if(hItem) SelectDropTarget(hItem);
	return DROPEFFECT_COPY;
}

DWORD CViewFilesT::OnDragOver(IDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
	ScreenToClient(&point);
	HTREEITEM hItem = GetDropItem(point,TVHT_ONITEM);
	if(hItem) SelectDropTarget(hItem);
	return DROPEFFECT_COPY;
}

DWORD CViewFilesT::OnDrop(IDataObject* pDataObject, DWORD grfKeyState, CPoint point, DWORD dropEffect) {
	HRESULT hRes;
	ScreenToClient(&point);
	HTREEITEM hItem = GetDropItem(point,TVHT_ONITEM);

	STGMEDIUM StgMed;
	FORMATETC fmte = { CF_HDROP, (DVTARGETDEVICE FAR *)NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

	hRes = pDataObject->GetData(&fmte,&StgMed);
	if(hRes==S_OK) {
		HDROP hDropInfo = (HDROP)StgMed.hGlobal;

		SetActiveWindow();      // activate us first !
		CString strFolder;
		GetCurrentFolder(strFolder,hItem);
		CFilesHelper helper(GetDocument());
		helper.OnDropFiles(m_hWnd,hDropInfo,strFolder);
		Populate();
	}
	return TRUE;
}

HTREEITEM CViewFilesT::GetDropItem(const CPoint& point,UINT uFlags) {
	UINT uHitFlags;
	HTREEITEM hItem = HitTest(point,&uHitFlags);
	if(hItem && (uHitFlags & uFlags)) 
		return hItem;
	return NULL;
}

LRESULT CViewFilesT::OnCreateIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CScriptLine* pItem = reinterpret_cast<CScriptLine*>(GetSelectedEntry());
	if(!pItem || pItem->GetSection()!=CInnoScript::SEC_FILES) return 0;

	CFilesHelper helper(GetDocument());
	helper.CreateIcon(m_hWnd,pItem);
	return 0;
}

LRESULT CViewFilesT::OnUpdateUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	if(wParam==ID_POPUP_CREATEICON)
		return GetSelectedEntry()!=NULL;
	else
		return CMyTreeView::OnUpdateUI(uMsg,wParam,lParam,bHandled);
}
