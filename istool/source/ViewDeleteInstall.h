#pragma once

#include "ViewDelete.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDeleteInstall view

class CViewDeleteInstall : public CViewDelete {
public:
	CViewDeleteInstall() : CViewDelete(CInnoScript::SEC_INSTALLDELETE) {}
};
