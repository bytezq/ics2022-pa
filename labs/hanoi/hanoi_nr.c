#include <stdio.h>
#include <assert.h>

// 理解栈帧, 栈帧保存的是当前函数的状态和执行行
// 定义一个结构体 Frame 用于表示一个栈帧
// pc很重要, 指向当前程序执行的位置
typedef struct
{
    int pc, n;          // pc表示下一条指令的执行位置，n 表示汉诺塔的层数
    char from, to, via; // from、to 和 via 分别表示起始柱、目标柱和辅助柱
} Frame;

// https://blog.csdn.net/Apollon_krj/article/details/82911180
// https://cloud.tencent.com/developer/article/2168335

// 函数调用, 入栈操作, 将一个新的 Frame 压入栈顶，并初始化 pc 为 0，__VA_ARGS__ 表示可变参数
#define call(...) ({ *(++top) = (Frame){.pc = 0, __VA_ARGS__}; })
// 函数返回, 出栈操作, ret 宏用于模拟函数的返回操作，通过将栈顶指针 top 减一来实现
#define ret() ({ top--; })
// goto 宏用于将当前的 pc 设置为指定的位置，从而实现类似于跳转的效果
#define goto(loc) ({ f->pc = (loc) - 1; })

void hanoi(int n, char from, char to, char via)
{
    Frame stk[64], *top = stk - 1; // 声明一个大小为 64 的 Frame 栈，并将 top 初始化为栈底前一个位置
    call(n, from, to, via);        // 执行函数调用, pc=0, top加1, 保存入参
    for (Frame *f; (f = top) >= stk; f->pc++)
    {
        switch (f->pc)
        {
        // case 0: 如果 n == 1，意味着只需将一个盘子从 from 移动到 to，直接打印移动步骤，并跳转到 case 4 进行返回操作。
        case 0:
            if (f->n == 1)
            {
                printf("%c -> %c\n", f->from, f->to);
                goto(4);
            }
            break;
        // case 1: 递归地将 n-1 个盘子从 from 移动到 via，此时 to 是辅助柱。
        case 1:
            call(f->n - 1, f->from, f->via, f->to);
            break;
        // case 2: 移动当前最大的盘子（即只剩下的一个盘子）从 from 到 to。
        case 2:
            call(1, f->from, f->to, f->via);
            break;
        // case 3: 递归地将 via 上的 n-1 个盘子从 via 移动到 to，此时 from 是辅助柱。
        case 3:
            call(f->n - 1, f->via, f->to, f->from);
            break;
        // case 4: 这是 goto(4) 后的返回操作，通过 ret() 将当前 Frame 从栈中弹出。
        case 4:
            ret();
            break;
        // default: assert(0); 用于保证 pc 的值永远不会超过 4，否则程序会异常终止。
        default:
            assert(0);
        }
    }
}

int main()
{
    hanoi(2, 'A', 'B', 'C');
}