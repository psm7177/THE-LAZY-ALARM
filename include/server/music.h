#ifndef _MUSIC
#define _MUSIC
#include <list.h>
#include <stdint.h>
list_t *music_list;
void init_music();
void get_music_path(uint32_t idx, char *path);
void play_music(char *music);
void stop_music();
#endif