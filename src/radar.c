/*
radar.c - watchdog
Modified 2021-11-30
*/

#include "radar.h"
#include "reporter.h"

#define WD_RADAR_SIZE ((size_t)4096)
#define WD_RADAR_EMPTY_SPOT NULL

wd_alloc *wd_radar = NULL;
size_t wd_radar_size = 0;

/*
Start the radar.
*/
void wd_radar_start(WD_STD_PARAMS)
{
  out("radar:start");
  assert(wd_radar == NULL);
  wd_radar_size = WD_RADAR_SIZE;
  wd_radar = malloc(wd_radar_size*sizeof(*wd_radar));
  if (wd_radar == NULL) {
    wd_alerts++;
    fail_at(WD_STD_PARAMS_PASS, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", wd_radar_size);
  }
}

/*
Stop the radar.
*/
void wd_radar_stop(WD_STD_PARAMS)
{
  assert(wd_radar != NULL);
  free(wd_radar);
  wd_radar = NULL;
  wd_radar_size = 0;
  out("radar:stop");
}

/*
Locate an address on the radar.
*/
wd_alloc *wd_radar_locate(WD_STD_PARAMS, char *memory)
{
  wd_alloc *alloc;
  for (alloc=wd_radar; alloc < wd_radar+wd_radar_size; alloc++)
    if (alloc->memory == memory)
      break;
  if (alloc >= wd_radar+wd_radar_size) {
    wd_radar_grow(WD_STD_PARAMS_PASS);
    alloc = wd_radar+wd_radar_size;
  }
  return alloc;
}

/*
Double the radar size.
*/
void wd_radar_grow(WD_STD_PARAMS)
{
  out("radar:grow");
  wd_radar_size *= 2;
  wd_radar = realloc(wd_radar, wd_radar_size*sizeof(*wd_radar));
  if (wd_radar == NULL) {
    wd_alerts++;
    fail_at(file, line, WD_MSG_OUT_OF_MEMORY " (malloc %zu b)", wd_radar_size);
  }
}

/*
Start tracking an address on the radar.
*/
wd_alloc *wd_radar_track(WD_STD_PARAMS, char *memory, size_t memory_size)
{
  out("radar:track %p", memory);
  wd_alloc *alloc = wd_radar_locate(WD_STD_PARAMS_PASS, WD_RADAR_EMPTY_SPOT);
  assert(alloc != NULL);
  alloc->memory = memory;
  alloc->memory_size = memory_size;
  alloc->origin = (wd_point){ WD_STD_PARAMS_PASS };
  return alloc;
}

/*
Stop tracking an address on the radar.
*/
bool wd_radar_drop(WD_STD_PARAMS, char *memory)
{
  out("radar:drop %p", memory);
  wd_alloc *alloc = wd_radar_locate(WD_STD_PARAMS_PASS, memory);
  if (alloc == NULL)
    return false;
  alloc->memory = NULL;
  return true;
}
