// ScintillaView.h : interface of the CScintillaView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ScintillaPrefs.h"

//class CScintillaView : public CScintillaWindowImpl<CScintillaView>
class CScintillaView : public CWindowImpl<CScintillaView, CScintilla>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CScintilla::GetWndClassName())
	//DECLARE_WND_CLASS(NULL)

	typedef CWindowImpl<CScintillaView,CScintilla> baseClass;

	CScintillaView() {
		m_iSearchFlags = 0;
	}

	BOOL PreTranslateMessage(MSG*) {
		return FALSE;
	}

	BEGIN_MSG_MAP(CScintillaView)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		MESSAGE_HANDLER(CFindReplaceDialog::GetFindReplaceMsg(), OnFindReplaceMsg)
		MESSAGE_HANDLER(UWM_FIND,OnFind)
		MESSAGE_HANDLER(UWM_REPEAT,OnRepeat)
		MESSAGE_HANDLER(UWM_REPLACE,OnReplace)
		MESSAGE_HANDLER(UWM_GOTO,OnGoto)
		MESSAGE_HANDLER(UWM_SELECTALL,OnSelectAll)
		MESSAGE_HANDLER(UWM_DELETE,OnDelete)
		MESSAGE_HANDLER(UWM_UNDO,OnUndo)
		MESSAGE_HANDLER(UWM_REDO,OnRedo)
#ifdef STODO
		COMMAND_ID_HANDLER(ID_EDIT_FIND_PREVIOUS,OnEditFindPrevious)
#endif
		COMMAND_ID_HANDLER(ID_EDIT_COMMENTSELECTION,OnEditCommentSelection)
		COMMAND_ID_HANDLER(ID_EDIT_UNCOMMENTSELECTION,OnEditUncommentSelection)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	CString				m_strSearch, m_strReplace;
	long				m_iSearchFlags;

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFindReplaceMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnFind(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnRepeat(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnReplace(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnGoto(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSelectAll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDelete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnUndo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnRedo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnEditCommentSelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditUncommentSelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void SetupPlain() {
		SetLexer(SCLEX_NULL);
		SetStyleBits(7);

		// Default style
		StyleSetFore(STYLE_DEFAULT,RGB(0,0,0));
		//StyleSetBack(STYLE_DEFAULT,white);
		StyleSetBack(STYLE_DEFAULT,RGB(0xFF, 0xFB, 0xF0));
		//StyleSetSize(STYLE_DEFAULT,8);
		//StyleSetFont(STYLE_DEFAULT,"Courier New");
		StyleSetSize(STYLE_DEFAULT,CMyApp::m_prefs.m_nFontHeight);
		StyleSetFont(STYLE_DEFAULT,CT2A(CMyApp::m_prefs.m_strFontName));
		StyleClearAll();

		// Other
		SetIndent(CMyApp::m_prefs.m_nTabStopValue);
		SetTabWidth(CMyApp::m_prefs.m_nTabStopValue);
		SetCaretPeriod(400);
		SetIndentationGuides(CMyApp::m_prefs.m_bShowVerticalTabLines);
	}

	void SetupScheme(UINT uSchemeID) {
		for(int i=0;i<CScintillaPrefs::GetKeyWordsCount();i++)
			if(CScintillaPrefs::m_keywords[i].uSchemeID == uSchemeID)
				SetKeyWords(CScintillaPrefs::m_keywords[i].nSet,CT2A(CScintillaPrefs::m_keywords[i].pszKeyWords));

		for(int i=0;i<CScintillaPrefs::GetStyleCount();i++) {
			CScintillaPrefs::Style& style = CScintillaPrefs::m_styles[i];
			if(style.uSchemeID == uSchemeID) {
				if(style.cFore!=CLR_DEFAULT)
					StyleSetFore(style.uStyle,style.cFore);
				if(style.cBack!=CLR_DEFAULT)
					StyleSetBack(style.uStyle,style.cBack);
				StyleSetBold(style.uStyle,style.bBold);
				StyleSetItalic(style.uStyle,style.bItalic);
				StyleSetUnderline(style.uStyle,style.bUnderline);
				if(style.uStyle==STYLE_DEFAULT) {
					StyleSetSize(STYLE_DEFAULT,CMyApp::m_prefs.m_nFontHeight);
					StyleSetFont(STYLE_DEFAULT,CT2A(CMyApp::m_prefs.m_strFontName));
					StyleClearAll();
				}
			}
		}

		// Other
		SetIndent(CMyApp::m_prefs.m_nTabStopValue);
		SetTabWidth(CMyApp::m_prefs.m_nTabStopValue);
		SetCaretPeriod(400);
		SetIndentationGuides(CMyApp::m_prefs.m_bShowVerticalTabLines);
		SetMarginWidthN(0,CMyApp::m_prefs.m_bShowLineNumbers ? TextWidth(STYLE_LINENUMBER,"_9999") : 0);
		SetMarginTypeN(0,SC_MARGIN_NUMBER);
		SetMarginWidthN(1,0);
		SetMarginWidthN(2,0);
	}

	void SetupISS() {
		SetupScheme(1);
		SetLexerLanguage("iss");
	}

	void SetupCode() {
		SetupScheme(2);
		SetLexer(SCLEX_PASCAL);
		SetStyleBits(7);
	}
};
