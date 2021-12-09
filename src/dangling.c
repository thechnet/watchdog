/*
dangling.c - watchdog
Modified 2021-12-09
*/

/* Header-specific includes. */
#include "dangling.h"

/* Implementation-specific includes. */
#include "reporter.h"

/*
*** Dangling globals.
*/

wd_dangling_pointer *wd_dangling = NULL;
size_t wd_dangling_size = 0;

/*
*** Dangling interface.
*/

/*
Open the dangling pointer record.
*/
void wd_dangling_open(void)
{
  assert(wd_dangling == NULL);
  assert(wd_dangling_size == 0);
  wd_dangling_size = WD_DANGLING_SIZE;
  wd_dangling = malloc(sizeof(*wd_dangling)*wd_dangling_size);
  WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_dangling, 0, sizeof(*wd_dangling)*wd_dangling_size);
  
  /* Clear spots. */
  for (size_t i=0; i<wd_dangling_size; i++)
    wd_dangling_clear(wd_dangling+i);
}

/*
Close the dangling pointer record.
*/
void wd_dangling_close(void)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_dangling != NULL);
  assert(wd_dangling_size != 0);
  
  free(wd_dangling);
  wd_dangling = NULL;
  wd_dangling_size = 0;
}

/*
Search for an address in the dangling pointer record.
*/
wd_dangling_pointer *wd_dangling_search(char *addr_user)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_dangling != NULL);
  assert(addr_user != NULL);
  
  for (size_t i=0; i<wd_dangling_size; i++)
    if (wd_dangling[i].addr_user == addr_user)
      return wd_dangling+i;
  return NULL;
}

/*
Find the next free spot in the dangling pointer record, or grow it.
*/
wd_dangling_pointer *wd_dangling_find_spot(void)
{
  wd_dangling_pointer *pointer = NULL;
  for (size_t i=0; i<wd_dangling_size; i++)
    if (wd_dangling[i].addr_user == NULL) {
      pointer = wd_dangling+i;
      break;
    }
  
  /* Grow the radar if no free spots are left. */
  if (pointer == NULL) {
    size_t size_old = wd_dangling_size;
    
    wd_dangling_size *= 2;
    wd_dangling = realloc(wd_dangling, wd_dangling_size*sizeof(*wd_dangling));
    WD_FAIL_IF_OUT_OF_MEMORY_INTERNAL(wd_dangling, 0, wd_dangling_size*sizeof(*wd_dangling));
    
    /* Initialize the newly added spots. */
    for (size_t i=size_old; i<wd_dangling_size; i++)
      wd_dangling_clear(wd_dangling+i);
    
    pointer = wd_dangling+size_old;
  }
  
  return pointer;
}

/*
Record an address as dangling pointer.
*/
void wd_dangling_record(WD_STD_PARAMS, char *addr_user)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_dangling != NULL);
  
  wd_dangling_pointer *pointer = wd_dangling_find_spot();
  *pointer = (wd_dangling_pointer){
    .addr_user = addr_user,
    .freed_at = (wd_point){
      .file = file,
      .line = line
    }
  };
}

/*
Erase an address from the record of dangling pointers.
*/
void wd_dangling_clear(wd_dangling_pointer *pointer)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_dangling != NULL);
  
  *pointer = (wd_dangling_pointer){
    .addr_user = NULL,
    .freed_at = (wd_point){
      .file = NULL,
      .line = 0
    }
  };
}
