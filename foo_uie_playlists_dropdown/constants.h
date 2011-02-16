// ========================================================================================================================
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Icons and overlays resource type names
#define RES_T_OVERLAY	_T("Overlay")
#define RES_T_ICON		_T("Image")
#define RES_T_INTERNAL	_T("Internal")

// Timers
enum {
	IDT_AUTOEXPAND,
	IDT_MWHEEL,
	IDT_AUTOSWITCH
};

// Custom window messages
enum {
	MSG_UPDATE = WM_APP,
	// Drag & Drop
	MSG_DRAG_OVER,
	MSG_DRAG_ENTER,
	MSG_DRAG_LEAVE,
	MSG_DROP_FILES,
	MSG_DROP_FILES_EX
};

// Context menu commands
enum {
	ID_PLAY = 1,
	ID_ACTIVATE,
	ID_RENAME,
	ID_REMOVE,
	ID_ICON_FIRST,
	ID_ICON_LAST = ID_ICON_FIRST + 1000,
	ID_ICON_CUSTOM,
	ID_ICON_REMOVE,
	ID_ICON_HISTORY_PURGE,
	ID_SORT_BY,
	ID_SORT_RANDOMIZE,
	ID_SORT_REVERSE,
	ID_SORT_BY_NAME,
	ID_SORT_BY_SIZE,
	ID_SORT_BY_FILESIZE,
	ID_SORT_BY_LENGTH,
	ID_SORT_BY_LOCK,
	ID_AUTOPLAYLIST,
	ID_CUT,
	ID_COPY,
	ID_PASTE_PLAYLIST,
	ID_PASTE_ITEMS_ADD,
	ID_PASTE_ITEMS_REPLACE,
	ID_PASTE_ITEMS_CREATE,
	ID_NEW,
	ID_LOAD,
	ID_SAVE_AS,
	ID_SAVE_ALL_AS,
	ID_HISTORY_FIRST,
	ID_HISTORY_LAST = ID_HISTORY_FIRST + 1000,
	ID_HISTORY_PURGE,
	ID_CONTEXT_FIRST,
	ID_CONTEXT_LAST = ID_CONTEXT_FIRST + 1000,
};

// ========================================================================================================================
#endif
// ========================================================================================================================
