/*
bounds.c - watchdog
Modified 2021-12-04
*/

/* Header-specific includes. */
#include "bounds.h"

/* Implementation-specific includes. */
#include "dogshed.h"
#include "radar.h"
#include "reporter.h"
#include "public.h"
#include "dangling.h"

/*
*** Bounds interface.
*/

/*
Check bounds on an array.
*/
int wd_bounds_check(WD_STD_PARAMS, char *array, size_t array_size, size_t item_size, int index)
{
  /* Assert that this function runs in the right circumstances. */
  if (array == NULL) {
    wd_alerts++;
    fail_at(file, line, WD_MSG_INCOMING_NULL);
  }
  assert(array_size != 0);
  assert(item_size != 0);
  
  /* Check state. */
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  wd_dangling_pointer *pointer = wd_dangling_find(WD_STD_PARAMS_PASS, array);
  if (pointer != NULL) {
    wd_alerts++;
    warn_at(file, line, WD_MSG_INCOMING_DANGLING, pointer->freed_at.file, pointer->freed_at.line);
  }
  
  /* Get array properties. */
  bool is_stack_array = false;
  wd_alloc *alloc = wd_radar_search(array);
  if (alloc == NULL)
    is_stack_array = true;
    // FIXME: OR the incoming pointer is wrong
    //        OR it points to an offset into an array.
    //        I don't know of any ways to detect these cases...
  size_t items_count;
  if (is_stack_array)
    items_count = array_size/item_size;
  else
    items_count = alloc->size/item_size;
  
  /* Fail if index out of bounds. */
  if (index < 0 || (size_t)index >= items_count) { // FIXME: Allow indices below?
    wd_alerts++;
    fail_at(__FILE__, __LINE__, WD_MSG_OUT_OF_BOUNDS, index, items_count);
  }
  
  return 1; /* See watchdog.h. */
}
