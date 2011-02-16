#include "component.h"
#include <shlwapi.h>

namespace {

	wchar_t * makeSortString(const char * in) {
		wchar_t * out = new wchar_t[pfc::stringcvt::estimate_utf8_to_wide(in) + 1];
		out[0] = ' ';//StrCmpLogicalW bug workaround.
		pfc::stringcvt::convert_utf8_to_wide_unchecked(out + 1, in);
		return out;
	}

	struct custom_sort_data {
		wchar_t * text;
		t_size index;
	};
}

namespace {

	class tfhook_sort : public titleformat_hook {
	public:
		tfhook_sort() {
			m_API->seed((unsigned)__rdtsc());
		}
		bool process_field(titleformat_text_out * p_out,const char * p_name,t_size p_name_length,bool & p_found_flag) {
			return false;
		}
		bool process_function(titleformat_text_out * p_out,const char * p_name,t_size p_name_length,titleformat_hook_function_params * p_params,bool & p_found_flag) {
			if (stricmp_utf8_ex(p_name, p_name_length, "rand", ~0) == 0) {
				t_size param_count = p_params->get_param_count();
				t_uint32 val;
				if (param_count == 1) {
					t_uint32 mod = (t_uint32)p_params->get_param_uint(0);
					if (mod > 0) {
						val = m_API->genrand(mod);
					} else {
						val = 0;
					}
				} else {
					val = m_API->genrand(0xFFFFFFFF);
				}
				p_out->write_int(titleformat_inputtypes::unknown, val);
				p_found_flag = true;
				return true;
			} else {
				return false;
			}
		}
	private:
		static_api_ptr_t<genrand_service> m_API;
	};
}

template<int direction>
static int custom_sort_compare(const custom_sort_data & elem1, const custom_sort_data & elem2) {
	int ret = direction * StrCmpLogicalW(elem1.text,elem2.text);
	if (ret == 0) ret = pfc::sgn_t((t_ssize)elem1.index - (t_ssize)elem2.index);
	return ret;
}

void playlists_helper::sort_by_format(const char * spec, int direction) {
	static_api_ptr_t<playlist_manager> pm;
	const t_size count = pm->get_playlist_count();
	pfc::array_t<t_size> order; order.set_size(count);
	sort_by_format_get_order(order.get_ptr(), spec, direction);
	pm->reorder(order.get_ptr(), count);
}

void playlists_helper::sort_by_format_get_order(t_size * order, const char * spec, int p_direction) {
	service_ptr_t<titleformat_object> ptr;
	if (!static_api_ptr_t<titleformat_compiler>()->compile(ptr, spec)) {
		return;
	}

	static_api_ptr_t<playlist_manager> pm;
	static_api_ptr_t<titleformat_compiler> tc;
	const t_size count = pm->get_playlist_count();
	t_size n;
	pfc::array_t<custom_sort_data> data;
	data.set_size(count);
	static_api_ptr_t<library_manager> api;
	
	tfhook_sort sort_hook;
	pfc::string8_fastalloc temp;
	temp.prealloc(512);
	for (n = 0; n < count; n++) {
		ptr->run(&titleformat_hook_impl_splitter(&sort_hook, &title_format_hook(n, false)), temp, NULL);
		data[n].index = n;
		data[n].text = makeSortString(temp);
	}

	pfc::sort_t(data, custom_sort_compare<1>, count);

	for (n = 0; n < count; n++) {
		order[n] = data[n].index;
		delete[] data[n].text;
	}
}

void playlists_helper::sort_reverse() {
	static_api_ptr_t<playlist_manager> pm;
	order_helper order(pm->get_playlist_count());
	order.reverse(0, order.get_count());
	pm->reorder(order.get_ptr(), order.get_count());
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

bool mainmenu_helper::get_group_path(const GUID & group, pfc::string_base & out) {
	if (group != pfc::guid_null) {
		service_enum_t<mainmenu_group> e; service_ptr_t<mainmenu_group> ptr;
		while (e.next(ptr)) {
			if (ptr->get_guid() == group) {
				get_group_path(ptr->get_parent(), out);
				service_ptr_t<mainmenu_group_popup> popup;
				if (ptr->service_query_t(popup)) {
					pfc::string8 tmp;
					popup->get_display_string(tmp);
					out << tmp << "/";
					return true;
				}
			}
		}
	}
	return false;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

int common_helper::axtoi(const char * p_string, t_size p_string_len) {
	int int_value = 0;
	pfc::list_t<int, pfc::alloc_fast> digits;
	for (t_size i=0; i<p_string_len; i++) {
		if (p_string[i] > 0x29 && p_string[i] < 0x40 ) {
			digits.add_item(p_string[i] & 0x0f);
		} else if (p_string[i] >='a' && p_string[i] <= 'f') {
			digits.add_item((p_string[i] & 0x0f) + 9);
		} else if (p_string[i] >='A' && p_string[i] <= 'F') {
			digits.add_item((p_string[i] & 0x0f) + 9);
		} else if (p_string[i] != 3) {
			break;
		}
	}

	int count = digits.get_count();
	if (count > 0) {
		int m = count - 1;
		int n = 0;
		while (n<count) {
			int_value = int_value | (digits[n] << (m << 2));
			m--;
			n++;
		}
	}
	return int_value;
}

common_helper::format_time_hours::format_time_hours(t_uint64 p_seconds) {
	t_uint64 length = p_seconds;
	unsigned hours,minutes,seconds;
	
	hours = (unsigned) ( ( length / (60 * 60) ) );
	minutes = (unsigned) ( ( length / (60 ) ) % 60 );
	seconds = (unsigned) ( ( length ) % 60 );

	if (hours) {
		m_buffer << hours << ":" << pfc::format_uint(minutes,2) << ":" << pfc::format_uint(seconds,2);
	} else {
		m_buffer << minutes << ":" << pfc::format_uint(seconds,2);
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void string_helper::string_list::build_list(const char * src, pfc::ptr_list_t<char> & out) {
	while (*src) {
		int ptr = 0;
		while (src[ptr] && src[ptr] != separator) ptr++;
		if (ptr > 0) {
			out.add_item(pfc::strdup_n(src, ptr));
			src += ptr;
		}
		while (*src == separator) src++;
	}
}

void string_helper::string_list::get_first(const char * src, pfc::string8 & out) {
	out.reset();
	while (*src) {
		int ptr = 0;
		while (src[ptr] && src[ptr] != separator) ptr++;
		if (ptr > 0) {
			out.add_string(src, ptr);
			return;
		}
		while (*src == separator) src++;
	}
}

void string_helper::string_list::parse_list(const pfc::ptr_list_t<char> & src, pfc::string8 & out) {
	out.reset();
	t_size n;
	for (n = 0; n < src.get_count(); n++) {
		out.add_string(src[n]);
		out.add_char(separator);
	}
}

void string_helper::string_list::add_item(pfc::string8 & src, const char * item, const unsigned max) {
	if (!item || !*item) return;

	pfc::string8 meh;
	if (strchr(item, separator)) {
		uReplaceChar(meh, item, -1, separator, '|', false);
		item = meh;
	}

	pfc::ptr_list_t<char> list;
	build_list(src, list);
	unsigned n;
	bool found = false;
	for (n = 0; n < list.get_count(); n++) {
		if (!strcmp(list[n], item)) {
			char* temp = list.remove_by_idx(n);
			list.insert_item(temp,0);
			found = true;
		}
	}

	if (!found) {
		while (list.get_count() >= max) list.delete_by_idx(list.get_count() - 1);
		list.insert_item(_strdup(item), 0);
	}

	parse_list(list, src);
	list.free_all();
}
