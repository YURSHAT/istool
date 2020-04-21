#pragma once

#include "ViewRun.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRunUninstall view

class CViewRunUninstall : public CViewRun {
public:
	CViewRunUninstall() : CViewRun(CInnoScript::SEC_UNINSTALLRUN) {}
};
