#pragma once

#include "WinInet.h"

// ---------------------------------------------------------
typedef struct {
	TCHAR			m_szURL[MAX_PATH];
	TCHAR			m_szFile[MAX_PATH];
	TCHAR			m_szName[MAX_PATH];		// The "real" name of the file
	ULONGLONG		m_uLength;
	bool			m_bDone;
} CFileInfo;

typedef struct {
	CString			m_strUserName;
	CString			m_strPassword;
} CUserPass;

enum {
	UWM_THREADDONE = WM_USER+0x100,
	UWM_ERRORDLG,
	UWM_MSGBOX,
	UWM_POSFILE,
	UWM_POSALL,
	UWM_TEXT,
	UWM_USERPASS
};

// ---------------------------------------------------------
class CDownload {
	friend class CSplitURL;
public:
	CDownload();
	virtual ~CDownload();
public:
	typedef CSimpleArray<CFileInfo*>	CFileArray;
	static CFileArray					m_files;
	bool								m_bAllOk;

	// Options
	static CString						m_strOptionTitle;
	static bool							m_bOptionDebug;
	static CString						m_strOptionSimple;
	static CString						m_strOptionLabel;
	static CString						m_strOptionDescription;
	static CString						m_strOptionLanguage;
#ifdef USE_RESUME
	static bool							m_bOptionResume;
#endif
	static CString						m_strSmallWizardImage;
protected:
	static HINTERNET			m_hInternet;
	static LPCTSTR m_ppszAcceptTypes[2];
	static Henden::CCriticalSection	m_cs;
	INTERNET_STATUS_CALLBACK	m_iscCallback;
	HINTERNET					m_hConn, m_hFile;
	CFileInfo*					m_pCurrentFile;
	HWND						m_hWndServer;
	UINT						m_nFileCount;
	UINT						m_nCount;
	bool						m_bAskRetry;
	volatile bool				m_bAbort;
	volatile bool				m_bErrorDlgResult;
	volatile int				m_nMsgBoxResult;
	DWORD						m_dwRunStart;
	CString						m_strCurrentFile;
	CString						m_strError;
	UINT						m_nTotalBytesDownloaded;

	volatile HANDLE				m_hEventWait;
	HANDLE						m_hThread;


	static void __stdcall CallMaster(HINTERNET, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD);
	void StatusCallback(DWORD dwInternetStatus,LPVOID lpvStatusInformation);
	void SetFileInfo(LPCTSTR pszURL,CFileInfo* m_pFileInfo);

	void SetServer(HWND hWndServer);

	static ULONGLONG GetTotalSize();
	void Reset();
	void SetGlobalOffline(BOOL fGoOffline);

	bool OpenInternet();
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	DWORD ThreadProc();
	bool Run();
	bool DoOperation(CAtlFile& file,ULONGLONG dwLength,ULONGLONG& dwTotal,ULONGLONG& dwTotalSize,CFileInfo& fileInfo,ULONGLONG nResumePos=0);
	bool Abort();
	void GetInetError(CString& ref,DWORD dwLastError=GetLastError());
	static bool GetSysError(CString& ref,DWORD dwError=GetLastError(),LPCTSTR pszModule=NULL);
	ULONGLONG GetURLLength(HINTERNET m_hInternet,LPCTSTR pszURL);
	void PostTxtMsg(UINT uMsg,UINT uID,CString& str);

	virtual bool IsSimple() = 0;

public:
	static UINT GetFileCount();
	static bool IsConnected();
	static void AddFile(LPCTSTR pszURL,LPCTSTR pszFile,DWORD dwSize=0);
	static void ClearFiles();
	static bool GetString(UINT uID,CString& str);
};
