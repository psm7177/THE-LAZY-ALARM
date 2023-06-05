#include <stdlib.h>
#include <stdio.h>

void reverse(char *first, char *last) {
    char tmp;
    while (last > first)
    {
        tmp = *last;
        *last --= *first;
        *first ++= tmp;
    }
}

void itoa(int val, char *str, int base) {
    static char idx[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *wstr = str;
    int sign;
    div_t res;

    if (base < 2 || base > 35) {
        *wstr = '\0';
        return;
    }
    if (sign = val < 0) val = -val;

    do
    {
        res = div(val, base);
        *wstr ++= idx[res.rem];
    } while (val = res.quot);
    if (sign)
    {
        *wstr ++= '-';
        *wstr = '\0';
    }
    reverse(str, wstr - 1);
}