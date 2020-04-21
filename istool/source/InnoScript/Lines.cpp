// Lines.cpp: implementation of the CLines class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lines.h"
#include "Token.h"
#include "Param.h"

//////////////////////////////////////////////////////////////////////
// CLine class.

CInnoScript::CLine::CLine(SECTION sec) : m_sec(sec), m_pParams(NULL) {
	m_dwUserFlags = 0;
}

CInnoScript::CLine::~CLine() {
	CParam* pParam = m_pParams;
	while(pParam) {
		CParam* pSave = pParam;
		pParam = pParam->GetNext();
		delete pSave;
	}
	m_pParams = NULL;
}

CInnoScript::SECTION CInnoScript::CLine::GetSection() {
	return m_sec;
}

LPCTSTR CInnoScript::CLine::GetComment() {
	return NULL;
}

void CInnoScript::CLine::SetIndent(const CString& strIndent) {
	m_strIndent = strIndent;
}

LPCTSTR CInnoScript::CLine::GetIndent() const {
	return m_strIndent;
}

LPSTR CInnoScript::CLine::UnQuote(LPSTR pszLine) {
	if(!pszLine) return pszLine;

	UINT nLength = _tcslen(pszLine);
	if(pszLine[0]!='"' || pszLine[nLength-1]!='"') {
		CString str(pszLine);
		str.TrimLeft();
		str.TrimRight();
		_tcscpy(pszLine,str);
		return pszLine;
	}

	LPSTR pszDest = pszLine;
	LPSTR pszSave = pszLine;

	nLength-=2;
	pszLine++;
	pszLine[nLength] = 0;

	while(*pszLine) {
		if(pszLine[0]=='"' && pszLine[1]=='"') {
			pszLine+=2;
			*pszDest++ = '"';
		} else
			*pszDest++ = *pszLine++;
	}

	*pszDest = 0;
	return pszSave;
}

LPCTSTR	CInnoScript::CLine::GetParameter(LPCTSTR pszName) {
	CParam* pParam = m_pParams;
	while(pParam) {
		if(!_stricmp(pParam->GetName(),pszName))
			return pParam->GetValue();

		pParam = pParam->GetNext();
	}
	return NULL;
}

void CInnoScript::CLine::AddParameter(LPCTSTR pszName,LPCTSTR pszValue) {
	CParam* pParam = new CParam(pszName,pszValue);
	if(m_pParams)
		m_pParams->AddTail(pParam);
	else
		m_pParams = pParam;
}

void CInnoScript::CLine::SetParameter(LPCTSTR pszName,LPCTSTR pszValue) {
	CParam* pParam = m_pParams;
	while(pParam) {
		if(!_stricmp(pParam->GetName(),pszName)) {
			pParam->SetValue(pszValue);
			return;
		}
		pParam = pParam->GetNext();
	}
	AddParameter(pszName,pszValue);
}

void CInnoScript::CLine::DeleteParameter(LPCTSTR pszName) {
	CParam* pParam = m_pParams;
	CParam* pParent = NULL;
	while(pParam) {
		if(!_stricmp(pParam->GetName(),pszName)) {
			if(pParent) 
				pParent->m_pNext = pParam->m_pNext;
			else
				m_pParams = pParam->m_pNext;
			break;
		}
		pParent = pParam;
		pParam = pParam->GetNext();
	}
	delete pParam;
}

bool CInnoScript::CLine::GetParameterFlag(LPCTSTR pszName,LPCTSTR pszFlag) {
	LPCTSTR pszValue = GetParameter(pszName);
	if(pszValue) {
		CToken flags(pszValue," ");
		LPCTSTR psz;
		while(psz = flags.GetNext()) {
			if(!_stricmp(pszFlag,psz))
				return true;
		}
	}
	return false;
}

void CInnoScript::CLine::SetParameterFlag(LPCTSTR pszName,LPCTSTR pszFlag,bool bSet) {
	char	szBuffer[1024];
	LPCTSTR pszValue = GetParameter(pszName);
	if(!pszValue) {
		if(bSet) SetParameter(pszName,pszFlag);
	} else {
		CToken flags(pszValue," ");
		if(bSet) {
			while(LPCTSTR psz=flags.GetNext()) {
				// Already there?
				if(!_stricmp(pszFlag,psz))
					return;
			}
			// Need to add it
			_tcscpy(szBuffer,pszValue);
			_tcscat(szBuffer," ");
			_tcscat(szBuffer,pszFlag);
			SetParameter(pszName,szBuffer);
		} else {
			szBuffer[0] = 0;
			bool bFound = false;
			while(LPCTSTR psz=flags.GetNext()) {
				if(_stricmp(pszFlag,psz)) {
					if(szBuffer[0]) _tcscat(szBuffer," ");
					_tcscat(szBuffer,psz);
				} else
					bFound = true;
			}
			// If found flags is empty, remove parameter
			if(!szBuffer[0])
				DeleteParameter(pszName);
			// If current flag was found, set new flags
			else if(bFound)
				SetParameter(pszName,szBuffer);
		}
	}
}

