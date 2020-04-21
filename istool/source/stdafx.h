// StdAfx.h

#pragma once

#define WINVER			0x0501
#define _WIN32_WINNT	0x0501		
#define _WIN32_IE		0x0501

// --------------------------------------------------------- WTL
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS

#include <atlstr.h>
#define _WTL_NO_WTYPES
#include <atltypes.h>
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlcoll.h>
#define _WTL_NO_CSTRING
#include <atlmisc.h>
#define _WTL_USE_CSTRING
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlctrlw.h>
#include <atldlgs.h>
#include <atlddx.h>
#include <atlcrack.h>
#include <atlsplit.h>
#include <atlprint.h>
#include <atlres.h>
#include <atlfile.h>
#include "Utils.h"
#include "misc.h"
#include "Thread.h"
#include "ToolTipDialog.h"
#include "ButtonFile.h"
#include "ButtonFolder.h"

// --------------------------------------------------------- WTL END

#include "InnoScript/InnoScript.h"
#include "InnoScript/Lines.h"

typedef CInnoScript::CLine					CScriptLine;
typedef CSimpleArray<CInnoScript::CLine*>	CScriptList;

#define SAFESTR(a)	((a)==NULL ? _T("") : (a))

#define VER_MAJOR		5
#define VER_MINOR		3
#define VER_REVISION	0
#define VER_BUILD		1

#define MY_EMAIL		_T("mailto:istool@istool.org")

#define ISVERSION		_T("5.3.0")

#include <htmlhelp.h>
#include <wininet.h>
#include "MyUtils.h"
#include "WebUpdate.h"

#pragma comment(lib,"htmlhelp.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"rpcrt4.lib")	// for UuidCreate

#include "Scintilla/AtlScintilla.h"
#include <AtlTheme.h>

typedef struct StatusBarInfo {
	CString	m_strRow, m_strCol;
} StatusBarInfo;

enum {
	UWM_FIND = WM_USER + 200,
	UWM_FINDPREV,
	UWM_REPLACE,
	UWM_NEWITEM,
	UWM_SELECTALL,
	UWM_DELETE,
	UWM_MOVEUP,
	UWM_MOVEDOWN,
	UWM_PROPERTIES,
	UWM_CUSTOMIZE,
	UWM_PRINT,
	UWM_CALLTIP,
	UWM_UNDO,
	UWM_REDO,
	UWM_GOTO,
	UWM_REPEAT,
	UWM_CREATEICON,
	UWM_STATUSBAR,
	UWM_COMMENTSELECTION,
	UWM_UPDATEUI
};

#include "DebugStruct.h"
