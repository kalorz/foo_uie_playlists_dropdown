#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlists Dropdown class implementation: callbacks
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

const unsigned playlists_dropdown::play_callback_impl::get_flags() {
	return flag_on_playback_new_track | flag_on_playback_stop | flag_on_playback_starting;
}

void playlists_dropdown::play_callback_impl::on_playback_new_track(metadb_handle_ptr p_track) {
	t_size playlist = static_api_ptr_t<playlist_manager>()->get_playing_playlist();
	if (m_last_playing_playlist != playlist) {
		m_last_playing_playlist = playlist;
		playlists_dropdown::g_redraw_all();
	}
}

void playlists_dropdown::play_callback_impl::on_playback_starting(play_control::t_track_command p_command, bool p_paused) {
	//g_redraw_all();
	on_playback_new_track(NULL);
}

void playlists_dropdown::play_callback_impl::on_playback_stop(play_control::t_stop_reason p_reason) {
	if (p_reason != play_control::stop_reason_shutting_down && p_reason != play_control::stop_reason_starting_another) {
		m_last_playing_playlist = pfc_infinite;
		g_redraw_all();
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlist callbacks
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

const unsigned playlists_dropdown::playlist_callback_impl::get_flags() {
	return flag_on_playlist_activate | flag_on_playlist_created | flag_on_playlists_reorder | 
		flag_on_playlists_removed | flag_on_playlist_renamed | flag_on_playlist_locked |
		flag_on_items_added | flag_on_items_removed;
}

void playlists_dropdown::playlist_callback_impl::on_items_added(t_size p_playlist, t_size p_start, const pfc::list_base_const_t<metadb_handle_ptr> & p_data,const bit_array & p_selection) {
	g_playlist_invalidate_cache(p_playlist);
	g_redraw_all();
}

void playlists_dropdown::playlist_callback_impl::on_items_removed(t_size p_playlist, const bit_array & p_mask, t_size p_old_count, t_size p_new_count) {
	g_playlist_invalidate_cache(p_playlist);
	g_redraw_all();
}

void playlists_dropdown::playlist_callback_impl::on_playlist_activate(t_size p_old, t_size p_new) {
	g_redraw_all();
}

void playlists_dropdown::playlist_callback_impl::on_playlist_created(t_size p_index, const char * p_name, t_size p_name_len) {
	g_redraw_all();
}

void playlists_dropdown::playlist_callback_impl::on_playlists_reorder(const t_size * p_order, t_size p_count) {
	g_redraw_all();
}

void playlists_dropdown::playlist_callback_impl::on_playlists_removed(const bit_array & p_mask, t_size p_old_count, t_size p_new_count) {
	g_reload_icons();
	g_redraw_all();
}

void playlists_dropdown::playlist_callback_impl::on_playlist_renamed(t_size p_index, const char * p_new_name, t_size p_new_name_len) {
	g_redraw_all();
}

void playlists_dropdown::playlist_callback_impl::on_playlist_locked(t_size p_playlist, bool p_locked) {
	g_reload_icons();
	g_redraw_all();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Font callbacks
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::fonts_callback_impl::on_font_changed() const {
	g_update_all_fonts();
	g_update_all_sizes();
	g_redraw_all();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Color callbacks
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::colors_callback_impl::on_colour_changed(t_size mask) const {
	g_redraw_all();
}
