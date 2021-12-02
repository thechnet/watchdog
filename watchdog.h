#ifndef WATCHDOG
#define WATCHDOG

#include "src/overrides.h"

#define malloc(size) wd_override_malloc(WD_STD_ARGS, size)
#define realloc(memory, new_size) wd_override_realloc(WD_STD_ARGS, memory, new_size);
#define free(memory) wd_override_free(WD_STD_ARGS, memory)

#endif /* !WATCHDOG */
