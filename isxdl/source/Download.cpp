#include "StdAfx.h"
#include "resource.h"
#include "Download.h"
#include "SplitURL.h"

#define MYSIZE(a)	DWORD((a)>>10)

class CStringEx : public CString {
public:
	inline void FormatNum(ULONGLONG n) {
		Format(_T("%I64d"),n);
		TCHAR buf[128], sDummy[16], sDecSep[16], sThowSep[16];

		LCID loc = LOCALE_USER_DEFAULT;
		//LCID loc = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);

		NUMBERFMT nf;
		nf.NumDigits = 0/*nDecimals*/;
		GetLocaleInfo( loc, LOCALE_ILZERO, sDummy, 16 );
		nf.LeadingZero = (UINT)myatol(sDummy);
		GetLocaleInfo( loc, LOCALE_SGROUPING, sDummy, 16 );
		nf.Grouping = (UINT)myatol(sDummy);
		GetLocaleInfo( loc, LOCALE_SDECIMAL, sDecSep, 16 );
		nf.lpDecimalSep = sDecSep;
		GetLocaleInfo( loc, LOCALE_STHOUSAND, sThowSep, 16 );
		nf.lpThousandSep = sThowSep;
		GetLocaleInfo( loc, LOCALE_INEGNUMBER, sDummy, 16 );
		nf.NegativeOrder = (UINT)myatol(sDummy);

		GetNumberFormat(loc,0,*this,&nf,buf,sizeof buf);
		//GetNumberFormat(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),0,tmp,NULL,buf,32);
		Format(_T("%s"),buf);
	}
};

HINTERNET					CDownload::m_hInternet;
CDownload::CFileArray		CDownload::m_files;
Henden::CCriticalSection	CDownload::m_cs;

LPCTSTR CDownload::m_ppszAcceptTypes[] = { _T("*/*"),NULL };

CDownload::CDownload() {
	m_hEventWait = CreateEvent(NULL,TRUE,FALSE,NULL);

	m_hThread = NULL;
	m_hFile = NULL;
	m_iscCallback = NULL;
	Reset();
}

CDownload::~CDownload() {
	if(m_hFile) inet.InternetCloseHandle(m_hFile);
	if(m_hConn) inet.InternetCloseHandle(m_hConn);
	if(m_hInternet) {
		inet.InternetSetStatusCallback(m_hInternet,m_iscCallback);
		m_iscCallback = NULL;
	}
	CloseHandle(m_hEventWait);
	Reset();
}

void CDownload::SetServer(HWND hWndServer) {
	m_hWndServer = hWndServer;
}

void __stdcall CDownload::CallMaster(HINTERNET, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD) {
	if(dwContext)
		reinterpret_cast<CDownload*>(dwContext)->StatusCallback(dwInternetStatus,lpvStatusInformation);
}

void CDownload::StatusCallback(DWORD dwInternetStatus,LPVOID lpvStatusInformation) {
	CString str;
	switch(dwInternetStatus) {
	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		GetString(IDS_STATUS_CONNECTED_TO_SERVER,str);
		str.Replace(_T("%1"),(LPCTSTR)lpvStatusInformation);
		break;
	case INTERNET_STATUS_REQUEST_SENT:
		GetString(IDS_STATUS_RECEIVING,str);
		break;
	case INTERNET_STATUS_SENDING_REQUEST:
		GetString(IDS_STATUS_SENDING_REQUEST,str);
		break;
	case INTERNET_STATUS_RESOLVING_NAME:
		GetString(IDS_STATUS_RESOLVING_NAME,str);
		str.Replace(_T("%1"),(LPCTSTR)lpvStatusInformation);
		break;
	case INTERNET_STATUS_REDIRECT:
		SetFileInfo((LPCTSTR)lpvStatusInformation,m_pCurrentFile);
		GetString(IDS_STATUS_REDIRECT,str);
		str.Replace(_T("%1"),(LPCTSTR)lpvStatusInformation);
		break;
	case INTERNET_STATUS_CONNECTING_TO_SERVER:
		GetString(IDS_STATUS_CONNECTING,str);
		str.Replace(_T("%1"),(LPCTSTR)lpvStatusInformation);
		break;
	default:
		return;
	}

	PostTxtMsg(UWM_TEXT,IDC_STATUS,str);
}

