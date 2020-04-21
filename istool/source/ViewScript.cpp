// ViewScript.cpp : implementation of the CViewScript class
//

#include "stdafx.h"
#include "ISTool.h"
#include "MyDoc.h"
#include "ViewScript.h"
#include "MainFrm.h"
#include "DlgGoto.h"
#include <vector>
#include <string>
#include <algorithm>
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewScript construction/destruction

CViewScript::CViewScript() : CMyView<CViewScript>(CInnoScript::PRJ_ISTOOL) {
	//SetSelectionMargin(FALSE);
	m_bUpdateModified = true;
	m_bModified = false;
}

/////////////////////////////////////////////////////////////////////////////
// CViewScript message handlers

LRESULT CViewScript::OnSavePointReached(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/) {
	m_bModified = false;
	return 0;
}

LRESULT CViewScript::OnSavePointLeft(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/) {
	if(m_bUpdateModified)
		AfxGetMainWnd().m_document.SetModifiedFlag(true);
	m_bModified = true;
	return 0;
}

void CViewScript::UpdateView() {
	m_bUpdateModified = false;
	if(m_bCode) {
		CString str;
		CScriptList list;
		GetDocument()->GetScript().GetList(CInnoScript::SEC_CODE,list);
		for(int nPos=0;nPos<list.GetSize();nPos++) {
			CScriptLine* pLine = list[nPos];
			char szLine[5000];

			// Write line
			pLine->Write(szLine,5000);
			str += szLine;
			//pLine = pLine->GetNext();
			//if(pLine) str += "\r\n";
			if(GetDocument()->GetScript().GetCount()-1>nPos)
				str += "\r\n";
		}
		SetText(CT2A(str));
	} else {
		CInnoScript::SECTION sec = CInnoScript::SEC_NONE;

		CString str;
		char szLine[5000];
		for(long i=0;i<GetDocument()->GetScript().GetCount();i++) {
			CScriptLine* pLine = GetDocument()->GetScript()[i];
			// New section?
			if(pLine->GetSection()!=sec) {
#if 0
				if(sec!=CInnoScript::SEC_NONE) {
					sprintf(szLine,"[/%s]",CInnoScript::GetSectionName(sec));
					str += szLine;
					str += "\r\n";
				}
#endif
				sec = pLine->GetSection();
				if(sec!=CInnoScript::SEC_NONE) {
					sprintf(szLine,"[%s]",CInnoScript::GetSectionName(sec));
					str += szLine;
					str += "\r\n";
				}
			}

			// Write line
			pLine->Write(szLine,5000);
			str += szLine;
			if(i+1<GetDocument()->GetScript().GetCount()) str += "\r\n";
		}
#if 0
		if(sec!=CInnoScript::SEC_NONE) {
			sprintf(szLine,"\r\n[/%s]",CInnoScript::GetSectionName(sec));
			str += szLine;
		}
#endif

		SetText(CT2A(str));
		if(GetDocument()->m_nScriptLine>=0)
			SetTimer(987,10);
	}
	EmptyUndoBuffer();
	SetSavePoint();
	m_bUpdateModified = true;
}

