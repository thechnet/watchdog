/*
radar.c - watchdog
Modified 2021-12-09
*/

/* Header-specific includes. */
#include "radar.h"

/* Implementation-specific includes. */
#include <memory.h>
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
  
  /* Create radar. */
  wd_radar_size = WD_RADAR_SIZE;
  wd_radar = malloc(wd_radar_size*sizeof(*wd_radar));
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_radar, 0, wd_radar_size*sizeof(*wd_radar));
  
  /* Zero the spots. */
  for (size_t i=0; i<wd_radar_size; i++)
    wd_radar_clear(wd_radar+i);
  
  wd_report("Radar: Enable");
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
  
  wd_report("Radar: Disable");
}

/*
Find the next free spot on the radar, or grow it.
*/
wd_alloc *wd_radar_find_spot(void)
{
  /* Look for an empty spot in the radar. */
  wd_alloc *alloc = NULL;
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].addr_user == NULL) {
      alloc = wd_radar+i;
      break;
    }
  
  /* Grow the radar if no empty spots are left. */
  if (alloc == NULL) {
    size_t size_old = wd_radar_size;
    wd_radar_size *= 2;
    wd_radar = realloc(wd_radar, wd_radar_size*sizeof(*wd_radar));
    WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_radar, 0, wd_radar_size*sizeof(*wd_radar));
    for (size_t i=size_old; i<wd_radar_size; i++) /* Initialize the newly added spots. */
      wd_radar_clear(wd_radar+i);
    alloc = wd_radar+size_old;
  }
  
  return alloc;
}

/*
Start tracking an address on the radar.
*/
wd_alloc *wd_radar_catch(WD_STD_PARAMS, char *addr_real, size_t size_user, bool protect, bool is_native, bool dependent, bool randomize_memory)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(addr_real != NULL);
  
  /* Find the next free spot. */
  wd_alloc *alloc = wd_radar_find_spot();
  
  /* Convert real address to user address. */
  char *addr_user;
  if (protect)
    addr_user = WD_ADDR_REAL_MAKE_USER(addr_real);
  else
    addr_user = addr_real;
  
  /* Store. */
  *alloc = (wd_alloc){
    .point = (wd_point){ WD_STD_PARAMS_PASS },
    .size_user = size_user,
    .addr_user = addr_user,
    .dependent = dependent,
    .is_protected = protect,
    .snapshot = NULL,
    .padding_check_left = protect,
    .padding_check_right = protect,
    .is_native = is_native,
    .original = NULL
  };
  
  /* Randomize memory before capturing original. */
  if (randomize_memory)
    wd_fill_with_random_bytes(addr_user, size_user);
  
  /* Protect. */
  if (alloc->is_protected) {
    /* Add padding. */
    wd_padding_write(alloc);
    
    /* Take first snapshot. */
    wd_snapshot_alloc(alloc);
    wd_snapshot_capture(alloc);
  }
  
  /* Capture original. */
  if (is_native)
    wd_usage_original_capture(alloc);
  
  /* Remove address from dangling pointer record if previously recorded. */
  wd_dangling_pointer *pointer = wd_dangling_search(alloc->addr_user);
  if (pointer != NULL)
    wd_dangling_clear(pointer);
  
  /* Update usage. */
  wd_usage_add(size_user);
  
  wd_report("Radar: Add %p (%zu)", alloc->addr_user, alloc->size_user);
  
  return alloc;
}

/*
Stop tracking an address on the radar.
*/
void wd_radar_release(WD_STD_PARAMS, wd_alloc *alloc)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(alloc != NULL);
  assert(alloc->addr_user != NULL);
  
  /* Assess usage. */
  if (alloc->is_native)
    wd_usage_original_compare(alloc);
  
  /* Free auxiliary memory. */
  if (alloc->is_protected) {
    assert(alloc->snapshot != NULL);
    free(alloc->snapshot);
  }
  if (alloc->is_native) {
    assert(alloc->original != NULL);
    free(alloc->original);
  }
  
  /* Record address in dangling pointer record. */
  wd_dangling_record(WD_STD_PARAMS_PASS, alloc->addr_user);
  
  /* Update usage. */
  wd_usage_add(-alloc->size_user);
  
  wd_report("Radar: Del %p", alloc->addr_user);
  
  /* Clear radar entry. */
  wd_radar_clear(alloc);
}

/*
Clear an entry in the radar.
*/
void wd_radar_clear(wd_alloc *alloc)
{
  *alloc = (wd_alloc){
    .point = (wd_point){ NULL, 0 },
    .size_user = 0,
    .addr_user = NULL,
    .dependent = false,
    .is_protected = false,
    .snapshot = NULL,
    .padding_check_left = false,
    .padding_check_right = false,
    .is_native = false,
    .original = NULL
  };
}

