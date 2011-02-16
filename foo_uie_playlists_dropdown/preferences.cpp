#include "component.h"

playlists_dropdown::preferences::preferences() {
}

playlists_dropdown::preferences::~preferences() {
}

HWND playlists_dropdown::preferences::create(HWND p_parent) {
	return uCreateDialog(IDD_PREFERENCES, p_parent, (DLGPROC) MainDlgProc);
}

const char * playlists_dropdown::preferences::get_name() {
	return COMPONENT_NAME;
}

GUID playlists_dropdown::preferences::get_guid() {
	return guid_preferences;
}

GUID playlists_dropdown::preferences::get_parent_guid() {
	return g_guid_columns_ui_extensions_branch;
}

bool playlists_dropdown::preferences::reset_query() {
	return true;
}

void playlists_dropdown::preferences::reset() {
	cfg::padding              = DEF_PADDING;
	cfg::mclick_action        = g_guid_columns_ui_activate_now_playing;
	cfg::mbutton_reorder      = true;
    cfg::wheel_use            = true;
    cfg::show_icons           = true;
	cfg::dd_autoexpand        = true;
	cfg::dd_autoswitch        = true;
    cfg::title_string         = DEF_TITLE_STRING;
	cfg::style_string         = DEF_STYLE_STRING;
	cfg::min_visible          = DEF_MIN_VISIBLE;
	cfg::dd_autoexpand_delay  = DEF_AUTOEXPAND_DELAY;
	cfg::dd_autoswitch_delay  = DEF_AUTOSWITCH_DELAY;
    apply_config();
}

bool playlists_dropdown::preferences::get_help_url(pfc::string_base & p_out) {
	p_out.set_string(COMPONENT_HELP_URL);
	return true;
}

void playlists_dropdown::preferences::apply_config() {
	g_compile_formatters();
    g_update_all_fonts();
    g_reload_icons();
    g_update_all_sizes();
	g_update_all_min_visible();
    g_redraw_all();
}
