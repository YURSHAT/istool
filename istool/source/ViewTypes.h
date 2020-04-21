#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTypes view

class CViewTypes : public CMyListView {
public:
	CViewTypes();           // protected constructor used by dynamic creation

protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }
};
