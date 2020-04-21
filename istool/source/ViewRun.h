#pragma once

#include "MyDoc.h"
#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRun view

class CViewRun : public CMyListView {
public:
	CViewRun(CInnoScript::SECTION sec);           // protected constructor used by dynamic creation

protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }
};
