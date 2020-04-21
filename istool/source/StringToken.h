// StringToken.h

#pragma once

class CStringToken {
public:
	CStringToken(const char* lpszLine,const char* lpszDel) : m_ptr(0) {
		m_ptr = new char[_tcslen(lpszLine)+1];
		_tcscpy(m_ptr,lpszLine);
		m_del = new char[_tcslen(lpszDel)+1];
		_tcscpy(m_del,lpszDel);
		m_next = m_ptr;
	}
	~CStringToken() {
		delete []m_ptr;
		delete []m_del;
	}
	const char* GetNext() {
		while(iswspace(*m_next)) m_next++;
		if(!*m_next) return 0;
		char* retptr = m_next;
		bool bQuote = false;
		while(*m_next) {
			if(*m_next=='"') {
				bQuote = !bQuote;
			}
			if(!bQuote && _tcschr(m_del,*m_next)) {
				char* sp = m_next-1;
				*m_next++ = 0;
				while(iswspace(*sp) && sp>retptr) *sp-- = 0;
				break;
			}
			m_next++;
		}
		return retptr;
	}
	const char* GetRest() {
		while(iswspace(*m_next)) m_next++;
		char* ptr = m_next;
		while(*ptr && *(ptr+1)) ptr++;
		while(iswspace(*ptr)) *ptr-- = 0;
		ptr = m_next;
		m_next = NULL;
		return ptr;
	}
	
private:
	char*	m_ptr;
	char*	m_del;
	char*	m_next;
};
