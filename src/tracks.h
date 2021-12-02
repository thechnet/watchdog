/*
tracks.h - watchdog
Modified 2021-12-02
*/

#ifndef WD_TRACKS_H
#define WD_TRACKS_H

/* Header-specific includes. */
#include "dogshed.h"

/*
*** Tracks globals.
*/

extern wd_point wd_track;

/*
*** Tracks interface.
*/

void wd_tracks_reset(void);
void wd_tracks_update(WD_STD_PARAMS);

#endif /* !WD_TRACKS_H */
