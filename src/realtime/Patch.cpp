#include <stdio.h>
#include "Patch.h"

#ifdef __linux__
void* libpatcher_handle = NULL;
__attribute__((constructor))
void on_load() {
  void* handle = dlopen("./libpatcher.so", RTLD_NOW);
  if (!handle) {
    info("libpatcher not loaded %s\n", dlerror());
  } else {
    info("libpatcher loaded\n");
  }
  libpatcher_handle = handle;
}

bool patcher_loaded() { return libpatcher_handle != NULL; }
void patcher_on_match_join() {
  if (libpatcher_handle) {
    void (*on_join)(void);
    on_join = (void(*)())dlsym(libpatcher_handle, "on_nakama_client_join_match");
    on_join();
  }
}
#endif

#ifdef WIN32
bool patcher_loaded() { return true; }
void patcher_on_match_join() {
  on_nakama_client_join_match();
}
#endif
