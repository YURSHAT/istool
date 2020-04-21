// isxbb.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "isxbb.h"

#define BORDER	10

CWork*		g_pWork;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved) {
	switch(ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		g_pWork = new CWork;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		delete g_pWork;
		break;
	}
    return TRUE;
}

CWork::CWork() {
	m_hWnd = NULL;
	m_nNextTimer = 602;
	m_hHook = NULL;
}

CWork::~CWork() {
	while(m_pictures.GetSize()) {
		if(m_pictures[0]->pPicture) m_pictures[0]->pPicture->Release();
		delete m_pictures[0];
		m_pictures.RemoveAt(0);
	}
	while(m_timers.GetSize()) {
		delete m_timers[0];
		m_timers.RemoveAt(0);
	}
	if(m_hHook) UnhookWindowsHookEx(m_hHook);
}

void CWork::DoShow(UINT_PTR nTimerID) {
	if(!m_hWnd || !IsWindow(m_hWnd)) return;
	for(long i=0;i<m_timers.GetSize();i++) {
		if(m_timers[i]->nTimerID!=nTimerID) continue;

		if(m_timers[i]->nCurrent>=0) {
			int nPos = m_timers[i]->nCurrent+1;
			m_timers[i]->nCurrent = -1;
			for(int n=nPos;n<m_pictures.GetSize();n++) {
				if((m_pictures[n]->dwFlags & PLACEMASK) == m_timers[i]->dwFlags) {
					m_timers[i]->nCurrent = n;
					break;
				}
			}
		} 
		if(m_timers[i]->nCurrent<0) {
			for(long n=0;n<m_pictures.GetSize();n++) {
				if((m_pictures[n]->dwFlags & PLACEMASK) == m_timers[i]->dwFlags) {
					m_timers[i]->nCurrent = n;
					break;
				}
			}
		}
		if(m_timers[i]->nCurrent>=0) {
			CClientDC dc(m_hWnd);
			RECT rc;
			GetClientRect(m_hWnd,&rc);
			RenderPicture(dc,m_pictures[m_timers[i]->nCurrent]->pPicture,rc,m_timers[i]->dwFlags);
		}
		break;
	}
}

void CWork::DoTheStuff() {
	if(!m_hWnd || !IsWindow(m_hWnd)) return;
	RECT rc;

	CClientDC dc(m_hWnd);
	GetClientRect(m_hWnd,&rc);

	for(long n=0;n<m_pictures.GetSize();n++)
		if(!(m_pictures[n]->dwFlags & TIMER))
			RenderPicture(dc,m_pictures[n]->pPicture,rc,m_pictures[n]->dwFlags);
	
	for(long n=0;n<m_timers.GetSize();n++)
		if(m_timers[n]->nTimerID)
			RenderPicture(dc,m_pictures[m_timers[n]->nCurrent]->pPicture,rc,m_pictures[m_timers[n]->nCurrent]->dwFlags);
}

LRESULT CWork::HookFunc(int nCode,WPARAM wParam,LPARAM lParam) {
	if(nCode==HC_ACTION) {
		PCWPRETSTRUCT p = (PCWPRETSTRUCT)lParam;
		if(p->hwnd==m_hWnd) {
			long i;

			switch(p->message) {
			case WM_PAINT: 
				DoTheStuff();
				break;
			case WM_ERASEBKGND:
				UpdateWindow(p->hwnd);
				break;
			case WM_DESTROY:
				for(i=0;i<m_timers.GetSize();i++)
					if(m_timers[i]->nTimerID)
						KillTimer(p->hwnd,m_timers[i]->nTimerID);
				break;
			}
		}
	}
	return CallNextHookEx(m_hHook,nCode,wParam,lParam);
}

int CWork::isxbb_Init(HWND hWnd) {
	if(m_hHook) return 1;
	if(!hWnd) return 0;
	
	m_hWnd = hWnd;
	m_hHook = SetWindowsHookEx(WH_CALLWNDPROCRET,CallWndRetProc,NULL,GetCurrentThreadId());
	DoTheStuff();
	return 1;
}

int CWork::isxbb_AddImage(LPCTSTR pszImage,DWORD dwFlags) {
	IPicture* pPicture;
	HRESULT hr = OleLoadPicturePath(CA2W(pszImage),0,0,0,IID_IPicture,(void**)&pPicture);
	if(hr!=S_OK)
		return 0;

	CPicInfo* p = new CPicInfo;
	p->pPicture = pPicture;
	p->dwFlags = dwFlags;
	m_pictures.Add(p);
	return 1;
}

void CWork::TimerFunc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime) {
	for(long i=0;i<m_timers.GetSize();i++)
		if(idEvent==m_timers[i]->nTimerID) {
			DoShow(idEvent);
			break;
		}
}

