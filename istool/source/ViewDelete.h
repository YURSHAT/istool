#pragma once

#include "MyDoc.h"
#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDelete view

class CViewDelete : public CMyListView {
protected:
	CViewDelete(CInnoScript::SECTION sec);           // protected constructor used by dynamic creation
protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }
};
