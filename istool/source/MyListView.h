#pragma once

#include "MyView.h"

typedef struct {
	LPCTSTR		m_pszParameter;
	UINT		m_nFormat;
	UINT		m_nWidth;
	DWORD		m_dwFlags;
} CListInfo;

enum {
	CLI_HIDE	= 1,	// Default hidden
};

/////////////////////////////////////////////////////////////////////////////
// CMyListView view

class CMyListView : public CWindowImpl<CMyListView, CListViewCtrl>, public CMyView<CMyListView> {
	friend class CDlgCustomize;
public:
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	CMyListView(CInnoScript::SECTION sec);           // protected constructor used by dynamic creation
	virtual ~CMyListView();
	virtual void UpdateView();
	virtual bool ApplyView();

protected:
	UINT					m_nCommentImage;
	UINT					m_nItemImage;

public:
	CScriptLine* GetSelectedEntry();

	UINT GetSelectedCount();
	bool MoveRow(int from, int to);
	bool CopyRow(int from, int to);

	// Generated message map functions
protected:
	virtual void Populate();
	virtual UINT InsertItem(CInnoScript::CLine* pLine);
	virtual void SetItemTexts(UINT nItem);
	virtual const CListInfo* GetListInfo();

	typedef struct {
		CHAR		m_szTitle[32];
		int			m_nFormat;
		bool		m_bVisible;
		int			m_nWidth;
	} CColumnInfo;
	CSimpleArray<CColumnInfo*>			m_columnList;
	void AddColumn(LPCTSTR lpszName,int nFormat,int nWidth,bool bVisible);
	void DoDisplay(bool bDontRead=false);
	void WriteDisplayInfo();
	void SetItemText(int nItem,int nDisplayColumn,LPCTSTR lpszText);

	void WriteRegistryInfo(WORD* pDisplayInfo,UINT nCount);
	bool ReadRegistryInfo(WORD* pDisplayInfo,UINT nCount);
	bool ReadColumnOrderInfo(WORD* pDisplayInfo,UINT nCount);
	int GetDisplayColumn(int nCol);


	CFont		m_boldFont;

	DWORD	m_dwFlags;	
	enum {
		VFL_DRAGACCEPTFILES	= 0x01,	// The section supports drag and drop
		VFL_SETUP			= 0x02	// For setup and messages section
	};

	BEGIN_MSG_MAP(CMyListView)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_CONTEXTMENU,OnContextMenu)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
		MESSAGE_HANDLER(WM_CUT,OnCut)
		MESSAGE_HANDLER(WM_COPY,OnCopy)
		MESSAGE_HANDLER(WM_PASTE,OnPaste)
		MESSAGE_HANDLER(UWM_NEWITEM,OnNewItem)
		MESSAGE_HANDLER(UWM_SELECTALL,OnSelectAll)
		MESSAGE_HANDLER(UWM_DELETE,OnDelete)
		MESSAGE_HANDLER(UWM_MOVEUP,OnMoveUp)
		MESSAGE_HANDLER(UWM_MOVEDOWN,OnMoveDown)
		MESSAGE_HANDLER(UWM_PROPERTIES,OnProperties)
		MESSAGE_HANDLER(UWM_CUSTOMIZE,OnCustomize)
		MESSAGE_HANDLER(UWM_UPDATEUI,OnUpdateUI)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_DBLCLK,OnDoubleClick)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW,OnCustomDraw)
		CHAIN_MSG_MAP(CMyView<CMyListView>)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnCustomDraw(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCut(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCopy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNewItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSelectAll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDelete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoveUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoveDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnProperties(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCustomize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateUI(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	virtual void OnFinalMessage(HWND /*hWnd*/);

protected:
	virtual void OnUpdate(LONG lHint,void* pParam);
	virtual void OnDeleteItem(CScriptLine* pLine) {}
};
