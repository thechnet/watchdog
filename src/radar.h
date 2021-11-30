/*
radar.h - watchdog
Modified 2021-11-30
*/

#ifndef WD_RADAR_H
#define WD_RADAR_H

#include "private.h"

#define WD_RADAR_EMPTY_SPOT NULL

void wd_radar_start(WD_STD_PARAMS);
void wd_radar_stop(WD_STD_PARAMS);
wd_alloc *wd_radar_locate(WD_STD_PARAMS, char *memory);
void wd_radar_grow(WD_STD_PARAMS);
wd_alloc *wd_radar_track(WD_STD_PARAMS, char *memory, size_t memory_size);
bool wd_radar_drop(WD_STD_PARAMS, char *memory);

extern wd_alloc *wd_radar;

#endif
