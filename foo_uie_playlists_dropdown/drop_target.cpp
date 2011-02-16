#include "component.h"

CDropTarget::CDropTarget(HWND hwnd) : m_lRefCount(1), m_hWnd(hwnd), m_fAllowDrop(false), m_tmpEx(false) {
}

pfc::com_ptr_t<IDropTarget> CDropTarget::g_create(HWND hWnd) {
	pfc::com_ptr_t<IDropTarget> temp;
	temp.attach(new CDropTarget(hWnd));
	return temp;
}

HRESULT CDropTarget::QueryInterface(REFIID iid, void ** ppvObject) {
	if (ppvObject == 0) {
		return E_INVALIDARG;
	} else if (iid == IID_IUnknown) {
		AddRef();
		*ppvObject = (IUnknown*) this;
		return S_OK;
	} else if (iid == IID_IDropTarget) {
		AddRef();
		*ppvObject = (IDropTarget*) this;
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG CDropTarget::AddRef() {
	return ++m_lRefCount;
}

ULONG CDropTarget::Release() {
	LONG rv = --m_lRefCount;
	if (rv == 0) {
		delete this;
	}
	return rv;
}

HRESULT CDropTarget::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect) {
    *pdwEffect = DROPEFFECT_COPY;
    m_pdo = pDataObject;
    m_pdo->AddRef();
    SendMessage(m_hWnd, MSG_DRAG_ENTER, (WPARAM) 0, (LPARAM) 0);
    return S_OK;
}

HRESULT CDropTarget::DragOver(DWORD grfKeyState, POINTL ptl, DWORD * pdwEffect) {
    //m_hwndTarget = 0;
    //*pdwEffect = DROPEFFECT_NONE;
    *pdwEffect = DROPEFFECT_COPY;

	m_tmpEx = (grfKeyState & MK_RBUTTON) != 0;

    if (m_pdo) {
		RECT rc;
        GetWindowRect(m_hWnd, &rc);
        POINT pt = (POINT&) ptl;
        if (PtInRect(&rc, pt)) {
            FORMATETC fmt = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
            if (!m_pdo->QueryGetData(&fmt)) {
                *pdwEffect = DROPEFFECT_COPY;
                ScreenToClient(m_hWnd, &pt);
                LPARAM lp = MAKELPARAM(pt.x, pt.y);
                SendMessage(m_hWnd, MSG_DRAG_OVER, (WPARAM) 0, (LPARAM) lp);
            }
        }
    }
    return S_OK;
	//QueryDrop(grfKeyState, pdwEffect);
}

HRESULT CDropTarget::DragLeave() {
    if (m_pdo) m_pdo->Release();
    m_fAllowDrop = FALSE;
    m_pdo = 0;
    SendMessage(m_hWnd, MSG_DRAG_LEAVE, (WPARAM) 0, (LPARAM) 0);
	return S_OK;
}

HRESULT CDropTarget::Drop(IDataObject * pDataObject, DWORD grfKeyState, POINTL ptl, DWORD * pdwEffect) {
	*pdwEffect = DROPEFFECT_COPY;
    POINT pt = (POINT&) ptl;
    ScreenToClient(m_hWnd, &pt);
	SendMessage(m_hWnd, m_tmpEx ? MSG_DROP_FILES_EX : MSG_DROP_FILES, (WPARAM) pDataObject, MAKELPARAM(pt.x, pt.y));
	pDataObject->Release();
	return S_OK;
}

/*
DWORD CDropTarget::DropEffect(DWORD grfKeyState, POINTL pt, DWORD dwAllowed) {
	return S_OK;
}
*/

bool CDropTarget::QueryDataObject(IDataObject *pDataObject) {
	FORMATETC fmtetc = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	return pDataObject->QueryGetData(&fmtetc) == S_OK ? true : false;
}
