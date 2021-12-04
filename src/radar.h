/*
radar.h - watchdog
Modified 2021-12-04
*/

#ifndef WD_RADAR_H
#define WD_RADAR_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Radar constants.
*/

#define WD_RADAR_SIZE ((size_t)1) // FIXME: 4096

/*
*** Radar types.
*/

typedef struct _wd_alloc {
  wd_point point; /* The point of allocation, reallocation, or deallocation. */
  size_t size;
  char *address;
  char *snapshot;
  char *original;
  bool padding_check_left;
  bool padding_check_right;
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

wd_alloc *wd_radar_add(WD_STD_PARAMS,
  char *memory_virtual,
  size_t size,
  bool add_padding,
  bool take_snapshots,
  bool capture_original
);
void wd_radar_drop(wd_alloc *alloc);

void wd_radar_clear(wd_alloc *alloc);

wd_alloc *wd_radar_search(char *memory);

#endif
