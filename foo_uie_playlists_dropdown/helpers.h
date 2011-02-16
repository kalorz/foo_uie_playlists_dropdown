// ========================================================================================================================
#ifndef _SDK_HELPERS_H_
#define _SDK_HELPERS_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace common_helper {
	int axtoi(const char * p_string, t_size p_string_len);
	class format_time_hours {
	public:
		format_time_hours(t_uint64 p_seconds);
		const char * get_ptr() const {return m_buffer;}
		operator const char * () const {return m_buffer;}
	protected:
		pfc::string_fixed_t<127> m_buffer;
	};
};

namespace string_helper {

	// Helpers for converting string to list / list to string
	namespace string_list {
		enum {separator = '\n'};
		void build_list(const char * src, pfc::ptr_list_t<char> & out);
		void parse_list(const pfc::ptr_list_t<char> & src, pfc::string8 & out);
		void add_item(pfc::string8 & src, const char * item, const unsigned max = 10);
		void get_first(const char * src, pfc::string8 & out);
	}

	class stream_writer_string : public stream_writer {
	public:
		void write(const void * p_buffer, t_size p_bytes, abort_callback & p_abort) {
			m_out.add_string((const char*) p_buffer,p_bytes);
		}
		stream_writer_string(pfc::string_base & p_out) : m_out(p_out) {m_out.reset();}
	private:
		pfc::string_base & m_out;
	};
};

namespace playlists_helper {
	void sort_by_format(const char * spec, int direction = 1);
	void sort_by_format_get_order(t_size * order, const char * spec, int p_direction = 1);
	void sort_reverse();
};

namespace mainmenu_helper {
	bool get_group_path(const GUID & group, pfc::string_base & out);
};

// ========================================================================================================================
#endif
// ========================================================================================================================
