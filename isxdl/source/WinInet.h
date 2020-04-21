#pragma once

class CWinInet {
protected:
	HMODULE m_hModule;
	typedef BOOL(WINAPI*tInternetGetConnectedState)(LPDWORD,DWORD);
	typedef BOOL(WINAPI*tInternetCloseHandle)(HINTERNET);
	typedef INTERNET_STATUS_CALLBACK(WINAPI*tInternetSetStatusCallback)(HINTERNET,INTERNET_STATUS_CALLBACK);
	typedef BOOL(WINAPI*tHttpSendRequest)(HINTERNET,LPCTSTR,DWORD,LPVOID,DWORD);
	typedef BOOL(WINAPI*tHttpQueryInfo)(HINTERNET,DWORD,LPVOID,LPDWORD,LPDWORD);	
	typedef HINTERNET(WINAPI*tFtpOpenFile)(HINTERNET,LPCTSTR,DWORD,DWORD,DWORD_PTR);
	typedef HINTERNET(WINAPI*tHttpOpenRequest)(HINTERNET,LPCTSTR,LPCTSTR,LPCTSTR,LPCTSTR,LPCTSTR*,DWORD,DWORD_PTR);
	typedef HINTERNET(WINAPI*tInternetConnect)(HINTERNET,LPCTSTR,INTERNET_PORT,LPCTSTR,LPCTSTR,DWORD,DWORD,DWORD_PTR);
	typedef DWORD(WINAPI*tInternetErrorDlg)(HWND,HINTERNET,DWORD,DWORD,LPVOID*);
	typedef BOOL(WINAPI*tInternetReadFile)(HINTERNET,LPVOID,DWORD,LPDWORD);
	typedef BOOL(WINAPI*tInternetCrackUrl)(LPCTSTR,DWORD,DWORD,LPURL_COMPONENTS);
	typedef HINTERNET(WINAPI*tInternetOpen)(LPCTSTR,DWORD,LPCTSTR,LPCTSTR,DWORD);
	typedef BOOL(WINAPI*tInternetSetOption)(HINTERNET,DWORD,LPVOID,DWORD);
	typedef HINTERNET(WINAPI*tFtpFindFirstFile)(HINTERNET,LPCTSTR,WIN32_FIND_DATA*,DWORD,DWORD_PTR);
	typedef BOOL(WINAPI*tInternetGetLastResponseInfo)(LPDWORD,LPTSTR,LPDWORD);
	typedef BOOL(WINAPI*tFtpCommand)(HINTERNET,BOOL,DWORD,LPCTSTR,DWORD_PTR,HINTERNET*);

public:
	CWinInet();
	virtual ~CWinInet();

	tInternetGetConnectedState		InternetGetConnectedState;
	tInternetCloseHandle			InternetCloseHandle;
	tInternetSetStatusCallback		InternetSetStatusCallback;
	tHttpSendRequest				HttpSendRequest;
	tHttpQueryInfo					HttpQueryInfo;
	tFtpOpenFile					FtpOpenFile;
	tHttpOpenRequest				HttpOpenRequest;
	tInternetConnect				InternetConnect;
	tInternetErrorDlg				InternetErrorDlg;
	tInternetReadFile				InternetReadFile;
	tInternetCrackUrl				InternetCrackUrl;
	tInternetOpen					InternetOpen;
	tInternetSetOption				InternetSetOption;
	tFtpFindFirstFile				FtpFindFirstFile;
	tInternetGetLastResponseInfo	InternetGetLastResponseInfo;
	tFtpCommand						FtpCommand;
};

extern CWinInet inet;
extern ULONGLONG myatol(LPCTSTR psz);
extern int mystricmp(LPCTSTR,LPCTSTR);