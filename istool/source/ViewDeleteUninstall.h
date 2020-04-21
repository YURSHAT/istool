#pragma once

#include "ViewDelete.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDeleteUninstall view

class CViewDeleteUninstall : public CViewDelete {
public:
	CViewDeleteUninstall() : CViewDelete(m_sec = CInnoScript::SEC_UNINSTALLDELETE) {}
};
