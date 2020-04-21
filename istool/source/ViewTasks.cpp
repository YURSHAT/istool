// ViewComponents.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewTasks.h"
#include "MyDoc.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTasks

CViewTasks::CViewTasks() : CMyListView(CInnoScript::SEC_TASKS) {}

/////////////////////////////////////////////////////////////////////////////
// CViewTasks message handlers

const CListInfo CViewTasks::m_colInfo[] = {
	_T("Name"),				LVCFMT_LEFT,	100, 0,
	_T("Description"),		LVCFMT_LEFT,	120, 0,
	_T("GroupDescription"),	LVCFMT_LEFT,	120, 0,
	_T("Flags"),			LVCFMT_LEFT,	150, 0,
	_T("MinVersion"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),	LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Components"),		LVCFMT_LEFT,	100, 0,
	_T("Languages"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};

void CViewTasks::OnDeleteItem(CScriptLine* pLine) {
	LPCTSTR pszTask = pLine->GetParameter(_T("Name"));
	if(!pszTask || !*pszTask) return;

	CScriptList list;
	UINT nCount = 0;
	GetDocument()->GetScript().GetCompleteList(list);
	for(int i=0;i<list.GetSize();i++) {
		if(list[i]->GetParameterFlag(_T("Tasks"),pszTask)) {
			list[i]->SetParameterFlag(_T("Tasks"),pszTask,false);
			nCount++;
		}
	}
#ifdef _DEBUG
	CString tmp;
	tmp.Format(_T("Removed %d references to the %s task."),nCount,pszTask);
	AtlMessageBox(m_hWnd,(LPCTSTR)tmp,IDR_MAINFRAME);
#endif
}
