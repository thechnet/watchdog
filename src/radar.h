/*
radar.h - watchdog
Modified 2021-12-04
*/

#ifndef WD_RADAR_H
#define WD_RADAR_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Radar types.
*/

typedef struct _wd_alloc {
  wd_point origin;
  void *memory;
  size_t size;
  bool check_padding;
  void *snapshot;
} wd_alloc;

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

wd_alloc *wd_radar_add(WD_STD_PARAMS, void *memory, size_t size, bool check_padding);
void wd_radar_drop(wd_alloc *alloc);

void wd_radar_clear(wd_alloc *alloc);

wd_alloc *wd_radar_search(void *memory);

#endif
