#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Display preferences dialog procedure                                                                                   
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define MSG_UPDATEPREVIEW (WM_USER + 1)

BOOL CALLBACK playlists_dropdown::preferences::DisplayDlgProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case MSG_UPDATEPREVIEW:
		{
			static_api_ptr_t<playlist_manager> pm;
			t_size playlist = pm->get_active_playlist();
			if (playlist != pfc_infinite) {
				static_api_ptr_t<titleformat_compiler> cmp;
				titleformat_object::ptr title_format;
				if (cmp->compile(title_format, cfg::title_string)) {
					titleformat_hook_impl_splitter hook(titleformat_hook_impl_splitter(
						&title_format_hook(playlist, false),
						&titleformat_hook_impl_list(playlist, pm->get_playlist_count())));
					pfc::string8 title;
					title_format->run(&hook, title, NULL);
					pfc::string8 preview;
					cmp->remove_color_marks(title, preview);
					preview.replace_char('\t', ' ');
					uSetDlgItemText(wnd, IDC_PREVIEW, preview);
				} else {
					uSetDlgItemText(wnd, IDC_PREVIEW, "COMPILATION ERROR");
				}
			} else {
				uSetDlgItemText(wnd, IDC_PREVIEW, "N/A");
			}
		}
		break;

	case WM_COMMAND:
		switch (wp)
		{
			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
			//  GENERAL
			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		case IDC_SHOW_ICONS:
			cfg::show_icons = uButton_GetCheck(wnd, IDC_SHOW_ICONS);
			playlists_dropdown::g_reload_icons();
			playlists_dropdown::g_update_all_sizes();
			playlists_dropdown::g_redraw_all();
			break;

		case IDC_ICON_UTILS:
			{
				HMENU menu = CreatePopupMenu();
				uAppendMenu(menu, MF_STRING, 1, "Clear custom icons history");
				uAppendMenu(menu, MF_STRING, 2, "Reset all icons to default");
				RECT rc;
				GetWindowRect(GetDlgItem(wnd, IDC_ICON_UTILS), &rc);
				int cmd = TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, rc.left, rc.bottom, 0, GetDlgItem(wnd, IDC_ICON_UTILS), 0);
				DestroyMenu(menu);

				if (cmd == 1 && uMessageBox(wnd, "Are you sure you want to clear all custom icons history?", "Continue?", MB_YESNO) == IDYES) {
					cfg::custom_icons_history.reset();
				} else if (cmd == 2 && uMessageBox(wnd, "Are you sure you want to reset all icons to default?", "Continue?", MB_YESNO) == IDYES) {
					try {
						static_api_ptr_t<playlist_manager_v2> pm2;
						for (int i = 0, total = pm2->get_playlist_count(); i < total; i++) {
							pm2->playlist_remove_property(i, guid_icon_path);
						}
					} catch(...) { }
				}

				playlists_dropdown::g_reload_icons();
				playlists_dropdown::g_update_all_sizes();
				playlists_dropdown::g_redraw_all();
			}
			break;

		case IDC_MIN_VISIBLE | EN_CHANGE << 16:
			cfg::min_visible = uGetDlgItemInt(wnd, IDC_MIN_VISIBLE, NULL, true);
			playlists_dropdown::g_update_all_min_visible();
			break;

		case IDC_FORMATTING_STRING | EN_CHANGE << 16:
			if (TabCtrl_GetDlgCurSel(wnd, IDC_FS_TAB) == 0) {
				uGetDlgItemText(wnd, IDC_FORMATTING_STRING, cfg::title_string);
				uSendMessage(wnd, MSG_UPDATEPREVIEW, 0, 0);
			} else {
				uGetDlgItemText(wnd, IDC_FORMATTING_STRING, cfg::style_string);
			}
			uEnableWindow(uGetDlgItem(wnd, IDC_APPLY), true);
			break;

			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
			//  PADDING
			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		case IDC_PADDING_TOP | EN_CHANGE << 16:
			cfg::padding.get_value().top = uGetDlgItemInt(wnd, IDC_PADDING_TOP, NULL, true);
			playlists_dropdown::g_update_all_sizes();
			playlists_dropdown::g_redraw_all();
			break;

		case IDC_PADDING_RIGHT | EN_CHANGE << 16:
			cfg::padding.get_value().right = uGetDlgItemInt(wnd, IDC_PADDING_RIGHT, NULL, true);
			playlists_dropdown::g_update_all_sizes();
			playlists_dropdown::g_redraw_all();
			break;

		case IDC_PADDING_BOTTOM | EN_CHANGE << 16:
			cfg::padding.get_value().bottom = uGetDlgItemInt(wnd, IDC_PADDING_BOTTOM, NULL, true);
			playlists_dropdown::g_update_all_sizes();
			playlists_dropdown::g_redraw_all();
			break;

		case IDC_PADDING_LEFT | EN_CHANGE << 16:
			cfg::padding.get_value().left = uGetDlgItemInt(wnd, IDC_PADDING_LEFT, NULL, true);
			playlists_dropdown::g_update_all_sizes();
			playlists_dropdown::g_redraw_all();
			break;

			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
			//  FIELDS BUTTON
			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		case IDC_FIELDS:
			{
				HMENU menu = CreatePopupMenu();
				for (int i = 1; i < tabsize(g_fields_list); i++) {
					uAppendMenu(menu, !!g_fields_list[i].label ? MF_STRING : MF_SEPARATOR, i, g_fields_list[i].label);
				}
				RECT rc;
				GetWindowRect(GetDlgItem(wnd, IDC_FIELDS), &rc);
				int cmd = TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, rc.left, rc.bottom, 0, GetDlgItem(wnd, IDC_FIELDS), 0);
				DestroyMenu(menu);
				if (cmd >= 0 && cmd < tabsize(g_fields_list) && !!g_fields_list[cmd].label) {
					uSendDlgItemMessageText(wnd, IDC_FORMATTING_STRING, EM_REPLACESEL, TRUE, g_fields_list[cmd].label);
					SetFocus(GetDlgItem(wnd, IDC_FORMATTING_STRING));
				}
			}
			break;

			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
			//  APPLY
			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		case IDC_APPLY:
			apply_config();
			uEnableWindow(uGetDlgItem(wnd, IDC_APPLY), FALSE);
			break;
		}
		break;

	case WM_INITDIALOG:
		{
			EnableTheming(wnd);

			// Setup "Display/Style" TabControl
			HWND hTabWnd = uGetDlgItem(wnd, IDC_FS_TAB);
			uTabCtrl_InsertItemText(hTabWnd, 0, "Display");
			uTabCtrl_InsertItemText(hTabWnd, 1, "Style");
			TabCtrl_SetCurSel(hTabWnd, cfg::last_fs_tab);
			uSetDlgItemText(wnd, IDC_FORMATTING_STRING, get_formatting_string(TabCtrl_GetCurSel(hTabWnd)));

			uButton_SetCheck(wnd, IDC_SHOW_ICONS, cfg::show_icons);

			SetDlgSpinner(wnd, IDC_MIN_VISIBLE_SPIN, IDC_MIN_VISIBLE, 1, 1000, cfg::min_visible);
			SetDlgSpinner(wnd, IDC_PADDING_TOP_SPIN, IDC_PADDING_TOP, 0, 1024, cfg::padding.get_value().top);
			SetDlgSpinner(wnd, IDC_PADDING_RIGHT_SPIN, IDC_PADDING_RIGHT, 0, 1024, cfg::padding.get_value().right);
			SetDlgSpinner(wnd, IDC_PADDING_BOTTOM_SPIN, IDC_PADDING_BOTTOM, 0, 1024, cfg::padding.get_value().bottom);
			SetDlgSpinner(wnd, IDC_PADDING_LEFT_SPIN, IDC_PADDING_LEFT, 0, 1024, cfg::padding.get_value().left);

			uEnableWindow(uGetDlgItem(wnd, IDC_APPLY), FALSE);
			SendMessage(wnd, MSG_UPDATEPREVIEW, 0, 0);
		}
		break;

	case WM_DESTROY:
		cfg::last_fs_tab = TabCtrl_GetDlgCurSel(wnd, IDC_FS_TAB);
		break;

	case WM_NOTIFY:
		if (((LPNMHDR) lp)->idFrom == IDC_FS_TAB && ((LPNMHDR) lp)->code == TCN_SELCHANGE) {
			uSetDlgItemText(wnd, IDC_FORMATTING_STRING, get_formatting_string(TabCtrl_GetDlgCurSel(wnd, IDC_FS_TAB)));
		}
		break;

	default:
		return false;
	}
	return true;
}
