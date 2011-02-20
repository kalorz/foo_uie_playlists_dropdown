// ========================================================================================================================
#ifndef _VSSTYLE_HACK_H_
#define _VSSTYLE_HACK_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define VSCLASS_BUTTONSTYLE	L"BUTTONSTYLE"
#define VSCLASS_BUTTON	L"BUTTON"

#define VSCLASS_MENUSTYLE	L"MENUSTYLE"
#define VSCLASS_MENU	L"MENU"

enum __POPUPITEMSTATES {
	MPI_NORMAL = 1,
	MPI_HOT = 2,
	MPI_DISABLED = 3,
	MPI_DISABLEDHOT = 4,
};

enum __MENUPARTS {
	MENU_MENUITEM_TMSCHEMA = 1,
	MENU_MENUDROPDOWN_TMSCHEMA = 2,
	MENU_MENUBARITEM_TMSCHEMA = 3,
	MENU_MENUBARDROPDOWN_TMSCHEMA = 4,
	MENU_CHEVRON_TMSCHEMA = 5,
	MENU_SEPARATOR_TMSCHEMA = 6,
	MENU_BARBACKGROUND = 7,
	MENU_BARITEM = 8,
	MENU_POPUPBACKGROUND = 9,
	MENU_POPUPBORDERS = 10,
	MENU_POPUPCHECK = 11,
	MENU_POPUPCHECKBACKGROUND = 12,
	MENU_POPUPGUTTER = 13,
	MENU_POPUPITEM = 14,
	MENU_POPUPSEPARATOR = 15,
	MENU_POPUPSUBMENU = 16,
	MENU_SYSTEMCLOSE = 17,
	MENU_SYSTEMMAXIMIZE = 18,
	MENU_SYSTEMMINIMIZE = 19,
	MENU_SYSTEMRESTORE = 20,
};

enum __POPUPCHECKBACKGROUNDSTATES {
	MCB_DISABLED = 1,
	MCB_NORMAL = 2,
	MCB_BITMAP = 3,
};

// ========================================================================================================================
#endif
// ========================================================================================================================
