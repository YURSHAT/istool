// ViewIconsB.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewIconsL.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewIconsL

CViewIconsL::CViewIconsL() : CMyListView(CInnoScript::SEC_ICONS) {
	m_nItemImage = 6;
}

/////////////////////////////////////////////////////////////////////////////
// CViewIconsL message handlers

const CListInfo CViewIconsL::m_colInfo[] = {
	"Name",				LVCFMT_LEFT,	140, 0,
	"Filename",			LVCFMT_LEFT,	200, 0,
	"Parameters",		LVCFMT_LEFT,	150, CLI_HIDE,
	"WorkingDir",		LVCFMT_LEFT,	160, 0,
	"IconFilename",		LVCFMT_LEFT,	140, CLI_HIDE,
	"IconIndex",		LVCFMT_RIGHT,	 60, CLI_HIDE,
	"Comment",			LVCFMT_LEFT,	120, CLI_HIDE,
	"Flags",			LVCFMT_LEFT,	150, 0,
	"MinVersion",		LVCFMT_LEFT,	 80, CLI_HIDE,
	"OnlyBelowVersion",	LVCFMT_LEFT,	 80, CLI_HIDE,
	"Components",		LVCFMT_LEFT,	100, CLI_HIDE,
	"Tasks",			LVCFMT_LEFT,	100, CLI_HIDE,
	"Languages",				LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};
