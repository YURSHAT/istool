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

// last revised: 24 Apr 98
// Registry.cpp : implementation file
//
// Description:
// CRegistryEx is a wrapper for the Windows Registry API.  It allows
//  easy modification of the Registry with easy to remember terms like
//  Read, Write, Open, and Close.

#include "stdafx.h"
#include "Registry.h"

CRegistryEx::CRegistryEx(HKEY hKeyRoot)
{
	m_hKey = hKeyRoot;
}

CRegistryEx::~CRegistryEx()
{
	Close();
}


BOOL CRegistryEx::VerifyKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	ATLASSERT (hKeyRoot);
	ATLASSERT (pszPath);

	LONG ReturnValue = RegOpenKeyEx (hKeyRoot, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);
	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	return FALSE;
}

BOOL CRegistryEx::VerifyKey (LPCTSTR pszPath)
{
	ATLASSERT (m_hKey);

	LONG ReturnValue = RegOpenKeyEx (m_hKey, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);
	
	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::VerifyValue (LPCTSTR pszValue)
{
	ATLASSERT(m_hKey);
	LONG lReturn = RegQueryValueEx(m_hKey, pszValue, NULL,
		NULL, NULL, NULL);

	m_Info.lMessage = lReturn;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::CreateKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	DWORD dw;

	LONG ReturnValue = RegCreateKeyEx (hKeyRoot, pszPath, 0L, NULL,
		0/*REG_OPTION_VOLATILE*/, KEY_ALL_ACCESS, NULL, 
		&m_hKey, &dw);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::Open (HKEY hKeyRoot, LPCTSTR pszPath)
{
	m_sPath = pszPath;

	LONG ReturnValue = RegOpenKeyEx (hKeyRoot, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

void CRegistryEx::Close()
{
	if (m_hKey)
	{
		RegCloseKey (m_hKey);
		m_hKey = NULL;
	}
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, int iVal)
{
	DWORD dwValue;

	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);
	
	dwValue = (DWORD)iVal;
	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwValue, sizeof(DWORD));

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = sizeof(DWORD);
	m_Info.dwType = REG_DWORD;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, DWORD dwVal)
{
	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);
	return RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));
}

BOOL CRegistryEx::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);
	ATLASSERT(pszData);

	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_SZ,
		(CONST BYTE*) pszData, _tcslen(pszData) + 1);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = _tcslen(pszData) + 1;
	m_Info.dwType = REG_SZ;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistryEx::Read(LPCTSTR pszKey, int& iVal)
{
	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		iVal = (int)dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::Read (LPCTSTR pszKey, CString& sVal)
{
	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = 200;
	char  szString[255];

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) szString, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		sVal = szString;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegistryEx::DeleteValue (LPCTSTR pszValue)
{
	ATLASSERT(m_hKey);
	LONG lReturn = RegDeleteValue(m_hKey, pszValue);


	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::DeleteValueKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	ATLASSERT(pszPath);
	ATLASSERT(hKeyRoot);

	LONG lReturn = RegDeleteKey(hKeyRoot, pszPath);

	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

static void ShowSysMsg(LONG res, char* str, char* file, DWORD line)
{
	char msg[256];
	LPVOID lpMsgBuf;
	FormatMessage(		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM |	 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,	 
						res,	
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language    
						(LPTSTR) &lpMsgBuf,    
						0,    
						NULL );
	sprintf(msg, "%u %s\n%s %u", res, str, file, line);
	MessageBox( NULL, msg, (char*)lpMsgBuf, MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf ); 
}

BOOL CRegistryEx::ListKey (LPCTSTR pszKey, CStringArray& scArray)
{
	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);
	DWORD	dwSubKeys;
	DWORD	idw;
	LONG	lReturn;
	HKEY	hQKey  = NULL;
	CString szSubKeyName;
	TCHAR	szStr[200];
	DWORD	dwSubKeyNameLen;
	FILETIME	LastFileTime;
	BOOL	Ok;

	lReturn = RegOpenKeyEx (m_hKey, pszKey, 0L,
		KEY_ALL_ACCESS, &hQKey);

	Ok = (lReturn == ERROR_SUCCESS);

	m_Info.lMessage = lReturn;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if (Ok) {

		lReturn = RegQueryInfoKey( hQKey, NULL, NULL, NULL
			, &dwSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		
		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
	}
	if (Ok) {
		scArray.Empty();
	}
	for ( idw=0; ((idw<dwSubKeys) && Ok); idw++) {
		dwSubKeyNameLen = 200;
		lReturn = RegEnumKeyEx( 
				hQKey
			,	idw
			,	szStr
			,	&dwSubKeyNameLen
			,	NULL
			,	NULL
			,	NULL
			,	&LastFileTime
			);

		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
		if (Ok)  {
			// Add name to array
			szSubKeyName = szStr;
			scArray.Add( szSubKeyName );
		}
		else {
			ShowSysMsg(lReturn, "RegEnumKeyEx", __FILE__, __LINE__);
		}

	}
	if (hQKey != NULL) {
		RegCloseKey (hQKey);
		hQKey = NULL;
	}
	if(Ok)
		return TRUE;

	return FALSE;
}

BOOL CRegistryEx::ListValues (LPCTSTR pszKey, CStringArray& scArray)
{
	ATLASSERT(m_hKey);
	ATLASSERT(pszKey);
	DWORD	dwValues;
	DWORD	idw;
	LONG	lReturn;
	HKEY	hQKey  = NULL;
	CString szValueName;
	TCHAR	szStr[200];
	DWORD	dwValueNameLen;
	DWORD	dwType;
	BOOL	Ok;

	lReturn = RegOpenKeyEx (m_hKey, pszKey, 0L,
		KEY_ALL_ACCESS, &hQKey);

	Ok = (lReturn == ERROR_SUCCESS);

	m_Info.lMessage = lReturn;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if (Ok) {

		lReturn = RegQueryInfoKey( hQKey, NULL, NULL, NULL
			, NULL, NULL, NULL, &dwValues, NULL, NULL, NULL, NULL );
		
		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
	}
	if (Ok) {
		scArray.Empty();
	}
	for ( idw=0; ((idw<dwValues) && Ok); idw++) {
		dwValueNameLen = 200;
		lReturn = RegEnumValue( 
				hQKey				// handle to key to query
			,	idw					// index of value to query
			,	szStr				// address of buffer for value string
			,	&dwValueNameLen		// address for size of value buffer
			,	NULL				// reserved
			,	&dwType				// address of buffer for type code
			,	NULL				// address of buffer for value data
			,	NULL				// address for size of data buffer
		);

		m_Info.lMessage = lReturn;
		m_Info.dwType = 0;
		m_Info.dwSize = 0;
		
		Ok = (lReturn == ERROR_SUCCESS);
		if (Ok)  {
			// Add name to array
			szValueName = szStr;
			scArray.Add( szValueName );
		}
		else {
			ShowSysMsg(lReturn, "RegEnumValue", __FILE__, __LINE__);
		}

	}
	if (hQKey != NULL) {
		RegCloseKey (hQKey);
		hQKey = NULL;
	}
	if(Ok)
		return TRUE;

	return FALSE;
}