/*
Locate an address on the radar.
*/
wd_alloc *wd_radar_search(char *addr_user)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(addr_user != NULL);
  
  for (size_t i=0; i<wd_radar_size; i++)
    if (wd_radar[i].addr_user == addr_user)
      return wd_radar+i;
  return NULL;
}

/*
Retrieve the real size.
*/
size_t wd_radar_size_real_get(wd_alloc *alloc)
{
  assert(alloc != NULL);
  return alloc->size_user+2*WD_PADDING_SIZE;
}

/*
Retrieve the real address.
*/
char *wd_radar_addr_real_get(wd_alloc *alloc)
{
  assert(alloc != NULL);
  if (alloc->is_protected)
    return alloc->addr_user-WD_PADDING_SIZE;
  else
    return alloc->addr_user;
}

/*
Update the virtual address.
*/
void wd_radar_addr_user_set(wd_alloc *alloc, char *migrated_real)
{
  assert(alloc != NULL);
  assert(migrated_real != NULL);
  if (alloc->is_protected)
    alloc->addr_user = migrated_real+WD_PADDING_SIZE;
  else
    alloc->addr_user = migrated_real;
}

/*
Unlock an allocation for reallocation.
*/
void wd_radar_unlock(wd_alloc *alloc)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(alloc != NULL);
  assert(alloc->addr_user != NULL);
  assert(!alloc->dependent);
  
  if (alloc->is_protected)
    /* We do not clear the left padding to avoid potentially loosing evidence. */
    wd_padding_clear_right(alloc);
}

/*
Lock an allocation after reallocation.
*/
void wd_radar_lock(WD_STD_PARAMS, wd_alloc *alloc, size_t resize_user, char *migrated_real)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_radar != NULL);
  assert(alloc != NULL);
  assert(alloc->addr_user != NULL);
  assert(migrated_real != NULL);
  assert(!alloc->dependent);
  
  /* Unprotected memory can now be protected. */
  bool previously_unprotected = false;
  if (!alloc->is_protected) {
    previously_unprotected = true;
    
    /* Enable padding. */
    char *migrated_real_padded = malloc(resize_user+WD_SIZE_REAL_MARKUP);
    WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(migrated_real_padded, resize_user, WD_SIZE_REAL_MARKUP);
    memcpy(WD_ADDR_REAL_MAKE_USER(migrated_real_padded), migrated_real, resize_user);
    free(migrated_real);
    migrated_real = migrated_real_padded;
    
    /* Enable snapshots. */
    wd_snapshot_alloc(alloc);
    
    alloc->is_protected = true;
    alloc->padding_check_left = true;
    alloc->padding_check_right = true;
  }
  
  /* Update entry. */
  wd_radar_addr_user_set(alloc, migrated_real);
  int growth = resize_user-alloc->size_user;
  alloc->size_user = resize_user;
  
  /* Record old address in dangling pointer record if the new address is different. */
  if (migrated_real != wd_radar_addr_real_get(alloc))
    wd_dangling_record(WD_STD_PARAMS_PASS, wd_radar_addr_real_get(alloc));
  
  /* Remove new address from dangling pointer record if previously recorded. */
  wd_dangling_pointer *pointer = wd_dangling_search(alloc->addr_user);
  if (pointer != NULL)
    wd_dangling_clear(pointer);
  
  /* Re-add padding. */
  if (alloc->is_protected) {
    if (previously_unprotected)
      wd_padding_write_left(alloc);
    wd_padding_write_right(alloc);
  }
  
  /* Randomize newly added memory. */
  if (growth > 0)
    wd_fill_with_random_bytes(alloc->addr_user+alloc->size_user-growth, growth);
  
  /* Update snapshot. */
  if (alloc->snapshot != NULL) {
    wd_snapshot_realloc(alloc);
    wd_snapshot_capture(alloc);
  }
  
  /* Update original. */
  if (alloc->is_native)
    wd_usage_original_update(alloc, growth);
  
  /* Update usage. */
  wd_usage_add(growth);
  
  wd_report("Radar: Mod %p", alloc->addr_user);
}

/*
Locate the allocation containing the provided address.
*/
wd_alloc *wd_radar_locate(char *address)
{
  assert(address != NULL);
  for (size_t i=0; i<wd_radar_size; i++) {
    if (wd_radar[i].dependent && wd_radar[i].size_user == 0)
      continue;
    if (address < wd_radar[i].addr_user)
      continue;
    if (address >= wd_radar[i].addr_user+wd_radar[i].size_user)
      continue;
    return wd_radar+i;
  }
  return NULL;
}
