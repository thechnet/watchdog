/*
overrides.h - watchdog
Modified 2021-12-02
*/

#ifndef WD_OVERRIDES_H
#define WD_OVERRIDES_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Overrides.
*/

void *wd_override_malloc(WD_STD_PARAMS, size_t size);
void *wd_override_realloc(WD_STD_PARAMS, void *memory, size_t new_size);
void wd_override_free(WD_STD_PARAMS, void *memory);

#endif /* !WD_OVERRIDES_H */
