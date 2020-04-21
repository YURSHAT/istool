// Param.cpp: implementation of the CParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Param.h"

#ifdef _MFC_VER
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////
// CParam class.

CInnoScript::CParam::CParam(LPCTSTR pszName,LPCTSTR pszValue) : m_pNext(NULL), m_pszName(NULL), m_pszValue(NULL) {
	m_pszName = new char[_tcslen(pszName)+1];
	_tcscpy(m_pszName,pszName);
	if(pszValue) {
		m_pszValue = new char[_tcslen(pszValue)+1];
		_tcscpy(m_pszValue,pszValue);
	} else {
		m_pszValue = new char[1];
		*m_pszValue = 0;
	}
}

CInnoScript::CParam::~CParam() {
	if(m_pszName) delete []m_pszName;
	if(m_pszValue) delete []m_pszValue;
}

CInnoScript::CParam* CInnoScript::CParam::GetNext() {
	return m_pNext;
}

void CInnoScript::CParam::AddTail(CParam* pParam) {
	CParam* p = this;
	while(p->m_pNext) p = p->m_pNext;
	p->m_pNext = pParam;
}

LPCTSTR CInnoScript::CParam::GetName() {
	return m_pszName;
}

LPCTSTR CInnoScript::CParam::GetValue() {
	return m_pszValue;
}

void CInnoScript::CParam::SetValue(LPCTSTR pszValue) {
	if(m_pszValue) delete []m_pszValue;
	m_pszValue = new char[_tcslen(pszValue)+1];
	_tcscpy(m_pszValue,pszValue);
}

void CInnoScript::CParam::SetName(LPCTSTR pszName) {
	if(m_pszName) delete []m_pszName;
	m_pszName = new char[_tcslen(pszName)+1];
	_tcscpy(m_pszName,pszName);
}
