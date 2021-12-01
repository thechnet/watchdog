/*
overrides.h - watchdog
Modified 2021-12-01
*/

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Overrides.
*/

void *wd_override_malloc(WD_STD_PARAMS, size_t size);
void wd_override_free(WD_STD_PARAMS, void *memory);
