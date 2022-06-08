/*
overrides.h - watchdog
Modified 2022-06-08
*/

#ifndef WD_OVERRIDES_H
#define WD_OVERRIDES_H

/* Header-specific includes. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*
*** Overrides helpers. FIXME: Where do we put these? They are also needed in bounds.c.
*/

#define WD_WARN_IF_SIZE_LEQ_0(size)\
  do {\
    if (size <= 0) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_SIZE_LEQ_0);\
    }\
  } while(0)

#define WD_WARN_IF_SIZE_REDUCED(size_before, size_now)\
  do {\
    if (size_now <= size_before) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_SIZE_NOT_BIGGER);\
    }\
  } while(0)
  
#define WD_WARN_IF_PTR_NULL(ptr)\
  do {\
    if (ptr == NULL) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_INCOMING_NULL);\
    }\
  } while(0)

#define WD_FAIL_IF_PTR_NULL(ptr)\
  do {\
    if (ptr == NULL) {\
      wd_alerts++;\
      fail_at(file, line, WD_MSG_INCOMING_NULL);\
    }\
  } while(0)

#define WD_FAIL_IF_OUT_OF_MEMORY(allocator_return_value, alloc_size, size_markup)\
  do {\
    if (allocator_return_value == NULL) {\
      wd_alerts++;\
      fail_at(file, line, WD_MSG_OUT_OF_MEMORY, alloc_size, size_markup);\
    }\
  } while(0)

#define WD_WARN_IF_PTR_DANGLING(ptr)\
  do {\
    wd_archived *_ptr = wd_archive_query(ptr);\
    if (_ptr != NULL) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_INCOMING_DANGLING, _ptr->freed_at.file, _ptr->freed_at.line);\
    }\
  } while(0)

#define WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(radar_search__response, id)\
  do {\
    if (radar_search__response == NULL) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_INCOMING_UNTRACKED, id);\
    }\
  } while(0)

#define WD_FAIL_IF_RADAR_FINDS_PTR_ENCLOSED(radar_orientate__response, id)\
  do {\
    if (radar_orientate__response == WD_RADAR_ORIENTATE_ENCLOSED) {\
      wd_alerts++;\
      fail_at(file, line, WD_MSG_INCOMING_ENCLOSED, id);\
    }\
  } while(0)

#define WD_INFO_IF_RADAR_FINDS_PTR_UNTRACKED(radar_search__response, id)\
  do {\
    if (radar_search__response == NULL) {\
      wd_alerts++;\
      info_at(file, line, WD_MSG_INCOMING_UNTRACKED, id);\
    }\
  } while(0)

/*
*** Overrides.
*/

void *wd_override_malloc(char *file, long line, ptrdiff_t size);
void *wd_override_realloc(char *file, long line, char *addr_user, ptrdiff_t resize_user);
void wd_override_free(char *file, long line, char *memory);
void wd_override_assert(char *file, long line, char *assertion_string, int assertion);
char *wd_override_strcpy(char *file, long line, char *dest_user, char *src_user);
char *wd_override_strdup(char *file, long line, char *src_user);

#endif /* !WD_OVERRIDES_H */
