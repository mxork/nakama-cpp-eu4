// #define PATCH_HOST "localhost"
// #define PATCH_KEY "default_key"
// #define PATCH_PORT 7350
// #define PATCH_SSL false

#define PATCH_HOST "34.152.37.181"
#define PATCH_KEY "default_key"
#define PATCH_PORT 443
#define PATCH_SSL true

void patcher_on_match_join();
bool patcher_loaded();

#ifdef __linux__
#include <dlfcn.h>
#include "patcher.h"
extern void* libpatcher_handle;
#endif

#ifdef WIN32
#include "patcher.h"
#endif
