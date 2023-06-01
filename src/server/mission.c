#include <mission.h>

mission_t *create_mission(char *name, int difficulty)
{
    mission_t *mission = malloc(sizeof(mission_t));

    mission->name = name;
    mission->difficulty = difficulty;
    mission->success = 0;
    return mission;
}

// missions
void press_buttons(int difficulty)
{

    printf("Mission: Press enter for n times and type 'done' command\n\n");
    printf("Caution! You should press correct number of times and type 'done' command if you are done\n-------------------------------------\n");

    char *answer;
    int count;
    int trial = 1;
    int a;

repeat:
    count = 0;
    a = 1 + difficulty * 10 + rand() % 10;
    printf("\nPress enter for %i times\n\n", a);

    while (1)
    {
        scanf("%s", answer);
        if (strcmp(answer, "") == 0)
            count++;
        else if (strcmp(answer, "done") == 0)
            break;
        else
        {
            printf("\n\nInput error. try again.\n");
            trial++;
            goto repeat;
        }
    }

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

    char *response;
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
            scanf("your answer: %s", response);
        }
        else if (difficulty == 1)
        {
            answer = "9";
            printf("Given equation: x^2 + 6x + 9 = 144\n");
            printf("What is x? (x >= -3)\n\n");
            scanf("your answer: %s", response);
        }
        else if (difficulty == 2)
        {
            answer = "xln(x) + 6x";
            printf("Given equation: f(x) = ln(x) + 7\n");
            printf("Assume that F(x) = integral(f(x)), what is eqution of F(x)? (in this case, F(0) = 0)\n\n");
            scanf("your answer: %s", response);
        }

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
            scanf("your answer: %s", response);
        }
        else if (difficulty == 1)
        {
            answer = "Hello world! nice to meet you.";
            printf("Given sentence: %s\n", answer);
            scanf("your answer: %s", response);
        }
        else if (difficulty == 2)
        {
            answer = "Hello world! My name is Siheon. I'm glad to meet you! Please type this sentence correctly.";
            printf("Given sentence: %s\n", answer);
            scanf("your answer: %s", response);
        }

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

void exe_mission(mission_t *mission)
{

    if (strcmp(mission->name, "press buttons") == 0)
        press_buttons(mission->difficulty);
    else if (strcmp(mission->name, "solve equation") == 0)
        solve_equation(mission->difficulty);
    else if (strcmp(mission->name, "type dictation") == 0)
        type_dictation(mission->difficulty);
    else if (strcmp(mission->name, "flash sensor") == 0)
        flash_sensor(mission->difficulty);
    else if (strcmp(mission->name, "tag card") == 0)
        tag_card(mission->difficulty);

    mission->success = 1;
}