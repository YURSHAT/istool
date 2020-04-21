// Token.h: interface for the CToken class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_TOKEN_H)
#define _TOKEN_H

#ifndef _INNOSCRIPT_H
#include "InnoScript.h"
#endif

class CInnoScript::CToken {
public:
	CToken(LPCTSTR pszLine,LPCTSTR pszDelim);
	virtual ~CToken();
	LPSTR GetNext();
	LPSTR GetRest();
protected:
	LPSTR		m_pszLine;
	LPSTR		m_pszDelim;
	LPSTR		m_pszCurrPos;
};

#endif // !defined(_TOKEN_H)
