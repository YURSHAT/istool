#pragma once

#include "MyDoc.h"
#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep view

class CViewCompileStep : public CMyListView {
public:
	CViewCompileStep(CInnoScript::SECTION sec);           // protected constructor used by dynamic creation
protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }
};
