/*
radar.c - watchdog
Modified 2021-12-04
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
  
  /* Allocate radar memory. */
  wd_radar_size = WD_RADAR_SIZE;
  wd_radar = malloc(wd_radar_size*sizeof(*wd_radar));
  if (wd_radar == NULL) {
    wd_alerts++;
    fail_at(WD_STD_ARGS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", wd_radar_size);
  }
  
  /* Clear uninitialized allocations. */
  for (size_t i=0; i<wd_radar_size; i++)
    wd_radar_clear(wd_radar+i);
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
Start tracking an address on the radar.
*/
wd_alloc *wd_radar_add(WD_STD_PARAMS, void *memory, size_t size, bool check_padding)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(memory != NULL);
  
  /* Find the next free spot. */
  wd_alloc *alloc = wd_radar+wd_radar_size;
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].memory == NULL)
      alloc = wd_radar+i;
  
  /* Grow the radar if no free spots are left. */
  if (alloc == wd_radar+wd_radar_size) {
    wd_radar_size *= 2;
    wd_radar = realloc(wd_radar, wd_radar_size*sizeof(*wd_radar));
    WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_radar, wd_radar_size*sizeof(*wd_radar), 0);
    
    /* Initialize the newly added spots. */
    for (size_t i=wd_radar_size/2; i<wd_radar_size; i++)
      wd_radar_clear(wd_radar+i);
  }
  
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
void wd_radar_drop(wd_alloc *alloc)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(alloc != NULL);
  assert(alloc->memory != NULL);
  
  // FIXME: Snapshots are allocated and reallocated at the function overrides.
  if (alloc->snapshot != NULL)
    free(alloc->snapshot);
  
  wd_radar_clear(alloc);
}

/*
Clear an entry in the radar.
*/
void wd_radar_clear(wd_alloc *alloc)
{
  *alloc = (wd_alloc){
    .memory = NULL,
    .size = 0,
    .origin = (wd_point){ NULL, 0 },
    .snapshot = NULL
  };
}

/*
Locate an address on the radar.
*/
wd_alloc *wd_radar_search(void *memory)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(memory != NULL);
  
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].memory == memory)
      return wd_radar+i;
  
  return NULL;
}
