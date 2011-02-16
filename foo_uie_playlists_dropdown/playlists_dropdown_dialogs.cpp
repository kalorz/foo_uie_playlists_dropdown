#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  "Rename Playlist" dialog procedure
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

BOOL CALLBACK playlists_dropdown::RenameDialogProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_INITDIALOG:
		{
			uSetWindowLong(wnd, DWL_USER, lp);
			pfc::string_base* ptr = reinterpret_cast<pfc::string_base*>(lp);
			pfc::string8 title("Rename playlist: \"");
			title << *ptr << "\"";
			uSetWindowText(wnd, title);
			uSetDlgItemText(wnd, IDC_PLAYLIST_NAME, *ptr);
			Edit_SetSel(uGetDlgItem(wnd, IDC_PLAYLIST_NAME), 0, -1);
			SetFocus(uGetDlgItem(wnd, IDC_PLAYLIST_NAME));
		}
		return 1;

	case WM_COMMAND:
		switch (wp)
		{
		case IDOK:
			uGetDlgItemText(wnd, IDC_PLAYLIST_NAME, *reinterpret_cast<pfc::string_base*>(uGetWindowLong(wnd, DWL_USER)));
			EndDialog(wnd, 1);
			break;

		case IDCANCEL:
			EndDialog(wnd, 0);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(wnd, 0);
		break;
	}
	return 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  "Sort Playlists By..." dialog procedure
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

BOOL CALLBACK playlists_dropdown::SortDialogProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_INITDIALOG:
		{
			uSetWindowLong(wnd, DWL_USER, lp);
			HWND hCombo = uGetDlgItem(wnd, IDC_SORT_STRING);
			cfg::sort_string.setup_dropdown(hCombo);
			if (!cfg::sort_string.is_empty()) {
				ComboBox_SetCurSel(hCombo, 0);
				ComboBox_SetEditSel(hCombo, 0, -1);
			}
			SetFocus(hCombo);
		}
		return 1;

	case WM_CONTEXTMENU:
		{
			const HWND source = (HWND) wp;
			if (source != NULL && source == GetDlgItem(wnd, IDC_SORT_STRING)) {
				cfg::sort_string.on_context(source, lp);
				return TRUE;
			}
		}
		break;

	case WM_COMMAND:
		switch (wp)
		{
		case IDOK:
			{
				uGetDlgItemText(wnd, IDC_SORT_STRING, *reinterpret_cast< pfc::string_base * >(uGetWindowLong(wnd, DWL_USER)));
				pfc::string8 pattern;
				if (uGetDlgItemText(wnd, IDC_SORT_STRING, pattern)) {
					cfg::sort_string.add_item(pattern);
				}
				EndDialog(wnd, 1);
			}
			break;

		case IDCANCEL:
			EndDialog(wnd, 0);
			break;

		case IDC_FIELDS:
			{
				HMENU menu = CreatePopupMenu();
				uAppendMenu(menu, MF_STRING | MF_GRAYED | MF_DISABLED, 0, "Copy to clipboard:");
				uAppendMenu(menu, MF_SEPARATOR, 0, 0);
				for (int i = 1; i < tabsize(g_fields_list); i++) {
					if (!g_fields_list[i].label) break; // Only basic fields
					uAppendMenu(menu, MF_STRING, i, g_fields_list[i].label);
				}
				RECT rc;
				GetWindowRect(GetDlgItem(wnd, IDC_FIELDS), &rc);
				int cmd = TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, rc.left, rc.bottom, 0, GetDlgItem(wnd, IDC_FIELDS), 0);
				DestroyMenu(menu);
				if (cmd >= 0 && cmd < tabsize(g_fields_list) && !!g_fields_list[cmd].label) {
					uSetClipboardString(g_fields_list[cmd].label);
				}
			}
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(wnd, 0);
		break;
	}
	return 0;
}
