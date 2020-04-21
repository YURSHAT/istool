#pragma once

#include "MyDoc.h"
#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep view

class CViewDownload : public CMyListView {
public:
	CViewDownload();           // protected constructor used by dynamic creation
protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }
};
