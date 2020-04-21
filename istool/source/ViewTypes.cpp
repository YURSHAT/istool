// ViewTypes.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewTypes.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTypes

CViewTypes::CViewTypes() : CMyListView(CInnoScript::SEC_TYPES) {}

/////////////////////////////////////////////////////////////////////////////
// CViewTypes message handlers

const CListInfo CViewTypes::m_colInfo[] = {
	_T("Name"),				LVCFMT_LEFT,	100, 0,
	_T("Description"),		LVCFMT_LEFT,	120, 0,
	_T("Flags"),			LVCFMT_LEFT,	150, 0,
	_T("MinVersion"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),	LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Languages"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};
