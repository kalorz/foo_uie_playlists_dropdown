// ========================================================================================================================
#ifndef _DROP_TARGET_H_
#define _DROP_TARGET_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class CDropTarget : public IDropTarget {
public:
	static pfc::com_ptr_t<IDropTarget> g_create(HWND hWnd);

	// IUnknown implementation
	HRESULT __stdcall QueryInterface (REFIID iid, void ** ppvObject);
	ULONG   __stdcall AddRef (void);
	ULONG   __stdcall Release (void);

	// IDropTarget implementation
	HRESULT __stdcall DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT __stdcall DragLeave(void);
	HRESULT __stdcall Drop(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);

private:
	// Constructor
	CDropTarget(HWND hwnd);

	// internal helper function
	void  InitFormat(FORMATETC &fmt,CLIPFORMAT cfFormat);
	//DWORD DropEffect(DWORD grfKeyState, POINTL pt, DWORD dwAllowed);
	bool  QueryDataObject(IDataObject *pDataObject);

	// Private member variables
	IDataObject * m_pdo;
	long          m_lRefCount;
	HWND          m_hWnd;
	bool          m_fAllowDrop;
	bool          m_tmpEx;

	// Other internal window members

};

// ========================================================================================================================
#endif
// ========================================================================================================================
