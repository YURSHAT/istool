/**
** @file
** @brief Easy add tooltips to dialogs
*/

#pragma once

namespace Henden {

#define BEGIN_TOOLTIP_MAP()	LPCTSTR _GetToolTip(UINT uID) {switch(uID) {	///< Start of a tooltip map
#define TOOLTIP_HANDLER(a,b) case (a): return (b);							///< Handles the tooltip for a command id
#define END_TOOLTIP_MAP() default: return "<missing tooltip>";}}			///< End of the tooltip map

#ifndef TTS_BALLOON
#define TTS_BALLOON 0	///< Should be defined in later versions of the Microsoft SDK
#endif

/**
** @brief A class to enable tooltips in dialogs
**
** Inherit your dialog/window class from this class to enable tooltips.
**
** To use this class, inherit from this class:
**
@code
class CMyDialog : ..., public Henden::CToolTipDialog<CMyDialog> {
    BEGIN_MSG_MAP(...)
        CHAIN_MSG_MAP(Henden::CToolTipDialog<CMyDialog>)
        ...
    END_MSG_MAP()

    BEGIN_TOOLTIP_MAP()
        TOOLTIP_HANDLER(MY_ID1,"My Tooltip")
    END_TOOLTIP_MAP()
}
@endcode
*/
template < class T ,class TT = CToolTipCtrl >
class CToolTipDialog
{
// Data declarations and members
public:
	/// Returns the tooltip control
	TT& GetTT(){return m_TT;}
protected:
	TT m_TT;			///< The class used for tooltips. Usually CToolTipCtrl
	UINT m_uTTStyle;	///< Style for the tooltip control
	UINT m_uToolFlags;	///< Flags for the tooltip control

	/// Construction
	CToolTipDialog( UINT uTTSTyle= TTS_NOPREFIX | TTS_BALLOON , UINT uToolFlags = TTF_IDISHWND | TTF_SUBCLASS ) 
		: m_TT( NULL ), m_uTTStyle( uTTSTyle ), 
		m_uToolFlags( uToolFlags | TTF_SUBCLASS )
		{}

	/// Initialize tool tips
	void TTInit() {
		T* pT= (T*)this;
		ATLASSERT( ::IsWindow( *pT ));
		m_TT.Create( *pT, NULL, NULL, m_uTTStyle );
		//CToolInfo ToolInfo( pT->m_uToolFlags, *pT , 0, 0, MAKEINTRESOURCE(pT->IDD) );
		CToolInfo ToolInfo( pT->m_uToolFlags, *pT , 0, 0, LPSTR_TEXTCALLBACK );
		m_TT.AddTool( &ToolInfo );
		::EnumChildWindows( *pT, SetTool, (LPARAM)pT );
		TTActivate( TRUE );
		TTSize(300);

		m_TT.SetDelayTime(TTDT_AUTOPOP,10000);
		m_TT.SetDelayTime(TTDT_INITIAL,1000);
		m_TT.SetDelayTime(TTDT_RESHOW,1000);
	}
// Operations
private:
	/// Activate or deactivate the tooltip control
	void TTActivate(BOOL bActivate) { 
		m_TT.Activate( bActivate ); 
	}

	/// Set the size of the tooltip control
	void TTSize( int nPixel ) { 
		m_TT.SetMaxTipWidth( nPixel );
	}

public:
	/// Message map and handlers
	BEGIN_MSG_MAP(CToolTipDialog)
//		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST,WM_MOUSELAST, OnMouse)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_CODE_HANDLER(TTN_NEEDTEXT,OnNeedText)
	END_MSG_MAP()
		
	/// WM_INITDIALOG event
	LRESULT OnInitDialog(UINT , WPARAM , LPARAM, BOOL& bHandled) {
		TTInit();
		bHandled = FALSE;
		return TRUE;
	}

#if 0
	/// Mouse event
	LRESULT OnMouse(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		T* pT = (T*)this;
		bHandled = FALSE;
		if(m_TT.IsWindow())
			m_TT.RelayEvent((LPMSG)pT->GetCurrentMessage());
		return 0;
	}
#endif

// Implementation
private:
	HWND GetHWND( UINT idTool ) { 
		return ::GetDlgItem( *(T*)this, idTool );
	}

	static BOOL CALLBACK SetTool( HWND hTool, LPARAM pDlg) {
		T* pT = (T*)pDlg;
		int idTool = ::GetWindowLong(hTool, GWL_ID);
		if ( idTool != IDC_STATIC ) {
			//CToolInfo ToolInfo( pT->m_uToolFlags, hTool, 0, 0, (LPTSTR)idTool );
			CToolInfo ToolInfo( pT->m_uToolFlags, hTool, 0, 0, LPSTR_TEXTCALLBACK );
			pT->m_TT.AddTool( &ToolInfo );
		}
		return TRUE;
	}

	LRESULT OnNeedText(int,NMHDR* pNMHDR,BOOL&) {
		T* pT = static_cast<T*>(this);
		NMTTDISPINFO* ptt = reinterpret_cast<NMTTDISPINFO*>(pNMHDR);

        UINT nID = ptt->hdr.idFrom;
        if (ptt->uFlags & TTF_IDISHWND) {
            // idFrom is actually the HWND of the tool
            nID = ::GetDlgCtrlID((HWND)nID);
            if(nID) {
				ptt->lpszText = (LPTSTR)pT->_GetToolTip(nID);
                //ptt->lpszText = MAKEINTRESOURCE(nID);
                //ptt->hinst = AfxGetResourceHandle();
                return TRUE;
            }
        }
		return FALSE;
	}

	LPCTSTR _GetToolTip(UINT uID) {
		ATLASSERT(FALSE);
		return NULL;
	}
};

} // namespace Henden