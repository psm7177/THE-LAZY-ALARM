#include <mission.h>
#include <string.h>
#include <list.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <pigpio.h>

mission_func_t mission_arr[3] = {press_buttons, type_dictation, solve_equation};
int button_wait;

void init_mission()
{
    if (gpioInitialise() < 0)
    {
        printf("failed\n");
    } else {
        gpioSetMode(21, PI_INPUT);
        gpioSetPullUpDown(21, PI_PUD_DOWN);
    }
}

void button_sig_handler() {
    button_wait = 0;
}

void reverse(char *first, char *last);
void itoa(int val, char *str, int base);

void press_GPIO(int difficulty) {

	gpioSetMode(21, PI_INPUT);
    printf("sucess\n");

    printf("Mission: Press button for n times and wait for 5 seconds\n\n");
    printf("Caution! You should press correct number of times and type 'done' command if you are done\n-------------------------------------\n");
    int count;
    int trial = 1;
    int a;
    srand(time(NULL));

    int prev = 0;

repeat:
    count = 0;
    a = 1 + difficulty * 10 + rand() % 10;
    printf("\nPress button for %i times\n\n", a);
    signal(SIGALRM, button_sig_handler);
    alarm(5);

    button_wait = 1;

    while (button_wait)
    {
        int curr = gpioRead(21);
        if (prev == 0 && curr == 1) {
            count++;
            printf("press\n");
            alarm(0);
            alarm(5);
        }
        usleep(300);
        prev = curr;
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
// missions
void press_buttons(int difficulty)
{
    printf("Mission: Press enter for n times and type 'done' command\n\n");
    printf("Caution! You should press correct number of times and type 'done' command if you are done\n-------------------------------------\n");

    char answer[16];
    int count;
    int trial = 1;
    int a;
    srand(time(NULL));

    memset(answer, 0, sizeof(answer));
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

    char response[16];
    char answer[16];
    int count = 0;
    memset(response, 0, sizeof(response));

    // rand() 함수로 equation을 다르게 바꾸기
    int c1, c2;

    while (1)
    {
        c1 = 1 + rand()%9;
        srand(time(NULL));
        c2 = 1 + rand()%9;
        count++;
        if (difficulty == 0)
        {
            sprintf(answer, "%i\n", c1 + c2);
            printf("%s\n",answer);
            printf("Given equation: x - %i = %i\n", c1, c2);
            printf("What is x?\n\n");
            fgets(response, sizeof(response),stdin);
        }
        else if (difficulty == 1)
        {
            sprintf(answer, "%i\n", c1 - 3);
            printf("Given equation: x^2 + 6x + 9 = %i\n", (int) pow(c1, 2));
            printf("What is x? (x >= -3)\n\n");
            fgets(response, sizeof(response),stdin);
        }
        else if (difficulty == 2)
        {
            strcpy(answer, "xln(x) + 6x\n");
            printf("Given equation: f(x) = ln(x) + 7\n");
            printf("Assume that F(x) = integral(f(x)), what is eqution of F(x)? (in this case, F(0) = 0)\n\n");
            fgets(response, sizeof(response),stdin);
        }

        printf("your answer: %s", response);
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
            answer = "hello world\n";
            printf("Given sentence: %s\n", answer);
            fgets(response, sizeof(response),stdin);
        }
        else if (difficulty == 1)
        {
            answer = "Hello world! nice to meet you.\n";
            printf("Given sentence: %s\n", answer);
            fgets(response, sizeof(response),stdin);
        }
        else if (difficulty == 2)
        {
            answer = "Hello world! My name is Siheon. I'm glad to meet you! Please type this sentence correctly.\n";
            printf("Given sentence: %s\n", answer);
            fgets(response, sizeof(response),stdin);
        }
        printf("your answer: %s\n", response);
        if (strcmp(answer, response) == 0)
        {
            break;
        }

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