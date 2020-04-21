// MyDropTarget.cpp: implementation of the CMyDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDropTarget.h"

CMyDropTarget::CMyDropTarget() {
	tb_RefCount=0;
}

CMyDropTarget::~CMyDropTarget()
{

}

// Interface IUnknown (is base of IDropTarget)

// get a pointer to our interface
HRESULT CMyDropTarget::QueryInterface(REFIID iid, void ** ppvObject) {
	// Simplified version...
	// TEST_ NULL 
	if(ppvObject == NULL)
		return E_POINTER;  

	// Supports IID_IUnknown and IID_IDropTarget 
	if(iid == IID_IUnknown || iid == IID_IDropTarget) {
		*ppvObject = (void*)this;
		AddRef();
		return NOERROR;
	}

	return E_NOINTERFACE; 
}

ULONG CMyDropTarget::AddRef(void) {
	return ++tb_RefCount;
}

ULONG CMyDropTarget::Release(void) {
	return --tb_RefCount;
}

// Interface IDropTarget

// the drag action continues
HRESULT CMyDropTarget::DragOver(
	DWORD grfKeyState, //Current state of keyboard modifier keys
	POINTL pt,         //Pointer to the current cursor coordinates
	DWORD * pdwEffect  //Pointer to the effect of the drag-and-drop 
					   // operation
	)
{
	DWORD de = OnDragScroll(NULL,grfKeyState,CPoint(pt.x,pt.y));
	if(de & DROPEFFECT_SCROLL) {
		*pdwEffect = de;
		return S_OK;
	}

	*pdwEffect = OnDragOver(NULL,grfKeyState,CPoint(pt.x,pt.y));
	return S_OK;
}

// the drag action leaves your window - no dropping
HRESULT CMyDropTarget::DragLeave() {
	OnDragLeave();
	return S_OK;
}

// The drag action enters your window - get the file
HRESULT CMyDropTarget::DragEnter
				( IDataObject * pDataObject, //Pointer to the interface of the source data object
				  DWORD grfKeyState,//Current state of keyboard modifier keys
				  POINTL pt,		//Pointer to the current cursor coordinates
				  DWORD * pdwEffect	//Pointer to the effect of the drag-and-drop 
									// operation
				)
{
	DWORD de = OnDragScroll(pDataObject,grfKeyState,CPoint(pt.x,pt.y));
	if(de & DROPEFFECT_SCROLL) {
		*pdwEffect = de;
		return S_OK;
	}

	*pdwEffect = OnDragEnter(pDataObject,grfKeyState,CPoint(pt.x,pt.y));
	return S_OK;
}

// the data have been dropped here, so process it
HRESULT CMyDropTarget::Drop(
	IDataObject * pDataObject,	// Pointer to the interface for the source data
	DWORD grfKeyState,			// Current state of keyboard modifier keys
	POINTL pt,					// Pointer to the current cursor coordinates
	DWORD * pdwEffect			// Pointer to the effect of the drag-and-drop operation
) {
	*pdwEffect = OnDrop(pDataObject,grfKeyState,CPoint(pt.x,pt.y),*pdwEffect/*DROPEFFECT_NONE*/);
	return S_OK;
}

DWORD CMyDropTarget::OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
	return DROPEFFECT_NONE;
}

DWORD CMyDropTarget::OnDragOver(IDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
	return DROPEFFECT_NONE;
}

DWORD CMyDropTarget::OnDrop(IDataObject* pDataObject, DWORD grfKeyState, CPoint point, DWORD dropEffect) {
	return DROPEFFECT_NONE;
}

void CMyDropTarget::OnDragLeave() {}

DWORD CMyDropTarget::OnDragScroll(IDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
	return DROPEFFECT_NONE;
}

static void _AfxFillFormatEtc(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtcFill) {
	ATLASSERT(lpFormatEtcFill != NULL);
	
	if(cfFormat!=0) {
		lpFormatEtcFill->cfFormat = cfFormat;
		lpFormatEtcFill->ptd = NULL;
		lpFormatEtcFill->dwAspect = DVASPECT_CONTENT;
		lpFormatEtcFill->lindex = -1;
		lpFormatEtcFill->tymed = (DWORD) -1;
	}
}

bool CMyDropTarget::IsDataAvailable(IDataObject* pDataObject,CLIPFORMAT cfFormat,LPFORMATETC) {
	return true;
	// fill in FORMATETC struct
	FORMATETC formatEtc;
	_AfxFillFormatEtc(cfFormat, &formatEtc);

	// attempt to get the data
	return pDataObject->QueryGetData(&formatEtc) == S_OK;
}
