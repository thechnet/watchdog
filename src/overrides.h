/*
overrides.h - watchdog
Modified 2021-12-09
*/

#ifndef WD_OVERRIDES_H
#define WD_OVERRIDES_H

/* Header-specific includes. */
#include <stdlib.h>
#include <stdint.h>

/*
*** Overrides helpers. FIXME: Where do we put these? They are also needed in bounds.c.
*/

#define WD_WARN_IF_SIZE_0(size)\
  {\
    if (size == 0) {\
      wd_alerts++;\
      warn_at(file, line, WD_MSG_SIZE_0);\
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
      fail_at(WD_STD_PARAMS_PASS, WD_MSG_OUT_OF_MEMORY, alloc_size, size_markup);\
    }\
  }

#define WD_WARN_IF_PTR_DANGLING(ptr)\
  {\
    wd_dangling_pointer *_ptr = wd_dangling_search(ptr);\
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
  }\

#define WD_FAIL_IF_RADAR_FINDS_PTR_ENCLOSED(radar_locate__response, id)\
  {\
    if (radar_locate__response != NULL) {\
      wd_alerts++;\
      fail_at(file, line, WD_MSG_INCOMING_ENCLOSED, id);\
    }\
  }\

#define WD_INFO_IF_RADAR_FINDS_PTR_UNTRACKED(radar_search__response, id)\
  {\
    if (radar_search__response == NULL) {\
      wd_alerts++;\
      info_at(file, line, WD_MSG_INCOMING_UNTRACKED, id);\
    }\
  }\

/*
*** Overrides.
*/

void *wd_override_malloc(char *file, size_t line, size_t size);
// char *wd_override_calloc(char *file, size_t line, size_t count, size_t size);
void *wd_override_realloc(char *file, size_t line, char *memory_virtual, size_t size_virtual_new);
void wd_override_free(char *file, size_t line, char *memory);
void wd_override_assert(char *file, size_t line, char *assertion_string, int assertion);
char *wd_override_strcpy(char *file, size_t line, char *dest_user, char *src_user);
char *wd_override_strdup(char *file, size_t line, char *src_user);

#endif /* !WD_OVERRIDES_H */
