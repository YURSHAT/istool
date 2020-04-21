// MyListView.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "MyListView.h"
#include "MainFrm.h"
#include "resource.h"
#include "DlgCustomize.h"
#include "StringToken.h"
#include "MyDoc.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CMyListView

CMyListView::CMyListView(CInnoScript::SECTION sec) : m_dwFlags(0), CMyView<CMyListView>(sec) {
	m_nCommentImage = 11;
	m_nItemImage = 0;
}

CMyListView::~CMyListView() {
	while(m_columnList.GetSize()) {
		delete m_columnList[0];
		m_columnList.RemoveAt(0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyListView message handlers

LRESULT CMyListView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	DefWindowProc();
	SetImageList(CMyApp::m_imageList,LVSIL_SMALL);
	SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	const CListInfo* pInfo = GetListInfo();
	if(pInfo) {
		while(pInfo->m_pszParameter) {
			AddColumn(pInfo->m_pszParameter,pInfo->m_nFormat,pInfo->m_nWidth,pInfo->m_dwFlags & CLI_HIDE ? false : true);
			pInfo++;
		}
		DoDisplay();
	}

	if(m_dwFlags & VFL_DRAGACCEPTFILES)
		DragAcceptFiles(TRUE);
	
//	bHandled = FALSE;
	return 0;
}

LRESULT CMyListView::OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	CPoint point(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (point.x == -1 && point.y == -1){
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP));

	CMenuHandle pPopup = menu.GetSubMenu(m_nSubMenu);
	ATLASSERT(pPopup != NULL);
	_L(pPopup,"Popup");

	AfxGetMainWnd().OnIdle();
	AfxGetMainWnd().TrackPopupMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y);

	return 0;
}

LRESULT CMyListView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	WriteDisplayInfo();
	return 0;
}

void CMyListView::OnFinalMessage(HWND /*hWnd*/) {
	delete this;
}

LRESULT CMyListView::OnDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/) {
	// Send message to make childs handle it
	SendMessage(UWM_PROPERTIES);
	return 0;
}

LRESULT CMyListView::OnCustomDraw(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pnmh);

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	switch(pLVCD->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
        return CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
	{
		LVITEM item = {0};
		item.mask = LVIF_IMAGE;
		item.iItem = pLVCD->nmcd.dwItemSpec;
		GetItem(&item);

		if(item.iImage==11) {
			CDC dc(pLVCD->nmcd.hdc);
			if(!m_boldFont.m_hFont) {
				CFontHandle pFont = dc.GetCurrentFont();
				LOGFONT logfont;
				pFont.GetLogFont(&logfont);
				logfont.lfWeight = FW_BOLD;
				m_boldFont.CreateFontIndirect(&logfont);
			}
			dc.SelectFont(m_boldFont);
			dc.Detach();
            pLVCD->clrText = RGB(0,128,0);
			return CDRF_NEWFONT;
		} else
			return CDRF_DODEFAULT;
		break;
	}
	default:
		return CDRF_DODEFAULT;
		break;
	}
}