void CDownload::SetFileInfo(LPCTSTR pszURL,CFileInfo* m_pFileInfo) {
	CString tmp;
	CString str, strFilePart(pszURL);
	int nPos = strFilePart.ReverseFind('/');
	if(nPos>=0) strFilePart = strFilePart.Mid(nPos+1);

	GetString(IDS_FILECOUNT,str);
	str.Replace(_T("%1"),strFilePart);
	tmp.Format(_T("%d"),m_nCount);
	str.Replace(_T("%2"),tmp);
	tmp.Format(_T("%d"),m_nFileCount);
	str.Replace(_T("%3"),tmp);
	_tcscpy(m_pFileInfo->m_szName,strFilePart);

	PostTxtMsg(UWM_TEXT,IDC_FILE,str);
}

bool CDownload::IsConnected() {
	// If wininet.dll is too old, assume we are connected
	if(!inet.InternetGetConnectedState) return true;

	DWORD dwFlags = 0;
	if(!inet.InternetGetConnectedState(&dwFlags,0))
		return false;

	return (dwFlags & (INTERNET_CONNECTION_LAN|INTERNET_CONNECTION_MODEM)) ? true : false;
}

void CDownload::AddFile(LPCTSTR pszURL,LPCTSTR pszFile,DWORD dwSize/*=0*/) {
	// Check if this file already is listed
	for(int i=0;i<m_files.GetSize();i++) {
		if(!mystricmp(m_files[i]->m_szURL,pszURL) && !mystricmp(m_files[i]->m_szFile,pszFile))
			// Already there
			return;
	}

	CFileInfo* fileInfo = new CFileInfo();
	ZeroMemory(fileInfo,sizeof CFileInfo);
	_tcscpy(fileInfo->m_szURL,pszURL);
	_tcscpy(fileInfo->m_szFile,pszFile);
	fileInfo->m_uLength = dwSize;
	m_files.Add(fileInfo);
}

void CDownload::ClearFiles() {
	for(int i=0;i<m_files.GetSize();i++) delete m_files[i];
	m_files.RemoveAll();
}

UINT CDownload::GetFileCount() {
	UINT nCount = 0;
	for(int n=0;n<m_files.GetSize();n++)
		if(!m_files[n]->m_bDone)
			nCount++;

	return nCount;
}

ULONGLONG CDownload::GetTotalSize() {
	ULONGLONG uTotalSize = 0;
	UINT nCount = m_files.GetSize();
	while(nCount--) {
		if(m_files[nCount]->m_uLength==(ULONGLONG)-1 || m_files[nCount]->m_uLength==0)
			return -1;
		uTotalSize += m_files[nCount]->m_uLength;
	}
	return uTotalSize;
}

void CDownload::Reset() {
	Henden::CSingleLock lock(m_cs,true);
	if(m_hConn) {
		inet.InternetCloseHandle(m_hConn);
		m_hConn = NULL;
	}
	if(m_hFile) {
		inet.InternetCloseHandle(m_hFile);
		m_hFile = NULL;
	}
	m_bAskRetry = true;
	m_bAbort = false;
	m_bAllOk = false;
}

// ms-help://MS.VSCC/MS.MSDNVS/ICom/workshop/components/offline/offline.htm
void CDownload::SetGlobalOffline(BOOL fGoOffline) {
	INTERNET_CONNECTED_INFO ci;

	memset(&ci, 0, sizeof(ci));
	if(fGoOffline) {
		ci.dwConnectedState = INTERNET_STATE_DISCONNECTED_BY_USER;
		ci.dwFlags = ISO_FORCE_DISCONNECTED;
	} else {
		ci.dwConnectedState = INTERNET_STATE_CONNECTED;
	}

	inet.InternetSetOption(NULL, INTERNET_OPTION_CONNECTED_STATE, &ci, sizeof(ci));
}

DWORD WINAPI CDownload::ThreadProc(LPVOID lpParameter) {
	CDownload* pDlg = reinterpret_cast<CDownload*>(lpParameter);
	return pDlg->ThreadProc();
}

DWORD CDownload::ThreadProc() {
	do {
		Reset();
		m_bAllOk = Run();
		if(m_bAllOk || m_bAbort) break;

		DWORD dwFlags = 0;
		if(m_bAskRetry)
			dwFlags = MB_ICONERROR | MB_RETRYCANCEL;
		else
			dwFlags = MB_ICONERROR | MB_OK;

		ResetEvent(m_hEventWait);
		PostMessage(m_hWndServer,UWM_MSGBOX,dwFlags,0);
		WaitForSingleObject(m_hEventWait,INFINITE);
		if(m_nMsgBoxResult!=IDRETRY) break;
	} while(true);
	PostMessage(m_hWndServer,UWM_THREADDONE,0,0);
	return 0;
}

