#ifndef _MUSIC
#define _MUSIC
#include <list.h>
list_t *music_list;
void init_music();
void play_music(char *music);
void stop_music();
#endif