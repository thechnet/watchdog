/*
archive.c - watchdog
Modified 2022-06-08
*/

/* Header-specific includes. */
#include "archive.h"

/* Implementation-specific includes. */
#include "reporter.h"

/*
*** Archive globals.
*/

wd_archived *wd_archive = NULL;
size_t wd_archive_size = 0;

/*
*** Archive static helpers.
*/

/*
Find the next free spot in the archive, or grow it.
*/
static wd_archived *wd_archive_find_or_create_slot(void)
{
  wd_archived *slot = NULL;
  for (size_t i=0; i<wd_archive_size; i++)
    if (wd_archive[i].addr_user == NULL) {
      slot = wd_archive+i;
      break;
    }
  
  /* Grow the archive if no free spots are left. */
  if (slot == NULL) {
    size_t size_old = wd_archive_size;
    
    wd_archive_size *= 2;
    wd_archive = realloc(wd_archive, wd_archive_size*sizeof(*wd_archive));
    WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_archive, 0, wd_archive_size*sizeof(*wd_archive));
    
    /* Initialize the newly added spots. */
    for (size_t i=size_old; i<wd_archive_size; i++)
      wd_archive_erase(wd_archive+i);
    
    slot = wd_archive+size_old;
  }
  
  return slot;
}

/*
*** Archive interface.
*/

/*
Initialize the archive.
*/
void wd_archive_initialize(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_archive == NULL);
  assert(wd_archive_size == 0);
  
  wd_archive_size = WD_ARCHIVE_SIZE;
  wd_archive = malloc(sizeof(*wd_archive)*wd_archive_size);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_archive, 0, sizeof(*wd_archive)*wd_archive_size);
  
  /* Clear spots. */
  for (size_t i=0; i<wd_archive_size; i++)
    wd_archive_erase(wd_archive+i);
}

/*
Terminate the archive.
*/
void wd_archive_terminate(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_archive != NULL);
  assert(wd_archive_size != 0);
  
  free(wd_archive);
  wd_archive = NULL;
  wd_archive_size = 0;
}

/*
Record an address in the archive.
*/
void wd_archive_record(WD_STD_PARAMS, char *addr_user)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_archive != NULL);
  
  wd_archived *slot = wd_archive_find_or_create_slot();
  *slot = (wd_archived){
    .addr_user = addr_user,
    .freed_at = (wd_point){
      .file = file,
      .line = line
    }
  };
}

/*
Erase an address from the archive.
*/
void wd_archive_erase(wd_archived *slot)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_archive != NULL);
  
  *slot = (wd_archived){
    .addr_user = NULL,
    .freed_at = (wd_point){
      .file = NULL,
      .line = 0
    }
  };
}

/*
Search for an address in the archive.
*/
wd_archived *wd_archive_query(char *addr_user)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_archive != NULL);
  assert(addr_user != NULL);
  
  for (size_t i=0; i<wd_archive_size; i++)
    if (wd_archive[i].addr_user == addr_user)
      return wd_archive+i;
  return NULL;
}
