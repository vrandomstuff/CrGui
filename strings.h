#pragma once

#include <stdio.h>
#include <string.h>
#include "dynamic.h"

daMaker(char, String);

char* str2cstr(String str) {
    char* cstr = malloc(str.count + 1);

    if (!cstr) return NULL;

    memcpy(cstr, str.items, str.count);
    cstr[str.count] = '\0';

    return cstr;
}
#define cstrAppend(da, cstr) \
	do { \
		size_t cstrLen = strlen((cstr)); \
		for (size_t i = 0; i < cstrLen; i++) { \
			daAppend((da), (cstr)[i]); \
		} \
	} while(0)

#define stringPrint(str) \
	do { \
		for (size_t i = 0; i < (str).count; i++) { \
			printf("%c", (str.items)[i]); \
		} \
	} while(0)
