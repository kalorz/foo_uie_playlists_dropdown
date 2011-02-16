// ========================================================================================================================
#ifndef _VERSION_H_
#define _VERSION_H_
// ========================================================================================================================

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define VER_MAJOR			0
#define VER_MINOR1			7
#define VER_MINOR2			6
#define VER_MINOR3			0

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define STR(value)			#value
#define STRINGIZE(value)	STR(value)

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if VER_MINOR3 > 0
#define COMPONENT_VERSION	STRINGIZE(VER_MAJOR) "." STRINGIZE(VER_MINOR1) "." STRINGIZE(VER_MINOR2) "." STRINGIZE(VER_MINOR3)
#elif VER_MINOR2 > 0
#define COMPONENT_VERSION	STRINGIZE(VER_MAJOR) "." STRINGIZE(VER_MINOR1) "." STRINGIZE(VER_MINOR2)
#else
#define COMPONENT_VERSION	STRINGIZE(VER_MAJOR) "." STRINGIZE(VER_MINOR1)
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define FILE_COMPANY		"Karol Sarnacki"
#define FILE_COPYRIGHT		"Copyright (C) 2006-2009 " FILE_COMPANY
#define FILE_VERSION		VER_MAJOR,VER_MINOR1,VER_MINOR2,VER_MINOR3
#define FILE_VERSION_STR	STRINGIZE(VER_MAJOR) ", " STRINGIZE(VER_MINOR1) ", " STRINGIZE(VER_MINOR2) ", " STRINGIZE(VER_MINOR3)
#define FILE_NAME			"foo_uie_playlists_dropdown"
#ifndef _DEBUG
#define FILE_FLAGS			0
#else
#define FILE_FLAGS			(VS_FF_DEBUG|VS_FF_PRIVATEBUILD)
#endif

// ========================================================================================================================
#endif
// ========================================================================================================================
