#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>

#include "window.h"

#define WINDOW_CLASS "MichaelsoftBinbows"

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_NCCREATE: {
		CREATESTRUCTA *cs = (CREATESTRUCTA*)lParam;
		Window *win = (Window*)cs->lpCreateParams;

		// Store the brush, not the temporary Window pointer.
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)win->_hbBg);
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}

	case WM_ERASEBKGND: {
		HBRUSH brush = (HBRUSH)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

		if (brush) {
			RECT rc;
			GetClientRect(hwnd, &rc);
			FillRect((HDC)wParam, &rc, brush);
		}

		return 1;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	}

	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

Window createWindow(int w, int h, Color bg, String title) {
	char* cTitle = str2cstr(title);
	if (!cTitle) die("unable to allocate cTitle");

	HINSTANCE instance = GetModuleHandleA(NULL);
	WNDCLASSA wc = {0};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = WINDOW_CLASS;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;

	if (!RegisterClassA(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
		die("[WIN32 wrapper] RegisterClassA");

	Window window = {0};
	window.title = title;
	window._hbBg = CreateSolidBrush(RGB(bg.r, bg.g, bg.b));

	if (!window._hbBg)
		die("[WIN32 wrapper] CreateSolidBrush");

	window._handle = CreateWindowExA(
		0, WINDOW_CLASS, cTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		w, h,
		NULL, NULL, instance,
		&window
	);

	free(cTitle);

	if (!window._handle)
		die("[WIN32 wrapper] CreateWindowExA");

	ShowWindow(window._handle, SW_SHOW);
	UpdateWindow(window._handle);

	daReset(title);
	return window;
}

void pollEvents(Window *win) {
	MSG msg;

	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			win->shouldClose = true;
			continue;
		}

		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

void closeWindow(Window *win) {
	if (win->_handle) {
		DestroyWindow(win->_handle);
		win->_handle = NULL;
	} else {
		die("[WIN32 wrapper] win->_handle is null");
	}
	if (win->_hbBg) {
		DeleteObject(win->_hbBg);
		win->_hbBg = NULL;
	} else {
		die("[WIN32 wrapper] win->_hbBg is null");
	}
}
