/**
** @file
** @brief File and directory utilities
*/

#pragma once

namespace Henden {

/**
** @brief Class to encapsulate WIN32_FIND_DATA
*/
class CFindData : protected WIN32_FIND_DATA {
public:
	/// Returns a pointer to the WIN32_FIND_DATA
	operator WIN32_FIND_DATA*() {
		return this;
	}

	/// Returns true if this is a directory called "." or ".."
	bool IsDots() const {
		return IsDirectory() && cFileName[0] == '.' && (cFileName[1] == '\0' || (cFileName[1] == '.' && cFileName[2] == '\0'));
	}

	/// Returns true if this is a directory
	bool IsDirectory() const {
		return (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
	}

	/// Returns true if the file is hidden
	bool IsHidden() const {
		return (dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
	}

	/// Returns the size of the file as a 64-bit integer
	ULONGLONG GetFileSize() const {
		ULARGE_INTEGER nFileSize = { 0 };

		nFileSize.LowPart = nFileSizeLow;
		nFileSize.HighPart = nFileSizeHigh;
		return nFileSize.QuadPart;
	}

	/// Returns only the file name
	CString GetFileName() const {
		return cFileName;
	}

	/// Returns the full path name including file name
	CString GetFilePath() const {
		CString strResult = m_strRoot;
		if(strResult[strResult.GetLength() - 1] != '\\' && strResult[strResult.GetLength() - 1] != '/')
			strResult += _T('\\');
		strResult += GetFileName();
		return strResult;
	}

	/// Constructor
	CFindData(LPCTSTR pszRoot) : m_strRoot(pszRoot) {
	}
private:
	const CString m_strRoot;	/// Holds the root directory
};

/// A list of files and/or directories.
typedef CSimpleArray<CFindData> CDirList;

/**
** @brief Utility functions
**
** @sa CDirList
*/
class CUtils {
public:
#ifdef __CSTRINGT_H__
	/// Makes sure a string ends with a certain character
	static void EndWith(CString& ref,TCHAR nChar) {
		int nLength = (int)_tcslen(ref);
		if(!nLength || ref[nLength-1]!=nChar)
			ref += _T('\\');
	}

	/// Lists all files in the given directory
	static CDirList ListDir(CString strDir) {
		EndWith(strDir,_T('\\'));
		CDirList dirList;

		CFindData wfd(strDir);
		HANDLE hFind = ::FindFirstFile(strDir+ _T("*.*"),wfd);
		if(hFind!=INVALID_HANDLE_VALUE) {
			do {
				if(wfd.IsDirectory() && !wfd.IsDots())
					dirList.Add(wfd);
			} while(::FindNextFile(hFind,wfd));
			::FindClose(hFind);
		}
		
		hFind = ::FindFirstFile(strDir+ _T("*.*"),wfd);
		if(hFind!=INVALID_HANDLE_VALUE) {
			do {
				if(!wfd.IsDirectory())
					dirList.Add(wfd);
			} while(::FindNextFile(hFind,wfd));
			::FindClose(hFind);
		}
		return dirList;
	}

	/// Lists all directories in the given directory
	static CDirList ListDirs(CString strDir) {
		CDirList dirList;

		CFindData wfd(strDir);
		HANDLE hFind = ::FindFirstFile(strDir+ _T("*.*"),wfd);
		if(hFind!=INVALID_HANDLE_VALUE) {
			do {
				if(wfd.IsDirectory() && !wfd.IsDots())
					dirList.Add(wfd);
			} while(::FindNextFile(hFind,wfd));
			::FindClose(hFind);
		}
		return dirList;
	}

	/**
	** @brief List files and possibly directories
	** @param dirList The list that will contain the added files/dirs
	** @param strMatch The path and wildcard mask of the files to find
	** @param bRecursive True if files should be added recursively
	** @param bDirsAlso If this is true, directories will be added to the list also
	*/
	/// List all files and possible directories
	static bool ListFiles(CDirList& dirList,CString strMatch,bool bRecursive,bool bDirsAlso) {
		bool bRet = false;
		int nRootLen = strMatch.ReverseFind(_T('\\'));
		CFindData wfd(strMatch.Left(nRootLen+1));

		HANDLE hFind = ::FindFirstFile(strMatch,wfd);
		if(hFind!=INVALID_HANDLE_VALUE) {
			do {
				if(!wfd.IsDots()) {
					if(!wfd.IsDirectory() || bDirsAlso)
						dirList.Add(wfd);
					if(wfd.IsDirectory() && bRecursive)
						ListFiles(dirList,wfd.GetFilePath() + _T("\\*.*"),bRecursive,bDirsAlso);
				}
			} while(::FindNextFile(hFind,wfd));
			bRet = ::GetLastError()==ERROR_NO_MORE_FILES;
			::FindClose(hFind);
		}
		return bRet;
	}
#endif
	/// Returns true if the given path name is a directory
	static bool IsDirectory(LPCTSTR pszPathName) {
#if 1
		DWORD dw = GetFileAttributes(pszPathName);
		return dw!=INVALID_FILE_ATTRIBUTES && (dw & FILE_ATTRIBUTE_DIRECTORY);
#else
		bool bIsDirectory = false;
		CFindData wfd(pszPathName);
		HANDLE hFind = ::FindFirstFile(pszPathName,wfd);
		if(hFind!=INVALID_HANDLE_VALUE) {
			bIsDirectory = wfd.IsDirectory();
			::FindClose(hFind);
		}
		return bIsDirectory;
#endif
	}

	/*----------------------------------------------------------------------
	Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
	File:	CreateDirectoryPath.cpp
	Owner:	russf@gipsysoft.com
	Purpose:	Create a dfirectory path, that is given "c:\blah\de\blah" it
						will create the entire path even if none of the parent directories
						exist.
						Also, it copes with UNC path names in the form of "\\Russ\C\blah\de\blah"
	BH 2004-05-23	Modified to not use CRT code
	----------------------------------------------------------------------*/
	/// Creates a full directory path
	static bool CreateDirectoryPath(LPCTSTR pcszDirectory) {
		//	Must be passed a valid string!
		//ASSERT_VALID_STR( pcszDirectory );

		static TCHAR cSlash = _T('\\');

		bool bRetVal = false;

		CString strAlloc(pcszDirectory);
		const int nLength = (int)_tcslen( pcszDirectory ) + 1;
		LPTSTR pszDirectoryPath = strAlloc.GetBuffer();
		if( pszDirectoryPath )
		{

			LPCTSTR pcszNextDirectory = pcszDirectory;

			//
			//	Determine if the path is a UNC path. We do this by looking at the first two bytes
			//	and checkin they are both backslashes
			if( nLength > 2 && *pcszNextDirectory == cSlash && *(pcszNextDirectory+1) == cSlash )
			{
				//	We need to skip passed this bit and copy it into out local path.
				//	"\\Russ\C\"
				pcszNextDirectory += 2;
				while( *pcszNextDirectory && *pcszNextDirectory != cSlash )	pcszNextDirectory++;
				pcszNextDirectory++;
				while( *pcszNextDirectory && *pcszNextDirectory != cSlash )	pcszNextDirectory++;
				_tcsncpy( pszDirectoryPath, pcszDirectory, pcszNextDirectory - pcszDirectory );
				pszDirectoryPath[ pcszNextDirectory - pcszDirectory ] = '\000';
			}

			//
			//	Set the return value to true because the nly thing that can fail now is the
			//	CreateDirectory. If that fails then we change the return value back to fals.
			bRetVal = true;

			//
			//	Now, loop over the path, creating directories as we go. If we fail at any point then get out of the loop
			do {
				if( *pcszNextDirectory )
					pcszNextDirectory++;

				while( *pcszNextDirectory && *pcszNextDirectory != cSlash && *pcszNextDirectory!='/')
					pcszNextDirectory++;

				_tcsncpy( pszDirectoryPath, pcszDirectory, pcszNextDirectory - pcszDirectory );
				pszDirectoryPath[ pcszNextDirectory - pcszDirectory ] = '\000';

				if(!IsDirectory(pszDirectoryPath)) {
					if(!CreateDirectory(pszDirectoryPath,NULL)) {
						bRetVal = false;
						break;
					}
				}
			} while( *pcszNextDirectory );
		}
		return bRetVal;
	}
};

} // namespace Henden
