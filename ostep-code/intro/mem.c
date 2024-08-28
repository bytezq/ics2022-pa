#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/**
 * 该实验需要linux关闭地址空间随机化
 * ubuntu2204 执行 `echo 0 > /proc/sys/kernel/randomize_va_space`
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: mem <value>\n");
        exit(1);
    }
    int *p;
    p = malloc(sizeof(int));
    assert(p != NULL);
    printf("(%d) addr pointed to by p: %p\n", (int)getpid(), p);
    *p = atoi(argv[1]); // assign value to addr stored in p
    while (1)
    {
        Spin(1);
        *p = *p + 1;
        printf("(%d) value of p: %d\n", getpid(), *p);
    }
    return 0;
}
