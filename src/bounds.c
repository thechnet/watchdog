/*
bounds.c - watchdog
Modified 2021-12-09
*/

/* Header-specific includes. */
#include "bounds.h"

/* Implementation-specific includes. */
#include "dogshed.h"
#include "radar.h"
#include "reporter.h"
#include "public.h"
#include "dangling.h"
#include "overrides.h"

/*
*** Bounds interface.
*/

/*
Check bounds on an array.
*/
int wd_bounds_check(WD_STD_PARAMS, char *array, size_t array_size, size_t item_size, int index)
{
  /* Assert that this function runs in the right circumstances. */
  WD_FAIL_IF_PTR_NULL(array);
  assert(array_size != 0);
  assert(item_size != 0);
  
  /* Check state. */
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_DANGLING(array);
  
  /* Get array properties. */
  
  /* FIXME: We can currently not confidently decide WHAT an incoming pointer is if we have not recorded it on the radar. Apart from a stack array, it could also be an invalid pointer or an offset into a valid piece of memory. I don't know (yet) if there is something I can do about these.
  
  On a related note, if we decide to just ignore these other possibilities, a radar search yielding NULL is the only condition for determining whether an incoming pointer is a stack array, meaning we can remove the is_stack_array variable.
  */
  
  bool is_stack_array = false;
  wd_alloc *alloc = wd_radar_search(array);
  if (alloc == NULL)
    is_stack_array = true;
  size_t items_count;
  if (is_stack_array)
    items_count = array_size/item_size;
  else
    items_count = alloc->size_user/item_size;
  
  /* Fail if index out of bounds. */
  if (index < 0 || (size_t)index >= items_count) { // FIXME: Allow indices below 0?
    wd_alerts++;
    fail_at(__FILE__, __LINE__, WD_MSG_OUT_OF_BOUNDS, index, items_count);
  }
  
  return 1; /* See watchdog.h. */
}
