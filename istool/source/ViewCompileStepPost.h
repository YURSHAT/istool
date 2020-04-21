#pragma once

#include "ViewCompileStep.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStepPost view

class CViewCompileStepPost : public CViewCompileStep {
public:
	CViewCompileStepPost() : CViewCompileStep(CInnoScript::PRJ_POSTCOMPILESTEPS) {}
};
