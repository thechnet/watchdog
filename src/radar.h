/*
radar.h - watchdog
Modified 2021-12-08
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
  wd_point point; /* The point of allocation or reallocation. */
  size_t size; /* FIXME: Rename to size_virtual. */
  char *address; /* FIXME: " */
  char *snapshot;
  char *original;
  bool is_padded;
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
  char *memory_real,
  size_t size_virtual,
  bool add_padding,
  bool take_snapshots,
  bool capture_original
);
void wd_radar_drop(wd_alloc *alloc);
void wd_radar_clear(wd_alloc *alloc);
wd_alloc *wd_radar_find_next_free_spot(void);
wd_alloc *wd_radar_search(char *memory);
size_t wd_radar_real_size_get(wd_alloc *alloc);
char *wd_radar_real_address_get(wd_alloc *alloc);
void wd_radar_virtual_address_set(wd_alloc *alloc, char *address_new_real);
void wd_radar_unlock(wd_alloc *alloc);
void wd_radar_lock(wd_alloc *alloc, size_t size_virtual_new, char *address_new_real);

#endif
