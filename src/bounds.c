/*
bounds.c - watchdog
Modified 2021-12-03
*/

/* Header-specific includes. */
#include "bounds.h"

/* Implementation-specific includes. */
#include "radar.h"
#include "reporter.h"

/*
*** Bounds interface.
*/

/*
Check bounds on an array.
*/
int wd_bounds_check(WD_STD_PARAMS, void *array, size_t array_size, size_t item_size, int index)
{
  /* Assert that this function runs in the right circumstances. */
  if (array == NULL) {
    wd_alerts++;
    fail_at(WD_STD_ARGS, WD_MSG_INCOMING_NULL);
  }
  assert(array_size != 0);
  assert(item_size != 0);
  
  bool is_stack_array = false;
  wd_alloc *alloc = wd_radar_find(WD_STD_PARAMS_PASS, array);
  if (alloc == NULL)
    is_stack_array = true;
    // FIXME: OR the incoming pointer is wrong OR it points to an offset into an array. I don't know of any ways to detect these cases...
  
  size_t items_count;
  if (is_stack_array)
    items_count = array_size/item_size;
  else
    items_count = alloc->size/item_size;
  
  if (index < 0 || (size_t)index >= items_count) { // FIXME:
    wd_alerts++;
    fail_at(WD_STD_ARGS, WD_MSG_OUT_OF_BOUNDS, index, items_count);
  }
  
  return &array[index*item_size];
}
