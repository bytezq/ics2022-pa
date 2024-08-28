#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /**
     * O_WRONLY 只读方式打开
     * O_CREAT 如果不存在则创建
     * O_TRUNC 如果文件存在，并且以可写的方式打开, 那么文件长度截断为0(清空文件内容)
     * S_IRUSR | S_IWUSR 文件所有者具有读权限和写权限
     */
    int fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(fd >= 0);
    char buffer[20];                  // 存储要写入文件的字符串
    sprintf(buffer, "hello world\n"); // 将字符串写入buffer中
    int rc = write(fd, buffer, strlen(buffer));
    assert(rc == (strlen(buffer))); // 检查写入的字节数与buffer长度是否一致
    fsync(fd);                      // 所有数据刷新到磁盘
    close(fd);                      // 关闭文件, 释放文件描述符fd
    return 0;
}