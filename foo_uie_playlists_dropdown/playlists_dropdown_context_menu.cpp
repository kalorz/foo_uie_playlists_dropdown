#include "component.h"
#include <shlwapi.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlists Dropdown class implementation: context menu
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

BOOL CALLBACK playlists_dropdown::EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam) {
	((pfc::string_list_impl *) lParam)->add_item(pfc::stringcvt::string_utf8_from_wide(lpszName));
	return TRUE;
}

void playlists_dropdown::on_context_menu(HWND wnd, t_size playlist, POINT pt) {
	static_api_ptr_t<ole_interaction> oi;
	static_api_ptr_t<playlist_manager> pm;
	service_ptr_t<contextmenu_manager> cmm;

	HMENU hMenu = CreatePopupMenu();
	HMENU hIconsMenu = NULL;
	HMENU hItemsMenu = NULL;
	HMENU hRestoreMenu = NULL;
	HMENU hSortMenu = NULL;

	pfc::string_list_impl list;
	pfc::ptr_list_t<CGdiPlusBitmap> icons;

	// Construct context menu
	if (playlist != pfc_infinite) {
		if (pm->playlist_get_item_count(playlist) > 0) {
			uAppendMenu(hMenu, MF_STRING, ID_PLAY, "Play");
			SetMenuDefaultItem(hMenu, ID_PLAY, FALSE);
		}
		if (playlist != pm->get_active_playlist()) {
			uAppendMenu(hMenu, MF_STRING, ID_ACTIVATE, "Activate");
			if (pm->playlist_get_item_count(playlist) <= 0) {
				SetMenuDefaultItem(hMenu, ID_ACTIVATE, FALSE);
			}
		}
		uAppendMenu(hMenu, MF_STRING, ID_RENAME, "Rename...");
		uAppendMenu(hMenu, MF_STRING, ID_REMOVE, "Remove");

		// Icons sub-menu
		if (cfg::show_icons) {
			try {
				pfc::string8 icon_path;
				static_api_ptr_t<playlist_manager_v2>()->playlist_get_property(playlist, guid_icon_path, &string_helper::stream_writer_string(icon_path), abort_callback_impl());
				hIconsMenu = CreatePopupMenu();

				CGdiPlusBitmap * bmp;
				if (EnumResourceNames(core_api::get_my_instance(), RES_T_ICON, EnumResNameProc, (LONG_PTR) &list) == TRUE) {
					for (t_size i = 0; i < list.get_count(); i++) {
						bmp = new CGdiPlusBitmapResource();
						if (((CGdiPlusBitmapResource *) bmp)->Load(pfc::stringcvt::string_wide_from_utf8(list[i]), RES_T_ICON, core_api::get_my_instance())) {
							icons.add_item(bmp);
							AppendMenu(hIconsMenu, MF_OWNERDRAW | (i>0&&i%5==0?MF_MENUBREAK:0) |
								(icon_path.get_length() > 0 && !stricmp_utf8_ex(icon_path, icon_path.get_length(), list[i], ~0) ? MF_CHECKED : 0), 
								ID_ICON_FIRST + i, (LPCWSTR) bmp);
						} else { delete bmp; bmp = NULL; }
					}
				}

				// Last used custom files
				pfc::ptr_list_t<char> tmp1, tmp2;
				string_helper::string_list::build_list(cfg::custom_icons_history, tmp1);
				
				t_size count = tmp1.get_count(), j = 0;
				for (t_size i = 0; i < count; i++) {
					bmp = new CGdiPlusBitmap();
					if (uFileExists(tmp1[i]) && bmp->Load(pfc::stringcvt::string_wide_from_utf8(tmp1[i])) && (**bmp).GetWidth() <= 64 && (**bmp).GetHeight() <= 64) {
							icons.add_item(bmp);
							tmp2.add_item(_strdup(tmp1[i]));
							list.add_item(tmp1[i]);
							AppendMenu(hIconsMenu, MF_OWNERDRAW | (j==0?MF_MENUBARBREAK:j%5==0?MF_MENUBREAK:0) |
								(icon_path.get_length() > 0 && !stricmp_utf8_ex(icon_path, icon_path.get_length(), tmp1[i], ~0) ? MF_CHECKED : 0), ID_ICON_FIRST + list.get_count() - 1, (LPCWSTR) bmp);
							j++;
					} else { delete bmp; bmp = NULL; }
				}
				string_helper::string_list::parse_list(tmp2, cfg::custom_icons_history);

				tmp1.free_all();
				tmp2.free_all();

				bmp = new CGdiPlusBitmapResource();
				if (((CGdiPlusBitmapResource *) bmp)->Load(_T("PlusCircle"), RES_T_INTERNAL, core_api::get_my_instance())) {
					icons.add_item(bmp);
					AppendMenu(hIconsMenu, MF_OWNERDRAW | (j==0?MF_MENUBARBREAK:j%5==0?MF_MENUBREAK:0), ID_ICON_CUSTOM, (LPCWSTR) bmp);
				} else { 
					delete bmp; bmp = NULL;
					uAppendMenu(hIconsMenu, MF_STRING | (j==0?MF_MENUBARBREAK:j%5==0?MF_MENUBREAK:0), ID_ICON_CUSTOM, "Custom ...");
				}
				SetMenuDefaultItem(hIconsMenu, ID_ICON_CUSTOM, FALSE); // Hack to mark special button
				uAppendMenu(hMenu, MF_POPUP, (UINT_PTR) hIconsMenu, "Icon");
			} catch(...) { }
		}
	
		if (pm->playlist_lock_is_present(playlist)) {
			pfc::string8 name;
			pm->playlist_lock_query_name(playlist, name);
			name << " ...";
			uAppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			uAppendMenu(hMenu, MF_STRING, ID_AUTOPLAYLIST, name.get_ptr());
		}
		uAppendMenu(hMenu, MF_SEPARATOR, 0, 0);
		uAppendMenu(hMenu, MF_STRING, ID_CUT, "Cut");
		uAppendMenu(hMenu, MF_STRING, ID_COPY, "Copy");
	}

	IDataObject* pDataObj;
	HRESULT hr = OleGetClipboard(&pDataObj);
	if (hr == S_OK) {
		bool isNative;
		DWORD dropEffect;
		hr = oi->check_dataobject(pfc::com_ptr_t<IDataObject>(pDataObj), dropEffect, isNative);
		if (hr == S_OK) {
			hr = oi->check_dataobject_playlists(pfc::com_ptr_t<IDataObject>(pDataObj));
			if (hr == S_OK) {
				uAppendMenu(hMenu, MF_STRING, ID_PASTE_PLAYLIST, "Paste");
			} else {
				HMENU hPasteMenu = CreatePopupMenu();
				if (playlist != pfc_infinite && !pm->playlist_lock_is_present(playlist)) {
					uAppendMenu(hPasteMenu, MF_STRING, ID_PASTE_ITEMS_ADD, "Add");
					uAppendMenu(hPasteMenu, MF_STRING, ID_PASTE_ITEMS_REPLACE, "Replace");
				}
				uAppendMenu(hPasteMenu, MF_STRING, ID_PASTE_ITEMS_CREATE, "New playlist");
				uAppendMenu(hMenu, MF_POPUP, (UINT_PTR) hPasteMenu, "Paste files");
				hr = S_OK;
			}
		}
	}
	if (playlist != pfc_infinite || hr == S_OK) {
		uAppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	}

	uAppendMenu(hMenu, MF_STRING, ID_NEW, "New");
	uAppendMenu(hMenu, MF_STRING, ID_LOAD, "Load...");
	if (playlist != pfc_infinite) {
		uAppendMenu(hMenu, MF_STRING, ID_SAVE_AS, "Save as...");
	}
	if (pm->get_playlist_count() > 0) {
		uAppendMenu(hMenu, MF_STRING, ID_SAVE_ALL_AS, "Save all as...");
	}
	if (pm->get_playlist_count() > 1) {
		hSortMenu = CreatePopupMenu();
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_BY, "Sort by ...");
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_RANDOMIZE, "Randomize");
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_REVERSE, "Reverse");
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_BY_NAME, "Sort by Name");
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_BY_SIZE, "Sort by Size");
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_BY_FILESIZE, "Sort by File Size");
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_BY_LENGTH, "Sort by Length");
		uAppendMenu(hSortMenu, MF_STRING, ID_SORT_BY_LOCK, "Sort by Lock Name");
		uAppendMenu(hMenu, MF_POPUP, (UINT_PTR) hSortMenu, "Sort playlists");
	}
	try {
		static_api_ptr_t<playlist_manager_v3> pm2;
		t_size recycler_count = min(pm2->recycler_get_count(), ID_HISTORY_LAST - ID_HISTORY_FIRST);
		if (recycler_count > 0) {
			hRestoreMenu = CreatePopupMenu();
			pfc::string8 tmp;
			for (t_size i = 0; i < recycler_count; i++) {
				pm2->recycler_get_name(i, tmp);
				uAppendMenu(hRestoreMenu, MF_STRING, ID_HISTORY_FIRST + i, tmp.get_ptr());
			}
			uAppendMenu(hRestoreMenu, MF_SEPARATOR, 0, 0);
			uAppendMenu(hRestoreMenu, MF_STRING, ID_HISTORY_PURGE, "Clear History");
			uAppendMenu(hMenu, MF_POPUP, (UINT_PTR) hRestoreMenu, "Restore");
		}
	} catch(...) { }

	if (playlist != pfc_infinite) {
		metadb_handle_list items;
		pm->playlist_get_all_items(playlist, items);
		contextmenu_manager::g_create(cmm);
		const bool show_shortcuts = config_object::g_get_data_bool_simple(standard_config_objects::bool_show_keyboard_shortcuts_in_menus, false);
		unsigned flags = show_shortcuts ? contextmenu_manager::FLAG_SHOW_SHORTCUTS : 0;
		cmm->init_context(items, flags);
		if (cmm->get_root()) {
			hItemsMenu = CreatePopupMenu();
			cmm->win32_build_menu(hItemsMenu, ID_CONTEXT_FIRST, ID_CONTEXT_LAST);
			uAppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			uAppendMenu(hMenu, MF_POPUP, (UINT_PTR) hItemsMenu, "Contents");
		}
	}

	menu_helpers::win32_auto_mnemonics(hMenu);
	int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, 0);

	// Free resources
	DestroyMenu(hIconsMenu);
	DestroyMenu(hSortMenu);
	DestroyMenu(hRestoreMenu);
	DestroyMenu(hItemsMenu);
	DestroyMenu(hMenu);
	icons.delete_all();

	if (m_block_capture) {
		m_block_capture = false;
		SetFocus(wnd);
		SetCapture(wnd);
	} else {
		ComboBox_ShowDropdown(m_hWnd, false);
	}

	// Process command
	if (cmd == ID_PLAY) {
		pm->set_playing_playlist(playlist);
		static_api_ptr_t<playback_control>()->start();
	} else if (cmd == ID_ACTIVATE) {
		pm->set_active_playlist(playlist);
	} else if (cmd == ID_RENAME) {
		pfc::string8 name;
		pm->playlist_get_name(playlist, name);
		if (g_rename_dialog(name, core_api::get_main_window())) {
			pm->playlist_rename(playlist, name, pfc_infinite);
		}
	} else if (cmd >= ID_ICON_FIRST && cmd <= ID_ICON_LAST) {
		try {
			static_api_ptr_t<playlist_manager_v2> pm2;
			pfc::string8 s = list[cmd - ID_ICON_FIRST];
			pfc::string8 current;
			pm2->playlist_get_property(playlist, guid_icon_path, &string_helper::stream_writer_string(current), abort_callback_impl());
			if (!stricmp_utf8(s, current)) {
				pm2->playlist_remove_property(playlist, guid_icon_path);
			} else {
				pm2->playlist_set_property(playlist, guid_icon_path,					
					&stream_reader_memblock_ref(s.get_ptr(), pfc::strlen_max(s.get_ptr(), s.get_length())),
					s.get_length(), abort_callback_impl());
			}
		} catch(...) { }
		g_reload_icons();
		g_update_all_sizes();
		g_redraw_all();
	} else if (cmd == ID_ICON_CUSTOM) {
		try {
			static_api_ptr_t<playlist_manager_v2> pm2;
			pfc::string8 icon_path;
			pfc::string8 last_icon_path;
			string_helper::string_list::get_first(cfg::custom_icons_history, last_icon_path);
			if (uGetOpenFileName(wnd, "Image files|*.png;*.gif;*.bmp", 0, "", "Choose image file...", last_icon_path, icon_path, false)) {
				CGdiPlusBitmap bmp;
				if (bmp.Load(pfc::stringcvt::string_wide_from_utf8(icon_path)) && (*bmp).GetWidth() <= 64 && (*bmp).GetHeight() <= 64) {
					string_helper::string_list::add_item(cfg::custom_icons_history, icon_path);
					pm2->playlist_set_property(playlist, guid_icon_path,					
						&stream_reader_memblock_ref(icon_path.get_ptr(), pfc::strlen_max(icon_path.get_ptr(), icon_path.get_length())),
						icon_path.get_length(), abort_callback_impl());
				} else {
					popup_message::g_complain("Selected image is too big. Maximum dimensions are 64x64.");
				}
			}
		} catch(...) { }
		g_reload_icons();
		g_update_all_sizes();
		g_redraw_all();
	} else if (cmd == ID_SORT_BY) {
		pfc::string8 pattern;
		if (g_sort_dialog(pattern, core_api::get_main_window()) && pattern.get_length() > 0) {
			playlists_helper::sort_by_format(pattern);
		}
	} else if (cmd == ID_SORT_REVERSE) {
		playlists_helper::sort_reverse();
	} else if (cmd == ID_SORT_RANDOMIZE) {
		playlists_helper::sort_by_format("$rand()");
	} else if (cmd == ID_SORT_BY_NAME) {
		playlists_helper::sort_by_format("%title%");
	} else if (cmd == ID_SORT_BY_SIZE) {
		playlists_helper::sort_by_format("%size%");
	} else if (cmd == ID_SORT_BY_FILESIZE) {
		playlists_helper::sort_by_format("%filesize%");
	} else if (cmd == ID_SORT_BY_LENGTH) {
		playlists_helper::sort_by_format("%length_seconds%");
	} else if (cmd == ID_SORT_BY_LOCK) {
		playlists_helper::sort_by_format("%lock%");
	} else if (cmd == ID_AUTOPLAYLIST) {
		pm->playlist_lock_show_ui(playlist);
	} else if (cmd == ID_CUT || cmd == ID_COPY) {
		playlist_dataobject_desc_impl pdod;
		pfc::string8 name;        pm->playlist_get_name(playlist, name);
		metadb_handle_list items; pm->playlist_get_all_items(playlist, items);
		pdod.set_entry_count(1);
		pdod.set_entry_name(0, name.get_ptr());
		pdod.set_entry_content(0, items);
		pfc::com_ptr_t<IDataObject> ido = oi->create_dataobject(pdod);
		OleSetClipboard(ido.get_ptr());
		if (cmd == ID_CUT) {
			pm->remove_playlist_switch(playlist);
		}
	} else if (cmd == ID_PASTE_PLAYLIST) {
		playlist_dataobject_desc_impl pdod;
		oi->parse_dataobject_playlists(pfc::com_ptr_t<IDataObject>(pDataObj), pdod);
		pfc::string8 name;
		metadb_handle_list items;
		t_size playlist = pfc_infinite;
		for (t_size i = 0, count = pdod.get_entry_count(); i < count; i++) {
			pdod.get_entry_name(i, name);
			pdod.get_entry_content(i, items);
			playlist = pm->create_playlist(name, pfc_infinite, pfc_infinite);
			pm->playlist_add_items(playlist, items, bit_array_false());
		}
		if (cfg::dd_activate && playlist != pfc_infinite) {
			pm->set_active_playlist(playlist);
		}
	} else if (cmd == ID_PASTE_ITEMS_ADD || cmd == ID_PASTE_ITEMS_REPLACE || cmd == ID_PASTE_ITEMS_CREATE) {
		t_size tmp = cmd == ID_PASTE_ITEMS_ADD ? process_locations_notify_impl::ADD :
			cmd == ID_PASTE_ITEMS_REPLACE ? process_locations_notify_impl::REPLACE :
			process_locations_notify_impl::CREATE;
		service_ptr_t<process_locations_notify> p_notify = new service_impl_t<process_locations_notify_impl>(playlist, tmp);
		static_api_ptr_t<playlist_incoming_item_filter_v2>()->process_dropped_files_async(pDataObj, playlist_incoming_item_filter_v2::op_flag_delay_ui, m_hWnd, p_notify);
	} else if (cmd == ID_REMOVE) {
		pm->remove_playlist_switch(playlist);
	} else if (cmd == ID_NEW) {
		pm->set_active_playlist(pm->create_playlist_autoname());
	} else if (cmd == ID_LOAD) {
		standard_commands::main_load_playlist();
	} else if (cmd == ID_SAVE_AS) {
		metadb_handle_list items;
		pm->playlist_get_all_items(playlist, items);
		standard_commands::context_save_playlist(items);
		items.remove_all();
	} else if (cmd == ID_SAVE_ALL_AS) {
		standard_commands::main_save_all_playlists();
	} else if (cmd >= ID_HISTORY_FIRST && cmd <= ID_HISTORY_LAST) {
		try {
			static_api_ptr_t<playlist_manager_v3>()->recycler_restore(cmd - ID_HISTORY_FIRST);
		} catch(...) { }
	} else if (cmd == ID_HISTORY_PURGE) {
		try {
			static_api_ptr_t<playlist_manager_v3>()->recycler_purge(bit_array_true());
		} catch(...) { }
	} else if (cmd >= ID_CONTEXT_FIRST && cmd <= ID_CONTEXT_LAST && cmm->get_root()) {
		cmm->execute_by_id(cmd - ID_CONTEXT_FIRST);
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::on_dd_context_menu(HWND wnd, t_size playlist, POINT pt, IDataObject * pDataObject) {
	HMENU menu;
	menu = CreatePopupMenu();
	uAppendMenu(menu, MF_STRING, process_locations_notify_impl::ADD, "Add files here");
	uAppendMenu(menu, MF_STRING, process_locations_notify_impl::REPLACE, "Replace files here");
	uAppendMenu(menu, MF_STRING, process_locations_notify_impl::CREATE, "Create new playlist");
	uAppendMenu(menu, MF_SEPARATOR, 0, 0);
	uAppendMenu(menu, MF_STRING, 0, "Cancel");
	int cmd = TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, 0, wnd, 0);
	DestroyMenu(menu);
	if (m_block_capture == true) {
		m_block_capture = false;
		SetFocus(wnd);
		SetCapture(wnd);
	}
	if (cmd > 0) {
		service_ptr_t<process_locations_notify> p_notify = new service_impl_t<process_locations_notify_impl>(playlist, cmd);
		static_api_ptr_t<playlist_incoming_item_filter_v2>()->process_dropped_files_async(pDataObject, playlist_incoming_item_filter_v2::op_flag_delay_ui, m_hWnd, p_notify);
	}
}
