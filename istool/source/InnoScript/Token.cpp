// Token.cpp: implementation of the CToken class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Token.h"

#ifdef _MFC_VER
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInnoScript::CToken::CToken(LPCTSTR pszLine,LPCTSTR pszDelim) {
	UINT nLength = _tcslen(pszLine);
	m_pszLine = new char[nLength+1];
	_tcscpy(m_pszLine,pszLine);

	m_pszDelim = new char[_tcslen(pszDelim)+1];
	_tcscpy(m_pszDelim,pszDelim);

	m_pszCurrPos = m_pszLine;
}

CInnoScript::CToken::~CToken() {
	if(m_pszLine) delete []m_pszLine;
	if(m_pszDelim) delete []m_pszDelim;
}

LPSTR CInnoScript::CToken::GetNext() {
	while(iswspace(*m_pszCurrPos)) m_pszCurrPos++;
	char* pszSave = m_pszCurrPos;

	if(!*pszSave) return NULL;

	bool bInQuote = false;
	while(*m_pszCurrPos) {
		if(*m_pszCurrPos=='"' && !bInQuote) {
			bInQuote = true;
		} else if(*m_pszCurrPos=='"' && m_pszCurrPos[1]=='"') {
			m_pszCurrPos++;	// Skip the extra quote
		} else if(*m_pszCurrPos=='"') {
			bInQuote = false;
		} else if(!bInQuote) {
			if(_tcschr(m_pszDelim,*m_pszCurrPos)) {
				*m_pszCurrPos++ = 0;
				return pszSave;
			}
		}
		m_pszCurrPos++;
	}
	return pszSave;
}

LPSTR CInnoScript::CToken::GetRest() {
	while(iswspace(*m_pszCurrPos)) m_pszCurrPos++;
	if(!*m_pszCurrPos) return NULL;
	return m_pszCurrPos;
}