void CInnoScript::CLine::Write(LPSTR pszOut,UINT nLength) {
	*pszOut = 0;
	if(!m_strIndent.IsEmpty())
		_tcscpy(pszOut,m_strIndent);

	CParam* pParam = m_pParams;
	bool bKey = false;													// Special for ini entries
	bool bString = false;												// Special for ini entries
	while(pParam) {
		// Special for ini entries
		if(m_sec==SEC_INI) {
			if(!_stricmp("Key",pParam->GetName())) bKey = true;				
			else if(!_stricmp("String",pParam->GetName())) bString = true;	
		}

		if(*pszOut) {
			if(_tcslen(pszOut)+2>nLength) return;
			_tcscat(pszOut,"; ");
		}
		if(_tcslen(pszOut)+_tcslen(pParam->GetName())>nLength) return;
		_tcscat(pszOut,pParam->GetName());
		if(_tcslen(pszOut)+2>nLength) return;
		_tcscat(pszOut,": ");

		LPCTSTR pszValue = pParam->GetValue();
		LPSTR pszQuoted = NULL;
		// Do we need to quote the damn thing?
		if(_tcschr(pszValue,'"') || _tcschr(pszValue,';') || pszValue[0]==' ' || pszValue[_tcslen(pszValue)-1]==' ') {
			pszQuoted = new char[_tcslen(pszValue)*2+1];
			char* pszDest = pszQuoted;
			*pszDest++ = '"';
			while(*pszValue) {
				if(*pszValue=='"') 
					*pszDest++ = *pszValue;
				*pszDest++ = *pszValue++;
			}
			*pszDest++ = '"';
			*pszDest = 0;

			pszValue = pszQuoted;
		}

		if(_tcslen(pszOut)+_tcslen(pszValue)>nLength) return;
		_tcscat(pszOut,pszValue);
		pParam = pParam->GetNext();

		if(pszQuoted) delete []pszQuoted;
	}

	// Special for ini entries
	if(m_sec==SEC_INI && bKey && !bString) {
		if(*pszOut) {
			if(_tcslen(pszOut)+2>nLength) return;
			_tcscat(pszOut,"; ");
		}
		if(_tcslen(pszOut)+10>nLength) return;
		_tcscat(pszOut,"String: \"\"");
	}
	if(m_dwUserFlags & FLG_CONCAT) {
		if(_tcslen(pszOut)+3>nLength) return;
		if(*pszOut) {
			_tcscat(pszOut,";");
		}
		_tcscat(pszOut," \\");
	}
}

LPCTSTR	CInnoScript::CLine::GetKey() {
	return NULL;
}

LPCTSTR	CInnoScript::CLine::GetValue() {
	return NULL;
}

CInnoScript::CLine* CInnoScript::CLine::Copy() {
	CLine* pLine = new CLine(m_sec);
	pLine->m_dwUserFlags = m_dwUserFlags;
	CParam* pParams = m_pParams;
	while(pParams) {
		pLine->AddParameter(pParams->GetName(),pParams->GetValue());
		pParams = pParams->GetNext();
	}
	return pLine;
}

bool CInnoScript::CLine::IsHash() {
	return false;
}

//////////////////////////////////////////////////////////////////////
// CLineComment class.

CInnoScript::CLineComment::CLineComment(SECTION sec,LPCTSTR pszLine) : CLine(sec) {
	int nLength = _tcslen(pszLine);
	m_pszLine = new char[nLength+1];
	_tcscpy(m_pszLine,pszLine);
}

CInnoScript::CLineComment::~CLineComment() {
	if(m_pszLine) delete []m_pszLine;
}

void CInnoScript::CLineComment::Write(LPSTR pszOut,UINT nLength) {
	if(_tcslen(m_pszLine)<nLength) {
		_tcscpy(pszOut,m_pszLine);

		if(m_dwUserFlags & FLG_CONCAT) {
			if(_tcslen(pszOut)+3>nLength) return;
			if(*pszOut) {
				_tcscat(pszOut,";");
			}
			_tcscat(pszOut," \\");
		}
	} else
		*pszOut = 0;
}

LPCTSTR CInnoScript::CLineComment::GetComment() {
	return m_pszLine;
}

CInnoScript::CLine* CInnoScript::CLineComment::Copy() {
	CLine* pLine = new CLineComment(m_sec,m_pszLine);
	pLine->m_dwUserFlags = m_dwUserFlags;
	CParam* pParams = m_pParams;
	while(pParams) {
		pLine->AddParameter(pParams->GetName(),pParams->GetValue());
		pParams = pParams->GetNext();
	}
	return pLine;
}

//////////////////////////////////////////////////////////////////////
// CLineHash class.

