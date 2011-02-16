#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlists Dropdown class implementation: main window procedure
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

LRESULT playlists_dropdown::on_message(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch(msg)
	{
	case MSG_UPDATE:
		redraw_list();
		break;

	case WM_CREATE:
		{
			Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

			OleInitialize(0);
			m_tracker.initialize(this);
			g_notify_list.add_item(wnd);

			m_hWnd = CreateWindowEx(0, WC_COMBOBOX, 0,
				CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
				0, 0, 0, 0, wnd, HMENU(0), core_api::get_my_instance(), NULL);
			ComboBox_SetExtendedUI(m_hWnd, true);
			ComboBox_SetMinVisible(m_hWnd, cfg::min_visible);
			SendMessage(m_hWnd, WM_SETFONT, (WPARAM) cui::fonts::helper(guid_fonts).get_font(), MAKELPARAM(TRUE, 0));
			m_hWndListBox = NULL;

			g_reload_icons();

			// Check if this is the first instance
			if (g_instances.get_count() == 1) {
				if (!g_playlist_callback) g_playlist_callback = new playlist_callback_impl();
				if (!g_play_callback) g_play_callback = new play_callback_impl();
				// Register callbacks
				static_api_ptr_t<playlist_manager>()->register_callback(g_playlist_callback, playlist_callback_impl::get_flags());
				static_api_ptr_t<play_callback_manager>()->register_callback(g_play_callback, play_callback_impl::get_flags(), false);
				// Compile formatters
				g_compile_formatters();
			}

			playlists_dropdown::g_update_all_sizes();

			redraw_list();

			SetWindowLongPtr(m_hWnd, GWL_USERDATA, (LPARAM) (this));
			OldComboBoxProc = (WNDPROC) SubclassWindow(m_hWnd, ComboBoxWndProcHook);

			m_combobox_dd = CDropTarget::g_create(m_hWnd);
			RegisterDragDrop(m_hWnd, m_combobox_dd.get_ptr());
		}
		break;

	case WM_SIZE:
		RedrawWindow(m_hWnd, 0, 0, RDW_INVALIDATE | RDW_ERASE);
		SetWindowPos(m_hWnd, 0, 0, 0, LOWORD(lp), HIWORD(lp), SWP_NOZORDER);
		break;

	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO mmi = (LPMINMAXINFO) lp;
			mmi->ptMinTrackSize.x = 120;
			mmi->ptMinTrackSize.y = mmi->ptMaxTrackSize.y = g_window_height;
		}
		break;

	case WM_DESTROY:
		{
			// Clean ListBox resources
			if (!!m_hWndListBox) {
				RemoveProp(m_hWndListBox, L"PROP_PDDCLASS");
				RevokeDragDrop(m_hWndListBox);
				m_listbox_dd.release();
			}

			// Check if this is the last instance
			if (g_instances.get_count() == 1) {
				// Unregister callbacks
				static_api_ptr_t<playlist_manager>()->unregister_callback(g_playlist_callback);
				static_api_ptr_t<play_callback_manager>()->unregister_callback(g_play_callback);
				delete g_playlist_callback; g_playlist_callback = NULL;
				delete g_play_callback; g_play_callback = NULL;

				// Release system resources
				g_unload_icons();
				if (!!g_playing_overlay) { delete g_playing_overlay; g_playing_overlay = NULL; }
				if (!!g_active_overlay)  { delete g_active_overlay;  g_active_overlay = NULL; }
			}

			m_tracker.uninitialize();
			g_notify_list.remove_item(wnd);
			if (m_hWndListBox) {
				SetWindowLong(m_hWndListBox, GWL_WNDPROC, (LONG) OldListBoxProc);
			}

			RevokeDragDrop(m_hWnd);
			m_combobox_dd.release();
			OleUninitialize();
			Gdiplus::GdiplusShutdown(m_gdiplusToken);

			m_hWnd = NULL;
		}
		break;

		/*
		This method interferes with owner drawn context menu
	case WM_ERASEBKGND:
		return TRUE;
		*/

	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lp;

			static_api_ptr_t<playlist_manager> pm;
			cui::colours::helper colours(guid_colours);

			//console::info("DRAW!");

			UINT playlist = lpdis->itemID;
			bool dropped  = ComboBox_GetDroppedState(m_hWnd) == TRUE;
			bool selected = (lpdis->itemState & ODS_SELECTED) != 0;

			if (playlist == -1 || playlist >= pm->get_playlist_count()) {
				SetDCBrushColor(lpdis->hDC, selected ?
					colours.get_colour(cui::colours::colour_selection_background) :
					colours.get_colour(cui::colours::colour_background));
				FillRect(lpdis->hDC, &(lpdis->rcItem), (HBRUSH) GetStockObject(DC_BRUSH));
#ifdef _DEBUG
				if (playlist != -1 && playlist >= pm->get_playlist_count()) {
					console::printf("WM_DRAWITEM: playlist >= pm->get_playlist_count() (%d >= %d)", playlist, pm->get_playlist_count());
				}
#endif
				break;
			}

			titleformat_hook_impl_splitter hook(titleformat_hook_impl_splitter(
				&title_format_hook(playlist, dropped),
				&titleformat_hook_impl_list(playlist, pm->get_playlist_count())));

			// Get formatted title
			pfc::string8 title;
			g_title_format->run(&hook, title, NULL);

			// Get style
			COLORREF text = colours.get_colour(selected ? cui::colours::colour_selection_text : cui::colours::colour_text);
			COLORREF background = colours.get_colour(selected ? cui::colours::colour_selection_background : cui::colours::colour_background);
			g_style_format->run(&titleformat_hook_impl_splitter(&hook, &style_format_hook(text, background, selected)), pfc::string8(), NULL);

			SetDCBrushColor(lpdis->hDC, background);
			FillRect(lpdis->hDC, &(lpdis->rcItem), (HBRUSH) GetStockObject(DC_BRUSH));

			lpdis->rcItem.left   += cfg::padding.get_value().left;
			lpdis->rcItem.right  -= cfg::padding.get_value().right;
			lpdis->rcItem.top    += cfg::padding.get_value().top;
			lpdis->rcItem.bottom -= cfg::padding.get_value().bottom;

			if (cfg::show_icons) {
				Gdiplus::Graphics g(lpdis->hDC);
				static_api_ptr_t<playback_control> pc;

				CGdiPlusBitmap * bmp = NULL;
				try {
					pfc::string8 icon_path;
					if (static_api_ptr_t<playlist_manager_v2>()->playlist_get_property(playlist, guid_icon_path, &string_helper::stream_writer_string(icon_path), abort_callback_impl())
						&& !g_icons->query(icon_path, bmp)) {
							delete bmp; bmp = NULL;
					}
				} catch(...) { }

				if (bmp == NULL && !g_icons->query(pm->playlist_lock_is_present(playlist) ? "Autoplaylist" : "Playlist", bmp)) {
					delete bmp; bmp = NULL;
				}

				RECT rc_icon = { lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.left + g_icons_width, lpdis->rcItem.top + g_icons_height };

				// Playlist icon
				if (!!bmp) {
					t_size w = (**bmp).GetWidth();
					t_size h = (**bmp).GetHeight();

					rc_icon.left  += (g_icons_width - w) >> 1;
					rc_icon.top   += (lpdis->rcItem.bottom - lpdis->rcItem.top - h) >> 1;
					rc_icon.right  = rc_icon.left + w;
					rc_icon.bottom = rc_icon.top + h;

					g.DrawImage(*bmp, Gdiplus::Rect(rc_icon.left, rc_icon.top, w, h), 0, 0, w, h, Gdiplus::UnitPixel);
				}

				// Playing Playlist Overlay
				if (pc->is_playing() && playlist == pm->get_playing_playlist() && !!g_playing_overlay) {
					t_size w = (**g_playing_overlay).GetWidth();
					t_size h = (**g_playing_overlay).GetHeight();
					g.DrawImage(*g_playing_overlay, Gdiplus::Rect(rc_icon.left, rc_icon.bottom - h, w, h), 0, 0, w, h, Gdiplus::UnitPixel);
				}

				// Active Playlist Overlay
				if (dropped && playlist == pm->get_active_playlist() && !!g_active_overlay) {
					t_size w = (**g_active_overlay).GetWidth();
					t_size h = (**g_active_overlay).GetHeight();
					g.DrawImage(*g_active_overlay, Gdiplus::Rect(rc_icon.right - w, rc_icon.bottom - h, w, h), 0, 0, w, h, Gdiplus::UnitPixel);
				}

				lpdis->rcItem.left += g_icons_width + cfg::padding.get_value().left;
			}

			uTextOutColorsTabbed(lpdis->hDC, title, pfc_infinite, &(lpdis->rcItem), 0, &(lpdis->rcItem), selected, selected ? (text ^ 0xffffff) : text, true);
		}
		return TRUE;

	case WM_COMMAND:
		//console::printf("COMMAND: %d (SRC: %d / %d) %d / %d", HIWORD(wp), LOWORD(wp), lp, m_hWnd, m_hWndListBox);
		switch (HIWORD(wp))
		{
		case CBN_SELENDOK:
			static_api_ptr_t<playlist_manager>()->set_active_playlist(ComboBox_GetCurSel(m_hWnd));
			break;
		case CBN_CLOSEUP:
			if (!!m_hWndListBox) {
				KillTimer(m_hWndListBox, IDT_AUTOSWITCH);
			}
			ComboBox_SetCurSel(m_hWnd, static_api_ptr_t<playlist_manager>()->get_active_playlist());
			break;
		}
	}

	return DefWindowProc(wnd, msg, wp, lp);
}
