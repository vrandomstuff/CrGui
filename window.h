#pragma once

typedef enum Platform {
	PLAT_WINDOWS,
	PLAT_UNSUPPORTED
} Platform;

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define PLAT PLAT_WINDOWS
// Every platform in this windowing group shares the Win32 implementation.

#define PLAT_IS_WINDOWS 1
#else
// TODO: X11 support (not doing wayland seems like more of a mess then win32 which is a mess already)
#define PLAT PLAT_UNSUPPORTED
#define PLAT_IS_WINDOWS 0
#error platform not supported 
#endif

#include <stdbool.h>
#include "strings.h"

typedef struct {
	int r;
	int b;
	int g;
} Color;

typedef struct {
	#if PLAT_IS_WINDOWS
	HWND _handle;
	HBRUSH _hbBg;
	#endif
	Color background;
	String title;
	bool shouldClose;
} Window;


Window createWindow(int w, int h, Color bg, String title);

void pollEvents(Window *win);
void closeWindow(Window *win);
