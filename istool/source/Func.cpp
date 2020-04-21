// Func.cpp: implementation of the CFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "istool.h"
#include "Func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UINT CFunc::GetDriveLength(LPCTSTR pszPath) {
	UINT nLength = _tcslen(pszPath);

	if(nLength>=2 && pszPath[1]==':') {
		return 2;
	} else if(nLength>=2 && pszPath[0]=='\\' && pszPath[1]=='\\') {
		nLength = 2;
		while(pszPath[nLength] && pszPath[nLength]!='\\')
			nLength++;
		if(!pszPath[nLength])
			return 0;
		else
			return nLength;
	}
	return 0;
}

int CFunc::SplitDirs(LPCTSTR pszPath,int* nLengths) {
	int nDirs = 0;
	int nLength = 0;

	while(*pszPath) {
		if(*pszPath=='\\' || *pszPath=='/') {
			nLengths[nDirs++] = nLength;
			nLength = 0;
		} else
			nLength++;
		pszPath++;
	}
	if(nLength) {
		nLengths[nDirs++] = nLength;
	}
	return nDirs;
}

CString CFunc::ExtractRelativePath(LPCTSTR pszBaseName,LPCTSTR pszDestName) {
	UINT nLength = GetDriveLength(pszBaseName);
	
	if(!nLength) {
		return "";
	} else if(_tcsnicmp(pszBaseName,pszDestName,nLength)) {
		return pszDestName;
	} else {
		pszBaseName += nLength;
		pszDestName += nLength;

		int nBaseLengths[100], nDestLengths[100];
		int nBaseDirCount = SplitDirs(pszBaseName,nBaseLengths);
		int nDestDirCount = SplitDirs(pszDestName,nDestLengths);

		int i = 0;
		while(i<nBaseDirCount && i<nDestDirCount) {
			int nLength = nBaseLengths[i];
			if(nLength==nDestLengths[i] && !_tcsnicmp(pszBaseName,pszDestName,nLength)) {
				pszBaseName += nBaseLengths[i] + 1;
				pszDestName += nDestLengths[i] + 1;
				i++;
			} else
				break;
		}

		CString strResult;
		while(i++<nBaseDirCount) strResult += "..\\";
		strResult += pszDestName;

		return strResult;
	}
}
