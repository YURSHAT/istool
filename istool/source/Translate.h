// Translate.h

#pragma once

#include <atlstr.h>
#include <atlcoll.h>
#include <atlapp.h>
#include <atluser.h>

class CTranslate {
public:
	static LPCTSTR Translate(LPCSTR pszDefault);
	static LPCTSTR Translate(LPCSTR pszKey,LPCSTR pszDefault);
	static HMENU Translate(HMENU hMenu,CString strParent=CString());
	static void Translate(HWND hWnd,const CString& strTitle);
	static void AddFile(const CString& strFileName);
	static void FixTextTitle(CString& str);

private:
	static CAtlMap<CString,CString>	m_map;
#ifdef _DEBUG
	static const CString			m_strOrgFile;
#endif

	CTranslate() {}

	static bool Lookup(LPCTSTR pszKey,CString& strTrans);
	static void Warning(const CString& strKey,const CString& strDefault);
	static void FixMenuTitle(CString& str,CString& strAcc);
	static BOOL CALLBACK EnumChildProc(HWND hWnd,LPARAM lParam);
	static bool IgnoreWord(LPCTSTR pszWord);
};

#define _L CTranslate::Translate

#if 0

class CMainFrame : ..., public CTranslateFrame<CMainFrame> {
public:
	BEGIN_MSG_MAP(CMainFrame)
		...
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(CTranslateFrame<CMainFrame>)
	END_MSG_MAP()
};

#endif

template<class T>
class CTranslateFrame {
public:
	BEGIN_MSG_MAP(CTranslateFrame)
		MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFOA, OnToolTipTextA)
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFOW, OnToolTipTextW)
	END_MSG_MAP()

	LRESULT OnMenuSelect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		T* pT = static_cast<T*>(this);
		//bHandled = FALSE;

		if(pT->m_hWndStatusBar == NULL)
			return 1;

		WORD wFlags = HIWORD(wParam);
		if(wFlags == 0xFFFF && lParam == NULL)   // menu closing
		{
			::SendMessage(pT->m_hWndStatusBar, SB_SIMPLE, FALSE, 0L);
		}
		else
		{
			const int cchBuff = 256;
			TCHAR szBuff[cchBuff];
			szBuff[0] = 0;
			CString strText, strKey;
			if(!(wFlags & MF_POPUP))
			{
				WORD wID = LOWORD(wParam);
				// check for special cases
				if(wID >= 0xF000 && wID < 0xF1F0)                              // system menu IDs
					wID = (WORD)(((wID - 0xF000) >> 4) + ATL_IDS_SCFIRST);
				else if(wID >= ID_FILE_MRU_FIRST && wID <= ID_FILE_MRU_LAST)   // MRU items
					wID = ATL_IDS_MRU_FILE;
				else if(wID >= ATL_IDM_FIRST_MDICHILD)                         // MDI child windows
					wID = ATL_IDS_MDICHILD;

#if (_ATL_VER >= 0x0700)
				int nRet = ::LoadString(ATL::_AtlBaseModule.GetResourceInstance(), wID, szBuff, cchBuff);
#else //!(_ATL_VER >= 0x0700)
				int nRet = ::LoadString(_Module.GetResourceInstance(), wID, szBuff, cchBuff);
#endif //!(_ATL_VER >= 0x0700)
				for(int i = 0; i < nRet; i++)
				{
					if(szBuff[i] == _T('\n'))
					{
						szBuff[i] = 0;
						strKey = &szBuff[i+1];
						break;
					}
				}
				if(wID!=0 || !strKey.IsEmpty()) {
					if(strKey.IsEmpty()) strKey.Format(_T("StatusInfo|%d"),wID);
					else strKey.Insert(0,_T("StatusInfo|"));
					strText = _L(strKey,szBuff);
				}
			}
			::SendMessage(pT->m_hWndStatusBar, SB_SIMPLE, TRUE, 0L);
			::SendMessage(pT->m_hWndStatusBar, SB_SETTEXT, (255 | SBT_NOBORDERS), (LPARAM)(LPCTSTR)strText);
		}

		return 1;
	}

	LRESULT OnToolTipTextA(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMTTDISPINFOA pDispInfo = (LPNMTTDISPINFOA)pnmh;
		pDispInfo->szText[0] = 0;

		if((idCtrl != 0) && !(pDispInfo->uFlags & TTF_IDISHWND))
		{
			const int cchBuff = 256;
			char szBuff[cchBuff];
			szBuff[0] = 0;
#if (_ATL_VER >= 0x0700)
			int nRet = ::LoadStringA(ATL::_AtlBaseModule.GetResourceInstance(), idCtrl, szBuff, cchBuff);
#else //!(_ATL_VER >= 0x0700)
			int nRet = ::LoadStringA(_Module.GetResourceInstance(), idCtrl, szBuff, cchBuff);
#endif //!(_ATL_VER >= 0x0700)
			for(int i = 0; i < nRet; i++)
			{
				if(szBuff[i] == '\n')
				{
					CString strKey, strTrans(&szBuff[i+1]);
					strKey.Format(_T("ToolTip|%s"),strTrans);
					strTrans = _L(strKey,strTrans);
					lstrcpynA(pDispInfo->szText, strTrans, sizeof(pDispInfo->szText) / sizeof(pDispInfo->szText[0]));
					break;
				}
			}
#if (_WIN32_IE >= 0x0300)
			if(nRet > 0)   // string was loaded, save it
				pDispInfo->uFlags |= TTF_DI_SETITEM;
#endif //(_WIN32_IE >= 0x0300)
		}

		return 0;
	}

	LRESULT OnToolTipTextW(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMTTDISPINFOW pDispInfo = (LPNMTTDISPINFOW)pnmh;
		pDispInfo->szText[0] = 0;

		if((idCtrl != 0) && !(pDispInfo->uFlags & TTF_IDISHWND))
		{
			const int cchBuff = 256;
			wchar_t szBuff[cchBuff];
			szBuff[0] = 0;
#if (_ATL_VER >= 0x0700)
			int nRet = ::LoadStringW(ATL::_AtlBaseModule.GetResourceInstance(), idCtrl, szBuff, cchBuff);
#else //!(_ATL_VER >= 0x0700)
			int nRet = ::LoadStringW(_Module.GetResourceInstance(), idCtrl, szBuff, cchBuff);
#endif //!(_ATL_VER >= 0x0700)
			for(int i = 0; i < nRet; i++)
			{
				if(szBuff[i] == L'\n')
				{
					CString strKey, strTrans(&szBuff[i+1]);
					strKey.Format("ToolTip|%s",strTrans);
					strTrans = _L(strKey,strTrans);
					lstrcpynW(pDispInfo->szText, CT2W(strTrans), sizeof(pDispInfo->szText) / sizeof(pDispInfo->szText[0]));
					break;
				}
			}
#if (_WIN32_IE >= 0x0300)
			if(nRet > 0)   // string was loaded, save it
				pDispInfo->uFlags |= TTF_DI_SETITEM;
#endif //(_WIN32_IE >= 0x0300)
		}

		return 0;
	}
};

