#include "StdAfx.h"
#include "wininet.h"

CWinInet::CWinInet() {
	// Init
	m_hModule = ::LoadLibrary(_T("wininet.dll"));
	if(!m_hModule) return;
	InternetGetConnectedState = (tInternetGetConnectedState)GetProcAddress(m_hModule,"InternetGetConnectedState");
	InternetCloseHandle = (tInternetCloseHandle)GetProcAddress(m_hModule,"InternetCloseHandle");
	InternetSetStatusCallback = (tInternetSetStatusCallback)GetProcAddress(m_hModule,"InternetSetStatusCallback");
	HttpSendRequest = (tHttpSendRequest)GetProcAddress(m_hModule,"HttpSendRequestA");
	HttpQueryInfo = (tHttpQueryInfo)GetProcAddress(m_hModule,"HttpQueryInfoA");
	FtpOpenFile = (tFtpOpenFile)GetProcAddress(m_hModule,"FtpOpenFileA");
	HttpOpenRequest = (tHttpOpenRequest)GetProcAddress(m_hModule,"HttpOpenRequestA");
	InternetConnect = (tInternetConnect)GetProcAddress(m_hModule,"InternetConnectA");
	InternetErrorDlg = (tInternetErrorDlg)GetProcAddress(m_hModule,"InternetErrorDlg");
	InternetReadFile = (tInternetReadFile)GetProcAddress(m_hModule,"InternetReadFile");
	InternetCrackUrl = (tInternetCrackUrl)GetProcAddress(m_hModule,"InternetCrackUrlA");
	InternetOpen = (tInternetOpen)GetProcAddress(m_hModule,"InternetOpenA");
	InternetSetOption = (tInternetSetOption)GetProcAddress(m_hModule,"InternetSetOptionA");
	FtpFindFirstFile = (tFtpFindFirstFile)GetProcAddress(m_hModule,"FtpFindFirstFileA");
	InternetGetLastResponseInfo = (tInternetGetLastResponseInfo)GetProcAddress(m_hModule,"InternetGetLastResponseInfoA");
	FtpCommand = (tFtpCommand)GetProcAddress(m_hModule,"FtpCommandA");
}

CWinInet::~CWinInet() {
	// Cleanup
	if(m_hModule) FreeLibrary(m_hModule);
}
