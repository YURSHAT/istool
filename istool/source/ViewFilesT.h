#pragma once

class CISToolDoc;

//#define SYSIMGLIST
#ifdef SYSIMGLIST
#include "SystemImageList.h"
#endif

#include "MyTreeView.h"
#include "MyDropTarget.h"

/////////////////////////////////////////////////////////////////////////////
// CViewFilesT view

class CViewFilesT : public CMyTreeView, public CMyDropTarget {
public:
	CViewFilesT();

	BEGIN_MSG_MAP(CViewFilesT)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(UWM_CREATEICON,OnCreateIcon)
		MESSAGE_HANDLER(UWM_UPDATEUI,OnUpdateUI)
		CHAIN_MSG_MAP(CMyTreeView)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreateIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateUI(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	virtual DWORD OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DWORD OnDragOver(IDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DWORD OnDrop(IDataObject* pDataObject, DWORD grfKeyState, CPoint point, DWORD dropEffect);

public:
	void GetCurrentFolder(CString& strFolder,HTREEITEM hItem=NULL);
protected:
//	CMyDropTarget		m_dropTarget;
#ifdef SYSIMGLIST
	CSystemImageList    m_ImgList;
#endif
	void GetItemDirectory(HTREEITEM,CString&);
	virtual void Populate();
	void SetItemData(HTREEITEM hItem,CInnoScript::CLine* pBase);
	void SortChildren(HTREEITEM hParent);
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void InsertFileName(LPCTSTR lpszFileName);
	HTREEITEM GetDropItem(const CPoint& point,UINT uFlags);

private:
	virtual HTREEITEM InsertItem(CScriptLine*);
	virtual void OnPreNewItem(CScriptLine* pLine);
};
