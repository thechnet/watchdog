/*
watchdog.h - watchdog
Modified 2021-12-02
*/

#if defined(WATCHDOG) && !defined(WATCHDOG_H)
#define WATCHDOG_H

#include "src/public.h"
#include "src/overrides.h"
#include "src/bounds.h"

#define WD_STD_ARGS __FILE__, __LINE__ // FIXME:

/*
Overrides.
*/

#define malloc(__size) wd_override_malloc(WD_STD_ARGS, __size)
#define realloc(__memory, __new_size) wd_override_realloc(WD_STD_ARGS, __memory, __new_size)
#define free(__memory) wd_override_free(WD_STD_ARGS, __memory)
#undef assert
#define assert(__expression...) wd_override_assert(WD_STD_ARGS, #__expression, __expression)

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

/*
Bounds checking.
*/

#define WD_ARRAY(array, index) array[wd_bounds_check(WD_STD_ARGS, (void*)array, sizeof(array), sizeof(array[0]), index)*index]

#else /* !WATCHDOG_H */

#define WD_ARRAY(array, index) array[index]

#endif /* !WATCHDOG_H */