bool CViewScript::ApplyView() {
	if(m_bCode)
		GetDocument()->m_nScriptLine = GetDocument()->GetScript().GetLineNumber(GetSection());
	
	if(m_bModified) {
		CWaitCursor wait;
		if(m_bCode) {
			// First delete all of the old script
			CScriptList list;
			AfxGetDocument()->GetScript().GetList(CInnoScript::SEC_CODE,list);
			for(int nPos=0;nPos<list.GetSize();nPos++) {
				CScriptLine* pLine = list[nPos];
				AfxGetDocument()->GetScript().DeleteLine(pLine);
			}

			UINT nLine = 0;
			CHAR szLine[8192];
			UINT nLineCount = GetLineCount();
			bool bRemoveSection = true;
			for(UINT n=0;n<nLineCount;n++) {
				UINT nLineLength = GetLine(n,szLine);
				szLine[nLineLength] = 0;
				CScriptLine* pLine = NULL;
				nLine++;
				int nLength = _tcslen(szLine);
				while(iswspace(szLine[nLength-1]))
					szLine[--nLength] = 0;

				if(bRemoveSection) {
					if(!szLine[0]) continue;
					bRemoveSection = false;
				}

				AfxGetDocument()->GetScript().AddComment(CInnoScript::SEC_CODE,CA2T(szLine));
			}
			return true;
		} else {
			try {
				CString str;
				long len = GetLength();
				GetText(len+1,str.GetBufferSetLength(len+1));
				AfxGetDocument()->GetScript().Clear();
				//AfxGetDocument()->GetScript().LoadScript(m_xTextBuffer);
				AfxGetDocument()->GetScript().LoadScriptBuffer(str.GetBuffer());
			}
			catch(...) {
				AtlMessageBox(m_hWnd,_L("ScriptParseError","An error occured while parsing the script."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return false;
			}
		}
		SetSavePoint();
	}
	return true;
}

void CViewScript::OnUpdate(LONG lHint,void* pParam) {
	CMyView<CViewScript>::OnUpdate(lHint,pParam);
}

LRESULT CViewScript::OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	CMainFrame& frame = AfxGetMainWnd();

	m_nNextPrintPos = 0;

	CPrintDialog dlg(FALSE);
	dlg.m_pd.hDevMode = frame.m_devmode.CopyToHDEVMODE();
	dlg.m_pd.hDevNames = frame.m_printer.CopyToHDEVNAMES();
	dlg.m_pd.nMinPage = 1;
	dlg.m_pd.nMaxPage = 1;

	if(dlg.DoModal(frame) == IDOK) {
		frame.m_devmode.CopyFromHDEVMODE(dlg.m_pd.hDevMode);
		frame.m_printer.ClosePrinter();
		frame.m_printer.OpenPrinter(dlg.m_pd.hDevNames, frame.m_devmode.m_pDevMode);

		CPrintJob job;
		job.StartPrintJob(false, frame.m_printer, frame.m_devmode.m_pDevMode, this, _T("ISTool"), 0, -1);
	}

	::GlobalFree(dlg.m_pd.hDevMode);
	::GlobalFree(dlg.m_pd.hDevNames);
	return 0;
}

LRESULT CViewScript::OnUpdateUI(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	switch(wParam) {
#ifdef _DEBUG
		case ID_FILE_PRINTPREVIEW:
			return 1;
#endif
		case ID_FILE_PRINT:
		case ID_EDIT_GOTO:
		case ID_EDIT_REPLACE:
		case ID_EDIT_REPEAT:
		case ID_EDIT_SELECT_ALL:
		case ID_EDIT_FIND:
			return 1;
		case ID_EDIT_CUT:
		case ID_EDIT_COPY:
		case ID_EDIT_DELETEITEM:
			return GetSelectionStart() < GetSelectionEnd();
		case ID_EDIT_PASTE:
			return CanPaste();
		case ID_EDIT_UNDO:
			return CanUndo();
		case ID_EDIT_REDO:
			return CanRedo();
		case ID_EDIT_COMMENTSELECTION:
			return true;//GetSelectionStart() < GetSelectionEnd();
		case ID_EDIT_UNCOMMENTSELECTION:
			return true;
		case ID_VIEW_PROPERTIES:
			return !m_bCode;
	}
	return 0;
}

LRESULT CViewScript::OnStatusBar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	StatusBarInfo* p = (StatusBarInfo*)lParam;
	long pos = GetCurrentPos();
	p->m_strRow.Format(_T("Row: %d"),LineFromPosition(pos) + 1);
	p->m_strCol.Format(_T("Col: %d"),GetColumn(pos) + 1);
	return 0;
}

