/*
radar.c - watchdog
Modified 2021-12-08
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
Find the next free spot on the radar, or grow it.
*/
wd_alloc *wd_radar_find_next_free_spot(void)
{
  wd_alloc *alloc = NULL;
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].address == NULL)
      alloc = wd_radar+i;
  
  /* Grow the radar if no free spots are left. */
  if (alloc == NULL) {
    size_t old_size = wd_radar_size;
    
    wd_radar_size *= 2;
    wd_radar = realloc(wd_radar, wd_radar_size*sizeof(*wd_radar));
    WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_radar, wd_radar_size*sizeof(*wd_radar), 0);
    
    /* Initialize the newly added spots. */
    for (size_t i=old_size; i<wd_radar_size; i++)
      wd_radar_clear(wd_radar+i);
    
    alloc = wd_radar+old_size;
  }
  
  return alloc;
}

/*
Start tracking an address on the radar.
*/
wd_alloc *wd_radar_add(WD_STD_PARAMS,
  char *memory_real,
  size_t size_virtual,
  bool add_padding,
  bool take_snapshots,
  bool capture_original
)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(memory_real != NULL);
  
  /* Find the next free spot. */
  wd_alloc *alloc = wd_radar_find_next_free_spot();
  
  /* Convert real address to virtual address. */
  char *memory_virtual;
  if (add_padding)
    memory_virtual = WD_TO_VIRTUAL(memory_real);
  else
    memory_virtual = memory_real;
  
  /* Store. */
  *alloc = (wd_alloc){
    .point = (wd_point){ WD_STD_PARAMS_PASS },
    .size = size_virtual,
    .address = memory_virtual,
    .snapshot = NULL,
    .original = NULL,
    .is_padded = add_padding,
    .padding_check_left = add_padding,
    .padding_check_right = add_padding
  };
  
  /* Add padding. */
  if (alloc->is_padded)
    wd_padding_write(alloc);
  
  /* Take first snapshot. */
  if (take_snapshots) {
    wd_snapshot_alloc(alloc);
    wd_snapshot_capture(alloc);
  }
  
  /* Capture original. */
  if (capture_original)
    wd_usage_original_capture(alloc);
  
  /* Remove address from dangling pointer record if recorded as dangling pointer. */
  wd_dangling_find_and_clear(alloc->address);
  
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
  if (alloc->original != NULL)
    free(alloc->original);
  
  wd_radar_clear(alloc);
}

/*
Clear an entry in the radar.
*/
void wd_radar_clear(wd_alloc *alloc)
{
  *alloc = (wd_alloc){
    .point = (wd_point){ NULL, 0 },
    .size = 0,
    .address = NULL,
    .snapshot = NULL,
    .original = NULL,
    .is_padded = false,
    .padding_check_left = false,
    .padding_check_right = false
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

/*
Retrieve the real size.
*/
size_t wd_radar_real_size_get(wd_alloc *alloc)
{
  assert(alloc != NULL);
  return alloc->size+2*WD_PADDING_SIZE;
}

/*
Retrieve the real address.
*/
char *wd_radar_real_address_get(wd_alloc *alloc)
{
  assert(alloc != NULL);
  if (alloc->is_padded)
    return alloc->address-WD_PADDING_SIZE;
  else
    return alloc->address;
}

/*
Update the virtual address.
*/
void wd_radar_virtual_address_set(wd_alloc *alloc, char *address_new_real)
{
  assert(alloc != NULL);
  assert(address_new_real != NULL);
  if (alloc->is_padded)
    alloc->address = address_new_real+WD_PADDING_SIZE;
  else
    alloc->address = address_new_real;
}

/*
Unlock an allocation for reallocation.
*/
void wd_radar_unlock(wd_alloc *alloc)
{
  if (alloc->is_padded)
    wd_padding_clear(alloc);
}

/*
Lock an allocation after reallocation.
*/
void wd_radar_lock(wd_alloc *alloc, size_t size_virtual_new, char *address_new_real)
{
  assert(alloc != NULL);
  assert(address_new_real != NULL);
  
  /* Update entry. */
  wd_radar_virtual_address_set(alloc, address_new_real);
  int growth = size_virtual_new-alloc->size;
  alloc->size = size_virtual_new;
  
  /* Remove address from dangling pointer record if previously recorded. */
  wd_dangling_find_and_clear(alloc->address);
  
  /* Re-add padding. */
  if (alloc->is_padded)
    wd_padding_write(alloc);
  
  /* Update snapshot. */
  if (alloc->snapshot != NULL) {
    wd_snapshot_realloc(alloc);
    wd_snapshot_capture(alloc);
  }
  
  /* Update original. */
  if (alloc->original != NULL)
    wd_usage_original_update(alloc, growth);
}
