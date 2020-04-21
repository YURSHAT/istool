#include "stdafx.h"
#include "resource.h"
#include "translate.h"

static bool IsNT4() {
	DWORD dwVersion = GetVersion();
	return dwVersion<=0x8000000 && (dwVersion&0xFF)==4;
}

CAtlMap<CString,CString>	CTranslate::m_map;
#ifdef _DEBUG
const CString				CTranslate::m_strOrgFile = _T("D:\\svnroot\\istool\\istool\\trunk\\languages\\istool-en.lng");
#endif

void CTranslate::Warning(const CString& strKey,const CString& strDefault) {
#ifdef _DEBUG
	if(!m_strOrgFile.IsEmpty()) {
		CString tmp(strDefault);
		tmp.Replace("\r\n","\\r\\n");
		tmp.Replace("\r","\\r");
		tmp.Replace("\n","\\n");
		tmp.Replace("\t","\\t");
		::WritePrivateProfileString("Original Translation",strKey,tmp,m_strOrgFile);
	} else {
		AtlMessageBox(::GetActiveWindow(),(LPCTSTR)strKey,IDR_MAINFRAME,MB_OK|MB_ICONWARNING);
	}
#endif
}

HMENU CTranslate::Translate(HMENU hMenu,CString strParent/*=CString()*/) {
	if(IsNT4()) return hMenu;
	CMenuHandle menu(hMenu);
	FixMenuTitle(strParent,CString());
	int iCount = menu.GetMenuItemCount();
	for(int i=0;i<iCount;i++) {
		CString			strName, strAcc;
		MENUITEMINFO	info = {sizeof MENUITEMINFO};
		info.fMask = MIIM_ID | MIIM_SUBMENU | MIIM_STRING /*| MIIM_FTYPE*/;
		info.cch = 1024;
		info.dwTypeData = strName.GetBuffer(info.cch);
		if(!menu.GetMenuItemInfo(i,TRUE,&info) || !info.cch) continue;
		strName.GetBufferSetLength(info.cch);
		CString strOrgText(strName);
		FixMenuTitle(strName,strAcc);

		CString strKey;
		if(strParent.IsEmpty())
			strKey = strName;
		else
			strKey.Format(_T("%s|%s"),strParent,strName);

		// Oversett undermeny
		if(info.hSubMenu) Translate(info.hSubMenu,strKey);

		if(!strAcc.IsEmpty()) {
			CString strAccKey;
			strAccKey.Format("ShortCut|%s",strAcc);

			Warning(strAccKey,strAcc);

			CString strAccTrans;
			if(m_map.Lookup(strAccKey,strAccTrans))
				strAcc = strAccTrans;
		}

		int pos = strOrgText.Find(_T('\t'));
		if(pos>0) strOrgText.ReleaseBufferSetLength(pos);
		Warning(strKey,strOrgText);

		CString strTrans;
		if(!m_map.Lookup(strKey,strTrans))
			continue;

		strTrans.Replace(_T("\\t"),_T("\t"));

		if(!strAcc.IsEmpty()) {
			strTrans += _T("\t");
			strTrans += strAcc;
		}

		info.fMask = MIIM_STRING;
		info.cch = strTrans.GetLength();
		info.dwTypeData = strTrans.GetBuffer();
		menu.SetMenuItemInfo(i,TRUE,&info);
	}
	return hMenu;
}

void CTranslate::AddFile(const CString& strFileName) {
	if(strFileName.IsEmpty())
		return;

	FILE* fp = fopen(strFileName,"rb");
	if(!fp) return;

	TCHAR szLine[10000];
	while(fgets(szLine,sizeof szLine/sizeof szLine[0],fp)) {
		CString strLine(szLine);
		strLine.TrimRight();
		int pos = strLine.Find('=');
		if(strLine.IsEmpty() || strLine[0]==_T(';') || strLine[0]==_T('[') || pos<0) continue;

		CString strTrans = strLine.Mid(pos+1).Trim();
		if(strTrans.IsEmpty()) continue;
#if 1
		strTrans.Replace("\\r","\r");
		strTrans.Replace("\\n","\n");
		strTrans.Replace("\\t","\t");
#endif
		m_map[strLine.Left(pos).Trim()] = strTrans;
	}

	fclose(fp);
}

bool CTranslate::Lookup(LPCTSTR pszKey,CString& strTrans) {
	CString strKey(pszKey);

	FixTextTitle(strKey);

	Warning(strKey,pszKey);
	if(!m_map.Lookup(strKey,strTrans))
		return false;

	return true;
}

