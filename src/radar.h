/*
radar.h - watchdog
Modified 2021-11-30
*/

#ifndef WD_RADAR_H
#define WD_RADAR_H

/* Header-specific includes. */
#include "private.h"

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

void wd_radar_open(void);
void wd_radar_close(void);
wd_alloc *wd_radar_locate(WD_STD_PARAMS, char *memory);
void wd_radar_grow(WD_STD_PARAMS);
wd_alloc *wd_radar_track(WD_STD_PARAMS, char *memory, size_t memory_size);
bool wd_radar_drop(WD_STD_PARAMS, char *memory);

#endif
