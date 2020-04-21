#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewLanguages view

class CViewLanguages : public CMyListView {
public:
	CViewLanguages();           // protected constructor used by dynamic creation

protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }
};
