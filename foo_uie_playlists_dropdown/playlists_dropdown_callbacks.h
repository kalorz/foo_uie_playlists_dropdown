// ========================================================================================================================
#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class playlists_dropdown::play_callback_impl : public play_callback {
private:
	t_size m_last_playing_playlist;
public:
	play_callback_impl() : m_last_playing_playlist(pfc_infinite) {}

	static const unsigned get_flags();
	virtual void on_playback_new_track (metadb_handle_ptr p_track);
	virtual void on_playback_stop (play_control::t_stop_reason p_reason);
	virtual void on_playback_starting (play_control::t_track_command p_command, bool p_paused);

	// Not used
	virtual void on_playback_seek (double p_time) {}
	virtual void on_playback_pause (bool p_state) {}
	virtual void on_playback_edited (metadb_handle_ptr p_track) {}
	virtual void on_playback_dynamic_info (const file_info &p_info) {}
	virtual void on_playback_dynamic_info_track (const file_info &p_info) {}
	virtual void on_playback_time (double p_time) {}
	virtual void on_volume_change (float p_new_val) {}
};

class playlists_dropdown::playlist_callback_impl : public playlist_callback {
public:
	static const unsigned get_flags();
	virtual void on_items_added(t_size p_playlist,t_size p_start, const pfc::list_base_const_t<metadb_handle_ptr> & p_data,const bit_array & p_selection);
	virtual void on_items_removed(t_size p_playlist,const bit_array & p_mask,t_size p_old_count,t_size p_new_count);
	virtual void on_playlist_activate(t_size p_old,t_size p_new);
	virtual void on_playlist_created(t_size p_index,const char * p_name,t_size p_name_len);
	virtual void on_playlists_reorder(const t_size * p_order,t_size p_count);
	virtual void on_playlists_removed(const bit_array & p_mask,t_size p_old_count,t_size p_new_count);
	virtual void on_playlist_renamed(t_size p_index,const char * p_new_name,t_size p_new_name_len);
	virtual void on_playlist_locked(t_size p_playlist,bool p_locked);

	// Not used
	virtual void on_items_reordered(t_size p_playlist,const t_size * p_order,t_size p_count) {}
	virtual void on_items_removing(t_size p_playlist,const bit_array & p_mask,t_size p_old_count,t_size p_new_count) {}
	virtual void on_items_selection_change(t_size p_playlist,const bit_array & p_affected,const bit_array & p_state) {}
	virtual void on_item_focus_change(t_size p_playlist,t_size p_from,t_size p_to) {}
	virtual void on_items_modified(t_size p_playlist,const bit_array & p_mask) {}
	virtual void on_items_modified_fromplayback(t_size p_playlist,const bit_array & p_mask,play_control::t_display_level p_level) {}
	virtual void on_items_replaced(t_size p_playlist,const bit_array & p_mask,const pfc::list_base_const_t<t_on_items_replaced_entry> & p_data) {}
	virtual void on_item_ensure_visible(t_size p_playlist,t_size p_idx) {}
	virtual void on_playlists_removing(const bit_array & p_mask,t_size p_old_count,t_size p_new_count) {}
	virtual void on_default_format_changed() {}
	virtual void on_playback_order_changed(t_size p_new_index) {}
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fonts callback
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class playlists_dropdown::fonts_callback_impl : public cui::fonts::client {
	virtual const GUID & get_client_guid() const { return guid_fonts; }
	virtual void get_name(pfc::string_base & p_out) const { p_out.set_string(COMPONENT_NAME); }
	virtual cui::fonts::font_type_t get_default_font_type() const { return cui::fonts::font_type_items; }
	virtual void on_font_changed() const;
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Colors callbacks
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class playlists_dropdown::colors_callback_impl : public cui::colours::client {
    virtual const GUID & get_client_guid() const { return guid_colours; }
    virtual void get_name (pfc::string_base & p_out) const { p_out.set_string(COMPONENT_NAME); }
    virtual t_size get_supported_bools() const { return 0; }
    virtual bool get_themes_supported() const { return false; }
	virtual void on_colour_changed(t_size mask) const;

	// Not used
    virtual void on_bool_changed(t_size mask) const {}
};

// ========================================================================================================================
#endif
// ========================================================================================================================
