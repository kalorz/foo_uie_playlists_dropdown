// ========================================================================================================================
#ifndef _PLAYLISTS_DROPDOWN_H_
#define _PLAYLISTS_DROPDOWN_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class playlists_dropdown : public uie::container_ui_extension {
public:
	playlists_dropdown();
	virtual ~playlists_dropdown();

	virtual const GUID & get_extension_guid() const;
	virtual void get_name(pfc::string_base & out) const;
	virtual bool get_short_name(pfc::string_base & out) const;
	virtual bool get_description(pfc::string_base & out) const;
	virtual void get_category(pfc::string_base & out) const;
	virtual unsigned get_type() const;
	virtual void get_menu_items(uie::menu_hook_t & p_hook);

	static double g_playlist_get_cached_length(const t_size & playlist);
	static t_filesize g_playlist_get_cached_filesize(const t_size & playlist);

	class colors_callback_impl;
	class fonts_callback_impl;
	class preferences;
private:
	class playlist_callback_impl;
	class play_callback_impl;

	LRESULT					on_message(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT CALLBACK		ComboBoxWndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT CALLBACK		ListBoxWndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI	ComboBoxWndProcHook(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI	ListBoxWndProcHook(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
    static BOOL CALLBACK    RenameDialogProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	static BOOL CALLBACK    SortDialogProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

    static bool				g_rename_dialog(pfc::string_base & param, HWND parent);
	static bool				g_sort_dialog(pfc::string_base & param, HWND parent);

	void					on_context_menu(HWND wnd, t_size playlist, POINT pt);
	void					on_dd_context_menu(HWND wnd, t_size playlist, POINT pt, IDataObject * pDataObject);

	void					redraw_list();
	static void				g_compile_formatters();
	static void				g_redraw_all();
	static void				g_update_all_sizes();
	static void				g_update_all_fonts();
	static void				g_update_all_min_visible();
	static void				g_reload_icons();
	static void				g_playlist_invalidate_cache(const t_size & playlist);

	static pfc::instance_tracker_server_t<playlists_dropdown> g_instances;
    static pfc::list_t<HWND>            g_notify_list;

	static playlist_callback_impl*		g_playlist_callback;
	static play_callback_impl*			g_play_callback;

	static titleformat_object::ptr		g_title_format;
	static titleformat_object::ptr		g_style_format;

	static unsigned                     g_window_height;
	static unsigned						g_icons_width;
	static unsigned						g_icons_height;

	typedef pfc::map_t<pfc::string8, CGdiPlusBitmap*, pfc::string::comparatorCaseSensitive> t_icons_map;
	static t_icons_map*					g_icons;
	static CGdiPlusBitmap*				g_playing_overlay;
	static CGdiPlusBitmap*				g_active_overlay;
	static void							g_unload_icons();
	static void							g_delete_icon_func(const pfc::string8 & icon_id, const CGdiPlusBitmap* bmp) { delete bmp; bmp = NULL; }
	static BOOL CALLBACK				EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam);

	pfc::instance_tracker_client_t<playlists_dropdown, g_instances> m_tracker;

	virtual class_data & get_class_data() const;

	bool	m_block_capture;
	bool	m_reordering;
	t_size	m_reordering_idx;
	t_size	m_dropping_idx;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
	pfc::com_ptr_t<IDropTarget> m_listbox_dd;
	pfc::com_ptr_t<IDropTarget> m_combobox_dd;

	WNDPROC OldComboBoxProc;
	WNDPROC OldListBoxProc;
	HWND m_hWnd;
	HWND m_hWndListBox;
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class playlists_dropdown_menu_preferences : public uie::menu_node_command_t {
public:
	bool get_display_data(pfc::string_base & p_out, unsigned & p_state) const {
		p_out = "Preferences...";
		p_state = 0;
		return true;
	}
	bool get_description(pfc::string_base & p_out) const {
		p_out = "Opens preferences window for Playlists Dropdown component.";
		return true;
	}
	virtual void execute() {
		static_api_ptr_t<ui_control>()->show_preferences(guid_preferences);
	}
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Nested classes
#include "playlists_dropdown_callbacks.h"
#include "playlists_dropdown_preferences.h"

// ========================================================================================================================
#endif
// ========================================================================================================================
