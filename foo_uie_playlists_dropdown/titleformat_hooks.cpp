#include "component.h"
#include <shlwapi.h>

#define COLOR(color) ((COLORREF)(((BYTE)(color & 0xff)|((WORD)(color & 0xff00)))|((DWORD)(color & 0xff0000))))
#define RED(color)   (BYTE)(color & 0xff)
#define GREEN(color) (BYTE)((color & 0xff00) >> 8)
#define BLUE(color)  (BYTE)((color & 0xff0000) >> 16)

const char title_format_hook::color_marker[] = { 3 };
const char title_format_hook::color_separator[] = { '|' };

bool style_format_hook::process_function(titleformat_text_out* p_out, const char* p_name, t_size p_name_length, titleformat_hook_function_params* p_params, bool& p_found_flag) {
	p_found_flag = false;

	// $set_style() function
	if (!stricmp_utf8_ex(p_name, p_name_length, "set_style", pfc_infinite) && (p_params->get_param_count() > 1)) {
		const char * name;
		t_size name_length;
		p_params->get_param(0, name, name_length);

		// $set_style(back,<bg_color>,<bg_sel_color>)
		if (!stricmp_utf8_ex(name, name_length, "back", pfc_infinite) && (p_params->get_param_count() == 3)) {
			const char * color;
			t_size color_len;

			p_params->get_param(m_selected ? 2 : 1, color, color_len);
			m_background = common_helper::axtoi(color, color_len);

			p_found_flag = true;
			return true;

			// $set_style(text,<text_color>,<text_sel_color>)
		} else if (!stricmp_utf8_ex(name, name_length, "text", pfc_infinite) && (p_params->get_param_count() == 3)) {
			const char * color;
			t_size color_len;

			p_params->get_param(m_selected ? 2: 1, color, color_len);
			m_text = common_helper::axtoi(color, color_len);

			p_found_flag = true;
			return true;
		}

		/*
		} else if ((stricmp_utf8_ex(name, name_length, "frame-bottom", pfc_infinite) == 0) && (p_params->get_param_count() == 4)) {
		// $set_style(frame-bottom,<frame_width>,<frame_style>,<frame_color>)
		int iStyle = PS_SOLID;
		const char * style, * color;
		t_size       style_len, color_len;
		int cWidth = p_params->get_param_uint(1);
		p_params->get_param(2, style, style_len);
		p_params->get_param(3, color, color_len);

		if (stricmp_utf8_ex(style, style_len, "dotted", pfc_infinite) == 0) {
		iStyle = PS_DOT;
		} else if (stricmp_utf8_ex(style, style_len, "dashed", pfc_infinite) == 0) {
		iStyle = PS_DASH;
		}

		if (cWidth > 0) {
		const char forbidden[1] = { (char)3 };
		pfc::string8 out;
		static_api_ptr_t<titleformat_compiler>()->remove_forbidden_chars_string(out, color, color_len, forbidden);
		p_style_info.frame_bottom = CreatePen(iStyle, cWidth, axtoi(out.get_ptr()));
		}

		p_found_flag = true;
		return true;
		*/
	}
	return false;
}

