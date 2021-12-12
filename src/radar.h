/*
radar.h - watchdog
Modified 2021-12-12
*/

#ifndef WD_RADAR_H
#define WD_RADAR_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Radar constants.
*/

#define WD_RADAR_SIZE ((size_t)1) // FIXME: 4096

#define WD_RADAR_ORIENTATE_UNFOUND 0
#define WD_RADAR_ORIENTATE_ENCLOSED 1
#define WD_RADAR_ORIENTATE_TOUCHING_LEFT 2
#define WD_RADAR_ORIENTATE_TOUCHING_RIGHT 3

/*
*** Radar types.
*/

typedef struct _wd_alloc {
  wd_point point;
  size_t size_user;
  char *addr_user;
  bool dependent;
  
  bool is_protected;
  char *snapshot;
  bool padding_check_left;
  bool padding_check_right;
  
  bool is_native;
  char *original;
} wd_alloc;

/*
*** Radar globals.
*/

extern wd_alloc *wd_radar;
extern size_t wd_radar_size;

/*
*** Radar interface.
*/

void wd_radar_initialize(void);
void wd_radar_terminate(void);

wd_alloc *wd_radar_add(
  WD_STD_PARAMS,
  char *addr_real,
  size_t size_user,
  bool protect,
  bool is_native,
  bool dependent,
  bool randomize_memory
);
void wd_radar_update(
  WD_STD_PARAMS,
  wd_alloc *alloc,
  size_t resize_user,
  char *migrated_real
);
void wd_radar_remove(
  WD_STD_PARAMS,
  wd_alloc *alloc
);

wd_alloc *wd_radar_search(
  char *addr_user
);
int wd_radar_orientate(
  char *address,
  wd_alloc **alloc
);

// FIXME: Is there a more elegant solution?
size_t wd_radar_size_real_get(wd_alloc *alloc);
char *wd_radar_addr_real_get(wd_alloc *alloc);
void wd_radar_addr_user_set(wd_alloc *alloc, char *migrated_real);

#endif
