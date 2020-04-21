// FileInfo.cpp: implementation of the CFileInfo class.
// (c) Codeguru & friends
// Coded by Antonio Tejada Lacaci. 1999
// atejada@espanet.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileInfo.h"

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define FILEBUFSIZE 4096

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileInfo::CFileInfo() {
   m_strFilePath = _T("");
   m_dwAttributes = 0xFFFFFFFF;
   m_uhFileSize = 0;
   m_lParam = 0;
}

CFileInfo::CFileInfo(const CFileInfo& finf) {
   m_strFilePath = finf.m_strFilePath;
   m_dwAttributes = finf.m_dwAttributes;
   m_uhFileSize = finf.m_uhFileSize;
   m_lParam = finf.m_lParam;
}

CFileInfo::~CFileInfo() {
}

void CFileInfo::Create(const WIN32_FIND_DATA* pwfd, const CString strFilePath, LPARAM lParam) {
   m_strFilePath = strFilePath + pwfd->cFileName;
   m_dwAttributes = pwfd->dwFileAttributes;
   m_uhFileSize = (((ULONGLONG) pwfd->nFileSizeHigh) << 32) + pwfd->nFileSizeLow;
   m_lParam = lParam;
}

void CFileInfo::Create(const CString strFilePath, LPARAM lParam) {
   WIN32_FIND_DATA wfd;
   HANDLE hFind;
   int nBarPos;

   hFind = FindFirstFile(strFilePath, &wfd);
   if (hFind != INVALID_HANDLE_VALUE) {
      nBarPos = strFilePath.ReverseFind(TCHAR('\\'));
      Create(&wfd, strFilePath.Left(strFilePath.GetLength()-nBarPos), lParam);
      FindClose(hFind);
   }
}

CString CFileInfo::GetFileDrive(void) const {
   TCHAR szDrive[_MAX_DRIVE];
   _tsplitpath(m_strFilePath, szDrive, NULL, NULL, NULL);
   return CString(szDrive);
}

CString CFileInfo::GetFileDir(void) const { 
   TCHAR szDir[_MAX_DIR];
   _tsplitpath(m_strFilePath, NULL, szDir,NULL, NULL);
   return CString(szDir);
}

CString CFileInfo::GetFileTitle(void) const {
   TCHAR szName[_MAX_FNAME];
   _tsplitpath(m_strFilePath, NULL, NULL, szName, NULL);
   return CString(szName);
}

CString CFileInfo::GetFileExt(void) const {
   TCHAR szExt[_MAX_EXT];

   _tsplitpath(m_strFilePath, NULL, NULL, NULL, szExt);
   return CString(szExt);
}

CFileInfoArray::CFileInfoArray() {}

int CFileInfoArray::AddFileInfo(CFileInfo& finf) {
   Add(finf);
   return GetSize();
}

int CFileInfoArray::AddDir(const CString strDirName, const CString strMask, BOOL bRecurse,
   BOOL bIncludeDirs, const volatile BOOL* pbAbort, 
   volatile ULONG* pulCount) { 

   WIN32_FIND_DATA wfd;
   HANDLE hFind;
   CString strText;
   CFileInfo finf;
   int nCount=0;

   strText = strDirName + _T("*");
   if (bRecurse) {
      // Iterate through dirs
      hFind = FindFirstFile(strText, &wfd);
      if (hFind != INVALID_HANDLE_VALUE) {
         do {
            if ((_tcscmp(wfd.cFileName,_T("."))) && (_tcscmp(wfd.cFileName,_T(".."))) && 
                (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
               strText = strDirName + wfd.cFileName;
               if (bIncludeDirs) {
                  // Add directory itself
                  finf.Create(&wfd, strDirName);
                  AddFileInfo(finf);
                  nCount++;
               }
               // Recurse
               nCount += AddDir(strText+_T("\\"), strMask, bRecurse, 
                  bIncludeDirs, pbAbort, pulCount);
            }
         } while (((pbAbort == NULL) || (!*pbAbort)) && (FindNextFile(hFind, &wfd)));
         FindClose(hFind);
      }
   }

   // Iterate through files
   if ((pbAbort == NULL) || (!*pbAbort)) {
      strText = strDirName + strMask;
      hFind = FindFirstFile(strText, &wfd);
      if (hFind != INVALID_HANDLE_VALUE) {
         do {
            if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
               strText = strDirName + wfd.cFileName;
               finf.Create(&wfd, strDirName);
               AddFileInfo(finf);
               nCount++;
               if (pulCount) InterlockedIncrement((long*)pulCount);
            }
         } while (((pbAbort== NULL) || (!*pbAbort)) && (FindNextFile(hFind, &wfd)));
         FindClose(hFind);
      }
   }
   return nCount;
}

int CFileInfoArray::AddFile(const CString strFilePath, LPARAM lAddParam) {
   CFileInfo finf;

   finf.Create(strFilePath);
   return AddFileInfo(finf);
}