void CDownload::GetInetError(CString& ref,DWORD dwLastError/*=GetLastError()*/) {
	if(dwLastError==ERROR_INTERNET_EXTENDED_ERROR) {
		DWORD dwError, dwChars = 0;
		inet.InternetGetLastResponseInfo(&dwError,NULL,&dwChars);
		inet.InternetGetLastResponseInfo(&dwError,ref.GetBuffer(++dwChars),&dwChars);
		ref.ReleaseBufferSetLength(dwChars);
	} else {
		GetSysError(ref,dwLastError,_T("wininet.dll"));
	}
}

bool CDownload::Run() {
	Henden::CSingleLock lock(m_cs,false);
	CString			str;

	if(!OpenInternet()) return false;

	m_nTotalBytesDownloaded = 0;
	m_nFileCount = 0;
	GetString(IDS_STATUS_FILEINFO,str);
	PostTxtMsg(UWM_TEXT,IDC_STATUS,str);

	ULONGLONG uTotalSize = 0;
	for(int i=0;i<m_files.GetSize();i++) {
		if(!m_files[i]->m_bDone) {
			m_nFileCount++;
			ULONGLONG uSize;
			if(m_files[i]->m_uLength) uSize = m_files[i]->m_uLength;
			else uSize = GetURLLength(m_hInternet,m_files[i]->m_szURL);
			if(uSize && uSize!=(ULONGLONG)-1) m_files[i]->m_uLength = uSize;
			if(uTotalSize==(ULONGLONG)-1 || uSize==(ULONGLONG)-1)
				uTotalSize = -1;
			else
				uTotalSize += uSize;
		}
		if(Abort()) return false;
	}

	if(uTotalSize!=(ULONGLONG)-1) ::PostMessage(m_hWndServer,UWM_POSALL,0,MYSIZE(uTotalSize));
	m_dwRunStart = GetTickCount();

	m_nCount = 0;
	ULONGLONG uTotal = 0;
	for(int i=0;i<m_files.GetSize();i++) {
		CFileInfo& fileInfo = *m_files[i];
		if(fileInfo.m_bDone) continue;

		m_pCurrentFile = m_files[i];
		m_strCurrentFile = fileInfo.m_szFile;
		++m_nCount;
		CString strURL(fileInfo.m_szURL);

		CAtlFile file;
#ifdef USE_RESUME
		LPCTSTR pszHeaders = NULL;
		CString strRangeHeader;
		ULONGLONG nResumePos = 0;
		if(m_bOptionResume && file.Create(fileInfo.m_szFile,GENERIC_WRITE,FILE_SHARE_READ,OPEN_EXISTING)==S_OK) {
			ULONGLONG nSize;
			if(file.GetSize(nSize)!=S_OK) {
				CString str;
				GetSysError(str,GetLastError(),_T("kernel32.dll"));
				GetString(IDS_ERROR_FILEOPEN,m_strError);
				m_strError.Replace(_T("%1"),fileInfo.m_szFile);
				m_strError.Replace(_T("%2"),str);
				return false;
			}
			nResumePos = (ULONG)nSize;
			strRangeHeader.Format(_T("Range: bytes=%d-"),nResumePos);
			pszHeaders = strRangeHeader;
			if(file.Seek(0,FILE_END)!=S_OK) {
				CString str;
				GetSysError(str,GetLastError(),_T("kernel32.dll"));
				GetString(IDS_ERROR_FILEOPEN,m_strError);
				m_strError.Replace(_T("%1"),fileInfo.m_szFile);
				m_strError.Replace(_T("%2"),str);
				return false;
			}
		} else 
#endif
		if(file.Create(fileInfo.m_szFile,GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS)!=S_OK) {
			CString str;
			GetSysError(str,GetLastError(),_T("kernel32.dll"));
			GetString(IDS_ERROR_FILEOPEN,m_strError);
			m_strError.Replace(_T("%1"),fileInfo.m_szFile);
			m_strError.Replace(_T("%2"),str);
			return false;
		}

status_moved:
		SetFileInfo(strURL,m_files[i]);
		if(Abort()) {
			return false;
		}

		DWORD dwService = INTERNET_SERVICE_HTTP;
		DWORD dwFlags = 0;

		CSplitURL url;
		if(!url.Split(strURL)) {
			GetString(IDS_ERROR_URL,m_strError);
			m_strError.Replace(_T("%1"),strURL);
			return false;
		}

		if(url.GetScheme()==INTERNET_SCHEME_HTTP) {
			dwService = INTERNET_SERVICE_HTTP;
		} else if(url.GetScheme()==INTERNET_SCHEME_FTP) {
			dwService = INTERNET_SERVICE_FTP;
			dwFlags |= INTERNET_FLAG_PASSIVE;
		} else {
			GetString(IDS_ERROR_PROTOCOL,m_strError);
			return false;
		}

		lock.Lock();
		CUserPass userpass;
		LPCTSTR pszUser = NULL, pszPass = NULL;
reconnect:
		if(url.GetUserNameLength()==0 || (pszUser && pszPass))
			m_hConn = inet.InternetConnect(m_hInternet, url.GetHostName(), url.GetPort(), pszUser, pszPass, dwService, dwFlags, (DWORD_PTR) this);
		else
			m_hConn = inet.InternetConnect(m_hInternet, url.GetHostName(), url.GetPort(), url.GetUserName(), url.GetPassword(), dwService, dwFlags, (DWORD_PTR) this);

		if(!m_hConn) {
			DWORD dwLastError = GetLastError();
			//if(url.GetScheme()==INTERNET_SCHEME_FTP && dwLastError==ERROR_INTERNET_LOGIN_FAILURE) {
			if(url.GetScheme()==INTERNET_SCHEME_FTP) {
				ResetEvent(m_hEventWait);
				PostMessage(m_hWndServer,UWM_USERPASS,(WPARAM)0,(LPARAM)&userpass);
				WaitForSingleObject(m_hEventWait,INFINITE);
				if(m_bErrorDlgResult) {
					pszUser = userpass.m_strUserName;
					pszPass = userpass.m_strPassword;
					goto reconnect;
				}
			}
			CString str;
			GetInetError(str,dwLastError);
			GetString(IDS_ERROR_CONNECT_SERVER,m_strError);
			m_strError.Replace(_T("%1"),url.GetHostName());
			m_strError.Replace(_T("%2"),str);
			return false;
		}

		CString strObject(url.GetURLPath());
		strObject += url.GetExtraInfo();

		if(url.GetScheme()==INTERNET_SCHEME_HTTP) {
			m_hFile = inet.HttpOpenRequest(m_hConn,NULL,strObject,NULL,NULL,m_ppszAcceptTypes,
				/*INTERNET_FLAG_NO_AUTO_REDIRECT|*/INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_KEEP_CONNECTION,(DWORD_PTR)this);
		} else if(url.GetScheme()==INTERNET_SCHEME_FTP) {
			// Remove leading '/'
			if(strObject.GetLength()>0 && strObject[0]==_T('/')) strObject.Delete(0);
			m_hFile = inet.FtpOpenFile(m_hConn,strObject,GENERIC_READ,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD,(DWORD_PTR)this);
		}

		if(!m_hFile) {
			CString str;
			GetInetError(str);
			GetString(IDS_ERROR_URLOPEN,m_strError);
			m_strError.Replace(_T("%1"),fileInfo.m_szURL);
			m_strError.Replace(_T("%2"),str);
			return false;
		}

		lock.Unlock();
resend:
		if(url.GetScheme()==INTERNET_SCHEME_HTTP) {
#ifdef USE_RESUME
			if(!inet.HttpSendRequest(m_hFile, pszHeaders, strRangeHeader.GetLength(), NULL, 0))
#else
			if(!inet.HttpSendRequest(m_hFile, NULL, 0, NULL, 0))
#endif
			{
				CString str;
				GetInetError(str);
				GetString(IDS_ERROR_REQUEST,m_strError);
				m_strError.Replace(_T("%1"),str);
				return false;
			}

			if(Abort()) {
				return false;
			}

			DWORD dwStatusCode = 0;
			ULONGLONG uLength = 0;
			DWORD dwDummy = sizeof DWORD;

			if(!inet.HttpQueryInfo(m_hFile,HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER,&dwStatusCode,&dwDummy,NULL)) {
				CString str;
				GetInetError(str);
				GetString(IDS_ERROR_STATUSCODE,m_strError);
				m_strError.Replace(_T("%1"),str);
				return false;
			}

			if(dwStatusCode==HTTP_STATUS_PROXY_AUTH_REQ || dwStatusCode==HTTP_STATUS_DENIED) {
				ResetEvent(m_hEventWait);
				PostMessage(m_hWndServer,UWM_ERRORDLG,(WPARAM)m_hFile,0);
				WaitForSingleObject(m_hEventWait,INFINITE);
				if(!m_bErrorDlgResult) {
					CString str;
					GetInetError(str);
					m_strError.LoadString(IDS_ERROR_REQUESTFILE);
					m_strError.Replace(_T("%1"),_T(""));
					return false;
				}
				goto resend;
			} else if(dwStatusCode==HTTP_STATUS_OK /*&& !pszHeaders*/) {
#ifdef USE_RESUME
				if(nResumePos) {
					// We tried resume, but http server doesn't support it (or something...)
					file.Seek(0,FILE_BEGIN);
					nResumePos = 0;
				}
#endif

				DWORD dwDummy2;
				if(!inet.HttpQueryInfo(m_hFile,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,&dwDummy2,&dwDummy,NULL))
					uLength = 0;
				else
					uLength = dwDummy2;

				if(!DoOperation(file,uLength,uTotal,uTotalSize,fileInfo)) {
					return false;
				}
#ifdef USE_RESUME
			} else if(dwStatusCode==HTTP_STATUS_PARTIAL_CONTENT && pszHeaders) {
				DWORD dwDummy2;
				if(!inet.HttpQueryInfo(m_hFile,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,&dwDummy2,&dwDummy,NULL))
					uLength = 0;
				else {
					uLength = dwDummy2 + nResumePos;
					uTotal += nResumePos;
				}

				if(!DoOperation(file,uLength,uTotal,uTotalSize,m_files[i],nResumePos)) {
					return false;
				}
			} else if(dwStatusCode==416) {
				// Requested range not satisfiable
				CString strContentRange;
				dwDummy = 256;
				bool bDone = false;
				if(inet.HttpQueryInfo(m_hFile,HTTP_QUERY_CONTENT_RANGE,strContentRange.GetBuffer(dwDummy),&dwDummy,NULL)) {
					strContentRange.ReleaseBufferSetLength(dwDummy);
					int nPos = strContentRange.ReverseFind('/');
					if(nPos>0) {
						ULONG nContentRange = (ULONG)myatol(strContentRange.Mid(nPos+1));
						bDone = nContentRange==nResumePos;
					}
				}
				if(!bDone) {
					strRangeHeader.Empty();
					pszHeaders = NULL;
					file.Seek(0,FILE_BEGIN);
					nResumePos = 0;
					goto status_moved;
				}
				m_files[i].m_uLength = nResumePos;

				uTotalSize = GetTotalSize();
				uTotal += nResumePos;

				// Current File
				PostMessage(m_hWndServer,UWM_POSFILE,MYSIZE(nResumePos),MYSIZE(nResumePos));
				// Overall Progress
				if(uTotalSize!=(ULONGLONG)-1) PostMessage(m_hWndServer,UWM_POSALL,MYSIZE(uTotal),MYSIZE(uTotalSize));
#endif
			} else if(dwStatusCode==HTTP_STATUS_REDIRECT || dwStatusCode==HTTP_STATUS_MOVED) {
				char szLocation[MAX_PATH+1];
				DWORD dwSize = MAX_PATH;
				inet.HttpQueryInfo(m_hFile, HTTP_QUERY_LOCATION, szLocation, &dwSize, NULL);
				strURL = szLocation;
				GetString(IDS_STATUS_REDIRECT,str);
				str.Replace(_T("%1"),strURL);
				PostTxtMsg(UWM_TEXT,IDC_STATUS,str);
				goto status_moved;
			} else {
				str.Format(_T("%d"),dwStatusCode);
				GetString(IDS_ERROR_HTTPOPEN,m_strError);
				m_strError.Replace(_T("%1"),fileInfo.m_szURL);
				m_strError.Replace(_T("%2"),str);
				return false;
			}
		} else if(url.GetScheme()==INTERNET_SCHEME_FTP) {
			file.Seek(0,FILE_BEGIN);	// No resume here yet
			if(!DoOperation(file,fileInfo.m_uLength,uTotal,uTotalSize,fileInfo)) {
				return false;
			}
		}

		file.Close();
		inet.InternetCloseHandle(m_hFile);
		m_hFile = NULL;

		fileInfo.m_bDone = true;
	}

	m_bAskRetry = false;
	return true;
}

