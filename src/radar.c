/*
radar.c - watchdog
Modified 2021-12-02
*/

/* Header-specific includes. */
#include "radar.h"

/* Implementation-specific includes. */
#include "reporter.h"

/*
*** Radar constants.
*/

#define WD_RADAR_SIZE ((size_t)4096)
#define WD_RADAR_EMPTY_SPOT NULL

/*
*** Radar globals.
*/

wd_alloc *wd_radar = NULL;
size_t wd_radar_size = 0;

/*
*** Radar interface.
*/

/*
Start the radar.
*/
void wd_radar_enable(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(!wd_unleashed);
  assert(wd_radar == NULL);
  
  wd_radar_size = WD_RADAR_SIZE;
  wd_radar = malloc(wd_radar_size*sizeof(*wd_radar));
  if (wd_radar == NULL) {
    wd_alerts++;
    fail_at(WD_STD_ARGS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", wd_radar_size);
  }
}

/*
Stop the radar.
*/
void wd_radar_disable(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  
  free(wd_radar);
  wd_radar = NULL;
  wd_radar_size = 0;
}

/*
Locate an address on the radar.
*/
wd_alloc *wd_radar_find(WD_STD_PARAMS, char *memory)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  
  wd_alloc *alloc;
  for (alloc=wd_radar; alloc<wd_radar+wd_radar_size; alloc++)
    if (alloc->memory == memory)
      break;
  
  assert (alloc <= wd_radar+wd_radar_size);
  if (alloc == wd_radar+wd_radar_size) {
    if (memory == WD_RADAR_EMPTY_SPOT) {
      wd_radar_grow(WD_STD_PARAMS_PASS);
      alloc = wd_radar+wd_radar_size/2;
    } else {
      return NULL;
    }
  }
  return alloc;
}

/*
Double the radar size.
*/
void wd_radar_grow(WD_STD_PARAMS)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(wd_radar_size > 0);
  
  wd_radar_size *= 2;
  wd_radar = realloc(wd_radar, wd_radar_size*sizeof(*wd_radar));
  if (wd_radar == NULL) {
    wd_alerts++;
    fail_at(file, line, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", wd_radar_size);
  }
}

/*
Start tracking an address on the radar.
*/
wd_alloc *wd_radar_watch(WD_STD_PARAMS, char *memory, size_t size, bool check_padding)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  
  wd_alloc *alloc = wd_radar_find(WD_STD_PARAMS_PASS, WD_RADAR_EMPTY_SPOT);
  assert(alloc != NULL);
  *alloc = (wd_alloc){
    .memory = memory,
    .size = size,
    .origin = (wd_point){ WD_STD_PARAMS_PASS },
    .check_padding = check_padding,
    .snapshot = NULL
  };
  return alloc;
}

/*
Stop tracking an address on the radar.
*/
bool wd_radar_drop(WD_STD_PARAMS, char *memory)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  
  wd_alloc *alloc = wd_radar_find(WD_STD_PARAMS_PASS, memory);
  if (alloc == NULL)
    return false;
  
  // FIXME: Snapshots are allocated and reallocated at the function overrides.
  if (alloc->snapshot != NULL)
    free(alloc->snapshot);
  
  *alloc = (wd_alloc){
    .memory = NULL,
    .size = 0,
    .origin = (wd_point){ NULL, 0 },
    .snapshot = NULL
  };
  return true;
}
