#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"


#define die(message) \
	do { \
		printf("[panic] "message" %s:%d\n", __FILE__, __LINE__); \
		abort(); \
	} while (0)

#define daAppend(da, item) \
	do { \
		if ((da).size < (da).count + 1) { \
			(da).size = (da).size ? (da).size * 2 : 16; \
			void *new_items = realloc((da).items, (da).size * sizeof(*(da).items)); \
			if (new_items == NULL) { \
				die("[dynamic utils] failed to realloc in daAppend"); \
			} \
			(da).items = new_items; \
		} \
		(da).items[(da).count++] = (item); \
	} while (0)

#define daMaker(type, name) \
	typedef struct { \
		size_t count; \
		size_t size; \
		type *items; \
	} name

#define daReset(da) \
	do { \
		nullFree((da).items); \
		(da).count = 0; \
		(da).size = 0; \
	} while(0)
