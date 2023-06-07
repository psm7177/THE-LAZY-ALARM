#include <shuffle.h>

int rand_comparison(const void *a, const void *b)
{
    (void)a;
    (void)b;

    return rand() % 2 ? +1 : -1;
}

void shuffle(void *base, size_t nmemb, size_t size)
{
    srand(time(NULL));
    qsort(base, nmemb, size, rand_comparison);
}