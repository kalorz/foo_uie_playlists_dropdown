// ========================================================================================================================
#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_
// ========================================================================================================================

#pragma once

class playlists_dropdown::preferences : public preferences_page_v3 {
public:
	preferences();
	virtual ~preferences();

	virtual preferences_page_instance::ptr instantiate(HWND parent, preferences_page_callback::ptr callback);
	virtual const char * get_name();
	virtual GUID get_guid();
	virtual GUID get_parent_guid();
	virtual bool get_help_url(pfc::string_base &p_out);
};

// ========================================================================================================================
#endif
// ========================================================================================================================

#ifdef ___REMOVE_THIS___

// ========================================================================================================================
#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// TODO
// TEMPORAL SOLUTION?
// {74FD83FD-6927-43c6-B673-01272CD8A54E}
static const GUID g_guid_columns_ui_extensions_branch = 
{ 0x74fd83fd, 0x6927, 0x43c6, { 0xb6, 0x73, 0x1, 0x27, 0x2c, 0xd8, 0xa5, 0x4e } };

namespace {
	preferences_branch_factory g_branch(g_guid_columns_ui_extensions_branch, preferences_page_v2::guid_display, "Columns UI extensions", 0);
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


typedef HRESULT (CALLBACK * DIALOGTHEMEPROC) (HWND, DWORD);
#define TabCtrl_GetDlgCurSel(hDlg, nIDDlgItem) TabCtrl_GetCurSel(uGetDlgItem(hDlg, nIDDlgItem))

struct tab_entry_t {
	LPSTR   pszName;
	DLGPROC lpDialogFunc;
	WORD    nID;
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Main preferences page class
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class playlists_dropdown::preferences : public preferences_page_v3 {
public:
	preferences();
	virtual ~preferences();

	virtual HWND create(HWND parent);
	virtual const char * get_name();
	virtual GUID get_guid();
	virtual GUID get_parent_guid();
	virtual bool reset_query();
	virtual bool get_help_url(pfc::string_base &p_out);
	virtual void reset();

private:
	static HINSTANCE				hUxtheme;
	static DIALOGTHEMEPROC			ThemeDialogProc;
	static ULONG_PTR				gdiplusToken;
	static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	static const tab_entry_t		tab_table[]; // List of all preferences sub-pages (tabs)
	static pfc::ptr_list_t<GUID>	g_guids; // For temporary guids list

	static pfc::string8 get_formatting_string(const int fs_tab) {
		return fs_tab == 0 ? cfg::title_string : fs_tab == 1 ? cfg::style_string : "";
	}

	static HWND CreateTabDialog(HWND hWndParent, tab_entry_t entry, LPARAM lInitParam = 0) {
		PFC_ASSERT(!!entry.lpDialogFunc);
		return CreateDialogParam(core_api::get_my_instance(), MAKEINTRESOURCE(entry.nID), hWndParent, entry.lpDialogFunc, lInitParam);
	}

	static void EnableTheming(HWND hWnd) {
		if (!!hUxtheme && !!ThemeDialogProc) {
			ThemeDialogProc(hWnd, ETDT_ENABLETAB);
		}
	}

	static void playlists_dropdown::preferences::SetDlgSpinner(HWND h_spinner, int nIDDlgItemSpinner, int nIDDlgItemBuddy, int iLow, int iHigh, int nPos) {
		SendDlgItemMessage(h_spinner, nIDDlgItemSpinner, UDM_SETBUDDY,   (WPARAM) GetDlgItem(h_spinner, nIDDlgItemBuddy), (LPARAM) 0);
		SendDlgItemMessage(h_spinner, nIDDlgItemSpinner, UDM_SETRANGE32, (WPARAM) iLow, (LPARAM) iHigh);
		SendDlgItemMessage(h_spinner, nIDDlgItemSpinner, UDM_SETPOS32,   (WPARAM) 0, (LPARAM) nPos);
	}

	static void apply_config();

	static BOOL CALLBACK MainDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK DisplayDlgProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	static BOOL CALLBACK GeneralDlgProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	static BOOL CALLBACK DragDropDlgProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
};

// ========================================================================================================================
#endif
// ========================================================================================================================

#endif // ___REMOVE_THIS___