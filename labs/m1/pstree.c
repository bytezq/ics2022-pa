#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PROCESS_SIZE 65525

typedef struct Process
{
    int pid;
    int ppid;
    char name[256];
    struct Process* children;
    struct Process* sibling;
} Process;

Process* process_table[MAX_PROCESS_SIZE] = { 0 };

Process* create_process(int pid, int ppid)
{
    Process* process = malloc(sizeof(Process));
    process->pid = pid;
    process->ppid = ppid;
    process->children = NULL;
    process->sibling = NULL;
    return process;
}

void add_process(int pid, int ppid)
{
    Process* process = create_process(pid, ppid);
    process_table[pid] = process;

    if (ppid > 0 && process_table[ppid]) {
        Process* parent = process_table[ppid];
        process->sibling = parent->children;  // Insert as the first child
        parent->children = process;           // Update parent's children pointer
    }
}

void print_process_tree(int pid, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("|--%d\n", process->pid);
    // Recursively print all children processes
    print_process_tree(process->children, level + 1);

    // Recursively print sibling processes
    print_process_tree(process->sibling, level);

}

int starts_with_digit(const char* str)
{
    return isdigit(str[0]);
}

// Function to read process information from /proc/[pid]/stat
void read_process_info(int pid)
{
    char path[64];
    sprintf(path, "/proc/%d/status", pid);

    FILE* file = fopen(path, "r");
    if (!file)
    {
        return;
    }

    char line[256];
    int ppid = -1;
    char name[256] = { 0 };

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line, "Name:\t%s", name);
        }
        else if (strncmp(line, "PPid:", 5) == 0) {
            sscanf(line, "PPid:\t%d", &ppid);
            break;
        }
    }
    fclose(file);

    process_table[pid].pid = pid;
    process_table[pid].ppid = ppid;
    strncpy(process_table[pid].name, name, sizeof(process_table[pid].name) - 1);
}

int main(int argc, char* argv[])
{
    struct dirent* entry;

    for (int i = 0; i < argc; i++)
    {
        assert(argv[i]);
        printf("argv[%d] = %s \n", i, argv[i]);
    }
    assert(!argv[argc]); // 命令行参数数组argv的最后一个元素应该是NULL，这个断言用于检查是否符合这个约定


    DIR* dp = opendir("/proc");
    if (!dp) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dp)) != NULL) {
        int pid = atoi(entry->d_name);  // 这样能判断目录是数字吗?
        if (pid > 0) {
            read_process_info(pid);
        }
    }
    closedir(dp);


    print_process_tree(1, 0);

    // Free allocated memory
    for (int i = 0; i < MAX_PROCESS_SIZE; i++) {
        if (process_table[i]) {
            free(process_table[i]);
        }
    }

    return 0;
}