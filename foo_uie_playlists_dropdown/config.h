// ========================================================================================================================
#ifndef _CONFIG_H_
#define _CONFIG_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// TODO
// TEMPORAL SOLUTION?
// {74FD83FD-6927-43c6-B673-01272CD8A54E}
static const GUID g_guid_columns_ui_activate_now_playing = { 0x450c17b0, 0x70e0, 0x42cc, { 0xaa, 0x7d, 0xdc, 0x5b, 0x7a, 0xdf, 0xf5, 0xa8 } };
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Default values
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define DEF_TITLE_STRING "$puts(dim,$blend($color(text,selected_text),$color(back,selected_text),1,2))\r\n" \
	"\r\n" \
	"$get(dim)%list_index% $rgb()\r\n" \
	"%title%\r\n" \
	"$tab()$get(dim) '['%size%']'"
#define DEF_STYLE_STRING "$if($and(%_list%,$not(%_themed%)),\r\n" \
	"// Dim even rows\r\n" \
	"$ifequal($mod(%list_index%,2),0,$set_style(back,$blend($color(back),$blend_target($color(back)),3,64),$color(selected_back)),)\r\n" \
	"// Highlight active playlist\r\n" \
	"$if(%isactive%,$set_style(text,$blend($color(text),$color(selected_back),1,2),$color(selected_text)))\r\n" \
	")"
#define DEF_SORT_STRING      "%lock% - %title%"
#define DEF_WHEEL_DELAY      100
#define DEF_AUTOEXPAND_DELAY 500
#define DEF_AUTOSWITCH_DELAY 500
#define DEF_MIN_VISIBLE      30
static const RECT DEF_PADDING = { 2, 1, 2, 1 };

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Configuration variables
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace config {
	extern cfg_int				last_tab;
	extern cfg_int				last_fs_tab;
	extern cfg_string			custom_icons_history;
	extern cfg_guid				mclick_action;
	extern cfg_bool				mbutton_reorder;
	extern cfg_bool				wheel_use;
	extern cfg_int				wheel_delay;
	extern cfg_string			title_string;
	extern cfg_string			style_string;
	extern cfg_dropdown_history	sort_string;
	extern cfg_struct_t<RECT>	padding;
	extern cfg_bool				show_icons;
	extern cfg_int				min_visible;
	extern cfg_bool				dd_autoexpand;
	extern cfg_int				dd_autoexpand_delay;
	extern cfg_bool				dd_autoswitch;
	extern cfg_int				dd_autoswitch_delay;
	extern cfg_bool				dd_activate;
}

#define cfg config

// ========================================================================================================================
#endif
// ========================================================================================================================
