#include "stdafx.h"
#include "ScintillaPrefs.h"
#include "ISTool.h"

CScintillaPrefs::Scheme CScintillaPrefs::m_schemes[] = {
	1,	"Inno Setup Script",
	2,	"Code Section"
};

CScintillaPrefs::Style CScintillaPrefs::m_styles[] = {
	1,	"Default",		STYLE_DEFAULT,		RGB(0,0,0),		RGB(255,255,255),false,false,false,
	1,	"Comment",		SCE_ISS_COMMENT,	RGB(0,128,0),	CLR_DEFAULT,false,false,false,
	1,	"Directive",	SCE_ISS_DIRECTIVE,	RGB(0,0,255),	CLR_DEFAULT,false,false,false,
	1,	"Section",		SCE_ISS_SECTION,	RGB(0,0,0),		CLR_DEFAULT,true,false,false,
	1,	"Constant",		SCE_ISS_CONSTANT,	RGB(255,64,64), CLR_DEFAULT,false,false,false,
	1,	"String",		SCE_ISS_STRING,		RGB(96,96,96),	CLR_DEFAULT,false,false,false,
	1,	"Flag",			SCE_ISS_FLAG,		RGB(128,32,32),	CLR_DEFAULT,false,false,false,
	1,	"PreProcessor",	SCE_ISS_PREPROC,	RGB(0,128,255),	CLR_DEFAULT,false,false,false,

	2,	"Default",		STYLE_DEFAULT,		RGB(0,0,0),		RGB(255,255,255),false,false,false,
	2,	"Comment",		SCE_C_COMMENT,		RGB(0,128,0),	CLR_DEFAULT,false,false,false,
	2,	"Line Comment",	SCE_C_COMMENTLINE,	RGB(0,128,0),	CLR_DEFAULT,false,false,false,
	2,	"Word",			SCE_C_WORD,			RGB(0,0,255),	CLR_DEFAULT,false,false,false
};

CScintillaPrefs::KeyWords CScintillaPrefs::m_keywords[] = {
	// Inno Setup Script
	1,	0,	"setup files dirs registry",
	1,	1,	"ignoreversion comparetimestamp",
	1,	2,	"flags source destdir destname",
	// Code Section
	2,	0,	"program const type var begin continue end array set packed record string if then else "
			"while for to downto do with repeat until case of goto exit label procedure "
			"function nil file and or not xor div mod unit uses implementation interface "
			"external asm inline object constructor destructor virtual far assembler near "
			"inherited stdcall cdecl library export exports end. "
			"class ansistring raise try except on index name finally "
			"resourcestring false true initialization finalization "
			"override overload",
	2,	1,	"write read default public protected private property published stored"
};

long CScintillaPrefs::GetKeyWordsCount() {
	return sizeof(m_keywords) / sizeof(m_keywords[0]);
}

long CScintillaPrefs::GetStyleCount() {
	return sizeof(m_styles) / sizeof(m_styles[0]);
}

void CScintillaPrefs::SavePrefs(CMyApp& app) {
	for(int i=0;i<NUMSTYLES;i++) {
		CString strEntry;
		strEntry.Format("Style%X",i);

		Style s = m_styles[i];
		s.pszName = 0;
		app.WriteProfileBinary("Editor",strEntry,&s,sizeof s);
	}
}

void CScintillaPrefs::LoadPrefs(CMyApp& app) {
	for(int i=0;i<NUMSTYLES;i++) {
		CString strEntry;
		strEntry.Format("Style%X",i);

		Style s;
		ULONG nSize = sizeof s;
		if(!app.GetProfileBinary("Editor",strEntry,&s,&nSize) || nSize!=sizeof s)
			break;
		
		s.pszName = m_styles[i].pszName;
		m_styles[i] = s;
	}
}
