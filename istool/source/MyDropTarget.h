// MyDropTarget.h

#pragma once

class CMyDropTarget : public IDropTarget {
public:
	CMyDropTarget();
	virtual ~CMyDropTarget();

	HRESULT Register(HWND hWnd) {
		return ::RegisterDragDrop(hWnd,this);
	}

	HRESULT Revoke(HWND hWnd) {
		return ::RevokeDragDrop(hWnd);
	}

	// Interface IDropTarget
	HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT STDMETHODCALLTYPE DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT STDMETHODCALLTYPE DragLeave(void);
	HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect);

	// Interface IUnknown	
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

	virtual DWORD OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DWORD OnDragOver(IDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DWORD OnDrop(IDataObject* pDataObject, DWORD grfKeyState, CPoint point, DWORD dropEffect);
	virtual void OnDragLeave();
	virtual DWORD OnDragScroll(IDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	static bool IsDataAvailable(IDataObject* pDataObject,CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc=NULL);

protected:
	ULONG		tb_RefCount;
};