bool CViewScript::PrintPage(UINT nPage, HDC hDC) {
	PRectangle pagesetupMargin;
	pagesetupMargin.left = AfxGetMainWnd().m_rcMargin.left;
	pagesetupMargin.top = AfxGetMainWnd().m_rcMargin.top;
	pagesetupMargin.right = AfxGetMainWnd().m_rcMargin.right;
	pagesetupMargin.bottom = AfxGetMainWnd().m_rcMargin.bottom;

	HDC hdc = hDC;
	PRectangle rectMargins, rectPhysMargins;
	Point ptPage;
	Point ptDpi;

	// Get printer resolution
	ptDpi.x = GetDeviceCaps(hdc, LOGPIXELSX);    // dpi in X direction
	ptDpi.y = GetDeviceCaps(hdc, LOGPIXELSY);    // dpi in Y direction

	// Start by getting the physical page size (in device units).
	ptPage.x = GetDeviceCaps(hdc, PHYSICALWIDTH);   // device units
	ptPage.y = GetDeviceCaps(hdc, PHYSICALHEIGHT);  // device units

	// Get the dimensions of the unprintable
	// part of the page (in device units).
	rectPhysMargins.left = GetDeviceCaps(hdc, PHYSICALOFFSETX);
	rectPhysMargins.top = GetDeviceCaps(hdc, PHYSICALOFFSETY);

	// To get the right and lower unprintable area,
	// we take the entire width and height of the paper and
	// subtract everything else.
	rectPhysMargins.right = ptPage.x						// total paper width
	                        - GetDeviceCaps(hdc, HORZRES) // printable width
	                        - rectPhysMargins.left;				// left unprintable margin

	rectPhysMargins.bottom = ptPage.y						// total paper height
	                         - GetDeviceCaps(hdc, VERTRES)	// printable height
	                         - rectPhysMargins.top;				// right unprintable margin

	// At this point, rectPhysMargins contains the widths of the
	// unprintable regions on all four sides of the page in device units.

	// Take in account the page setup given by the user (if one value is not null)
	if (pagesetupMargin.left != 0 || pagesetupMargin.right != 0 ||
	        pagesetupMargin.top != 0 || pagesetupMargin.bottom != 0) {
		PRectangle rectSetup;

		// Convert the hundredths of millimeters (HiMetric) or
		// thousandths of inches (HiEnglish) margin values
		// from the Page Setup dialog to device units.
		// (There are 2540 hundredths of a mm in an inch.)

		TCHAR localeInfo[3];
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, localeInfo, 3);

		if (localeInfo[0] == '0') {	// Metric system. '1' is US System
			rectSetup.left = MulDiv (pagesetupMargin.left, ptDpi.x, 2540);
			rectSetup.top = MulDiv (pagesetupMargin.top, ptDpi.y, 2540);
			rectSetup.right	= MulDiv(pagesetupMargin.right, ptDpi.x, 2540);
			rectSetup.bottom	= MulDiv(pagesetupMargin.bottom, ptDpi.y, 2540);
		} else {
			rectSetup.left	= MulDiv(pagesetupMargin.left, ptDpi.x, 1000);
			rectSetup.top	= MulDiv(pagesetupMargin.top, ptDpi.y, 1000);
			rectSetup.right	= MulDiv(pagesetupMargin.right, ptDpi.x, 1000);
			rectSetup.bottom	= MulDiv(pagesetupMargin.bottom, ptDpi.y, 1000);
		}

		// Dont reduce margins below the minimum printable area
		rectMargins.left	= max(rectPhysMargins.left, rectSetup.left);
		rectMargins.top		= max(rectPhysMargins.top, rectSetup.top);
		rectMargins.right	= max(rectPhysMargins.right, rectSetup.right);
		rectMargins.bottom	= max(rectPhysMargins.bottom, rectSetup.bottom);
	} else {
		rectMargins.left	= rectPhysMargins.left;
		rectMargins.top		= rectPhysMargins.top;
		rectMargins.right	= rectPhysMargins.right;
		rectMargins.bottom	= rectPhysMargins.bottom;
	}

	// rectMargins now contains the values used to shrink the printable
	// area of the page.

	// Convert device coordinates into logical coordinates
	DPtoLP(hdc, (LPPOINT) &rectMargins, 2);
	DPtoLP(hdc, (LPPOINT)&rectPhysMargins, 2);

	// Convert page size to logical units and we're done!
	DPtoLP(hdc, (LPPOINT) &ptPage, 1);

	// We must substract the physical margins from the printable area
	RangeToFormat frPrint;
	frPrint.hdc = hdc;
	frPrint.hdcTarget = hdc;
	frPrint.rc.left = rectMargins.left - rectPhysMargins.left;
	frPrint.rc.top = rectMargins.top - rectPhysMargins.top;
	frPrint.rc.right = ptPage.x - rectMargins.right - rectPhysMargins.left;
	frPrint.rc.bottom = ptPage.y - rectMargins.bottom - rectPhysMargins.top;
	frPrint.rcPage.left = 0;
	frPrint.rcPage.top = 0;
	frPrint.rcPage.right = ptPage.x - rectPhysMargins.left - rectPhysMargins.right - 1;
	frPrint.rcPage.bottom = ptPage.y - rectPhysMargins.top - rectPhysMargins.bottom - 1;
#ifdef STODO
	if (headerFormat.size()) {
		frPrint.rc.top += headerLineHeight + headerLineHeight / 2;
	}
	if (footerFormat.size()) {
		frPrint.rc.bottom -= footerLineHeight + footerLineHeight / 2;
	}