LRESULT CMyListView::OnCut(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CString strClip;
	UINT nItem = GetItemCount();
	while(nItem--)
		if(GetItemState(nItem,LVIS_SELECTED)==LVIS_SELECTED) {
			CScriptLine* pBase = reinterpret_cast<CScriptLine*>(GetItemData(nItem));

			CString str;
			pBase->Write(str.GetBuffer(5000),5000);
			str.ReleaseBuffer();
			strClip += str;
			strClip += "\r\n";
		}

	if(OpenClipboard()) {
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,strClip.GetLength()+1);
		LPVOID lp = GlobalLock(hGlobal);
		_tcscpy(reinterpret_cast<LPSTR>(lp),strClip);
		GlobalUnlock(lp);
		EmptyClipboard();
		SetClipboardData(CF_TEXT,hGlobal);
		CloseClipboard();

		SendMessage(UWM_DELETE);
	} else {
		AtlMessageBox(m_hWnd,_L("Failed to open clipboard."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}

	return 0;
}

LRESULT CMyListView::OnCopy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CString strClip;
	UINT nItem = GetItemCount();
	while(nItem--)
		if(GetItemState(nItem,LVIS_SELECTED)==LVIS_SELECTED) {
			CScriptLine* pBase = reinterpret_cast<CScriptLine*>(GetItemData(nItem));

			CString str;
			pBase->Write(str.GetBuffer(5000),5000);
			str.ReleaseBuffer();
			strClip += str;
			strClip += "\r\n";
		}

	if(OpenClipboard()) {
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,strClip.GetLength()+1);
		LPVOID lp = GlobalLock(hGlobal);
		_tcscpy(reinterpret_cast<LPSTR>(lp),strClip);
		GlobalUnlock(lp);
		EmptyClipboard();
		SetClipboardData(CF_TEXT,hGlobal);
		CloseClipboard();
	} else {
		AtlMessageBox(m_hWnd,_L("Failed to open clipboard."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}

	return 0;
}

LRESULT CMyListView::OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(!OpenClipboard()) {
		AtlMessageBox(m_hWnd,_L("Failed to open clipboard."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return 0;
	}

	HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
	LPVOID lp = GlobalLock(hGlobal);
	CString str(reinterpret_cast<LPCTSTR>(lp));
	GlobalUnlock(lp);
	CloseClipboard();

	CStringToken token(str,"\n");
	bool bModified = false;
	do {
		LPCTSTR lpsz = token.GetNext();
		if(!lpsz) break;
		CString str(lpsz);
		if(str.IsEmpty()) continue;

		CScriptLine* pLine = NULL;
		try {
			if(m_sec==CInnoScript::SEC_FILES) {
				// TODO: differ between dirs and files
				pLine = new CInnoScript::CLineParam(m_sec,str);
			} else if(m_sec==CInnoScript::SEC_MESSAGES) {
				pLine = new CInnoScript::CLineSetup(m_sec,str);
			} else {
				pLine = new CInnoScript::CLineParam(m_sec,str);
			}
			GetDocument()->GetScript().AddLine(pLine);
			bModified = true;
			continue;
		} catch(...) {
			if(pLine) delete pLine;
		}
	} while(true);

	if(bModified) {
		Populate();
		GetDocument()->SetModifiedFlag();
	} else {
		AtlMessageBox(m_hWnd,_L("Incorrect clipboard format."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}

	return 0;
}

void CMyListView::WriteRegistryInfo(WORD* pDisplayInfo,UINT nCount) {
	CString strClassName;
	strClassName.Format("LVx%02X",GetSection());
	LPCTSTR lpszName = strClassName;

	CString str;
	str.Format("%sColumns",lpszName);
	AfxGetApp()->WriteProfileBinary("Settings",str,(LPBYTE)pDisplayInfo,nCount*sizeof WORD);

	int iOrderArray[20] = {0};
	UINT nVisibleCount = 0;
	for(UINT n=1;n<nCount;n++)
		if(pDisplayInfo[n] & 0x8000)
			++nVisibleCount;

	if(!GetColumnOrderArray(nVisibleCount,iOrderArray)) {
		AtlMessageBox(m_hWnd,_L("Error|GetColumnOrder","Failed to get column order."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}
	str.Format("%sOrder",lpszName);
	AfxGetApp()->WriteProfileBinary("Settings",str,(LPBYTE)iOrderArray,nVisibleCount*sizeof (int));
}

bool CMyListView::ReadRegistryInfo(WORD* pDisplayInfo,UINT nCount) {
	CString strClassName;
	strClassName.Format("LVx%02X",GetSection());
	LPCTSTR lpszName = strClassName;

	CString str;
	str.Format("%sColumns",lpszName);
	LPWORD pInfo = new WORD[nCount];
	ULONG nCount2 = nCount * sizeof WORD;
	if(!AfxGetApp()->GetProfileBinary("Settings",str,pInfo,&nCount2))
		return false;
	if(nCount2!=nCount*sizeof WORD) {
		delete []pInfo;
		return false;
	}

	memcpy(pDisplayInfo,pInfo,nCount2);
	delete []pInfo;
	return true;
}

bool CMyListView::ReadColumnOrderInfo(WORD* pDisplayInfo,UINT nCount) {
	CString strClassName;
	strClassName.Format("LVx%02X",GetSection());
	LPCTSTR lpszName = strClassName;

	UINT nVisibleCount = 0;
	for(UINT n=1;n<nCount;n++)
		if(pDisplayInfo[n] & 0x8000)
			++nVisibleCount;

	CString str;
	LPINT pInfo = new INT[nVisibleCount];
	ULONG nCount2 = sizeof(INT) * nVisibleCount;

	str.Format("%sOrder",lpszName);
	if(!AfxGetApp()->GetProfileBinary("Settings",str,pInfo,&nCount2)) {
		return false;
	}
	if(nCount2 != sizeof(int)*nVisibleCount) {
		delete []pInfo;
		return false;
	}

	SetColumnOrderArray(nVisibleCount,(LPINT)pInfo);

	delete []pInfo;
	return true;
}

int CMyListView::GetDisplayColumn(int nCol) {
	long nPos = 0;
	CColumnInfo *pInfo = m_columnList[nPos];
	for(int nVisible=0;nPos<m_columnList.GetSize() && nVisible<nCol;nVisible++) {
		pInfo = m_columnList[nPos];
	}
	if(pInfo && !pInfo->m_bVisible) return -1;

	int nDisplayCol = 0, nTempCol = 0;

	for(nPos=0;nPos<m_columnList.GetSize(),nTempCol<nCol;nPos++,nTempCol++) {
		CColumnInfo* pInfo = m_columnList[nPos];
		if(pInfo->m_bVisible) nDisplayCol++;
	}
	return nDisplayCol;
}

void CMyListView::AddColumn(LPCTSTR lpszName,int nFormat,int nWidth,bool bVisible) {
	CColumnInfo* pInfo = new CColumnInfo;
	_tcscpy(pInfo->m_szTitle,lpszName);
	pInfo->m_nFormat = nFormat;
	pInfo->m_bVisible = bVisible;
	pInfo->m_nWidth = nWidth;
	m_columnList.Add(pInfo);
}

void CMyListView::DoDisplay(bool bDontRead/*=false*/) {
	WORD	wDisplayInfo[20];

	if(!bDontRead) {
		if(ReadRegistryInfo(wDisplayInfo,m_columnList.GetSize()+1)) {
			if(wDisplayInfo[0] == m_columnList.GetSize()) {
				UINT nPos = 1;
				for(int uPos=0;uPos<m_columnList.GetSize();uPos++) {
					CColumnInfo* pInfo = m_columnList[uPos];
					pInfo->m_bVisible = wDisplayInfo[nPos] & 0x8000 ? true : false;
					pInfo->m_nWidth = wDisplayInfo[nPos] & 0x7FFF;
					if(pInfo->m_nWidth>999) pInfo->m_nWidth = 200;
					nPos++;
				}
			}
		}
	}

	UINT nCol = 0;
	for(int nPos=0;nPos<m_columnList.GetSize();nPos++) {
		CColumnInfo* pInfo = m_columnList[nPos];
		if(pInfo->m_bVisible) {
			if(pInfo->m_nWidth<1) pInfo->m_nWidth = 100;
			int nCol2 = InsertColumn(nCol++,pInfo->m_szTitle,pInfo->m_nFormat,pInfo->m_nWidth,0);
			ATLASSERT(nCol2>=0);
		}
	}

	if(!bDontRead)
		ReadColumnOrderInfo(wDisplayInfo,m_columnList.GetSize()+1);
}

void CMyListView::WriteDisplayInfo() {
	WORD	wDisplayInfo[20];

	wDisplayInfo[0] = m_columnList.GetSize();
	UINT nPos = 1;
	UINT nDisplayColumn = 0;
	for(long nPos2=0;nPos2<m_columnList.GetSize();nPos2++) {
		CColumnInfo* pInfo = m_columnList[nPos2];
		if(pInfo->m_bVisible) {
			wDisplayInfo[nPos] = GetColumnWidth(nDisplayColumn++);
#if 1	// Bug i VC som optimaliserer bort et eller annet uten denne linja?
			//if(GetSection()==CInnoScript::SEC_FILES && !nPos2) {
			if(true) {
				int nTmp1 = wDisplayInfo[1] & 0x7FFF;
				CString str;
				str.Format("%d",nTmp1);
				//::MessageBox(NULL,str,"DEBUG",MB_OK);
				nTmp1 *= 1;
			}
			CString tmp;
#endif
			wDisplayInfo[nPos] |= 0x8000;
		} else {
			wDisplayInfo[nPos] = pInfo->m_nWidth;
		}
		nPos++;
	}
	WriteRegistryInfo(wDisplayInfo,nPos);
}

void CMyListView::SetItemText(int nItem,int nDisplayColumn,LPCTSTR lpszText) {
	int nRealCol = GetDisplayColumn(nDisplayColumn);
	if(nRealCol<0) return;

	CListViewCtrl::SetItemText(nItem,nRealCol,lpszText);
}

LRESULT CMyListView::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
	UINT nChar = wParam;
	if(nChar==VK_RETURN)
		SendMessage(UWM_PROPERTIES);
	else if(nChar==VK_DELETE)
		SendMessage(UWM_DELETE);
	else
		bHandled = FALSE;

	return 0;
}

UINT CMyListView::GetSelectedCount() {
	UINT nCount = 0;

	for(int nItem=0;nItem<GetItemCount();nItem++) {
		if(GetItemState(nItem,LVIS_SELECTED)==LVIS_SELECTED) {
			nCount++;
		}
	}

	return nCount;
}

bool CMyListView::MoveRow(int from, int to)
{
	//Can't move to the same place, or from or to a negative index
	if(from == to || from < 0 || to < 0)
		return FALSE;

	//First Copy the row to the new location
	if(CopyRow(from, to))
	{
		//If we have just inserted a row before
		//this one in the list, we need to increment
		//our index.
		if(from > to)
			DeleteItem(from + 1);
		else
			DeleteItem(from);

		return true;
	}
	else
		return false;
}

bool CMyListView::CopyRow(int from, int to)
{	
	//Can't move to the same place, or from or to a negative index
	if(from == to || from < 0 || to < 0)
		return false;

	//Copy the row to the new index
	CString strTmp;
	GetItemText(from,0,strTmp);
	CListViewCtrl::InsertItem(to, strTmp);

	//If row has been inserted before original
	//increment the original
	if(from > to)
		from++;

	LVITEM lvitem;
	memset(&lvitem,0,sizeof lvitem);
	lvitem.mask = LVIF_IMAGE|LVIF_PARAM;
	lvitem.iItem = from;
	GetItem(&lvitem);
	lvitem.iItem = to;
	SetItem(&lvitem);

	//SetItemData(to, GetItemData(from));
	SetItemTexts(to);

	return true;
}

void CMyListView::Populate() {
	DeleteAllItems();

	CScriptList	messages;
	GetDocument()->GetScript().GetList(m_sec,messages);

	for(int nPos=0;nPos<messages.GetSize();nPos++)
		InsertItem(messages[nPos]);
}

UINT CMyListView::InsertItem(CInnoScript::CLine* pLine) {
	int nItem = CListViewCtrl::InsertItem(GetItemCount(),NULL,pLine->GetComment() ? m_nCommentImage : m_nItemImage);
	SetItemData(nItem,(DWORD)pLine);
	SetItemTexts(nItem);
	return nItem;
}

const CListInfo* CMyListView::GetListInfo() {
	return NULL;
}

void CMyListView::SetItemTexts(UINT nItem) {
	CScriptLine* pLine = (CScriptLine*)GetItemData(nItem);

	if(pLine->GetComment()) {
		SetItemText(nItem,0,pLine->GetComment());
	} else {
		const CListInfo* pInfo = GetListInfo();
		UINT nColumn = 0;
		while(pInfo && pInfo->m_pszParameter) {
			SetItemText(nItem,nColumn++,pLine->GetParameter(pInfo->m_pszParameter));
			pInfo++;
		}
	}
}

void CMyListView::UpdateView() {
	Populate();
}

bool CMyListView::ApplyView() {
	UINT nCount = GetItemCount();
	long nLine = -1;
	for(UINT nItem=0;nItem<nCount;nItem++) {
		// Only selected items
		if(GetItemState(nItem,LVIS_FOCUSED)==LVIS_FOCUSED) {
			CScriptLine* pLine = (CScriptLine*)GetItemData(nItem);
			nLine = GetDocument()->GetScript().GetLineNumber(pLine);
			break;
		}
	}
	if(nLine<0) nLine = GetDocument()->GetScript().GetLineNumber(GetSection());
	GetDocument()->m_nScriptLine = nLine;
	return true;
}

void CMyListView::OnUpdate(LONG lHint,void* pParam) {
	CMyView<CMyListView>::OnUpdate(lHint,pParam);
	switch(lHint) {
	case CUpdate::HINT_INITVIEW:
		UpdateView();
		break;
	}
}

LRESULT CMyListView::OnUpdateUI(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	switch(wParam) {
		case ID_EDIT_NEWITEM:
			return 1;
		case ID_EDIT_MOVEUP:
		case ID_EDIT_MOVEDOWN:
		case ID_EDIT_CUT:
		case ID_EDIT_COPY:
		case ID_EDIT_DELETEITEM:
		case ID_VIEW_PROPERTIES:
			return GetSelectedCount()>0;
		case ID_EDIT_SELECT_ALL:
			return GetItemCount()>0;
		case ID_EDIT_PASTE:
			return IsClipboardFormatAvailable(CF_TEXT);
		case ID_VIEW_CUSTOMIZE:
			return 1;
	}
	return 0;
}

LRESULT CMyListView::OnNewItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CScriptList	list;
	CScriptLine* pItem;
	
	if(m_dwFlags & VFL_SETUP) pItem = new CInnoScript::CLineSetup(m_sec);
	else pItem = new CScriptLine(m_sec);

	list.Add(pItem);

	if(!CSheets::ShowSheet(m_hWnd,list,true)) {
		delete pItem;
		return 0;
	}

	CWaitCursor wait;
	GetDocument()->GetScript().AddLine(pItem);
	InsertItem(pItem);
	GetDocument()->SetModifiedFlag();

	if(GetItemCount()==1 && GetSection()==CInnoScript::PRJ_DOWNLOAD) {
		GetDocument()->GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_CODE,"// Function generated by ISTool."));
		GetDocument()->GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_CODE,"function NextButtonClick(CurPage: Integer): Boolean;"));
		GetDocument()->GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_CODE,"begin"));
		GetDocument()->GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_CODE,"	Result := istool_download(CurPage);"));
		GetDocument()->GetScript().AddLine(new CInnoScript::CLineComment(CInnoScript::SEC_CODE,"end;"));
	}

	return 0;
}

