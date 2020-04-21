// ViewComponents.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewComponents.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewComponents

CViewComponents::CViewComponents() : CMyListView(CInnoScript::SEC_COMPONENTS) {
}


/////////////////////////////////////////////////////////////////////////////
// CViewComponents message handlers

const CListInfo CViewComponents::m_colInfo[] = {
	"Name",						LVCFMT_LEFT,	100, 0,
	"Description",				LVCFMT_LEFT,	120, 0,
	"ExtraDiskSpaceRequired",	LVCFMT_RIGHT,	120, 0,
	"Types",					LVCFMT_LEFT,	150, 0,
	"Flags",					LVCFMT_LEFT,	150, 0,
	"MinVersion",				LVCFMT_LEFT,	 80, CLI_HIDE,
	"OnlyBelowVersion",			LVCFMT_LEFT,	 80, CLI_HIDE,
	"Languages",				LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};

void CViewComponents::OnDeleteItem(CScriptLine* pLine) {
	LPCTSTR pszComponent = pLine->GetParameter("Name");
	if(!pszComponent || !*pszComponent) return;

	CScriptList list;
	UINT nCount = 0;
	GetDocument()->GetScript().GetCompleteList(list);
	for(int i=0;i<list.GetSize();i++) {
		if(list[i]->GetParameterFlag("Components",pszComponent)) {
			list[i]->SetParameterFlag("Components",pszComponent,false);
			nCount++;
		}
	}
#ifdef _DEBUG
	CString tmp;
	tmp.Format("Removed %d references to the %s component.",nCount,pszComponent);
	AtlMessageBox(m_hWnd,(LPCTSTR)tmp,IDR_MAINFRAME);
#endif
}