#endif

	// BH Code
	frPrint.chrg.cpMin = m_nNextPrintPos;
	frPrint.chrg.cpMax = GetLength();

	m_nNextPrintPos = FormatRange(true,&frPrint);
	return true;
}

bool CViewScript::IsValidPage(UINT nPage) {
	return m_nNextPrintPos<GetLength();
}

LRESULT CViewScript::OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	CWindow::SetFocus();
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

	CMenuHandle pPopup = menu.GetSubMenu(5);
	ATLASSERT(pPopup != NULL);
	_L(pPopup,"Popup");

	AfxGetMainWnd().OnIdle();
	AfxGetMainWnd().TrackPopupMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y);
	return 0;
}

LRESULT CViewScript::OnMoveToLine(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
	if(wParam==987 && GetDocument()->m_nScriptLine>=0) {
		KillTimer(987);

		long nPos = PositionFromLine(GetDocument()->m_nScriptLine);
		SetCurrentPos(nPos);
		SetSel(nPos,nPos);

		GetDocument()->m_nScriptLine = -1;
		::SetFocus(m_hWnd);
	} else
		bHandled = FALSE;
	return 0;
}

static void WrapString(CString& tmp,bool bFunction) {
	long pos = 0;
	for(long i=0;i<tmp.GetLength();i++) {
		if(!bFunction && pos++>25 && iswspace(tmp.GetAt(i))) {
			pos = 0;
			tmp.SetAt(i,'\n');
		} else if(bFunction && pos++>45 && i>0 && iswspace(tmp.GetAt(i)) && (tmp.GetAt(i-1)==';' || tmp.GetAt(i-1)==',')) {
			pos = 0;
			tmp.SetAt(i,'\n');
		}
	}
}

static bool compare_strings(const std::string &a, const std::string &b) 
{
	return _stricmp(a.c_str(),b.c_str())<0;
}