bool CDownload::DoOperation(CAtlFile& file,ULONGLONG uLength,ULONGLONG& uTotal,ULONGLONG& uTotalSize,CFileInfo& fileInfo,ULONGLONG nResumePos/*=0*/) {
	PostMessage(m_hWndServer,UWM_POSFILE,0,MYSIZE(uLength));

	char szBuf[1024];
	DWORD dwBytes;
	ULONGLONG uBytesFile = nResumePos;
	DWORD dwLastSeconds = 0;
	do {
		if(Abort()) return false;

		if(!inet.InternetReadFile(m_hFile,szBuf,sizeof szBuf,&dwBytes)) {
			CString str;
			GetInetError(str);
			GetString(IDS_ERROR_READ,m_strError);
			m_strError.Replace(_T("%1"),str);
			return false;
		}

		CString str;
		if(dwBytes) {
			m_nTotalBytesDownloaded += dwBytes;
			uTotal += dwBytes;
			uBytesFile += dwBytes;
			DWORD dwWritten;
			HRESULT hr = file.Write(szBuf,dwBytes,&dwWritten);
			if(hr!=S_OK || dwWritten!=dwBytes) {
				CString str;
				GetSysError(str);
				GetString(IDS_ERROR_WRITE,m_strError);
				m_strError.Replace(_T("%1"),fileInfo.m_szFile);
				m_strError.Replace(_T("%2"),str);
				return false;
			}
		}
		
		DWORD dwSeconds = (GetTickCount() - m_dwRunStart) / 1000;
		if(!dwSeconds) dwSeconds = 1;

		// Only update info once every second
		if(dwSeconds>dwLastSeconds) {
			CStringEx tmp1, tmp2;
			dwLastSeconds = dwSeconds;

			// Overall Progress
			if(uLength && uTotalSize!=(ULONGLONG)-1) PostMessage(m_hWndServer,UWM_POSALL,MYSIZE(uTotal),MYSIZE(uTotalSize));
			// Current File
			if(uLength) PostMessage(m_hWndServer,UWM_POSFILE,MYSIZE(uBytesFile),MYSIZE(uLength));
			// Speed
			tmp1.FormatNum((m_nTotalBytesDownloaded>>10) / dwSeconds);
			//tmp1.Format("%d",);
			//FixNum(tmp1);
			str.Format(_T("%s KB/s"),tmp1);
			PostTxtMsg(UWM_TEXT,IDC_SPEED,str);
			// Current File
			if(uLength) {
				GetString(IDS_BYTECOUNT2,str);
				tmp1.FormatNum(uBytesFile>>10);
				str.Replace(_T("%1"),tmp1);
				tmp1.FormatNum(uLength>>10);
				str.Replace(_T("%2"),tmp1);
				tmp1.FormatNum(100*uBytesFile/uLength);
				str.Replace(_T("%3"),tmp1);
				PostTxtMsg(UWM_TEXT,IDC_STATUS_FILE,str);
			} else {
				tmp1.FormatNum(uBytesFile>>10);
				GetString(IDS_BYTECOUNT1,str);
				str.Replace(_T("%1"),tmp1);
				PostTxtMsg(UWM_TEXT,IDC_STATUS_FILE,str);
			}
			// Overall Progress
			if(uLength && uTotalSize!=(ULONGLONG)-1) {
				GetString(IDS_BYTECOUNT2,str);
				tmp1.FormatNum(uTotal>>10);
				str.Replace(_T("%1"),tmp1);
				tmp1.FormatNum(uTotalSize>>10);
				str.Replace(_T("%2"),tmp1);
				tmp1.FormatNum(100*uTotal/uTotalSize);
				str.Replace(_T("%3"),tmp1);
				PostTxtMsg(UWM_TEXT,IDC_STATUS_OVERALL,str);
			} else {
				tmp1.FormatNum(uTotal>>10);
				GetString(IDS_BYTECOUNT1,str);
				str.Replace(_T("%1"),tmp1);
				PostTxtMsg(UWM_TEXT,IDC_STATUS_OVERALL,str);
			}
			// Elapsed Time
			str.Format(_T("%d:%02d:%02d"),dwSeconds/3600,(dwSeconds/60)%60,dwSeconds%60);
			PostTxtMsg(UWM_TEXT,IDC_ELAPSED,str);
			// Remaining Time
			if(uLength && uTotalSize!=(ULONGLONG)-1 && uTotal>0 && m_nTotalBytesDownloaded>0) {
				// Adjust for resume
				dwSeconds = DWORD(dwSeconds * uTotal / m_nTotalBytesDownloaded);
				DWORD dwRemaining = (DWORD)((ULONGLONG)uTotalSize * (DWORD)dwSeconds / (ULONGLONG)uTotal - dwSeconds);
				str.Format(_T("%d:%02d:%02d"),dwRemaining/3600,(dwRemaining/60)%60,dwRemaining%60);
				PostTxtMsg(UWM_TEXT,IDC_REMAINING,str);
			}
		}
	} while(dwBytes>0);

	// Update file with the real length
	m_pCurrentFile->m_uLength = uBytesFile;
	uTotalSize = GetTotalSize();

	// Current File
	PostMessage(m_hWndServer,UWM_POSFILE,MYSIZE(uBytesFile),MYSIZE(uBytesFile));
	// Overall Progress
	if(uTotalSize!=(ULONGLONG)-1) PostMessage(m_hWndServer,UWM_POSALL,MYSIZE(uTotal),MYSIZE(uTotalSize));

	return true;
}