LRESULT CMyListView::OnSelectAll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	LV_ITEM theItem;

	theItem.mask		= LVIF_STATE;
	theItem.iItem		= -1;
	theItem.iSubItem	= 0;
	theItem.state		= LVIS_SELECTED;
	theItem.stateMask	= 2;

	SetItemState(-1, &theItem);
	return 0;
}

LRESULT CMyListView::OnDelete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	SetRedraw(FALSE);
	UINT nPos = GetItemCount();
	while(nPos--)
		if(GetItemState(nPos,LVIS_SELECTED)==LVIS_SELECTED) {
			CScriptLine* p = (CScriptLine*)GetItemData(nPos);
			OnDeleteItem(p);
			GetDocument()->GetScript().DeleteLine(p);
			DeleteItem(nPos);
		}
	GetDocument()->SetModifiedFlag();
	SetRedraw(TRUE);
	RedrawWindow();
	return 0;
}

LRESULT CMyListView::OnMoveUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	UINT nCount = GetItemCount();
	for(UINT nItem=0;nItem<nCount;nItem++) {
		// Only selected items
		if(!GetItemState(nItem,LVIS_SELECTED)) continue;

		// Already at top
		if(nItem==0) continue;

		// Don't move up if previous item is selected
		if(GetItemState(nItem-1,LVIS_SELECTED)) continue;

		CScriptLine* pLine = (CScriptLine*)GetItemData(nItem);
		CScriptLine* pLinePrev = (CScriptLine*)GetItemData(nItem-1);
		// Don't move between sections
		if(!pLine || !pLinePrev || pLine->GetSection()!=pLinePrev->GetSection()) continue;

		GetDocument()->GetScript().MoveUp(pLine);

		GetDocument()->SetModifiedFlag();
		MoveRow(nItem,nItem-1);
		SetItemState(nItem-1,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
	}

	return 0;
}

