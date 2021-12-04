/*
radar.c - watchdog
Modified 2021-12-04
*/

/* Header-specific includes. */
#include "radar.h"

/* Implementation-specific includes. */
#include "reporter.h"
#include "usage.h"
#include "snapshots.h"
#include "dangling.h"
#include "padding.h"

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
  assert(wd_radar_size == 0);
  
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
  assert(wd_radar_size != 0);
  
  free(wd_radar);
  wd_radar = NULL;
  wd_radar_size = 0;
}

/*
Start tracking an address on the radar.
*/
wd_alloc *wd_radar_add(WD_STD_PARAMS,
  char *memory_virtual,
  size_t size,
  bool add_padding,
  bool take_snapshots,
  bool capture_original
)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(memory_virtual != NULL);
  
  /* Find the next free spot. */
  wd_alloc *alloc = wd_radar+wd_radar_size;
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].address == NULL)
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
  
  /* Store. */
  *alloc = (wd_alloc){
    .point = (wd_point){ WD_STD_PARAMS_PASS },
    .size = size,
    .address = memory_virtual,
    .snapshot = NULL,
    .original = NULL,
    .padding_check_left = add_padding,
    .padding_check_right = add_padding
  };
  
  /* Add padding. */
  wd_padding_write(alloc);
  
  /* Take first snapshot. */
  wd_snapshot_alloc(alloc);
  wd_snapshot_capture(alloc);
  
  /* Capture original. */
  wd_usage_original_capture(alloc);
  
  /* Remove address from dangling pointer record if recorded as dangling pointer. */
  wd_dangling_find_and_erase(alloc->address);
  
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
  assert(alloc->address != NULL);
  
  // FIXME: Snapshots are allocated and reallocated at the function overrides.
  if (alloc->snapshot != NULL)
    free(alloc->snapshot);
  
  // FIXME: " ?
  assert(alloc->original != NULL);
  free(alloc->original);
  
  wd_radar_clear(alloc);
}

/*
Clear an entry in the radar.
*/
void wd_radar_clear(wd_alloc *alloc)
{
  *alloc = (wd_alloc){
    .address = NULL,
    .size = 0,
    .point = (wd_point){ NULL, 0 },
    .snapshot = NULL
  };
}

/*
Locate an address on the radar.
*/
wd_alloc *wd_radar_search(char *memory)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(memory != NULL);
  
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].address == memory)
      return wd_radar+i;
  
  return NULL;
}
