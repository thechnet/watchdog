/*
watchdog.h - watchdog
Modified 2022-06-08
*/

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "src/public.h"

#ifdef WATCHDOG

#include "src/overrides.h"
#include "src/bounds.h"

#define WD_STD_ARGS __FILE__, __LINE__ // FIXME:

/*
Overrides.
*/

#define malloc(__size) wd_override_malloc(WD_STD_ARGS, (ptrdiff_t)(__size))
#define realloc(__memory, __new_size) wd_override_realloc(WD_STD_ARGS, (char*)(__memory), (ptrdiff_t)(__new_size))
#define free(__memory) wd_override_free(WD_STD_ARGS, (char*)(__memory))
#undef assert
#define assert(__expression...) wd_override_assert(WD_STD_ARGS, #__expression, __expression)
#undef strcpy
#define strcpy(__dest, __src) wd_override_strcpy(WD_STD_ARGS, (char*)(__dest), (char*)(__src))
#define strdup(__src) wd_override_strdup(WD_STD_ARGS, (char*)(__src))

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

#define WD_ARRAY(array, index) array[wd_bounds_check(WD_STD_ARGS, (void*)array, sizeof(array), sizeof(array[0]), index)]

#else /* !WATCHDOG */

#define WD_ARRAY(array, index) array[index]

#endif /* !WATCHDOG */

#endif /* !WATCHDOG_H */
