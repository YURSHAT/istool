// ViewDelete.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewDelete.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDelete

CViewDelete::CViewDelete(CInnoScript::SECTION sec) : CMyListView(sec) {
}

const CListInfo CViewDelete::m_colInfo[] = {
	"Name",				LVCFMT_LEFT,	250, 0,
	"Type",				LVCFMT_LEFT,	180, 0,
	"MinVersion",		LVCFMT_LEFT,	 80, CLI_HIDE,
	"OnlyBelowVersion",	LVCFMT_LEFT,	 80, CLI_HIDE,
	"Components",		LVCFMT_LEFT,	100, CLI_HIDE,
	"Tasks",			LVCFMT_LEFT,	100, CLI_HIDE,
	"Languages",		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};
