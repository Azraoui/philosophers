#include "./philo/header/philosophers.h"

typedef struct test
{
    int i;
    struct test *next;
}           t_test;

int main()
{
    t_test *test;
    t_test  *tmp;
    t_test  *swp;

    test = NULL;
    for (int i = 0; i < 500; i++)
    {
        tmp = (t_test *)malloc(sizeof(t_test));
        tmp->i = i;
        tmp->next = test;
        test = tmp;
        free(tmp);
    }

    printf("%d, ", test->next->i);
    // while (test)
    // {
    //     printf("%d, ", test->i);
    //     test = test->next;
    // }
    // while (test)
    // {
    //     swp = test->next;
    //     free(test);
    //     test = swp;
    // }
    // system("leaks a.out");
    return (0);
}