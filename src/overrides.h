/*
overrides.h - watchdog
Modified 2021-11-30
*/

#include "private.h"
#include "radar.h"

void *wd_override_malloc(WD_STD_PARAMS, size_t size);
void wd_override_free(WD_STD_PARAMS, void *memory);
