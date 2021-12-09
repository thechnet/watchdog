/*
overrides.c - watchdog
Modified 2021-12-09
*/

/* Header-specific includes. */
#include "overrides.h"

/* Implementation-specific includes. */
#include <memory.h>
#include <string.h>
#include "dogshed.h"
#include "radar.h"
#include "reporter.h"
#include "public.h"
#include "padding.h"
#include "dangling.h"
#include "snapshots.h"
#include "usage.h"

/*
*** Overrides.
*/

/*
Override malloc.
*/
void *wd_override_malloc(WD_STD_PARAMS, ptrdiff_t size_user)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_SIZE_LEQ_0(size_user);
  
  /* Allocate memory. */
  size_t size_real = WD_SIZE_USER_MAKE_REAL(size_user);
  char *addr_real = malloc(size_real);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(addr_real, size_user, WD_SIZE_REAL_MARKUP);
  
  /* Add new allocation to radar. */
  wd_alloc *alloc = wd_radar_catch(WD_STD_PARAMS_PASS, addr_real, size_user, true, true, false, true);
  
  return (void*)alloc->addr_user;
}

/*
Override realloc.
*/
void *wd_override_realloc(WD_STD_PARAMS, char *addr_user, ptrdiff_t resize_user)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_NULL(addr_user);
  WD_WARN_IF_PTR_DANGLING(addr_user);
  WD_WARN_IF_SIZE_LEQ_0(resize_user);
  wd_alloc *alloc = wd_radar_search(addr_user);
  
  /* If the incoming address is untracked, attempt to locate its surrounding allocation. */
  if (alloc == NULL) {
    wd_alloc *surrounding = wd_radar_locate(addr_user);
    WD_FAIL_IF_RADAR_FINDS_PTR_ENCLOSED(surrounding, "");
  }
  /* Otherwise, warn the user... */
  WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(alloc, "");
  /* ... and start tracking the new address. */
  if (alloc == NULL)
    alloc = wd_radar_catch(WD_STD_PARAMS_PASS, addr_user, 0, false, false, false, false);
  
  WD_WARN_IF_SIZE_REDUCED(alloc->size_user, resize_user);
  
  /* Prepare allocation for reallocation. */
  wd_radar_unlock(alloc);
  
  /* Reallocate memory. */
  char *addr_real = wd_radar_addr_real_get(alloc);
  size_t size_real = wd_radar_size_real_get(alloc);
  size_t resize_real = size_real+(resize_user-alloc->size_user);
  char *migrated_real = realloc(addr_real, resize_real);
  
  /* Verify outgoing values. */
  WD_FAIL_IF_OUT_OF_MEMORY(migrated_real, resize_user, resize_real-resize_user);
  
  /* Lock allocation. */
  wd_radar_lock(WD_STD_PARAMS_PASS, alloc, resize_user, migrated_real);
  
  return (void*)alloc->addr_user;
}

/*
Override free.
*/
void wd_override_free(WD_STD_PARAMS, char *addr_user)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_WARN_IF_PTR_NULL(addr_user);
  WD_WARN_IF_PTR_DANGLING(addr_user);
  wd_alloc *alloc = wd_radar_search(addr_user);
  /* If the incoming address is untracked, attempt to locate its surrounding allocation. */
  if (alloc == NULL) {
    wd_alloc *surrounding = wd_radar_locate(addr_user);
    WD_FAIL_IF_RADAR_FINDS_PTR_ENCLOSED(surrounding, "");
  }
  /* Otherwise, warn the user. */
  WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(alloc, "");
  
  /* Determine real address. */
  char *addr_real;
  if (alloc != NULL)
    addr_real = wd_radar_addr_real_get(alloc);
  else
    addr_real = addr_user;
  
  /* Drop allocation if tracked. */
  if (alloc != NULL)
    wd_radar_release(WD_STD_PARAMS_PASS, alloc);
  
  /* Free memory. */
  free(addr_real);
}

/*
Override assert.
*/
void wd_override_assert(WD_STD_PARAMS, char *assertion_string, int assertion)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  if (assertion)
    return;
  fail_at(file, line, WD_MSG_ASSERT, assertion_string);
}

/*
Override strdup.
*/
char *wd_override_strdup(WD_STD_PARAMS, char *src_user)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_FAIL_IF_PTR_NULL(src_user);
  WD_WARN_IF_PTR_DANGLING(src_user);
  
  /* Check if incoming memory recorded. */
  wd_alloc *src_alloc = wd_radar_search(src_user);
  /* If the incoming address is untracked, attempt to locate its surrounding allocation. */
  if (src_alloc == NULL) {
    wd_alloc *surrounding = wd_radar_locate(src_user);
    if (surrounding == NULL)
      WD_INFO_IF_RADAR_FINDS_PTR_UNTRACKED(src_alloc, "");
  }
  
  /* Get string length. */
  size_t size_user = (strlen(src_user)+1)*sizeof(*src_user);
  size_t size_real = size_user+WD_SIZE_REAL_MARKUP*sizeof(*src_user);
  
  /* Copy string. */
  char *addr_real = malloc(size_real);
  WD_FAIL_IF_OUT_OF_MEMORY(addr_real, size_user, size_real-size_user);
  char *addr_user = WD_ADDR_REAL_MAKE_USER(addr_real);
  strcpy(addr_user, src_user);
  
  /* Record on radar. */
  wd_alloc *alloc = wd_radar_catch(WD_STD_PARAMS_PASS, addr_real, size_user, true, true, false, false);
  
  return alloc->addr_user;
}

/*
Override strcpy.
*/
char *wd_override_strcpy(WD_STD_PARAMS, char *dest_user, char *src_user)
{
  WD_ENSURE_UNLEASHED();
  wd_bark(WD_STD_PARAMS_PASS);
  
  /* Verify incoming values. */
  WD_FAIL_IF_PTR_NULL(dest_user);
  WD_WARN_IF_PTR_DANGLING(dest_user);
  WD_FAIL_IF_PTR_NULL(src_user);
  WD_WARN_IF_PTR_DANGLING(src_user);
  
  /* Check if incoming memory recorded. */
  wd_alloc *dest_alloc = wd_radar_search(dest_user);
  WD_INFO_IF_RADAR_FINDS_PTR_UNTRACKED(dest_alloc, " <dest>");
  wd_alloc *src_alloc = wd_radar_search(src_user);
  WD_INFO_IF_RADAR_FINDS_PTR_UNTRACKED(src_alloc, " <src>");
  
  /* Compare string lengths. */
  size_t dest_len;
  if (dest_alloc != NULL && !dest_alloc->dependent)
    dest_len = dest_alloc->size_user/sizeof(*dest_user)-1 /* NUL. */;
  else
    dest_len = strlen(dest_user); /* Estimation. */
  size_t src_len;
  if (src_alloc != NULL && !src_alloc->dependent)
    src_len = src_alloc->size_user/sizeof(*src_user)-1 /* NUL. */;
  else
    src_len = strlen(src_user); /* Estimation. */
  if (src_len > dest_len) {
    wd_alerts++;
    info_at(file, line, "Source (~%zu) may be too big for destination (~%zu).", src_len, dest_len);
  }
  
  /* Copy string. */
  char *strcpy_return = strcpy(dest_user, src_user);
  
  /* Check if destination string recorded. */
  if (dest_alloc != NULL && dest_alloc->is_protected) {
    wd_padding_check(WD_STD_PARAMS_PASS, dest_alloc);
    wd_snapshot_capture(dest_alloc);
  }
  
  return strcpy_return;
}
