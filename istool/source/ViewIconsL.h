#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewIconsL view

class CViewIconsL : public CMyListView {
public:
	CViewIconsL();           // protected constructor used by dynamic creation

protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }
};
