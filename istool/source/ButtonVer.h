#pragma once

enum VERTYPE {
	VER_WIN,
	VER_NT,
};

namespace WTL {

class CButtonVer : public CWindowImpl<CButtonVer,CButton> {
public:
	BEGIN_MSG_MAP(CButtonVer)
		REFLECTED_COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
	END_MSG_MAP()

	CButtonVer(VERTYPE type) : m_type(type) {}

	LRESULT OnClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
	const VERTYPE	m_type;
};

};
