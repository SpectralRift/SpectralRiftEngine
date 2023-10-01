#include <cassert>
#include <cstdio>

#define ASSERT_MSG(condition, message) assert(condition && message);

#ifndef DEBUG_MSG
#ifndef BUILD_RELEASE
#define DEBUG_MSG(...) \
	printf("[%s:%i] ", __FILE__, __LINE__); \
	printf(__VA_ARGS__);
#else
#define DEBUG_MSG
#endif
#endif