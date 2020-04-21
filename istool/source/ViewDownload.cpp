// ViewDelete.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewDownload.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep

CViewDownload::CViewDownload() : CMyListView(CInnoScript::PRJ_DOWNLOAD) {
}

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep message handlers

const CListInfo CViewDownload::m_colInfo[] = {
	"Source",		LVCFMT_LEFT,	250,	0,
	"DestDir",		LVCFMT_LEFT,	180,	0,
	"DestName",		LVCFMT_LEFT,	180,	0,
	NULL
};
