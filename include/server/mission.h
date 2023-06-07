#ifndef _MISSION
#define _MISSOIN

#include <stdlib.h>
#include <stdio.h>
#include <list.h>

typedef void (*mission_func_t)(int difficulty);
list_t *mission_list;

void press_GPIO(int difficulty);
void init_mission();
void press_buttons(int difficulty);
void solve_equation(int difficulty);
void type_dictation(int difficulty);
void flash_sensor();
void tag_card();
void exe_mission(int difficulty);

#endif