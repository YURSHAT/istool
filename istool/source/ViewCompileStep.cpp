// ViewDelete.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewCompileStep.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep

CViewCompileStep::CViewCompileStep(CInnoScript::SECTION sec) : CMyListView(sec) {
}

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep message handlers

const CListInfo CViewCompileStep::m_colInfo[] = {
	"Name",			LVCFMT_LEFT,	250,	0/*Flags*/,
	"Parameters",	LVCFMT_LEFT,	180,	0,
	"Flags",		LVCFMT_LEFT,	180,	0,
	NULL
};
