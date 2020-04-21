// SectionView.h

#pragma once

class CSectionView {
public:
	virtual void InsertHeader(LPCTSTR pszTitle) = 0;
	virtual void InsertItem(LPCTSTR pszTitle,UINT nID,int iImage) = 0;
	virtual HWND Create(HWND hParent) = 0;
	virtual void SelectSection(UINT nID) = 0;
	virtual HWND GetWindow() = 0;
};

class CSectionTree : public CWindowImpl<CSectionTree, CTreeViewCtrl>, public CSectionView {
public:
	typedef CWindowImpl<CSectionTree, CTreeViewCtrl> baseClass;

	DECLARE_WND_SUPERCLASS(NULL, CTreeViewCtrl::GetWndClassName())

	BEGIN_MSG_MAP(CSectionList)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		REFLECTED_NOTIFY_CODE_HANDLER(TVN_SELCHANGING, OnSelChanging)
		REFLECTED_NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnSelChanged)
		REFLECTED_NOTIFY_CODE_HANDLER(TVN_ITEMEXPANDING, OnItemExpanding)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		bHandled = FALSE;

		DWORD dwMajor = 0;
		DWORD dwMinor = 0;
		HRESULT hRet = AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
		bool bXP = SUCCEEDED(hRet) && dwMajor>=6;

		if(bXP) {
			HIMAGELIST hImageList = ImageList_LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_IMAGELIST32), 16, 1, CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			SetImageList(hImageList,TVSIL_NORMAL);
		} else {
			CImageList imageList;
			imageList.Create(16,16,ILC_MASK|ILC_COLOR16,8,1);
			CBitmap bm;
			bm.LoadBitmap(IDB_IMAGELIST16);
			imageList.Add(bm.Detach(),RGB(255,255,255));
			SetImageList(imageList.Detach(),TVSIL_NORMAL);
		}
		return lRet;
	}

	LRESULT OnSelChanging(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
		LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pnmh;
		if(pnmtv->itemNew.lParam)
			return 0;
		else
			return 1;
	}

	LRESULT OnSelChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
		LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pnmh;
		if(pnmtv->itemNew.lParam)
			::SendMessage(AfxGetMainHWnd(),WM_COMMAND,pnmtv->itemNew.lParam,0);

		return 0;
	}

	LRESULT OnItemExpanding(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
		LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pnmh;
		if(pnmtv->action!=TVE_EXPAND)
			return 1;
		else
			return 0;
	}

	CSectionTree() : m_hRoot(TVI_ROOT) {}

	void InsertHeader(LPCTSTR pszHeader) {
		m_hRoot = baseClass::InsertItem(pszHeader,14,14,TVI_ROOT,TVI_LAST);
		baseClass::SetItemData(m_hRoot,0);
	}

	void InsertItem(LPCTSTR pszName,UINT nID,int iImage=3) {
		HTREEITEM hItem = baseClass::InsertItem(pszName,iImage,iImage,m_hRoot,TVI_LAST);
		baseClass::Expand(m_hRoot,TVE_EXPAND);
		baseClass::SetItemData(hItem,nID);
	}

	HWND Create(HWND hParent) {
		return baseClass::Create(hParent, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN|TVS_HASLINES|TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	}

	virtual HWND GetWindow() {
		return m_hWnd;
	}

	void SelectSection(UINT nID) {
		FindAndSelectItem(TVI_ROOT,nID);
	}

protected:
	HTREEITEM	m_hRoot;

	bool FindAndSelectItem(HTREEITEM hParent,UINT nID) {
		HTREEITEM hItem = GetChildItem(hParent);
		while(hItem) {
			if(GetItemData(hItem)==nID) {
				SelectItem(hItem);
				return true;
			}
			if(FindAndSelectItem(hItem,nID)) 
				return true;
			hItem = GetNextSiblingItem(hItem);
		}
		return false;
	}
};
