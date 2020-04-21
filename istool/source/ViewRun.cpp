// ViewRun.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewRun.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRun

CViewRun::CViewRun(CInnoScript::SECTION sec) : CMyListView(sec)
{
	m_nItemImage = 5;
}

/////////////////////////////////////////////////////////////////////////////
// CViewRun message handlers

const CListInfo CViewRun::m_colInfo[] = {
	_T("Filename"),			LVCFMT_LEFT,	250, 0,
	_T("Parameters"),		LVCFMT_LEFT,	180, 0,
	_T("WorkingDir"),		LVCFMT_LEFT,	120, 0,
	_T("Flags"),			LVCFMT_LEFT,	150, 0,
	_T("RunOnceId"),		LVCFMT_LEFT,	 60, CLI_HIDE,
	_T("Description"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("StatusMsg"),		LVCFMT_LEFT,	100, CLI_HIDE,
	_T("MinVersion"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),	LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Components"),		LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Tasks"),			LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Languages"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};
