#pragma once

class CScriptComponent;

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewComponents view

class CViewComponents : public CMyListView {
public:
	CViewComponents();           // protected constructor used by dynamic creation
	
protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }

	virtual void OnDeleteItem(CScriptLine* pLine);
};