CInnoScript::CLineHash::CLineHash(SECTION sec,LPCTSTR pszLine) : CLine(sec) {
	int nLength = _tcslen(pszLine);
	m_pszLine = new char[nLength+1];
	_tcscpy(m_pszLine,pszLine);
}

CInnoScript::CLineHash::~CLineHash() {
	if(m_pszLine) delete []m_pszLine;
}

void CInnoScript::CLineHash::Write(LPSTR pszOut,UINT nLength) {
	if(_tcslen(m_pszLine)<nLength) {
		_tcscpy(pszOut,m_pszLine);

		if(m_dwUserFlags & FLG_CONCAT) {
			if(_tcslen(pszOut)+3>nLength) return;
			if(*pszOut) {
				_tcscat(pszOut,";");
			}
			_tcscat(pszOut," \\");
		}
	} else
		*pszOut = 0;
}

LPCTSTR CInnoScript::CLineHash::GetComment() {
	return m_pszLine;
}

CInnoScript::CLine* CInnoScript::CLineHash::Copy() {
	CLine* pLine = new CLineHash(m_sec,m_pszLine);
	pLine->m_dwUserFlags = m_dwUserFlags;
	CParam* pParams = m_pParams;
	while(pParams) {
		pLine->AddParameter(pParams->GetName(),pParams->GetValue());
		pParams = pParams->GetNext();
	}
	return pLine;
}

bool CInnoScript::CLineHash::IsHash() {
	return true;
}

//////////////////////////////////////////////////////////////////////
// CLineSetup class.

CInnoScript::CLineSetup::CLineSetup(SECTION sec,LPCTSTR pszLine) : CLine(sec) {
	CToken params(pszLine,"=");
	LPCTSTR pszParam = params.GetNext();
	LPCTSTR pszValue = UnQuote(params.GetRest());
	if(!pszParam /*|| !pszValue*/) throw "Invalid line.";

	// Add all parameters as strings
	if(m_pParams)
		m_pParams->AddTail(new CParam(pszParam,pszValue));
	else
		m_pParams = new CParam(pszParam,pszValue);
}

CInnoScript::CLineSetup::CLineSetup(SECTION sec) : CLine(sec) {
}

CInnoScript::CLineSetup::~CLineSetup() {}

void CInnoScript::CLineSetup::Write(LPSTR pszOut,UINT nLength) {
	*pszOut = 0;
	if(!m_strIndent.IsEmpty())
		_tcscpy(pszOut,m_strIndent);

	CParam* pParam = m_pParams;
	while(pParam) {
		_tcscpy(pszOut,pParam->GetName());
		_tcscat(pszOut,"=");
		_tcscat(pszOut,pParam->GetValue());

		pParam = pParam->GetNext();
	}
}

LPCTSTR	CInnoScript::CLineSetup::GetKey() {
	return m_pParams ? m_pParams->GetName() : NULL;
}

LPCTSTR	CInnoScript::CLineSetup::GetValue() {
	return m_pParams ? m_pParams->GetValue() : NULL;
}

void CInnoScript::CLineSetup::SetKey(LPCTSTR pszKey) {
	if(m_pParams)
		m_pParams->SetName(pszKey);
	else
		m_pParams = new CParam(pszKey,NULL);
}

void CInnoScript::CLineSetup::SetValue(LPCTSTR pszValue) {
	m_pParams->SetValue(pszValue);
}

CInnoScript::CLine* CInnoScript::CLineSetup::Copy() {
	CLine* pLine = new CLineSetup(m_sec);
	pLine->m_dwUserFlags = m_dwUserFlags;
	CParam* pParams = m_pParams;
	while(pParams) {
		pLine->AddParameter(pParams->GetName(),pParams->GetValue());
		pParams = pParams->GetNext();
	}
	return pLine;
}

//////////////////////////////////////////////////////////////////////
// CLineParam class.

CInnoScript::CLineParam::CLineParam(SECTION sec,LPCTSTR pszLine) : CLine(sec) {
	CToken tokens(pszLine,";");
	LPCTSTR psz;
	while(psz=tokens.GetNext()) {
		CToken params(psz,":");
		LPCTSTR pszParam = params.GetNext();
		LPCTSTR pszValue = UnQuote(params.GetRest());
		if(!pszParam) throw "Invalid line.";

		// Add all parameters as strings
		if(m_pParams)
			m_pParams->AddTail(new CParam(pszParam,pszValue));
		else
			m_pParams = new CParam(pszParam,pszValue);
	}
}

CInnoScript::CLineParam::~CLineParam() {}

CInnoScript::CLine* CInnoScript::CLineParam::Copy() {
	CLine* pLine = new CLineParam(m_sec,"");
	pLine->m_dwUserFlags = m_dwUserFlags;
	CParam* pParams = m_pParams;
	while(pParams) {
		pLine->AddParameter(pParams->GetName(),pParams->GetValue());
		pParams = pParams->GetNext();
	}
	return pLine;
}