bool title_format_hook::process_field(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, bool & p_found_flag) {
	p_found_flag = false;

	if (!stricmp_utf8_ex(p_name, p_name_length, "title", pfc_infinite)) {
		pfc::string8 name;
		if (!m_api->playlist_get_name(m_index, name)) {
			name = "N/A";
		}
		p_out->write(titleformat_inputtypes::unknown, name);
		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "lock", pfc_infinite)) {
		if (m_api->playlist_lock_is_present(m_index)) {
			pfc::string8 lock;
			if (!m_api->playlist_lock_query_name(m_index, lock)) {
				lock = "N/A";
			}
			p_out->write(titleformat_inputtypes::unknown, lock);
			p_found_flag = true;
		} else {
			p_found_flag = false;
		}
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "size", pfc_infinite)) {
		p_out->write_int(titleformat_inputtypes::unknown, m_api->playlist_get_item_count(m_index));
		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "length", pfc_infinite)) {
		p_out->write(titleformat_inputtypes::unknown, pfc::format_time(pfc::rint64(playlists_dropdown::g_playlist_get_cached_length(m_index))));
		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "length_hours", pfc_infinite)) {
		p_out->write(titleformat_inputtypes::unknown, common_helper::format_time_hours(pfc::rint64(playlists_dropdown::g_playlist_get_cached_length(m_index))).get_ptr());
		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "length_seconds", pfc_infinite)) {
		p_out->write_int(titleformat_inputtypes::unknown, pfc::rint64(playlists_dropdown::g_playlist_get_cached_length(m_index)));
		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "filesize_natural", pfc_infinite)) {
		p_out->write(titleformat_inputtypes::unknown, pfc::format_file_size_short(playlists_dropdown::g_playlist_get_cached_filesize(m_index)));
		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "filesize", pfc_infinite)) {
		p_out->write_int(titleformat_inputtypes::unknown, playlists_dropdown::g_playlist_get_cached_filesize(m_index));
		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "isplaying", pfc_infinite)) {
		if (m_pc_api->is_playing() && m_index == m_api->get_playing_playlist()) {
			p_found_flag = true;
		}
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "isactive", pfc_infinite)) {
		if (m_index == m_api->get_active_playlist()) {
			p_found_flag = true;
		}
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "_list", pfc_infinite)) {
		p_found_flag = m_dropped;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "_themed", pfc_infinite)) {
		// TODO: For future Vista/7 Theming support
		p_found_flag = false;
		return true;
	}

	return false;
}

bool title_format_hook::process_function(titleformat_text_out* p_out, const char* p_name, t_size p_name_length, titleformat_hook_function_params* p_params, bool& p_found_flag) {
	p_found_flag = false;

	// $color() function
	if (!stricmp_utf8_ex(p_name, p_name_length, "color", pfc_infinite) && p_params->get_param_count() > 0 && p_params->get_param_count() < 5) {
		cui::colours::helper colors(guid_colours);
		const char * name;
		t_size name_length;

		p_out->write(titleformat_inputtypes::unknown, color_marker, 1);
		for (t_size i = 0, j = 0, count = p_params->get_param_count(); i < count; i++) {
			p_params->get_param(i, name, name_length);
			if (!stricmp_utf8_ex(name, name_length, "text", ~0)) {
				if (j++ > 0) p_out->write(titleformat_inputtypes::unknown, color_separator, 1);
				p_out->write(titleformat_inputtypes::unknown, pfc::format_hex(colors.get_colour(cui::colours::colour_text), 6));
			} else if (!stricmp_utf8_ex(name, name_length, "selected_text", ~0)) {
				if (j++ > 0) p_out->write(titleformat_inputtypes::unknown, color_separator, 1);
				p_out->write(titleformat_inputtypes::unknown, pfc::format_hex(colors.get_colour(cui::colours::colour_selection_text), 6));
			} else if (!stricmp_utf8_ex(name, name_length, "back", ~0)) {
				if (j++ > 0) p_out->write(titleformat_inputtypes::unknown, color_separator, 1);
				p_out->write(titleformat_inputtypes::unknown, pfc::format_hex(colors.get_colour(cui::colours::colour_background), 6));
			} else if (!stricmp_utf8_ex(name, name_length, "selected_back", ~0)) {
				if (j++ > 0) p_out->write(titleformat_inputtypes::unknown, color_separator, 1);
				p_out->write(titleformat_inputtypes::unknown, pfc::format_hex(colors.get_colour(cui::colours::colour_selection_background), 6));
			}
		}
		p_out->write(titleformat_inputtypes::unknown, color_marker, 1);

		p_found_flag = true;
		return true;

	} else if (!stricmp_utf8_ex(p_name, p_name_length, "blend_target", pfc_infinite) && p_params->get_param_count() == 1) {
		const char * p_val;
		unsigned p_val_length;
		p_params->get_param(0, p_val, p_val_length);
		int colour = (unsigned) common_helper::axtoi(p_val, p_val_length);

		p_out->write(titleformat_inputtypes::unknown, color_marker, 1);
		if (RED(colour) + GREEN(colour) + BLUE(colour) > 128*3) {
			p_out->write(titleformat_inputtypes::unknown, pfc::format_hex(0, 6));
		} else {
			p_out->write(titleformat_inputtypes::unknown, pfc::format_hex(0xffffff, 6));
		}
		p_out->write(titleformat_inputtypes::unknown, color_marker, 1);

		p_found_flag = true;
		return true;
	}

	return false;
}