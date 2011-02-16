// ========================================================================================================================
#ifndef _DROP_PROCESS_LOCATIONS_H_
#define _DROP_PROCESS_LOCATIONS_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class process_locations_notify_impl : public process_locations_notify {
public:
	enum {
		ADD = 1,
		REPLACE,
		CREATE
	};
	virtual void on_completion(const pfc::list_base_const_t<metadb_handle_ptr> & p_items);
	virtual void on_aborted() {}
protected:
	process_locations_notify_impl() : m_playlist(pfc_infinite), m_action(CREATE) {}
	process_locations_notify_impl(t_size p_playlist, t_size p_action = ADD) : m_playlist(p_playlist), m_action(p_action) {}
	~process_locations_notify_impl() {}
private:
	t_size m_playlist;
	t_size m_action;
};

// ========================================================================================================================
#endif
// ========================================================================================================================
