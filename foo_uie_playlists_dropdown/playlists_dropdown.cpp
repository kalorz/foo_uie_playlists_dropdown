#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlists Dropdown class implementation
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

pfc::instance_tracker_server_t<playlists_dropdown>
									playlists_dropdown::g_instances;

playlists_dropdown::t_icons_map*	playlists_dropdown::g_icons = NULL;
CGdiPlusBitmap*						playlists_dropdown::g_playing_overlay = NULL;
CGdiPlusBitmap*						playlists_dropdown::g_active_overlay = NULL;
pfc::list_t<HWND>					playlists_dropdown::g_notify_list;
unsigned							playlists_dropdown::g_window_height = 0;
unsigned							playlists_dropdown::g_icons_width = 0;
unsigned							playlists_dropdown::g_icons_height = 0;

playlists_dropdown::playlist_callback_impl*
									playlists_dropdown::g_playlist_callback;
playlists_dropdown::play_callback_impl*
									playlists_dropdown::g_play_callback;

titleformat_object::ptr				playlists_dropdown::g_title_format;
titleformat_object::ptr				playlists_dropdown::g_style_format;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::redraw_list() {
	static_api_ptr_t<playlist_manager> pm;

	t_size c_count = ComboBox_GetCount(m_hWnd);
	t_size t_count = pm->get_playlist_count();

	if (c_count != t_count) {
		if (c_count < t_count) {
			for (t_size n = 0; n < t_count-c_count; n++) ComboBox_AddString(m_hWnd, 0);
		} else {
			for (t_size n = 0; n < c_count-t_count; n++) ComboBox_DeleteString(m_hWnd, 0);
		}
	}

	RedrawWindow(m_hWnd, 0, 0, RDW_INVALIDATE);
	RedrawWindow(m_hWndListBox, 0, 0, RDW_INVALIDATE);
	ComboBox_SetCurSel(m_hWnd, pm->get_active_playlist());
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Static helpers / controllers
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

bool playlists_dropdown::g_rename_dialog(pfc::string_base & param, HWND parent) {
	return !!uDialogBox(IDD_RENAME, parent, RenameDialogProc, reinterpret_cast<long>(&param));
}

bool playlists_dropdown::g_sort_dialog(pfc::string_base & param, HWND parent) {
	return !!uDialogBox(IDD_SORT, parent, SortDialogProc, reinterpret_cast<long>(&param));
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_compile_formatters() {
	static_api_ptr_t<titleformat_compiler> cmp;
	if (!cmp->compile(g_title_format, cfg::title_string)) { cmp->compile_force(g_title_format, "%title%"); }
	if (!cmp->compile(g_style_format, cfg::style_string)) { cmp->compile_force(g_style_format, ""); }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_redraw_all() {
	if (g_instances.get_count() == 0) return;

    for (t_size n = 0, count = g_notify_list.get_count(); n < count; n++) {
        HWND wnd = g_notify_list[n];
        if (!!wnd) {
            uSendMessage(wnd, MSG_UPDATE, 0, 0);
        }
    }
	/*
	for (t_size n = 0, count = g_instances.get_count(); n < count; n++) {
		g_instances[n]->redraw_list();
	}
	*/
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_update_all_fonts() {
	if (g_instances.get_count() == 0) return;

	HFONT hFont = cui::fonts::helper(guid_fonts).get_font();
	for (t_size n = 0, count = g_instances.get_count(); n < count; n++) {
		HWND wnd = g_instances[n]->m_hWnd;
		if (!!wnd) {
			PostMessage(wnd, WM_SETFONT, (WPARAM) hFont, MAKELPARAM(TRUE, 0));
		}
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_update_all_sizes() {
	if (g_instances.get_count() == 0) return;
	
	unsigned font_height = uGetFontHeight(cui::fonts::helper(guid_fonts).get_font());
	unsigned item_height = (cfg::show_icons ? max(g_icons_height, font_height) : font_height)
		+ cfg::padding.get_value().top + cfg::padding.get_value().bottom;

	for (t_size n = 0, count = g_instances.get_count(); n < count; n++) {
		HWND wnd = g_instances[n]->m_hWnd;
		if (!!wnd) {
			ComboBox_SetItemHeight(wnd, -1, item_height);
			ComboBox_SetItemHeight(wnd,  0, item_height);
			RECT rc;
			GetWindowRect(wnd, &rc);
			g_window_height = rc.bottom - rc.top;
			g_instances[n]->get_host()->on_size_limit_change(g_notify_list[n], uie::size_limit_minimum_height | uie::size_limit_maximum_height);
		}
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_update_all_min_visible() {
	if (g_instances.get_count() == 0) return;

	for (t_size n = 0, count = g_instances.get_count(); n < count; n++) {
		HWND wnd = g_instances[n]->m_hWnd;
		if (!!wnd) {
			ComboBox_SetMinVisible(wnd, cfg::min_visible);
        }
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_unload_icons() {
	if (!!g_icons) {
		g_icons->enumerate(g_delete_icon_func);
		g_icons->remove_all();
		delete g_icons;
		g_icons = NULL;
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_reload_icons() {
#ifdef _DEBUG
	console::info("playlists_dropdown::g_reload_icons()");
#endif
	g_icons_width = g_icons_height = 0;

	if (!cfg::show_icons || g_instances.get_count() == 0) {
		g_unload_icons();
		if (!!g_playing_overlay) { delete g_playing_overlay; g_playing_overlay = NULL; }
		if (!!g_active_overlay)  { delete g_active_overlay;  g_active_overlay  = NULL; }
		return;
	}

	// Load Playing Playlist Overlay
	if (!g_playing_overlay) {
		g_playing_overlay = new CGdiPlusBitmapResource();
		if (!((CGdiPlusBitmapResource*) g_playing_overlay)->Load(_T("Playing"), RES_T_OVERLAY, core_api::get_my_instance())) {
			delete g_playing_overlay; g_playing_overlay = NULL;
		}
	}
	if (!!g_playing_overlay) {
		g_icons_width = (**g_playing_overlay).GetWidth();
		g_icons_height = (**g_playing_overlay).GetHeight();
	}

	// Load Active Playlist Overlay
	if (!g_active_overlay) {
		g_active_overlay = new CGdiPlusBitmapResource();
		if (!((CGdiPlusBitmapResource*) g_active_overlay)->Load(_T("Active"), RES_T_OVERLAY, core_api::get_my_instance())) {
			delete g_active_overlay; g_active_overlay = NULL;
		}
	}
	if (!!g_active_overlay) {
		g_icons_width += (**g_active_overlay).GetWidth();
		if ((**g_active_overlay).GetHeight() > g_icons_height) g_icons_height = (**g_active_overlay).GetHeight();
	}

	// Load playlist icons reusing already loaded icons where possible
	t_icons_map* tmp_icons = new t_icons_map();
	static_api_ptr_t<playlist_manager> pm;
	const t_size total = pm->get_playlist_count();
	for (t_size i = 0; i < total; i++) {
		pfc::string8 icon_path;
		CGdiPlusBitmap* bmp = NULL;

		try {
			static_api_ptr_t<playlist_manager_v2> pm2;
			if (!pm2->playlist_get_property(i, guid_icon_path, &string_helper::stream_writer_string(icon_path), abort_callback_impl())) {
				icon_path = pm->playlist_lock_is_present(i) ? "Autoplaylist" : "Playlist";
			}
		} catch(...) { }

		if (icon_path.get_length() == 0) {
			icon_path = pm->playlist_lock_is_present(i) ? "Autoplaylist" : "Playlist";
		}

		if (!tmp_icons->query(icon_path, bmp)) { 
			if (!!g_icons && g_icons->query(icon_path, bmp)) {
				tmp_icons->set(icon_path, bmp);
				g_icons->remove(icon_path);
			} else {
				pfc::stringcvt::string_wide_from_utf8 icon_path_w(icon_path);
				bmp = new CGdiPlusBitmapResource();
				if (((CGdiPlusBitmapResource*) bmp)->Load(icon_path_w, RES_T_ICON, core_api::get_my_instance())
					|| (uFileExists(icon_path.get_ptr()) && bmp->Load(icon_path_w))) {
						tmp_icons->set(icon_path, bmp);
				} else {
					delete bmp; bmp = NULL;
					try {
						static_api_ptr_t<playlist_manager_v2>()->playlist_remove_property(i, guid_icon_path);
					} catch(...) { }						
				}
			}
		}

		if (!!bmp) {
			if ((**bmp).GetWidth()  > g_icons_width)  g_icons_width  = (**bmp).GetWidth();
			if ((**bmp).GetHeight() > g_icons_height) g_icons_height = (**bmp).GetHeight();
		}
	}

	g_unload_icons();
	g_icons = tmp_icons;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

LRESULT WINAPI playlists_dropdown::ComboBoxWndProcHook(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
    playlists_dropdown* p_this = reinterpret_cast<playlists_dropdown*>(GetWindowLongPtr(wnd, GWL_USERDATA));
    return p_this ? p_this->ComboBoxWndProc(wnd, msg, wp, lp) : DefWindowProc(wnd, msg, wp, lp);
}

LRESULT WINAPI playlists_dropdown::ListBoxWndProcHook(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
    playlists_dropdown* p_this = reinterpret_cast<playlists_dropdown*>(GetProp(wnd, L"PROP_PDDCLASS"));
    return p_this ? p_this->ListBoxWndProc(wnd, msg, wp, lp) : DefWindowProc(wnd, msg, wp, lp);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

playlists_dropdown::playlists_dropdown() : 
	m_hWnd(NULL), 
	m_hWndListBox(NULL), 
	m_block_capture(false), 
	m_reordering(false), 
	m_reordering_idx(pfc_infinite), 
	m_dropping_idx(pfc_infinite) {
#ifdef _DEBUG
#ifdef _CRTDBG_MAP_ALLOC
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif
}

playlists_dropdown::~playlists_dropdown() {
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

unsigned playlists_dropdown::get_type() const {
	return uie::type_toolbar;
}

playlists_dropdown::class_data & playlists_dropdown::get_class_data() const {
	__implement_get_class_data(_T("{00D8E874-2909-486a-A8CB-69B5AB11CA46}"), false);
}

const GUID & playlists_dropdown::get_extension_guid() const {
	return guid_extension;
}

void playlists_dropdown::get_name(pfc::string_base & out) const {
	out.set_string(COMPONENT_NAME);
}

bool playlists_dropdown::get_short_name(pfc::string_base & out) const {
	out.set_string(COMPONENT_SHORT_NAME);
	return true;
}

bool playlists_dropdown::get_description(pfc::string_base & out) const {
	out.set_string(COMPONENT_DESCRIPTION);
	return true;
}

void playlists_dropdown::get_category(pfc::string_base & out) const {
	out.set_string(COMPONENT_CATEGORY);
}

void playlists_dropdown::get_menu_items(uie::menu_hook_t & p_hook) {
	p_hook.add_node(new playlists_dropdown_menu_preferences());
}
