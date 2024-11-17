#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// sleep 10就是暂停到10之后
// 首先先获取参数，具体实现后面再加

int
main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2,"请输入参数 ");
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        write(1, argv[i], strlen(argv[1]));
        if (i + 1 < argc)
        {
            write(1, " ", 1);
        }
        else
        {
            write(1, "\n", 1);
        }
    }
    int s = atoi(argv[1]);
    sleep(s);
    exit(0);
}