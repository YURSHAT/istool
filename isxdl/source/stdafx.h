// Copyright © 2002 Bjørnar Henden
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// --------------------------------------------------------- WTL
#include <atlstr.h>
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlutil.h>
#include <atlfile.h>
#include <atltypes.h>
#include "Thread.h"

// --------------------------------------------------------- Internet
#include <wininet.h>

#undef ASYNC
#undef USE_RESUME
