#pragma once

class CISToolDoc;
class CScriptComponent;
#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTasks view

class CViewTasks : public CMyListView {
public:
	CViewTasks();           // protected constructor used by dynamic creation

protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }

	virtual void OnDeleteItem(CScriptLine* pLine);
};
