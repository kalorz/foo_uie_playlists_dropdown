// ========================================================================================================================
#ifndef _COMPONENT_H_
#define _COMPONENT_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define COMPONENT_NAME			"Playlists Dropdown"
#define COMPONENT_SHORT_NAME	"Playlists"
#define COMPONENT_DESCRIPTION	"Drop-down list for displaying and manipulating the playlists"
#define COMPONENT_CATEGORY		"Toolbars"
#define COMPONENT_HELP_URL		"http://www.hydrogenaudio.org/forums/index.php?showtopic=44703"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define _INC_MALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "../../SDK/foobar2000.h"
#include "../../helpers/helpers.h"

#include <windowsx.h>
#include <gdiplus.h>
#include <oleidl.h>
#include <uxtheme.h>
#include <winuser.h>
#include <vssym32.h>
#include <commctrl.h>

#include "../../columns_ui-sdk/ui_extension.h"

#include "CGdiPlusBitmap.h"
#include "config.h"
#include "constants.h"
#include "drop_process_locations.h"
#include "drop_target.h"
#include "guids.h"
#include "helpers.h"
#include "playlists_dropdown.h"
#include "resource.h"
#include "titleformat_hooks.h"
#include "version.h"
#include "DropSourceImpl.h"

// ========================================================================================================================
#endif
// ========================================================================================================================
