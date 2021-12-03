/*
dangling.c - watchdog
Modified 2021-12-02
*/

/* Header-specific includes. */
#include "dangling.h"

/*
*** Dangling globals.
*/

void **wd_dangling = NULL;
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
  wd_dangling_size = WD_DANGLING_SIZE;
  wd_dangling = malloc(sizeof(*wd_dangling)*wd_dangling_size);
  if (wd_dangling == NULL) {
    wd_alerts++;
    fail_at(WD_STD_ARGS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", sizeof(*wd_dangling)*wd_dangling_size);
  }
}

/*
Close the dangling pointer record.
*/
void wd_dangling_close(void)
{
  assert(wd_dangling != NULL);
  wd_dangling_size = 0;
  free(wd_dangling);
}

/*
Double the dangling pointer record size.
*/
void wd_dangling_grow(WD_STD_PARAMS)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_dangling != NULL);
  assert(wd_dangling_size > 0);
  
  wd_dangling_size *= 2;
  wd_dangling = realloc(wd_dangling, wd_dangling_size*sizeof(*wd_dangling));
  if (wd_dangling == NULL) {
    wd_alerts++;
    fail_at(file, line, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", wd_dangling_size*sizeof(*wd_dangling));
  }
}

/*
Find an address in the dangling pointer record.
*/
void **wd_dangling_find(void *address)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_unleashed);
  assert(wd_dangling != NULL);
  
  void **pointer;
  for (pointer=wd_dangling; pointer<wd_dangling+wd_dangling_size; pointer++)
    if (*pointer == address)
      break;
  
  assert(pointer <= wd_dangling+wd_dangling_size);
  if (pointer == wd_dangling+wd_dangling_size) {
    if (address == WD_DANGLING_EMPTY_SPOT) {
      wd_dangling_grow(WD_STD_PARAMS_PASS);
      pointer = wd_dangling+wd_dangling_size/2;
    } else {
      return NULL;
    }
  }
  return pointer;
}

/*
Record an address as dangling pointer.
*/
void wd_dangling_record(void *address)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_dangling != NULL);
  
  void **pointer = wd_dangling_find(WD_DANGLING_EMPTY_SPOT);
  *pointer = address;
}

/*
Erase an address from the record of dangling pointers.
*/
void wd_dangling_erase(void **pointer)
{
  /* Assert that this function runs in the right circumstances. */
  assert(wd_dangling != NULL);
  
  *pointer = NULL;
}
