#include "component.h"

// Enable WindowsXP Theme Support
HINSTANCE       playlists_dropdown::preferences::hUxtheme;
DIALOGTHEMEPROC playlists_dropdown::preferences::ThemeDialogProc;
ULONG_PTR		playlists_dropdown::preferences::gdiplusToken;
Gdiplus::GdiplusStartupInput playlists_dropdown::preferences::gdiplusStartupInput;

const tab_entry_t playlists_dropdown::preferences::tab_table[] = {
	{ "Display", &DisplayDlgProc, IDD_PREFERENCES_DISPLAY },
	{ "General", &GeneralDlgProc, IDD_PREFERENCES_GENERAL }
};

BOOL CALLBACK playlists_dropdown::preferences::MainDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static HWND g_hWndTabDialog[tabsize(tab_table)] = { NULL };
	static HWND g_hWndCurrentTab = NULL;
	static t_uint32 g_current_tab;

	switch (msg)
	{
	case WM_DESTROY:
		pfc::fill_array_t(g_hWndTabDialog, (HWND) NULL);
		g_hWndCurrentTab = NULL;
		cfg::last_tab = TabCtrl_GetDlgCurSel(hWnd, IDC_TAB1);
		apply_config();
		if (!!hUxtheme) {
			FreeLibrary(hUxtheme);
		}
		Gdiplus::GdiplusShutdown(gdiplusToken);
		gdiplusToken = NULL;
		break;

	case WM_INITDIALOG:
		{
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
			hUxtheme = uLoadLibrary("uxtheme.dll");
			ThemeDialogProc = !!hUxtheme ? (DIALOGTHEMEPROC) GetProcAddress(hUxtheme, "EnableThemeDialogTexture") : NULL;

			// get handle of tab control
			HWND hWndTab = GetDlgItem(hWnd, IDC_TAB1);

			// set up tabs and create (invisible) subdialogs
			for (t_size n = 0; n < tabsize(tab_table); n++) {
				uTabCtrl_InsertItemText(hWndTab, n, tab_table[n].pszName);
				g_hWndTabDialog[n] = CreateTabDialog(hWnd, tab_table[n]);
			}

			// position the subdialogs in the inner part of the tab control
			RECT rcTab;
			GetChildWindowRect(hWnd, IDC_TAB1, &rcTab);
			SendMessage(hWndTab, TCM_ADJUSTRECT, FALSE, (LPARAM) &rcTab);
			for (t_size n = 0; n < tabsize(g_hWndTabDialog); n++) {
				SetWindowPos(g_hWndTabDialog[n], NULL,
					rcTab.left, rcTab.top, rcTab.right - rcTab.left, rcTab.bottom - rcTab.top,
					SWP_NOZORDER | SWP_NOACTIVATE);
			}

			// select last used tab and show corresponding subdialog
			g_current_tab = cfg::last_tab < tabsize(g_hWndTabDialog) ? cfg::last_tab : 0;
			g_hWndCurrentTab = g_hWndTabDialog[g_current_tab];

			uSendMessage(hWndTab, TCM_SETCURSEL, g_current_tab, 0);
			uSetDlgItemText(hWnd, IDC_VERSION, "v" COMPONENT_VERSION);

			ShowWindow(g_hWndCurrentTab, SW_SHOW);
		}
		break;

	case WM_NOTIFY:
		if (((LPNMHDR) lParam)->idFrom == IDC_TAB1 && ((LPNMHDR) lParam)->code == TCN_SELCHANGE) {
			if (!!g_hWndCurrentTab) {
				ShowWindow(g_hWndCurrentTab, SW_HIDE);
			}
			g_hWndCurrentTab = NULL;

			g_current_tab = TabCtrl_GetDlgCurSel(hWnd, IDC_TAB1);
			if (g_current_tab < tabsize(g_hWndTabDialog)) {
				g_hWndCurrentTab = g_hWndTabDialog[g_current_tab];
				ShowWindow(g_hWndCurrentTab, SW_SHOW);
			}
		}
		break;

	default:
		return false;
	}
	return true;
}
