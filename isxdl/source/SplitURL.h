// Implementation of the CURLComponents and CSplitURL classes.

#pragma once

// Class to wrap the Win32 URL_COMPONENTS structure
class CURLComponents : public URL_COMPONENTS {
public:    
    CURLComponents() {
        memset(this, 0, sizeof(URL_COMPONENTS));
        dwStructSize = sizeof(URL_COMPONENTS);
    }
};

// Class used to split a URL into component parts.
// Note: Uses WININET InternetCrackUrl function.
class CSplitURL {
private:
    CString m_strScheme;
    INTERNET_SCHEME m_nScheme;
    CString m_strHostName;
    INTERNET_PORT m_nPort;
    CString m_strUserName;
    CString m_strPassword;
    CString m_strURLPath;
    CString m_strExtraInfo;
public:    
    CSplitURL() : m_nScheme(INTERNET_SCHEME_DEFAULT), m_nPort(0) {}
    
    // Split a URL into component parts
    bool Split(LPCTSTR lpsz) {
		if(!inet.InternetCrackUrl) return false;

        // Be defensive
        ATLASSERT(lpsz != NULL && *lpsz != '\0');
        // Get the URL length
        DWORD dwLength = (DWORD)_tcslen(lpsz);

        CURLComponents url;        
        // Fill structure
        url.lpszScheme = m_strScheme.GetBuffer(dwLength);
        url.dwSchemeLength = dwLength;
        url.lpszHostName = m_strHostName.GetBuffer(dwLength);
        url.dwHostNameLength = dwLength;
        url.lpszUserName = m_strUserName.GetBuffer(dwLength);
        url.dwUserNameLength = dwLength;
        url.lpszPassword = m_strPassword.GetBuffer(dwLength);
        url.dwPasswordLength = dwLength;
        url.lpszUrlPath = m_strURLPath.GetBuffer(dwLength);
        url.dwUrlPathLength = dwLength;
        url.lpszExtraInfo = m_strExtraInfo.GetBuffer(dwLength);
        url.dwExtraInfoLength = dwLength;
        // Split
		bool bRet = inet.InternetCrackUrl(lpsz, 0, 0, &url) != FALSE;
        // Release buffers
        m_strScheme.ReleaseBuffer();
        m_strHostName.ReleaseBuffer();
        m_strUserName.ReleaseBuffer();
        m_strPassword.ReleaseBuffer();
        m_strURLPath.ReleaseBuffer();
        m_strExtraInfo.ReleaseBuffer();
        // Get the scheme/port
        m_nScheme = url.nScheme;
        m_nPort = url.nPort;
        // Done
        return bRet;
    }

    // Get the scheme number
    inline INTERNET_SCHEME GetScheme(void) const { return m_nScheme; }
    // Get the port number
    inline INTERNET_PORT GetPort(void) const { return m_nPort; }
    // Get the scheme name
    inline LPCTSTR GetSchemeName(void) const { return m_strScheme; }
    // Get the host name
    inline LPCTSTR GetHostName(void) const { return m_strHostName; }
    // Get the user name
    inline LPCTSTR GetUserName(void) const { return m_strUserName; }
    // Get the password
    inline LPCTSTR GetPassword(void) const { return m_strPassword; }
    // Get the URL path
    inline LPCTSTR GetURLPath(void) const { return m_strURLPath; }
    // Get the extra info
    inline LPCTSTR GetExtraInfo(void) const { return m_strExtraInfo; }

	inline UINT GetUserNameLength() const { return m_strUserName.GetLength(); }
};
