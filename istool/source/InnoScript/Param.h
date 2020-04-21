// Param.h: interface for the CParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PARAM_H)
#define _PARAM_H

#ifndef _INNOSCRIPT_H
#include "InnoScript.h"
#endif

class CInnoScript::CParam {
	friend class CLine;
public:
	CParam(LPCTSTR pszName,LPCTSTR pszValue);
	virtual ~CParam();
	void AddTail(CParam* pParam);
	CParam* GetNext();
	LPCTSTR GetName();
	LPCTSTR GetValue();
	void SetValue(LPCTSTR pszValue);
	void SetName(LPCTSTR pszName);
protected:
	LPSTR		m_pszName;
	LPSTR		m_pszValue;
	CParam*		m_pNext;
};

#endif // !defined(_PARAM_H)
