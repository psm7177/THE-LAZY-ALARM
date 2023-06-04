#include <mission.h>
#include <string.h>
#include <list.h>
#include <time.h>
#include <stdlib.h>

mission_func_t mission_arr[3] = {press_buttons, type_dictation, solve_equation};

void init_mission_list()
{
}

// missions
void press_buttons(int difficulty)
{
    printf("Mission: Press enter for n times and type 'done' command\n\n");
    printf("Caution! You should press correct number of times and type 'done' command if you are done\n-------------------------------------\n");

    char answer[16];
    int count;
    int trial = 1;
    int a;

repeat:
    count = 0;
    a = 1 + difficulty * 10 + rand() % 10;
    printf("\nPress enter for %i times\n\n", a);

    while (1)
    {
        fgets(answer, 16, stdin);
        if (strcmp(answer, "\n") == 0)
        {
            count++;
        }
        else if (strcmp(answer, "done\n") == 0)
            break;
        else
        {
            printf("\n\nInput error. try again.\n");
            trial++;
            goto repeat;
        }
    }
    printf("press: %d times", count);
    if (count != a)
    {
        printf("\n\nYou pressed wrong number of times. try again.");
        trial++;
        goto repeat;
    }
    else
    {
        printf("\n\nCorrect!\n");
        printf("You succeeded in %i times\n\n", trial);
    }
}
void solve_equation(int difficulty)
{
    printf("Mission: Solve an equation\n\n");
    printf("Caution! You should use the correct format of math symbols\n-------------------------------------\n");

    char response[64];
    char *answer;
    int count = 0;

    // rand() 함수로 equation을 다르게 바꾸기

    while (1)
    {
        count++;
        if (difficulty == 0)
        {
            answer = "6";
            printf("Given equation: x - 4 = 2\n");
            printf("What is x?\n\n");
            scanf("%s", response);
        }
        else if (difficulty == 1)
        {
            answer = "9";
            printf("Given equation: x^2 + 6x + 9 = 144\n");
            printf("What is x? (x >= -3)\n\n");
            scanf("%s", response);
        }
        else if (difficulty == 2)
        {
            answer = "xln(x) + 6x";
            printf("Given equation: f(x) = ln(x) + 7\n");
            printf("Assume that F(x) = integral(f(x)), what is eqution of F(x)? (in this case, F(0) = 0)\n\n");
            scanf("%s", response);
        }

        printf("your answer: %s\n", response);
        if (strcmp(answer, response) == 0)
            break;

        printf("\n\nIncorrect answer. Try again.\n\n");
    }
    printf("\n\nCorrect!\n");
    printf("You succeeded in %i times\n\n", count);
}
void type_dictation(int difficulty)
{
    printf("Mission: Type the dictation\n\n");
    printf("Caution! You should type correct sentence\n-------------------------------------\n");

    char response[512];
    char *answer;
    int count = 0;

    while (1)
    {
        count++;
        if (difficulty == 0)
        {
            answer = "hello world";
            printf("Given sentence: %s\n", answer);
            scanf("%[^\n]", response);
        }
        else if (difficulty == 1)
        {
            answer = "Hello world! nice to meet you.";
            printf("Given sentence: %s\n", answer);
            scanf("%[^\n]", response);
        }
        else if (difficulty == 2)
        {
            answer = "Hello world! My name is Siheon. I'm glad to meet you! Please type this sentence correctly.";
            printf("Given sentence: %s\n", answer);
            scanf("%[^\n]", response);
        }
        printf("your answer: %s\n", response);
        if (strcmp(answer, response) == 0)
            break;

        printf("\n\nIncorrect senctence. Try again.\n\n");
    }
    printf("\n\nCorrect!\n");
    printf("You succeeded in %i times\n\n", count);
}
void flash_sensor()
{
    // TODO: implement
}
void tag_card()
{
    // TODO: implement
}

void exe_mission(int difficulty)
{
    srand(time(NULL));
    int r = rand() % 3;
    mission_arr[r](difficulty);
}