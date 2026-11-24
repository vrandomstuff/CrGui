#include "crgui.h"
#include <stdio.h>

int main(void) {
	String title = {0};
	cstrAppend(title, "Hello, World!");
	Color bg = {255,100,100};
	Window window = createWindow(800, 600, bg, title);
	while(!window.shouldClose) {
		pollEvents(&window);
	}
	closeWindow(&window);
	return 0;
}
