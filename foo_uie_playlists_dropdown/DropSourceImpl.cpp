#include "component.h"

IDropSource_tutorial1::IDropSource_tutorial1(HWND p_hWnd) : m_refcount(1), m_hWnd(p_hWnd)
{
}

pfc::com_ptr_t<IDropSource> IDropSource_tutorial1::g_create(HWND hWnd)
{
	pfc::com_ptr_t<IDropSource> temp;
	temp.attach(new IDropSource_tutorial1(hWnd));
	return temp;
}


/////////////////////////////////////////////////////////
// IUnknown methods

// We support the IUnknown and IDropSource interfaces.
STDMETHODIMP IDropSource_tutorial1::QueryInterface(REFIID iid, void ** ppvObject) {
	if (ppvObject==0) return E_INVALIDARG;
	else if (iid == IID_IUnknown) {
		AddRef();
		*ppvObject = (IUnknown*)this;
		return S_OK;
	} else if (iid == IID_IDropSource) {
		AddRef();
		*ppvObject = (IDropSource*)this;
		return S_OK;
	} else return E_NOINTERFACE;
}

// Increase reference count.
STDMETHODIMP_(ULONG) IDropSource_tutorial1::AddRef() {
	return ++m_refcount;
}

// Decrease reference count.
// Delete object, if reference count reaches zero.
STDMETHODIMP_(ULONG) IDropSource_tutorial1::Release() {
	LONG rv = --m_refcount;
	if (rv == 0) delete this;
	return rv;
}

// IDropSource methods

// Determine whether the drag operation should be continued.
// Return S_OK to continue, DRAGDROP_S_CANCEL to cancel the operation,
// or DRAGDROP_S_DROP to perform a drop.
STDMETHODIMP IDropSource_tutorial1::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) {
	// Cancel if escape was pressed.
	if (fEscapePressed) return DRAGDROP_S_CANCEL;

	// Cancel if right mouse button was pressed.
	else if (grfKeyState & MK_RBUTTON) return DRAGDROP_S_CANCEL;

	// Drop or cancel if left mouse button was released.
	else if (!(grfKeyState & MK_LBUTTON)) return DRAGDROP_S_DROP;

	// Continue otherwise.
	else return S_OK;
}

// Provide visual feedback (through mouse pointer) about the state of the
// drag operation.
STDMETHODIMP IDropSource_tutorial1::GiveFeedback(DWORD dwEffect) {
	// Let OLE show the default cursor.
	return DRAGDROP_S_USEDEFAULTCURSORS;
}
