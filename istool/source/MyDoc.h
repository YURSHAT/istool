// MyDoc.h : interface of the CMyDoc class

#pragma once

#include "InnoScriptEx.h"
#include "Update.h"

class CMyDoc : public CUpdate {
	friend class CMainFrame;
	friend CMyDoc* AfxGetDocument();
public:
	CMyDoc();
	virtual ~CMyDoc();

// Operations
public:
	void SetModifiedFlag(bool bModified=true);
	bool IsModified() { return m_bModified; }
	const CString& GetPathName();
	void SetPathName(const CString& strPathName);
	bool SaveModified(HWND hWnd);
	bool DoFileSave(HWND hWnd);
	bool DoSave(HWND hWnd,LPCTSTR lpszPathName, bool bReplace=true);
	bool DoPrompt(HWND hWnd,CString& newName,bool bOpen,UINT nID);

	bool			GetSourceDir(CString& rStr);
	void			GetScriptFileName(CString& ref,LPCTSTR pszFileName);
	bool			GetOutputExe(CInnoScriptEx* pScript,HWND hWnd,CString& rStr,bool bNoWarning=false);
	bool			RunCompileSteps(CInnoScript::SECTION sec);
	bool			GetCompilerPath(CString& str);
	CInnoScriptEx&	GetScript() { ATLASSERT(m_pScript); return *m_pScript; }
	bool			GetUseAbsolutePaths();
	CInnoScriptEx*	GetMessages() { return m_pMessages; }
	bool			IsValid() { return m_pScript!=NULL; }

	void RenameDir(LPCTSTR,LPCTSTR);
	void RenameFile(HWND hWnd,LPCTSTR,LPCTSTR);
	void AddDirs(CComboBox&);
	void AddDirConstants(CComboBox&);
	void AddFiles(CComboBox&);
	void AddIniFilenames(CComboBox&);
	void AddIniSections(CComboBox&);
	void AddRegistrySubkeys(CComboBox&);
	void AddFilesExeFiles(CComboBox&);
	void AddValidMessages(HWND hWnd,CComboBox&);
	void AddFileIfDoesntExist(CScriptLine* pLine);
	// Preference functions
	BOOL GetCompiler(CString& str,bool bDLL=false,bool bDLS=false);
	BOOL GetMessageFile(CString& str);
	bool RunISWizard();
	void GetOutputFolder(CString& rStr);
	void SetCurrentDir();

	virtual BOOL OnNewDocument(HWND hWnd,bool bHideWizard);
	virtual BOOL OnOpenDocument(HWND hWnd,LPCTSTR lpszPathName);
	virtual BOOL OnMergeDocument(HWND hWnd,LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(HWND hWnd,LPCTSTR lpszPathName);

// Attributes
private:
	void ClearScript();
	void OpenInnoSetup(HWND hWnd);
	virtual void OnUpdate(LONG lHint,void* pParam);

	// Functions in MyDoc_imports.cpp
	void VBImportFileSection(CIniFile& file,LPCTSTR lpszPathName);
	void VBImportIconSection(CIniFile& file,UINT nGroup);
	bool VBReplaceFileConstant(CString& str,bool bPrivate);
	bool VBLoadFile(LPCTSTR lpszPathName);
	bool VBIsVBFile(LPCTSTR pszPathName);
	bool iwz_IsIWZFile(LPCTSTR lpszPathName);
	bool iwz_LoadFile(LPCTSTR lpszPathName);

	CInnoScriptEx*	m_pScript;
	CInnoScriptEx*	m_pMessages;
	bool			m_bModified;
	CString			m_strPathName;

public:
	long			m_nScriptLine;
	bool			m_bCompileAndExit;
};
