#include <stdio.h>
#include <stdarg.h>
#include "Patch.h"

void patcher_on_match_join() {
  on_nakama_client_join_match();
}
