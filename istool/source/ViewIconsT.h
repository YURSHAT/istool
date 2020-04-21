#pragma once

#include "MyTreeView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewIconsT view

class CViewIconsT : public CMyTreeView {
public:
	CViewIconsT();

protected:
	virtual void Populate();
	void GetCurrentFolder(CString&);

	virtual void OnPreNewItem(CScriptLine* pLine);
	virtual HTREEITEM InsertItem(CScriptLine* pLine);
};
