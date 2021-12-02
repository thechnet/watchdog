/*
tracks.c - watchdog
Modified 2021-12-02
*/

/* Header-specific includes. */
#include "tracks.h"

/*
Tracks globals.
*/

wd_point wd_track;

/*
*** Tracks interface.
*/

/*
Reset tracks.
*/
void wd_tracks_reset(void)
{
  wd_track = (wd_point){
    .file = NULL,
    .line = 0
  };
}

/*
Place tracks.
*/
void wd_tracks_update(WD_STD_PARAMS)
{
  wd_track = (wd_point){
    .file = file,
    .line = line
  };
}
