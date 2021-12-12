/*
overrides.h - watchdog
Modified 2021-12-12
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
  {\
    if (size <= 0) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_SIZE_LEQ_0);\
    }\
  }

#define WD_WARN_IF_SIZE_REDUCED(size_before, size_now)\
  {\
    if (size_now <= size_before) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_SIZE_NOT_BIGGER);\
    }\
  }
  
#define WD_WARN_IF_PTR_NULL(ptr)\
  {\
    if (ptr == NULL) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_INCOMING_NULL);\
    }\
  }

#define WD_FAIL_IF_PTR_NULL(ptr)\
  {\
    if (ptr == NULL) {\
      wd_alerts++;\
      fail_at(file, line, WD_MSG_INCOMING_NULL);\
    }\
  }

#define WD_FAIL_IF_OUT_OF_MEMORY(allocator_return_value, alloc_size, size_markup)\
  {\
    if (allocator_return_value == NULL) {\
      wd_alerts++;\
      fail_at(file, line, WD_MSG_OUT_OF_MEMORY, alloc_size, size_markup);\
    }\
  }

#define WD_WARN_IF_PTR_DANGLING(ptr)\
  {\
    wd_archived *_ptr = wd_archive_query(ptr);\
    if (_ptr != NULL) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_INCOMING_DANGLING, _ptr->freed_at.file, _ptr->freed_at.line);\
    }\
  }

#define WD_WARN_IF_RADAR_FINDS_PTR_UNTRACKED(radar_search__response, id)\
  {\
    if (radar_search__response == NULL) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_INCOMING_UNTRACKED, id);\
    }\
  }

#define WD_FAIL_IF_RADAR_FINDS_PTR_ENCLOSED(radar_locate__response, id)\
  {\
    if (radar_locate__response == WD_RADAR_ORIENTATE_ENCLOSED) {\
      wd_alerts++;\
      fail_at(file, line, WD_MSG_INCOMING_ENCLOSED, id);\
    }\
  }

#define WD_INFO_IF_RADAR_FINDS_PTR_UNTRACKED(radar_search__response, id)\
  {\
    if (radar_search__response == NULL) {\
      wd_alerts++;\
      info_at(file, line, WD_MSG_INCOMING_UNTRACKED, id);\
    }\
  }

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