LRESULT CViewScript::OnCharAdded(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
	static CString	strConstantsComplete;
	char szLine[4000];
	SCNotification* scn = (SCNotification*)pnmh;

	if(AutoCActive() || CallTipActive())
		return 0;

	// AutoComplete constants
	if(scn->ch=='{') {
		if(strConstantsComplete.IsEmpty()) {
			for(long i=0;i<CMyApp::m_constants.GetSize();i++)
				strConstantsComplete += CMyApp::m_constants[i]->m_strName + " ";
			strConstantsComplete.Trim();
		}

		AutoCShow(1,strConstantsComplete);
		return 0;
	}

	// Show function CallTip
	if(scn->ch=='(') {
		long current = GetCurLine(sizeof szLine,szLine);
		long pos = GetCurrentPos();
		long startword = current - 1;
		while(startword>0 && isalpha(szLine[startword - 1]))
			startword--;
		szLine[current - 1] = '\0';

		LPCTSTR word = szLine + startword;
		for(long i=0;i<CMyApp::m_functions.GetSize();i++) {
			if(!_stricmp(word,CMyApp::m_functions[i]->m_strName)) {
				CString tmp(CMyApp::m_functions[i]->m_strDescription);
				WrapString(tmp,true);
				CallTipShow(GetCurrentPos() - current + startword, tmp);
				return 0;
			}
		}
	}

	// Auto Indent
	if(scn->ch=='\r' && CMyApp::m_prefs.m_bAutoIndent) {
		long nPos = GetCurrentPos();
		long nLine = LineFromPosition(nPos);
		long nFrom = PositionFromLine(nLine-1);
		long nTo = PositionFromLine(nLine);
		long nLength = LineLength(nLine-1);

		CHAR ins[2];
		ins[0] = GetCharAt(nFrom);
		ins[1] = 0;
		while(iswspace(ins[0]) && nLength>0 && ins[0]!='\n' && ins[0]!='\r') {
			--nLength;
			InsertText(nTo++,ins);
			nPos++;
			ins[0] = GetCharAt(++nFrom);
		}
		SetCurrentPos(nPos);
		SetSelectionStart(nPos);
		SetSelectionEnd(nPos);
	}

	// Show com intellisense
	if(scn->ch=='.') {
		long current = GetCurLine(sizeof szLine,szLine);
		long pos = GetCurrentPos();
		long startword = current - 1;
		while(startword>0 && isalpha(szLine[startword - 1]))
			startword--;
		szLine[current - 1] = '\0';

		CString strWord = szLine + startword;
		strWord.Trim();
		if(!strWord.IsEmpty()) {
			CString strVarName = strWord.MakeLower();
			long nLine = LineFromPosition(pos);
			while(nLine--) {
				TextRange tr;
				tr.chrg.cpMin = PositionFromLine(nLine);
				tr.chrg.cpMax = PositionFromLine(nLine+1);
				tr.lpstrText = strWord.GetBuffer(tr.chrg.cpMax-tr.chrg.cpMin);
				int len = GetTextRange(&tr);
				strWord.ReleaseBufferSetLength(len);
				strWord.MakeLower();
				long pos = strWord.Find("createoleobject");
				if(pos>=0 && strWord.Find(strVarName)>=0) {
					long iStart = strWord.Find('(',pos);
					long iEnd = strWord.Find(')',iStart);
					if(iStart>pos && iEnd>iStart) {
						strWord.ReleaseBufferSetLength(iEnd);
						strWord.Delete(0,iStart+1);
						strWord.Trim();
						if(strWord.GetLength()>2 && strWord[0]=='\'' && strWord[strWord.GetLength()-1]=='\'') {
							strWord = strWord.Mid(1,strWord.GetLength()-2);

							CLSID clsidObj;
							if(::CLSIDFromProgID(CA2W(strWord), &clsidObj)==S_OK) {
								strWord.Empty();
								// declare a pointer to the com interface
								IUnknown* pUnk = NULL;
								// create the com object
								if(::CoCreateInstance(clsidObj,NULL,CLSCTX_ALL,IID_IDispatch,(void**)&pUnk)==S_OK) {
									IDispatch* pDisp = NULL;
									if(pUnk->QueryInterface(__uuidof(IDispatch),(void**)&pDisp)==S_OK) {
										UINT ctinfo;
										pDisp->GetTypeInfoCount(&ctinfo);
										if(ctinfo>0) {
											ITypeInfo* pTypeInfo;
											if(pDisp->GetTypeInfo(0,LOCALE_SYSTEM_DEFAULT,&pTypeInfo)==S_OK) {
												TYPEATTR* pTypeAttr;
												if(pTypeInfo->GetTypeAttr(&pTypeAttr)==S_OK) {
													std::vector<std::string>	myArray;
													for(int i=0;i<pTypeAttr->cFuncs;i++) {
														FUNCDESC* pFuncDesc;
														if(pTypeInfo->GetFuncDesc(i, &pFuncDesc)==S_OK) {
															UINT nCount;
															BSTR bstrName = NULL;
															if(pTypeInfo->GetNames(pFuncDesc->memid,&bstrName,1,&nCount)==S_OK) {
																if(bstrName!=NULL) {
																	std::string str = CW2A(bstrName);
																	SysFreeString(bstrName);
																	myArray.push_back(str);
																}
															}
															pTypeInfo->ReleaseFuncDesc(pFuncDesc);
														}
													}
													pTypeInfo->ReleaseTypeAttr(pTypeAttr);
													std::sort(myArray.begin(),myArray.end(),compare_strings);
													strWord.Empty();
													for(std::vector<std::string>::iterator it=myArray.begin();it!=myArray.end();it++) {
														strWord.Append(it->c_str());
														strWord.Append(" ");
													}
												}
												pTypeInfo->Release();

												strWord.Trim();
												AutoCShow(0,strWord);
											}
										}

										// release the com interface
										pDisp->Release();
									}
									pUnk->Release();
								}
							}

							return 0;
						}
					}
					break;
				}
			}
		}
	}

	return 0;
}

LRESULT CViewScript::OnCallTip(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	static CString str;

	if(str.IsEmpty()) {
		for(long i=0;i<CMyApp::m_functions.GetSize();i++)
			str += CMyApp::m_functions[i]->m_strName + " ";
		str.Trim();
	}

	char szLine[1000];
	long current = GetCurLine(sizeof szLine,szLine);
	long linelength = LineLength(LineFromPosition(GetCurrentPos()));
	long pos = GetCurrentPos();
	long startword = current - 0;
	while(startword>0 && isalpha(szLine[startword - 1]))
         startword--;

	AutoCShow(current-startword,str);
	
	return 0;
}

