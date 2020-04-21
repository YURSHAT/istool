// ViewLanguages.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewLanguages.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewLanguages

CViewLanguages::CViewLanguages() : CMyListView(CInnoScript::SEC_LANGUAGES) {
}

/////////////////////////////////////////////////////////////////////////////
// CViewLanguages message handlers

const CListInfo CViewLanguages::m_colInfo[] = {
	"Name",				LVCFMT_LEFT,	100, 0,
	"MessagesFile",		LVCFMT_LEFT,	120, 0,
	"LicenseFile",		LVCFMT_LEFT,	120, 0,
	"InfoBeforeFile",	LVCFMT_LEFT,	120, 0,
	"InfoAfterFile",	LVCFMT_LEFT,	120, 0,
	NULL
};
