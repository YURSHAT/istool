// FilesHelper.h: interface for the CFilesHelper class.

#pragma once

class CMyDoc;

class CFilesHelper {
public:
	CFilesHelper(CMyDoc*);
	virtual ~CFilesHelper();

	void OnDropFiles(HWND hWnd,HDROP hDropInfo,LPCTSTR lpszCurrentFolder);
	void OnDropFilesRegistry(HWND hWnd,HDROP hDropInfo);
	void InsertFileName(LPCTSTR lpszFileName,LPCTSTR lpszCurrentFolder);
	void CreateIcon(HWND hWnd,CScriptLine*);
	void OnFileAddFiles(LPCTSTR);
	bool ImportRegistry(HWND hWnd,LPCTSTR lpszRegFile);
	bool CreateODBCProfile();
	bool ImportIni(HWND hWnd,LPCTSTR pszPathName);
	void AutoComponentSelect(CScriptList& list);
	void AutoComponentSelect(CScriptLine* pLine);
	void RenameComponent(LPCTSTR pszFrom,LPCTSTR pszTo);
	void RenameType(LPCTSTR pszFrom,LPCTSTR pszTo);
	void RenameTask(LPCTSTR pszFrom,LPCTSTR pszTo);
protected:
	CMyDoc*	m_pDoc;
};