int CWork::isxbb_StartTimer(int nSeconds,DWORD dwFlags) {
	if(!m_hWnd) return 0;

	for(long i=0;i<m_timers.GetSize();i++) {
		if(m_timers[i]->dwFlags == dwFlags) {
			if(m_timers[i]->nTimerID) return 0;
			break;
		}
	}

	if(nSeconds>0)
		nSeconds *= 1000;
	else if(nSeconds<0)
		nSeconds *= -1;
	else
		nSeconds = 1;

	CTimerInfo* p = new CTimerInfo;
	p->dwFlags = dwFlags;
	p->nTimerID = ++m_nNextTimer;
	p->nCurrent = -1;
	m_timers.Add(p);
	SetTimer(m_hWnd,p->nTimerID,nSeconds,TimerProc);
	DoShow(p->nTimerID);
	return 1;
}

int CWork::isxbb_KillTimer(DWORD dwFlags) {
	if(!m_hWnd) return 0;

	for(long i=0;i<m_timers.GetSize();i++) {
		if(m_timers[i]->dwFlags == dwFlags) {
			KillTimer(m_hWnd,m_timers[i]->nTimerID);
			m_timers[i]->nTimerID = 0;
			m_timers[i]->nCurrent = -1;

			// Force a repaint of the background
			RECT rc;
			GetClientRect(m_hWnd,&rc);
			InvalidateRect(m_hWnd,&rc,TRUE);
			UpdateWindow(m_hWnd);
			return 1;
		}
	}
	return 0;
}

void CWork::RenderPicture(HDC hdc,IPicture* p,RECT& rc,DWORD place) {
	if(!p) return;

	place &= PLACEMASK;

	CDCHandle dc(hdc);
	POINT pt;

	long nWidth, nHeight;
	p->get_Width(&nWidth);
	p->get_Height(&nHeight);
	long nWidth2, nHeight2;

	nWidth2 = MulDiv(nWidth,  dc.GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
	nHeight2 = MulDiv(nHeight, dc.GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);

	switch(place) {
	case TOPLEFT:
		pt.x = rc.left + BORDER;
		pt.y = rc.top + BORDER + nHeight2;
		break;
	case TOPRIGHT:
		pt.x = rc.right - BORDER - nWidth2;
		pt.y = rc.top + BORDER + nHeight2;
		break;
	case BOTTOMLEFT:
		pt.x = rc.left + BORDER;
		pt.y = rc.bottom - BORDER;
		break;
	case BOTTOMRIGHT:
		pt.x = rc.right - BORDER - nWidth2;
		pt.y = rc.bottom - BORDER;
		break;
	case CENTER:
		pt.x = rc.left + (rc.right - rc.left - nWidth2)/2;
		pt.y = rc.top + (rc.bottom - rc.top + nHeight2)/2;
		break;
	case TOP:
		pt.x = rc.left + (rc.right - rc.left - nWidth2)/2;
		pt.y = rc.top + BORDER + nHeight2;
		break;
	case BOTTOM:
		pt.x = rc.left + (rc.right - rc.left - nWidth2)/2;
		pt.y = rc.bottom - BORDER;
		break;
	case LEFT:
		pt.x = rc.left + BORDER;
		pt.y = rc.top + (rc.bottom - rc.top + nHeight2)/2;
		break;
	case RIGHT:
		pt.x = rc.right - BORDER - nWidth2;
		pt.y = rc.top + (rc.bottom - rc.top + nHeight2)/2;
		break;
	}

	if(place==BACKGROUND)
		p->Render(dc,rc.left,rc.bottom,rc.right,rc.top-rc.bottom,0,0,nWidth,nHeight,NULL);
	else
		p->Render(dc,pt.x,pt.y,nWidth2,-nHeight2,0,0,nWidth,nHeight,NULL);
}

LRESULT CALLBACK CWork::CallWndRetProc(int nCode,WPARAM wParam,LPARAM lParam) {
	return g_pWork->HookFunc(nCode,wParam,lParam);
}

VOID CALLBACK CWork::TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime) {
	g_pWork->TimerFunc(hwnd,uMsg,idEvent,dwTime);
}

extern "C" int __stdcall isxbb_Init(HWND _hWnd) {
	return g_pWork->isxbb_Init(_hWnd);
}

extern "C" int __stdcall isxbb_AddImage(LPCTSTR pszImage,DWORD dwFlags) {
	return g_pWork->isxbb_AddImage(pszImage,dwFlags);
}

extern "C" int __stdcall isxbb_StartTimer(int nSeconds,DWORD dwFlags) {
	return g_pWork->isxbb_StartTimer(nSeconds,dwFlags);
}

extern "C" int __stdcall isxbb_KillTimer(DWORD dwFlags) {
	return g_pWork->isxbb_KillTimer(dwFlags);
}
