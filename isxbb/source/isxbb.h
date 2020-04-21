#pragma once

class CWork {
public:
	CWork();
	~CWork();
	int isxbb_Init(HWND hWnd);
	int isxbb_AddImage(LPCTSTR pszImage,DWORD dwFlags);
	int isxbb_StartTimer(int nSeconds,DWORD dwFlags);
	int isxbb_KillTimer(DWORD dwFlags);

protected:
	enum {
		TOPLEFT			= 0x01,
		TOPRIGHT		= 0x02,
		BOTTOMLEFT		= 0x03,
		BOTTOMRIGHT		= 0x04,
		CENTER			= 0x05,
		BACKGROUND		= 0x06,
		TOP				= 0x07,
		BOTTOM			= 0x08,
		LEFT			= 0x09,
		RIGHT			= 0x0A,
		PLACEMASK		= 0x0F,
		TIMER			= 0x10
	};

	typedef struct {
		IPicture*	pPicture;
		DWORD		dwFlags;
	} CPicInfo;

	typedef struct {
		DWORD		dwFlags;
		UINT_PTR	nTimerID;
		int			nCurrent;
	} CTimerInfo;

	void DoShow(UINT_PTR nTimerID);
	void DoTheStuff();
	LRESULT HookFunc(int nCode,WPARAM wParam,LPARAM lParam);
	void TimerFunc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	void RenderPicture(HDC hdc,IPicture* p,RECT& rc,DWORD place);
	static LRESULT CALLBACK CallWndRetProc(int nCode,WPARAM wParam,LPARAM lParam);
	static VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);

	CSimpleArray<CPicInfo*>		m_pictures;
	CSimpleArray<CTimerInfo*>	m_timers;
	HHOOK						m_hHook;
	HWND						m_hWnd;
	UINT_PTR					m_nNextTimer;
};
