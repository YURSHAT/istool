#pragma once
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Shane Martin
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    shane.kim@kaiserslautern.netsurf.de
/////////////////////////////////////////////////////////////////////////////
#include <winreg.h>

#define REG_RECT	0x0001
#define REG_POINT	0x0002

class CStringArray : public CSimpleArray<LPSTR> {
public:
	~CStringArray() {
		Empty();
	}
	void Empty() {
		while(GetSize()) {
			delete []m_aT[0];
			RemoveAt(0);
		}
	}
	void Add(LPCTSTR psz) {
		LPSTR p = new CHAR[_tcslen(psz)+1];
		_tcscpy(p,psz);
		CSimpleArray<LPSTR>::Add(p);
	}
private:
	BOOL Remove(const LPSTR& t) {
		return FALSE;
	}
	BOOL RemoveAt(int nIndex) {
		return CSimpleArray<LPSTR>::RemoveAt(nIndex);
	}
	void RemoveAll() {
	}
};

class CRegistryEx {
// Construction
public:
	CRegistryEx(HKEY hKeyRoot = HKEY_LOCAL_MACHINE);
	virtual ~CRegistryEx();

	struct REGINFO
	{
		LONG lMessage;
		DWORD dwType;
		DWORD dwSize;
	} m_Info;
// Operations
public:
	BOOL VerifyKey (HKEY hKeyRoot, LPCTSTR pszPath);
	BOOL VerifyKey (LPCTSTR pszPath);
	BOOL VerifyValue (LPCTSTR pszValue);
	BOOL CreateKey (HKEY hKeyRoot, LPCTSTR pszPath);
	BOOL Open (HKEY hKeyRoot, LPCTSTR pszPath);
	void Close();

	BOOL DeleteValue (LPCTSTR pszValue);
	BOOL DeleteValueKey (HKEY hKeyRoot, LPCTSTR pszPath);

	BOOL Write (LPCTSTR pszKey, int iVal);
	BOOL Write (LPCTSTR pszKey, DWORD dwVal);
	BOOL Write (LPCTSTR pszKey, LPCTSTR pszVal);
	BOOL Write (LPCTSTR pszKey, LPCRECT rcRect);
	BOOL Write (LPCTSTR pszKey, LPPOINT& lpPoint);

	BOOL Read (LPCTSTR pszKey, int& iVal);
	BOOL Read (LPCTSTR pszKey, DWORD& dwVal);
	BOOL Read (LPCTSTR pszKey, CString& sVal);
	BOOL Read (LPCTSTR pszKey, LPPOINT& lpPoint);
	BOOL Read (LPCTSTR pszKey, LPRECT& rcRect);

	BOOL ListKey (LPCTSTR pszKey, CStringArray& scArray);
	BOOL ListValues (LPCTSTR pszKey, CStringArray& scArray);

protected:	
	HKEY 	m_hKey;
	CString m_sPath;
};
