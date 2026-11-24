#include <stdio.h>
#include <stdlib.h>

#define die(message) \
	do { \
		printf("[panic] "message" %s:%d\n", __FILE__, __LINE__); \
		abort(); \
	} while (0)

int main() {
	die("test");
	return 0;
}
