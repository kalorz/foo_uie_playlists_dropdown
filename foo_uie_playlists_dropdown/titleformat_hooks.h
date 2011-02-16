// ========================================================================================================================
#ifndef _TITLEFORMAT_HOOKS_H_
#define _TITLEFORMAT_HOOKS_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

struct field_desc_t {
    char * label;
    char * help;
};

static const field_desc_t g_fields_list[] = {
	{ NULL, NULL },
	{ "%title%", "Name of the playlist." },
	{ "%lock%", "Name of the lock (if playlist is locked). Can be used with $if()." },
	{ "%length%", "Playlist's length (duration), formatted as [WK][D][HH:]MM:SS." },
	{ "%length_hours%", "Playlist's length (duration), formatted as [HH:]MM:SS." },
	{ "%length_seconds%", "Playlist's length (duration), in seconds, formatted as an integer." },
	{ "%size%", "Number of items (songs) the playlist contains." },
	{ "%filesize%", "Total filesize of the playlist in bytes." },
	{ "%filesize_natural%", "Formatted total filesize of the playlist." },
	{ "%isplaying%", "Present if the playlist is currently being played (to be used as a switch for $if())." },
	{ "%isactive%", "Present if the playlist is active (to be used as a switch for $if())." },
	{ "%list_index%", "Position of the playlist on dropdown list." },
	{ "%list_total%", "Total number of playlists." },
	//{ NULL, NULL },
	//{ "%dropped%", "Present if the drop list is visible (to be used as a switch for $if())." },
	{ NULL, NULL },
	{ "$color(text)", "Returns default text color." },
	{ "$color(selected_text)", "Returns default selected text color." },
	{ "$color(back)", "Returns default background color." },
	{ "$color(selected_back)", "Returns default selected background color." }
};

static const field_desc_t g_functions_list[] = {
	{ NULL, NULL },
	{ "$set_style(back,<background_color>,<selected_background_color>)", "Change the default color of background." },
	{ "$set_style(text,<text_color>,<selected_text_color>)", "Change the default color of text." }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Class: title_format_hook
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class title_format_hook : public titleformat_hook {
public:
	title_format_hook(const t_size p_index, const bool p_dropped) : m_index(p_index), m_dropped(p_dropped) {}
	bool process_field(titleformat_text_out* p_out, const char* p_name, t_size p_name_length, bool& p_found_flag);
	bool process_function(titleformat_text_out* p_out, const char* p_name, t_size p_name_length, titleformat_hook_function_params* p_params, bool& p_found_flag);
private:
	const t_size   m_index;
	const bool     m_dropped;
	static_api_ptr_t<playlist_manager> m_api;
	static_api_ptr_t<play_control> m_pc_api;
	static const char color_marker[];
	static const char color_separator[];
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Class: title_format_hook
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class style_format_hook : public titleformat_hook {
public:
	style_format_hook(COLORREF & p_text, COLORREF & p_background, const bool p_selected) : m_text(p_text), m_background(p_background), m_selected(p_selected) {};
	bool process_function(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, titleformat_hook_function_params * p_params, bool & p_found_flag);
	// Not used
	bool process_field(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, bool & p_found_flag) { return false; }
private:
	COLORREF & m_text;
	COLORREF & m_background;
	bool m_selected;
};

// ========================================================================================================================
#endif
// ========================================================================================================================
