#pragma once

#include "ViewCompileStep.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStepPre view

class CViewCompileStepPre : public CViewCompileStep {
public:
	CViewCompileStepPre() : CViewCompileStep(CInnoScript::PRJ_PRECOMPILESTEPS) {}
};