LRESULT CMyListView::OnMoveDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	UINT nCount = GetItemCount();
	UINT nItem = nCount;
	while(nItem--) {
		// Only selected items
		if(!GetItemState(nItem,LVIS_SELECTED)) continue;

		// Already at bottom
		if(nItem>=nCount-1) continue;

		// Don't move down if next item is selected
		if(GetItemState(nItem+1,LVIS_SELECTED)) continue;

		CScriptLine* pLine = (CScriptLine*)GetItemData(nItem);
		CScriptLine* pLineNext = (CScriptLine*)GetItemData(nItem+1);
		// Don't move between sections
		if(!pLine || !pLineNext || pLine->GetSection()!=pLineNext->GetSection()) continue;

		GetDocument()->GetScript().MoveDown(pLine);

		MoveRow(nItem,nItem+2);
		GetDocument()->SetModifiedFlag();
		SetItemState(nItem+1,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
	}

	return 0;
}

LRESULT CMyListView::OnProperties(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(AfxGetMainWnd().UIGetState(ID_VIEW_PROPERTIES) & CUpdateUIBase::UPDUI_DISABLED)
		return 0;

	CScriptList	list;

	UINT nItem = GetItemCount();
	while(nItem--)
		if(GetItemState(nItem,LVIS_SELECTED)==LVIS_SELECTED) {
			CScriptLine* pLine = (CScriptLine*)GetItemData(nItem);
			list.Add(pLine);
		}

	if(!CSheets::ShowSheet(m_hWnd,list,false))
		return 0;

	nItem = GetItemCount();
	while(nItem--)
		if(GetItemState(nItem,LVIS_SELECTED)==LVIS_SELECTED)
			SetItemTexts(nItem);

	GetDocument()->SetModifiedFlag();

	return 0;
}

LRESULT CMyListView::OnCustomize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CDlgCustomize dlg(*this);
	if(dlg.DoModal(AfxGetMainWnd())==IDOK) {
		DeleteAllItems();
		while(DeleteColumn(0));
		DoDisplay(true);
		Populate();
	}
	return 0;
}
