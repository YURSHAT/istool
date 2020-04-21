#pragma once

enum {
	SCE_ISS_DEFAULT = 0,
	SCE_ISS_COMMENT,
	SCE_ISS_DIRECTIVE,
	SCE_ISS_SECTION,
	SCE_ISS_CONSTANT,
	SCE_ISS_FLAG,
	SCE_ISS_ERROR,
	SCE_ISS_STRING,
	SCE_ISS_ARGUMENT,
	SCE_ISS_PREPROC
};

class CMyApp;

class CScintillaPrefs {
public:
	typedef struct {
		UINT		uSchemeID;
		LPCTSTR		pszName;
	} Scheme;
	
	typedef struct {
		UINT		uSchemeID;
		LPCTSTR		pszName;
		UINT		uStyle;
		COLORREF	cFore;
		COLORREF	cBack;
		bool		bBold;
		bool		bItalic;
		bool		bUnderline;
	} Style;

	typedef struct {
		UINT		uSchemeID;
		UINT		nSet;
		LPCTSTR		pszKeyWords;
	} KeyWords;

	static Scheme	m_schemes[];
	static Style	m_styles[];
	static KeyWords	m_keywords[];

	enum {
		NUMSTYLES = 12,
		STYLESIZE = NUMSTYLES * sizeof Style
	};

	static long GetKeyWordsCount();
	static long GetStyleCount();
	static void SavePrefs(CMyApp& app);
	static void LoadPrefs(CMyApp& app);
};
