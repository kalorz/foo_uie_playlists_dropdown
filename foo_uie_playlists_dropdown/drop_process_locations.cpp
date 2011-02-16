#include "component.h"

void process_locations_notify_impl::on_completion(const pfc::list_base_const_t<metadb_handle_ptr> & p_items) {
	static_api_ptr_t<playlist_manager> pm;
	if (m_action != CREATE && m_playlist >= 0 && m_playlist < pm->get_playlist_count()) {
		if (m_action == REPLACE) {
			pm->playlist_clear(m_playlist);
		} else {
			pm->playlist_clear_selection(m_playlist);
		}
		pm->playlist_add_items(m_playlist, p_items, bit_array_true());
	} else {
		pfc::string8 name;
		bool passed = false;
		try {
			passed = static_api_ptr_t<playlist_incoming_item_filter_v3>()->auto_playlist_name(p_items, name);
		} catch(...) { }
		m_playlist = passed ? pm->create_playlist(name.get_ptr(), pfc_infinite, pfc_infinite) : pm->create_playlist_autoname();
		if (m_playlist != pfc_infinite) {
			pm->playlist_add_items(m_playlist, p_items, bit_array_true());
		}
	}
	if (cfg::dd_activate) {
		pm->set_active_playlist(m_playlist);
	}
}
