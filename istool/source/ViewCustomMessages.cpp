// ViewMessages.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewMessages.h"
#include "ViewCustomMessages.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "MainFrm.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewMessages

CViewCustomMessages::CViewCustomMessages() : CViewMessages(CInnoScript::SEC_CUSTOMMESSAGES) {
	SetSubMenu(4);
	m_nItemImage = 17;
	m_dwFlags |= VFL_SETUP;
}