LRESULT CViewScript::OnDwellStart(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
	SCNotification* scn = (SCNotification*)pnmh;

	if(scn->position<0 || AutoCActive() || CallTipActive())
		return 0;

	long nLine = LineFromPosition(scn->position);
	long current = GetColumn(scn->position);

	char szLine[8000];
	long nLineLength = GetLine(nLine,szLine);
	long startword = current - 1;
	
	while(current<nLineLength-1 && isalpha(szLine[current]))
		current++;

	while(startword>0 && isalpha(szLine[startword - 1]))
         startword--;
    szLine[current] = '\0';

    LPCTSTR word = szLine + startword;
	for(long i=0;i<CMyApp::m_calltips.GetSize();i++) {
		if(!_tcsicmp(word,CMyApp::m_calltips[i]->m_strName)) {
			CString tmp(CMyApp::m_calltips[i]->m_strDescription);
			WrapString(tmp,false);
			CallTipShow(scn->position, tmp);
			return 0;
		}
	}

	return 0;
}

LRESULT CViewScript::OnDwellEnd(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/) {
	if(CallTipActive()) CallTipCancel();
	return 0;
}

LRESULT CViewScript::OnCommentSelection(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	long nSaveStart = GetSelectionStart();
	long nSavePos = GetCurrentPos();
	long nSaveEnd = GetSelectionEnd();
	long nStartLine = LineFromPosition(GetSelectionStart());
	long nStopLine = LineFromPosition(GetSelectionEnd());
	bool bAdd = wParam==0;

	for(long nLine=nStartLine;nLine<=nStopLine;nLine++) {
		if(nLine==nStopLine && nSaveEnd==PositionFromLine(nLine))
			break;

		if(m_bCode) {
			if(bAdd) {
				InsertText(PositionFromLine(nLine),_T("//"));
				nSaveEnd += 2;
			} else if(GetCharAt(PositionFromLine(nLine))=='/' && GetCharAt(PositionFromLine(nLine)+1)=='/') {
				SetSelectionStart(PositionFromLine(nLine));
				SetSelectionEnd(PositionFromLine(nLine)+2);
				DeleteBack();
				nSaveEnd -= 2;
			}
		} else {
			if(bAdd) {
				InsertText(PositionFromLine(nLine),_T(";"));
				nSaveEnd += 1;
			} else if(GetCharAt(PositionFromLine(nLine))==';') {
				SetSelectionStart(PositionFromLine(nLine));
				SetSelectionEnd(PositionFromLine(nLine)+1);
				DeleteBack();
				nSaveEnd -= 1;
			}
		}
	}
	SetSelectionStart(nSaveStart);
	SetSelectionEnd(nSaveEnd);
	return 0;
}

CInnoScript::SECTION CViewScript::GetSectionFromLine(long nLine) {
	if(m_bCode)
		return CInnoScript::SEC_CODE;

	CInnoScript::SECTION sec = CInnoScript::SEC_NONE;

	TCHAR szLine[1024];
	do {
		*reinterpret_cast<WORD*>(szLine) = sizeof szLine;
		int len = GetLine(nLine,szLine);
		szLine[len] = 0;
		if(szLine[0]==_T('[')) {
			if(szLine[1]!=_T('/'))
				sec = CInnoScript::GetSectionCode(szLine);
			break;
		}
		--nLine;
	} while(nLine>=0);

	return sec;
}

LRESULT CViewScript::OnProperties(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	TCHAR szLine[2048];

	CInnoScript::SECTION sec = GetSectionFromLine(LineFromPosition(GetCurrentPos()));
	if(sec==CInnoScript::SEC_NONE || sec==CInnoScript::SEC_SETUP)
		return 0;

	GetCurLine(sizeof szLine,szLine);
	CScriptLine* pLine;
	if(sec==CInnoScript::SEC_MESSAGES || sec==CInnoScript::SEC_CUSTOMMESSAGES)
		pLine = new CInnoScript::CLineSetup(sec,szLine);
	else
		pLine = new CInnoScript::CLineParam(sec,szLine);

	CScriptList	list;
	list.Add(pLine);
	if(!CSheets::ShowSheet(m_hWnd,list,false)) {
		delete pLine;
		return 0;
	}

	pLine->Write(szLine,sizeof szLine);
	_tcscat(szLine,_T("\r\n"));

	long nPos = GetCurrentPos();
	long nLine = LineFromPosition(nPos);
	long nFrom = PositionFromLine(nLine);
	SetSelectionStart(nFrom);
	SetSelectionEnd(nFrom + LineLength(nLine));
	ReplaceSel(szLine);

	delete pLine;
	return 0;
}
