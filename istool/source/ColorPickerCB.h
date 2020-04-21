#pragma once

//
//	Constants...
//
#define		CCB_MAX_COLORS			17							// Colors In List
#define		CCB_MAX_COLOR_NAME		16							// Max Chars For Color Name - 1


//
//	Internal Structure For Color/Name Storage...
//
struct	SColorAndName
{
	/**/	SColorAndName()
	{
		ZeroMemory( this, sizeof( SColorAndName ) );		// Init Structure
	};
	/**/	SColorAndName( COLORREF crColor, PCSTR cpColor )
	{
		ZeroMemory( this, sizeof( SColorAndName ) );		// Init Structure
		m_crColor = crColor;								// Set Color RGB Value
		_tcsncpy( m_cColor, cpColor, CCB_MAX_COLOR_NAME );	// Set Color Name
	};
	COLORREF	m_crColor;									// Actual Color RGB Value
	char		m_cColor[ CCB_MAX_COLOR_NAME ];				// Actual Name For Color
};



class CColorPickerCB : public CWindowImpl<CColorPickerCB,CComboBox> {
public:
	BOOL SubclassWindow(HWND hWnd);

	BEGIN_MSG_MAP(CColorPickerCB)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_DRAWITEM,OnDrawItem)
	    MESSAGE_HANDLER(OCM__BASE + WM_DRAWITEM, OnDrawItem)
		COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnSelChange)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	CString			m_sColorName;							// Name Of Selected Color
	static
	SColorAndName	ms_pColors[ CCB_MAX_COLORS ];			// Array Of Colors And Names

	void			Initialize( void );						// Initialize Control/Colors

public:
	COLORREF		GetSelectedColorValue( void );			// Get Selected Color Value
	CString			GetSelectedColorName( void );			// Get Selected Color Name

	void			SetSelectedColorValue( COLORREF crClr );// Set Selected Color Value
	void			SetSelectedColorName( PCSTR cpColor );	// Set Selected Color Name

	bool			RemoveColor( PCSTR cpColor );			// Remove Color From List
	bool			RemoveColor( COLORREF crClr );			// Remove Color From List
	
	int				AddColor( PCSTR cpNam, COLORREF crClr );// Insert A New Color

	void			SetInnoColor(LPCTSTR);
	void			GetInnoColor(CString&);
};
