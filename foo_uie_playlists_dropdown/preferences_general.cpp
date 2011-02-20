#include "component.h"

#ifdef ___REMOVE_THIS___

pfc::ptr_list_t<GUID>	playlists_dropdown::preferences::g_guids;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  General preferences dialog procedure                                                                                   
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
BOOL CALLBACK playlists_dropdown::preferences::GeneralDlgProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
			//  MOUSE
			// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		case IDC_MBUTTON_REORDER:
			cfg::mbutton_reorder = IsDlgButtonChecked(wnd, IDC_MBUTTON_REORDER) & BST_CHECKED;
			break;

		case IDC_MW_USE:
			cfg::wheel_use = IsDlgButtonChecked(wnd, IDC_MW_USE) & BST_CHECKED;
			break;

		case IDC_MW_DELAY | EN_CHANGE << 16:
			cfg::wheel_delay = uGetDlgItemInt(wnd, IDC_MW_DELAY, NULL, true);
			break;

		case IDC_MCLICK | CBN_SELCHANGE << 16:
			{
				LRESULT res = uSendDlgItemMessage(wnd, IDC_MCLICK, CB_GETCURSEL, 0, 0);
				LRESULT cmd = uSendDlgItemMessage(wnd, IDC_MCLICK, CB_GETITEMDATA, res, 0);
				if (cmd != CB_ERR) {
					cfg::mclick_action = *((LPIID) cmd);
				}
			}
			break;

		case IDC_DD_AUTOEXPAND:
			cfg::dd_autoexpand = IsDlgButtonChecked(wnd, IDC_DD_AUTOEXPAND) & BST_CHECKED;
			break;

		case IDC_DD_AUTOSWITCH:
			cfg::dd_autoswitch = IsDlgButtonChecked(wnd, IDC_DD_AUTOSWITCH) & BST_CHECKED;
			break;

		case IDC_DD_ACTIVATE:
			cfg::dd_activate = IsDlgButtonChecked(wnd, IDC_DD_ACTIVATE) & BST_CHECKED;
			break;

		case IDC_DD_AUTOEXPAND_DELAY | EN_CHANGE << 16:
			cfg::dd_autoexpand_delay = uGetDlgItemInt(wnd, IDC_DD_AUTOEXPAND_DELAY, NULL, false);
			break;

		case IDC_DD_AUTOSWITCH_DELAY | EN_CHANGE << 16:
			cfg::dd_autoswitch_delay = uGetDlgItemInt(wnd, IDC_DD_AUTOSWITCH_DELAY, NULL, false);
			break;

		}
		break;

	case WM_INITDIALOG:
		{
			EnableTheming(wnd);

			// Prepare main menu commands list
			LRESULT res;
			LPIID cmd;
			service_enum_t<mainmenu_commands> e; service_ptr_t<mainmenu_commands> ptr;
			while(e.next(ptr)) {
				const t_uint32 total = ptr->get_command_count();
				for (t_uint32 walk = 0; walk < total; ++walk) {
					pfc::string8 path, name;

					g_guids.add_item(cmd = new GUID(ptr->get_command(walk)));
					mainmenu_helper::get_group_path(ptr->get_parent(), path);
					ptr->get_name(walk, name);
					path << name;

					res = uSendDlgItemMessageText(wnd, IDC_MCLICK, CB_ADDSTRING, 0, path);
					uSendDlgItemMessage(wnd, IDC_MCLICK, CB_SETITEMDATA, res, (LPARAM) cmd);
					if (*cmd == cfg::mclick_action) {
						uSendDlgItemMessage(wnd, IDC_MCLICK, CB_SETCURSEL, res, 0);
					}
				}
			}
			g_guids.add_item(cmd = new GUID(pfc::guid_null));
			res = uSendDlgItemMessageText(wnd, IDC_MCLICK, CB_INSERTSTRING, 0, "(None)");
			uSendDlgItemMessage(wnd, IDC_MCLICK, CB_SETITEMDATA, res, (LPARAM) cmd);
			if (cfg::mclick_action == pfc::guid_null) {
				uSendDlgItemMessage(wnd, IDC_MCLICK, CB_SETCURSEL, res, 0);
			}

			uButton_SetCheck(wnd, IDC_MBUTTON_REORDER, cfg::mbutton_reorder);
			uButton_SetCheck(wnd, IDC_MW_USE, cfg::wheel_use);
			SetDlgSpinner(wnd, IDC_MW_DELAY_SPIN, IDC_MW_DELAY, 50, 1000, cfg::wheel_delay);

			uButton_SetCheck(wnd, IDC_DD_AUTOEXPAND, cfg::dd_autoexpand);
			uButton_SetCheck(wnd, IDC_DD_ACTIVATE, cfg::dd_activate);
			uButton_SetCheck(wnd, IDC_DD_AUTOSWITCH, cfg::dd_autoswitch);

			SetDlgSpinner(wnd, IDC_DD_AUTOEXPAND_DELAY_SPIN, IDC_DD_AUTOEXPAND_DELAY, 0, 2000, cfg::dd_autoexpand_delay);
			SetDlgSpinner(wnd, IDC_DD_AUTOSWITCH_DELAY_SPIN, IDC_DD_AUTOSWITCH_DELAY, 0, 2000, cfg::dd_autoswitch_delay);
		}
		break;

	case WM_DESTROY:
		g_guids.delete_all();
		break;

	}
	return FALSE;
}

#endif ___REMOVE_THIS___