bool CDownload::Abort() {
	if(m_bAbort) m_bAskRetry = false;
	return m_bAbort;
}

bool CDownload::OpenInternet() {
	Henden::CSingleLock lock(m_cs,true);
	if(m_hInternet) {
		if(!m_iscCallback)
			m_iscCallback = inet.InternetSetStatusCallback(m_hInternet,(INTERNET_STATUS_CALLBACK)CallMaster);
		return true;
	}

	SetGlobalOffline(false);

	m_hInternet = inet.InternetOpen(_T("IS Download DLL"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if(!m_hInternet) {
		CString str;
		GetInetError(str);
		GetString(IDS_ERROR_CONNECT,m_strError);
		m_strError.Replace(_T("%1"),str);
		return false;
	}

	m_iscCallback = inet.InternetSetStatusCallback(m_hInternet,(INTERNET_STATUS_CALLBACK)CallMaster);
	return true;
}

ULONGLONG CDownload::GetURLLength(HINTERNET m_hInternet,LPCTSTR pszURL) {
	if(IsSimple()) return -1;

	CString strURL(pszURL);
status_moved:
	CSplitURL url;
	if(!url.Split(strURL))
		return -1;

	DWORD dwFlags = 0;
	DWORD dwService = 0;

	if(url.GetScheme()==INTERNET_SCHEME_HTTP) {
		dwService = INTERNET_SERVICE_HTTP;
	} else if(url.GetScheme()==INTERNET_SCHEME_FTP) {
		dwService = INTERNET_SERVICE_FTP;
		dwFlags |= INTERNET_FLAG_PASSIVE;
	} else {
		return -1;
	}

	HINTERNET hConn;
	if(url.GetUserNameLength()==0)
		hConn = inet.InternetConnect(m_hInternet, url.GetHostName(), url.GetPort(), NULL, NULL, dwService, dwFlags, 0);
	else
		hConn = inet.InternetConnect(m_hInternet, url.GetHostName(), url.GetPort(), url.GetUserName(), url.GetPassword(), dwService, dwFlags, 0);
	if(!hConn)
		return -1;

	HINTERNET hFile;
	ULONGLONG dwLength = 0;
	CString strObject = url.GetURLPath();
	strObject += url.GetExtraInfo();

	if(url.GetScheme()==INTERNET_SCHEME_HTTP) {
		hFile = inet.HttpOpenRequest(hConn,_T("HEAD"),strObject,NULL,NULL,m_ppszAcceptTypes,INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_RELOAD,0);
		if(!hFile) {
			inet.InternetCloseHandle(hConn);
			return -1;
		}

		DWORD dwStatusCode = 0;
		DWORD dwDummy = sizeof DWORD;

resend:
		if(!inet.HttpSendRequest(hFile, NULL, 0, NULL, 0)) {
			inet.InternetCloseHandle(hFile);
			inet.InternetCloseHandle(hConn);
			return -1;
		}

		if(!inet.HttpQueryInfo(hFile,HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER,&dwStatusCode,&dwDummy,NULL))
			return -1;

		if(dwStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || dwStatusCode == HTTP_STATUS_DENIED) {
			ResetEvent(m_hEventWait);
			PostMessage(m_hWndServer,UWM_ERRORDLG,(WPARAM)hFile,0);
			WaitForSingleObject(m_hEventWait,INFINITE);
			if(!m_bErrorDlgResult) {
				inet.InternetCloseHandle(hFile);
				inet.InternetCloseHandle(hConn);
				return -1;
			}
			goto resend;
		} else if(dwStatusCode==HTTP_STATUS_REDIRECT || dwStatusCode==HTTP_STATUS_MOVED) {
			char szLocation[MAX_PATH+1];
			DWORD dwSize = MAX_PATH;
			inet.HttpQueryInfo(hFile, HTTP_QUERY_LOCATION, szLocation, &dwSize, NULL);
			strURL = szLocation;
			inet.InternetCloseHandle(hFile);
			hFile = NULL;
			inet.InternetCloseHandle(hConn);
			hConn = NULL;
			goto status_moved;
		} else if(dwStatusCode != HTTP_STATUS_OK) {
			inet.InternetCloseHandle(hFile);
			inet.InternetCloseHandle(hConn);
			return -1;
		}

		if(!inet.HttpQueryInfo(hFile,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,&dwLength,&dwDummy,NULL)) {
			inet.InternetCloseHandle(hFile);
			inet.InternetCloseHandle(hConn);
			return -1;
		}
	} else if(url.GetScheme()==INTERNET_SCHEME_FTP) {
		if(strObject.GetLength()>0 && strObject[0]==_T('/')) strObject.Delete(0);
		dwLength = 0;
		CString strCmd;
		strCmd.Format(_T("SIZE %s\r\n"),strObject);
		if(inet.FtpCommand(hConn,FALSE,0,strCmd,(DWORD_PTR)this,NULL)) {
			DWORD dwError, dwChars = 0;
			inet.InternetGetLastResponseInfo(&dwError,NULL,&dwChars);
			inet.InternetGetLastResponseInfo(&dwError,strCmd.GetBuffer(++dwChars),&dwChars);
			strCmd.ReleaseBufferSetLength(dwChars);
			if(strCmd.Left(3)==_T("213"))
				dwLength = myatol(strCmd.Mid(3).Trim());
#if 0
		} else {
			GetInetError(strCmd);
			AtlMessageBox(m_hWndServer,(LPCTSTR)strCmd);
#endif
		}
	}

	if(hFile) inet.InternetCloseHandle(hFile);
	if(hConn) inet.InternetCloseHandle(hConn);
	return dwLength;
}

bool CDownload::GetSysError(CString& ref,DWORD dwError/*=GetLastError()*/,LPCTSTR pszModule/*=NULL*/) {
	LPVOID lpMsgBuf;
	HMODULE hModule = NULL;
	if(pszModule) hModule = GetModuleHandle(pszModule);
	if(FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
		hModule,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	)) {
		ref = (LPCTSTR)lpMsgBuf;
		LocalFree(lpMsgBuf);
		return true;
	} else {
		if(pszModule)
			ref.Format(_T("Error %d from %s."),dwError,pszModule);
		else
			ref.Format(_T("Error %d."),dwError);
		return false;
	}
}

void CDownload::PostTxtMsg(UINT uMsg,UINT uID,CString& str) {
	LPTSTR pszText = new TCHAR[str.GetLength()+1];
	_tcscpy(pszText,str);
	PostMessage(m_hWndServer,uMsg,(WPARAM)uID,(LPARAM)pszText);
}

bool CDownload::GetString(UINT uID,CString& str) {
	str.Empty();
	if(!m_strOptionLanguage.IsEmpty() && uID!=IDS_ABOUT) {
		CString strKey;
		strKey.Format(_T("%d"),uID);
		DWORD nSize = GetPrivateProfileString(_T("strings"),strKey,_T(""),str.GetBuffer(1000),1000,m_strOptionLanguage);
		str.ReleaseBufferSetLength(nSize);
		while(str.Replace(_T("\\n"),_T("\n")));
	}

	if(str.IsEmpty()) str.LoadString(uID);
	return !str.IsEmpty();
}
