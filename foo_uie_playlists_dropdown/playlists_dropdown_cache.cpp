#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlists Dropdown class implementation: cached playlist properties
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template<typename T>
class NOVTABLE playlist_property_t : public service_base {
public:
	virtual T get_value() = 0;
	FB2K_MAKE_SERVICE_INTERFACE(playlist_property_t, service_base);
};

typedef playlist_property_t<double> double_playlist_property;
typedef playlist_property_t<t_filesize> filesize_playlist_property;

// {1b3c6d69-5944-4a92-b3dd-5041b2ef0c73}
const GUID double_playlist_property::class_guid = { 0x1b3c6d69, 0x5944, 0x4a92, { 0xb3, 0xdd, 0x50, 0x41, 0xb2, 0xef, 0x0c, 0x73 } };

// {2a7a0cbe-b98b-4546-b6de-999d1c3b0118}
const GUID filesize_playlist_property::class_guid = { 0x2a7a0cbe, 0xb98b, 0x4546, { 0xb6, 0xde, 0x99, 0x9d, 0x1c, 0x3b, 0x01, 0x18} };

template<typename T>
class playlist_property_impl_t : public playlist_property_t<T> {
public:
	playlist_property_impl_t(const T & p_value) : value(p_value) {}
	T get_value() {return value;}
	T value;
};

typedef playlist_property_impl_t<double> double_playlist_property_impl;
typedef playlist_property_impl_t<t_filesize> filesize_playlist_property_impl;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

double playlists_dropdown::g_playlist_get_cached_length(const t_size & playlist) {
	try {
		static_api_ptr_t<playlist_manager_v2> pm;
		double_playlist_property_impl::ptr ptr;
		if (!pm->playlist_get_runtime_property(playlist, guid_playlist_length_prop, ptr)) {
			metadb_handle_list items;
			pm->playlist_get_all_items(playlist, items);
			ptr = new service_impl_t<double_playlist_property_impl>(items.calc_total_duration());
			pm->playlist_set_runtime_property(playlist, guid_playlist_length_prop, ptr);
			items.remove_all();
		}
		return ptr->get_value();
	} catch(...) { }
	return 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

t_filesize playlists_dropdown::g_playlist_get_cached_filesize(const t_size & playlist) {
	try {
		static_api_ptr_t<playlist_manager_v2> pm;
		filesize_playlist_property_impl::ptr ptr;
		if (!pm->playlist_get_runtime_property(playlist, guid_playlist_filesize_prop, ptr)) {
			metadb_handle_list items;
			pm->playlist_get_all_items(playlist, items);
			ptr = new service_impl_t<filesize_playlist_property_impl>(metadb_handle_list_helper::calc_total_size(items, true));
			pm->playlist_set_runtime_property(playlist, guid_playlist_filesize_prop, ptr);
			items.remove_all();
		}
		return ptr->get_value();
	} catch(...) { }
	return 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void playlists_dropdown::g_playlist_invalidate_cache(const t_size & playlist) {
	try {
		static_api_ptr_t<playlist_manager_v2> pm;
		pm->playlist_remove_runtime_property(playlist, guid_playlist_length_prop);
		pm->playlist_remove_runtime_property(playlist, guid_playlist_filesize_prop);
	} catch(...) { }
}
