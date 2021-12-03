/*
radar.h - watchdog
Modified 2021-12-02
*/

#ifndef WD_RADAR_H
#define WD_RADAR_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Radar constants.
*/

#define WD_RADAR_EMPTY_SPOT NULL

/*
*** Radar globals.
*/

extern wd_alloc *wd_radar;
extern size_t wd_radar_size;

/*
*** Radar interface.
*/

void wd_radar_enable(void);
void wd_radar_disable(void);
wd_alloc *wd_radar_find(WD_STD_PARAMS, void *memory);
void wd_radar_grow(WD_STD_PARAMS);
wd_alloc *wd_radar_watch(WD_STD_PARAMS, void *memory, size_t size, bool check_padding);
bool wd_radar_drop(WD_STD_PARAMS, void *memory);

#endif
