#ifndef _MISSION
#define _MISSOIN

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <list.h>

typedef struct _mission mission_t;

struct _mission
{
    char *name;
    int success;
    int difficulty;
};

mission_t *create_mission(char *name, int difficulty);

void press_buttons(int difficulty);
void solve_equation(int difficulty);
void type_dictation(int difficulty); 
void flash_sensor();
void tag_card();
void exe_mission(mission_t *mission);

#endif