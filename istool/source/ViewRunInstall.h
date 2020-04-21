#pragma once

#include "ViewRun.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRunInstall view

class CViewRunInstall : public CViewRun {
public:
	CViewRunInstall() : CViewRun(CInnoScript::SEC_RUN) {}
};
