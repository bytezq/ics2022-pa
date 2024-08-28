#include <stdio.h>
#include <assert.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * 怎样得到 /proc 目录下的所有以数字为开头的目录
 */
int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        assert(argv[i]);
        printf("argv[%d] = %s \n", i, argv[i]);
    }
    assert(!argv[argc]); // 命令行参数数组argv的最后一个元素应该是NULL，这个断言用于检查是否符合这个约定

    // 读取/proc下的目录
    DIR *dp = opendir("/proc");
    return 0;
}