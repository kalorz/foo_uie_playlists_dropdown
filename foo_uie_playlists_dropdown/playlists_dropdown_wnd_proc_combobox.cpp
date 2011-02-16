#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlists Dropdown class implementation: ComboBox window procedure
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

LRESULT CALLBACK playlists_dropdown::ComboBoxWndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_MEASUREITEM:
		{
			  LPMEASUREITEMSTRUCT lpmi = (LPMEASUREITEMSTRUCT) lp;
			  if (lpmi->CtlType == ODT_MENU) {
				  CGdiPlusBitmapResource * bmp = (CGdiPlusBitmapResource *) lpmi->itemData;
				  if (!!bmp) {
					  lpmi->itemWidth = max(16, (**bmp).GetWidth()) + 8;
					  lpmi->itemHeight = max(16, (**bmp).GetHeight()) + 8;
				  }
				  return TRUE;
			  }
		}
		break;

	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lp;
			if (lpdis->CtlType == ODT_MENU) {
				Gdiplus::Graphics g(lpdis->hDC);
				CGdiPlusBitmap * bmp = (CGdiPlusBitmap *) lpdis->itemData;
				RECT rect = lpdis->rcItem;

				const bool checked = (lpdis->itemState & ODS_CHECKED) != 0;
				const bool selected = (lpdis->itemState & (ODS_HOTLIGHT | ODS_SELECTED)) != 0;
				const bool _default = (lpdis->itemState & ODS_DEFAULT) != 0;

				HTHEME hTheme = NULL;
				if (_default) {
					hTheme = OpenThemeData(wnd, VSCLASS_BUTTON);
					if (!!hTheme) {
						const int state = selected ? PBS_HOT : PBS_NORMAL;
						if (IsThemeBackgroundPartiallyTransparent(hTheme, BP_PUSHBUTTON, state)) {
							DrawThemeParentBackground(wnd, lpdis->hDC, &rect);
						}
						DrawThemeBackground(hTheme, lpdis->hDC, BP_PUSHBUTTON, state, &lpdis->rcItem, 0);
					} else {
						DrawFrameControl(lpdis->hDC, &lpdis->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH | (selected ? DFCS_HOT : 0));
					}
				} else {
					HTHEME hTheme = OpenThemeData(wnd, VSCLASS_MENU);
					if (!!hTheme) {
						const int state = selected ? MPI_HOT : MPI_NORMAL;
						if (IsThemeBackgroundPartiallyTransparent(hTheme, MENU_POPUPITEM, state)) {
							DrawThemeParentBackground(wnd, lpdis->hDC, &rect);
						}
						DrawThemeBackground(hTheme, lpdis->hDC, MENU_POPUPITEM, state, &rect, 0);
						if (checked) {
							DrawThemeBackground(hTheme, lpdis->hDC, MENU_POPUPCHECKBACKGROUND, MCB_NORMAL, &rect, 0);
						}
						GetThemeBackgroundContentRect(hTheme, lpdis->hDC, MENU_POPUPITEM, state, &(lpdis->rcItem), &rect);
					} else {
						SetBkColor(lpdis->hDC, GetSysColor(selected ? COLOR_HIGHLIGHT : checked ? COLOR_MENUHILIGHT : COLOR_MENU));
						ExtTextOut(lpdis->hDC, rect.left, rect.top, ETO_OPAQUE, &rect, NULL, 0, NULL);
					}
				}

				if (!!bmp) {
					t_size w = (**bmp).GetWidth();
					t_size h = (**bmp).GetHeight();
					g.DrawImage(*bmp, Gdiplus::Rect(rect.left + ((rect.right-rect.left-w)>>1), rect.top + ((rect.bottom-rect.top-h)>>1), w, h), 0, 0, w, h, Gdiplus::UnitPixel);
				}

				if (checked && selected) {
					CGdiPlusBitmapResource rb;
					if (rb.Load(_T("Cross"), RES_T_INTERNAL, core_api::get_my_instance())) {
						t_size w = (*rb).GetWidth();
						t_size h = (*rb).GetHeight();
						g.DrawImage(rb, Gdiplus::Rect(lpdis->rcItem.right - w, lpdis->rcItem.top, w, h), 0, 0, w, h, Gdiplus::UnitPixel);
					}
				}

				if (!!hTheme) {
					CloseThemeData(hTheme);
				}

				return TRUE;
			}
		}
		break;

	case WM_TIMER:
		if (wp == IDT_AUTOEXPAND) {
			KillTimer(wnd, IDT_AUTOEXPAND);
			ComboBox_ShowDropdown(wnd, true);
		} else if (wp == IDT_MWHEEL) {
			KillTimer(wnd, IDT_MWHEEL);
			static_api_ptr_t<playlist_manager>()->set_active_playlist(ComboBox_GetCurSel(m_hWnd));
			InvalidateRect(m_hWndListBox, NULL, TRUE);
		}
		break;

	case MSG_DROP_FILES_EX:
		{
			KillTimer(wnd, IDT_AUTOEXPAND);
			POINT pt = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
			ClientToScreen(wnd, &pt);
			on_dd_context_menu(wnd, ComboBox_GetCurSel(wnd), pt, (IDataObject *) wp);
		}
		break;

	case MSG_DROP_FILES:
		{
			KillTimer(wnd, IDT_AUTOEXPAND);
			IDataObject * pDataObject = (IDataObject *) wp;
			service_ptr_t<process_locations_notify_impl> p_notify = new service_impl_t<process_locations_notify_impl>(ComboBox_GetCurSel(m_hWnd));
			static_api_ptr_t<playlist_incoming_item_filter_v2>()->process_dropped_files_async(pDataObject, playlist_incoming_item_filter_v2::op_flag_delay_ui, m_hWnd, p_notify);
		}
		break;

	case MSG_DRAG_ENTER:
		KillTimer(wnd, IDT_AUTOEXPAND);
		if (config::dd_autoexpand) {
			UINT_PTR timer = SetTimer(wnd, (UINT_PTR) IDT_AUTOEXPAND, config::dd_autoexpand_delay, NULL);
		}
		break;

	case MSG_DRAG_LEAVE:
		KillTimer(wnd, IDT_AUTOEXPAND);
		break;

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	case WM_CONTEXTMENU:
		{
			RECT  rect;

			// Check if cursor is in 'edit' area or 'scrollbar' area
			GetWindowRect(wnd, &rect);
			InflateRect(&rect, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));
			rect.right -= GetSystemMetrics(SM_CXHSCROLL) - GetSystemMetrics(SM_CXEDGE);

			POINT pt = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
			if (PtInRect(&rect, pt)) {
				t_size idx = ComboBox_GetCurSel(wnd);
				on_context_menu(wnd, idx, pt);
				return TRUE;
			}
		}
		break;

	case WM_MBUTTONUP:
		if (cfg::mclick_action.get_value() != pfc::guid_null) {
			mainmenu_commands::g_execute(cfg::mclick_action.get_value());
		}
		break;

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	case WM_MOUSEWHEEL:
		KillTimer(wnd, IDT_MWHEEL);
		if (config::wheel_use) {
			//int fwKeys = GET_KEYSTATE_WPARAM(wp);
			int zDelta = GET_WHEEL_DELTA_WPARAM(wp);
			int active = ComboBox_GetCurSel(m_hWnd);
			int total  = ComboBox_GetCount(m_hWnd);
			active += (zDelta < 0 ? (active + 1 < total ? 1 : 0) : (active > 0 ? -1 : 0));
			ComboBox_SetCurSel(m_hWnd, active);
			UINT_PTR timer = SetTimer(wnd, (UINT_PTR) IDT_MWHEEL, config::wheel_delay, NULL);
		}
		return 0;

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	case WM_KEYDOWN:
		if (get_host()->get_keyboard_shortcuts_enabled() && wp != VK_F2 && wp != VK_RETURN && wp != VK_ESCAPE && wp != VK_DELETE && wp != VK_LEFT && wp != VK_RIGHT && wp != VK_UP && wp != VK_DOWN && static_api_ptr_t<keyboard_shortcut_manager>()->on_keydown_auto(wp)) {
			return 0;
		} else {
			switch (wp)
			{
			case VK_TAB:
				uie::window::g_on_tab(wnd);
				return 0;
			case VK_DELETE:
				{
					t_size playlist = ComboBox_GetCurSel(wnd);
					static_api_ptr_t<playlist_manager> pm;
					if (playlist != CB_ERR) {
						pfc::string8 name;
						pm->playlist_get_name(playlist, name);
						pfc::string8 title("Are you sure you want to delete the playlist \"");
						title << name << "\"?";
						if (uMessageBox(wnd, title, "Continue?", MB_YESNO) == IDYES) {
							static_api_ptr_t<playlist_manager>()->remove_playlist_switch(playlist);
						}
					}
				}
				return 0;
			case VK_F2:
				{
					t_size playlist = ComboBox_GetCurSel(wnd);
					if (playlist != CB_ERR) {
						pfc::string8 name;
						static_api_ptr_t<playlist_manager> pm;
						pm->playlist_get_name(playlist, name);
						if (g_rename_dialog(name, core_api::get_main_window())) {
							pm->playlist_rename(playlist, name, pfc_infinite);
						}
					}
				}
				return 0;
			case VK_F4:
				{
					t_size playlist = ComboBox_GetCurSel(wnd);
					if (playlist != CB_ERR) {
						metadb_handle_list items;
						static_api_ptr_t<playlist_manager>()->playlist_get_all_items(playlist, items);
						standard_commands::context_save_playlist(items);
						items.remove_all();
					}
				}
				return 0;
			case VK_ESCAPE:
			case VK_RETURN:
				if (!ComboBox_GetDroppedState(m_hWnd)) {
					SetFocus(core_api::get_main_window());
				}
				break;
			}
		}
		break;

	case WM_SYSKEYDOWN: 
		if (get_host()->get_keyboard_shortcuts_enabled() && static_api_ptr_t<keyboard_shortcut_manager>()->on_keydown_auto(wp)) {
			return 0;
		}
		break;

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	case WM_CTLCOLORLISTBOX:
		if (!m_hWndListBox) {
			m_hWndListBox = reinterpret_cast<HWND>(lp);
			SetProp(m_hWndListBox, L"PROP_PDDCLASS", this);
			OldListBoxProc = SubclassWindow(m_hWndListBox, ListBoxWndProcHook);

			m_listbox_dd = CDropTarget::g_create(m_hWndListBox);
			RegisterDragDrop(m_hWndListBox, m_listbox_dd.get_ptr());
		}
		break;
	}

	return CallWindowProc(OldComboBoxProc, wnd, msg, wp, lp);
}
