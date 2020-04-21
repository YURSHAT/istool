// Func.h: interface for the CFunc class.

#pragma once

class CFunc {
public:
	static CString ExtractRelativePath(LPCTSTR pszBaseName,LPCTSTR pszDestName);
	static UINT GetDriveLength(LPCTSTR pszPath);
	static int SplitDirs(LPCTSTR pszPath,int* nLengths);
};
