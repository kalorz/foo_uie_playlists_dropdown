#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Component version
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

DECLARE_COMPONENT_VERSION(
COMPONENT_NAME,
COMPONENT_VERSION,
COMPONENT_NAME " v" COMPONENT_VERSION " - Columns UI Extension\n"
COMPONENT_DESCRIPTION "\n\n"
"Compilation date:  " __DATE__ "\n"
"Panel API version: " UI_EXTENSION_VERSION "\n\n"
FILE_COPYRIGHT
);

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static uie::window_factory<playlists_dropdown> g_playlists_dropdown_factory;
static preferences_page_factory_t<playlists_dropdown::preferences> g_playlists_dropdown_preferences_factory;
static cui::colours::client::factory<playlists_dropdown::colors_callback_impl> g_playlists_dropdown_colors_callback_factory;
static cui::fonts::client::factory<playlists_dropdown::fonts_callback_impl> g_playlists_dropdown_fonts_callback_factory;

/*
// {ab026d09-c0e8-4049-89c0-ffed1eeb0658}
static const GUID guid_fcl_title_scripts = { 0xab026d09, 0xc0e8, 0x4049, { 0x89, 0xc0, 0xff, 0xed, 0x1e, 0xeb, 0x06, 0x58 } };

// {4f121cad-3511-4fa4-8692-059410d39774}
static const GUID ggg = { 0x4f121cad, 0x3511, 0x4fa4, { 0x86, 0x92, 0x05, 0x94, 0x10, 0xd3, 0x97, 0x74 } };

static cui::fcl::group_impl_factory eee(ggg, "Tralala", "Opis", cui::fcl::groups::title_scripts);
class my_dataset : public cui::fcl::dataset {
public:
	void get_name (pfc::string_base & p_out) const { p_out = COMPONENT_NAME " Scripts"; }
	const GUID & get_guid () const { return guid_fcl_title_scripts; }
	const GUID & get_group () const { return ggg; }
	void get_data (stream_writer * p_writer, t_uint32 type, cui::fcl::t_export_feedback & feedback, abort_callback & p_abort) const {}
	void set_data (stream_reader * p_reader, t_size size, t_uint32 type, cui::fcl::t_import_feedback & feedback, abort_callback & p_abort) {}
};
static cui::fcl::dataset_factory<my_dataset> g_playlists_dropdown_dataset_factory;
*/

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
