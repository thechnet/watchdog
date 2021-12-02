#ifndef WATCHDOG
#define WATCHDOG

#include "src/overrides.h"

/*
Overrides.
*/

#define malloc(__size) wd_override_malloc(WD_STD_ARGS, __size)
#define realloc(__memory, __new_size) wd_override_realloc(WD_STD_ARGS, __memory, __new_size);
#define free(__memory) wd_override_free(WD_STD_ARGS, __memory)

/*
Keywords.
*/

#define if(__expr...) if (wd_bark(WD_STD_ARGS) && (__expr))
#define break { wd_bark(WD_STD_ARGS); break; }
#define continue { wd_bark(WD_STD_ARGS); continue; }
#define while(__expr...) while (wd_bark(WD_STD_ARGS) && (__expr))
#define for(__opt...) for (__opt*wd_bark(WD_STD_ARGS))
#define switch(__expr...) switch ((__expr)*wd_bark(WD_STD_ARGS))
#define sizeof(__type) (sizeof(__type)*wd_bark(WD_STD_ARGS))

#endif /* !WATCHDOG */
