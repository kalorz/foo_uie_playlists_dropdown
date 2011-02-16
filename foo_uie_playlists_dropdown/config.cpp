#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Configuration variables
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace config {
	// {c984b5e0-02f9-48c3-b003-1f4ba1c249c8}
	static const GUID guid_cfg_last_tab = { 0xc984b5e0, 0x02f9, 0x48c3, {0xb0, 0x03, 0x1f, 0x4b, 0xa1, 0xc2, 0x49, 0xc8 } };
	cfg_int last_tab(guid_cfg_last_tab, 0);

	// {aca2bc79-c0eb-4512-a37c-440eb62c530e}
	static const GUID guid_cfg_last_fs_tab = { 0xaca2bc79, 0xc0eb, 0x4512, {0xa3, 0x7c, 0x44, 0x0e, 0xb6, 0x2c, 0x53, 0x0e } };
	cfg_int last_fs_tab(guid_cfg_last_fs_tab, 0);

	// {6035324e-39fc-458b-9a33-cdbe2df69e1a}
	static const GUID guid_cfg_custom_icons_history = { 0x6035324e, 0x39fc, 0x458b, { 0x9a, 0x33, 0xcd, 0xbe, 0x2d, 0xf6, 0x9e, 0x1a } };
	cfg_string custom_icons_history(guid_cfg_custom_icons_history, "");

	// {e1b4a142-27a4-419a-ac34-065b24d99850}
	static const GUID guid_cfg_mclick_action = { 0xe1b4a142, 0x27a4, 0x419a, {0xac, 0x34, 0x06, 0x5b, 0x24, 0xd9, 0x98, 0x50 } };
	cfg_guid mclick_action(guid_cfg_mclick_action, g_guid_columns_ui_activate_now_playing);

	// {67100422-fe3e-43b6-be63-2e90c3f4578e}
	static const GUID guid_cfg_mbutton_reorder = { 0x67100422, 0xfe3e, 0x43b6, { 0xbe, 0x63, 0x2e, 0x90, 0xc3, 0xf4, 0x57, 0x8e } };
	cfg_bool mbutton_reorder(guid_cfg_mbutton_reorder, true);

	// {722501A0-E477-44e6-8C3B-6C25D4FEB3D8}
	static const GUID guid_cfg_wheel_use = { 0x722501a0, 0xe477, 0x44e6, { 0x8c, 0x3b, 0x6c, 0x25, 0xd4, 0xfe, 0xb3, 0xd8 } };
	cfg_bool wheel_use(guid_cfg_wheel_use, true);

	// {5ad97d0e-7b32-4cab-9020-d105976a4d43}
	static const GUID guid_cfg_wheel_delay = { 0x5ad97d0e, 0x7b32, 0x4cab, {0x90, 0x20, 0xd1, 0x05, 0x97, 0x6a, 0x4d, 0x43 } };
	cfg_int wheel_delay(guid_cfg_wheel_delay, DEF_WHEEL_DELAY);

	// Title formatting
	// {0f84a68a-2eb9-485c-a7e8-ea162d9c0376}
	static const GUID guid_cfg_title_string = { 0x0f84a68a, 0x2eb9, 0x485c, { 0xa7, 0xe8, 0xea, 0x16, 0x2d, 0x9c, 0x03, 0x76 } };
	cfg_string title_string(guid_cfg_title_string, DEF_TITLE_STRING);

	// Style formatting string
	// {cd2c4f9c-c307-4ec5-9582-9074b99004a8}
	static const GUID guid_cfg_style_string = { 0xcd2c4f9c, 0xc307, 0x4ec5, { 0x95, 0x82, 0x90, 0x74, 0xb9, 0x90, 0x04, 0xa8 } };
	cfg_string style_string(guid_cfg_style_string, DEF_STYLE_STRING);

	// Sort formatting string
	// {ccc50960-b4e7-481f-b010-aee624aff84c}
	static const GUID guid_cfg_sort_string = { 0xccc50960, 0xb4e7, 0x481f, { 0xb0, 0x10, 0xae, 0xe6, 0x24, 0xaf, 0xf8, 0x4c } };
	cfg_dropdown_history sort_string(guid_cfg_sort_string, 10, DEF_SORT_STRING);

	// Padding (in pixels)
	// {927B3CA8-510B-4168-BE16-D4BA6C3DE435}
	static const GUID guid_cfg_padding = { 0x927b3ca8, 0x510b, 0x4168, { 0xbe, 0x16, 0xd4, 0xba, 0x6c, 0x3d, 0xe4, 0x35 } };
	cfg_struct_t<RECT> padding(guid_cfg_padding, DEF_PADDING);

	// Show icons
	// {43FF2DFF-D78B-46c3-96CB-EF27A9AC4AD2}
	static const GUID guid_cfg_show_icons = { 0x43ff2dff, 0xd78b, 0x46c3, { 0x96, 0xcb, 0xef, 0x27, 0xa9, 0xac, 0x4a, 0xd2 } };
	cfg_bool show_icons(guid_cfg_show_icons, true);

	// Minimum visible items
	// {f9c917db-6274-46b8-816c-939708ba67dc}
	static const GUID guid_cfg_min_visible = { 0xf9c917db, 0x6274, 0x46b8, { 0x81, 0x6c, 0x93, 0x97, 0x08, 0xba, 0x67, 0xdc } };
	cfg_int min_visible(guid_cfg_min_visible, DEF_MIN_VISIBLE);

	// {23ED2850-B461-41d4-B835-35E88AE7B8B9}
	static const GUID guid_cfg_dd_autoexpand = { 0x23ed2850, 0xb461, 0x41d4, { 0xb8, 0x35, 0x35, 0xe8, 0x8a, 0xe7, 0xb8, 0xb9 } };
	cfg_bool dd_autoexpand(guid_cfg_dd_autoexpand, true);

	// {BDAAC958-3940-4e94-9DBA-8B7C71866A8F}
	static const GUID guid_cfg_dd_autoexpand_delay = { 0xbdaac958, 0x3940, 0x4e94, { 0x9d, 0xba, 0x8b, 0x7c, 0x71, 0x86, 0x6a, 0x8f } };
	cfg_int dd_autoexpand_delay(guid_cfg_dd_autoexpand_delay, DEF_AUTOEXPAND_DELAY);

	// {EC14052E-B92E-460e-B3DE-8C84463782D8}
	static const GUID guid_cfg_dd_autoswitch = { 0xec14052e, 0xb92e, 0x460e, { 0xb3, 0xde, 0x8c, 0x84, 0x46, 0x37, 0x82, 0xd8 } };
	cfg_bool dd_autoswitch(guid_cfg_dd_autoswitch, true);

	// {CE881E2C-D5EC-4e54-A8E7-314BAB3B2BE2}
	static const GUID guid_cfg_dd_autoswitch_delay = { 0xce881e2c, 0xd5ec, 0x4e54, { 0xa8, 0xe7, 0x31, 0x4b, 0xab, 0x3b, 0x2b, 0xe2 } };
	cfg_int dd_autoswitch_delay(guid_cfg_dd_autoswitch_delay, DEF_AUTOSWITCH_DELAY);

	// {D7024D20-AFB6-4608-A9AB-C5AEFDA6B371}
	static const GUID guid_cfg_dd_activate = { 0xd7024d20, 0xafb6, 0x4608, { 0xa9, 0xab, 0xc5, 0xae, 0xfd, 0xa6, 0xb3, 0x71 } };
	cfg_bool dd_activate(guid_cfg_dd_activate, true);

	// TODO: Remove it in future.
	// BACKUP OF OLD (PRE-0.7.6) USER'S TITLE AND STYLE STRINGS
	// {880D21D8-1565-427b-B819-FF535EB6D994}
	static const GUID _guid_cfg_old_title_string = { 0x880d21d8, 0x1565, 0x427b, { 0xb8, 0x19, 0xff, 0x53, 0x5e, 0xb6, 0xd9, 0x94 } };
	cfg_string _old_title_string(_guid_cfg_old_title_string, DEF_TITLE_STRING);
	// {A88AB4B8-CCE0-44c0-AB77-613DBC80D083}
	static const GUID _guid_cfg_old_style_string = { 0xa88ab4b8, 0xcce0, 0x44c0, { 0xab, 0x77, 0x61, 0x3d, 0xbc, 0x80, 0xd0, 0x83 } };
	cfg_string _old_style_string(_guid_cfg_old_style_string, DEF_STYLE_STRING);
}
