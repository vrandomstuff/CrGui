#pragma once

#include <stdlib.h>

#define nullFree(ptr) \
	do { \
		free(ptr); \
		ptr = NULL; \
	} while(0)