void CTranslate::FixMenuTitle(CString& str,CString& strAcc) {
	strAcc.Empty();
	int len = str.GetLength();
	for(int i=0;i<len;i++) {
		if(str[i]==_T('\t')) {
			strAcc = str.Mid(i+1);
			str.ReleaseBufferSetLength(len=i);
			break;
		} else if(str[i]==_T(' ') || str[i]==_T('-') || str[i]==_T('=')) {
			str.Delete(i);
			str.SetAt(i,toupper(str[i]));
			i--;
			len--;
		} else if(str[i]>=128 || str[i]==_T('&') || str[i]==_T(':') || str[i]==_T('.') || str[i]==_T('\'') || str[i]==_T('(') || str[i]==_T(')') || str[i]==_T(',') || str[i]==_T('/') || str[i]==_T('%') || str[i]==_T('!') || str[i]==_T('?') || str[i]==_T('\n') || str[i]==_T('\r') || str[i]==_T('\t')) {
			str.Delete(i--);
			len--;
		}
	}
	if(len>3 && str[len-3]==_T('.') && str[len-2]==_T('.') && str[len-1]==_T('.'))
		str.ReleaseBufferSetLength(len-3);
}

void CTranslate::FixTextTitle(CString& str) {
	int len = str.GetLength();
	for(int i=0;i<len;i++) {
		if(str[i]==_T(' ') || str[i]==_T('-') || str[i]==_T('=')) {
			str.Delete(i);
			str.SetAt(i,toupper(str[i]));
			i--;
			len--;
		} else if(str[i]>=128 || str[i]==_T('&') || str[i]==_T(' ') || str[i]==_T(':') || str[i]==_T('.') || str[i]==_T('\'') || str[i]==_T('(') || str[i]==_T(')') || str[i]==_T(',') || str[i]==_T('/') || str[i]==_T('%') || str[i]==_T('!') || str[i]==_T('?') || str[i]==_T('\n') || str[i]==_T('\r') || str[i]==_T('\t')) {
			str.Delete(i--);
			len--;
		}
	}
}

void CTranslate::Translate(HWND hWnd,const CString& strTitle) {
#if 1
	EnumChildProc(hWnd,(LPARAM)(LPCTSTR)strTitle);
#else
	CString strTrans;
	_GetProfileString(m_strTransFile,m_strSection,"0",strTrans);
	if(!strTrans.IsEmpty())
		m_wnd.SetWindowText(strTrans);
#endif

	::EnumChildWindows(hWnd,EnumChildProc,(LPARAM)(LPCTSTR)strTitle);
}

BOOL CALLBACK CTranslate::EnumChildProc(HWND hWnd,LPARAM lParam) {
	LPCTSTR pszTitle = (LPCTSTR)lParam;

	CString str;
	CWindow wnd(hWnd);
	wnd.GetWindowText(str);

	if(str.IsEmpty() || IgnoreWord(str)) 
		return TRUE;

	CString strKey, strTrans;
	strKey.Format("%s|%s",pszTitle,str);

	FixTextTitle(strKey);

	Warning(strKey,str);
	if(!m_map.Lookup(strKey,strTrans))
		return TRUE;

	strTrans.Replace("\\n","\n");
	strTrans.Replace("\\r","\r");
	strTrans.Replace("\\t","\t");
	wnd.SetWindowText(strTrans);
	return TRUE;
}

bool CTranslate::IgnoreWord(LPCTSTR pszWord) {
	static LPCTSTR pszIgnoreList[] = {
		"...", 
		"0", 
		"List1", 
		"Spin1", 
		"X.X.X",
		"http://www.istool.org/",
		"ISTool",
		"Bjørnar Henden",
		"Copyright © 1999 - 2004 Bjørnar Henden.",
		"http://www.innosetup.com/",
		"Inno Setup",
		"IWZ2ISS",
		NULL
	};
	LPCTSTR* pszWordList = pszIgnoreList;
	while(*pszWordList) {
		if(!_tcscmp(pszWord,*pszWordList))
			return true;
		pszWordList++;
	}

	return false;
}

LPCTSTR CTranslate::Translate(LPCTSTR pszDefault) {
	CString strKey(pszDefault);
	FixTextTitle(strKey);

	Warning(strKey,pszDefault);
	if(m_map.Lookup(strKey,CString()))
		return m_map[strKey];

	return pszDefault;
}

LPCTSTR CTranslate::Translate(LPCTSTR pszKey,LPCTSTR pszDefault) {
	CString strKey(pszKey);
	FixTextTitle(strKey);

	Warning(strKey,pszDefault);
	if(m_map.Lookup(strKey,CString()))
		return m_map[strKey];

	return pszDefault;
}
