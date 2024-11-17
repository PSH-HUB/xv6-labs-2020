#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// 首先定义读取命令的函数
char *readline()
{
    // 先定义内存用malloc来分配
    char *buf = malloc(100);
    char *p = buf;
    // 开始读取数据
    while (read(0, p, 1) > 0)
    {
        if (*p == '\n' || *p == '\0')
        {
            // 注意这里不能直接返回，要替换到最后的字符，表明是最后一个字符
            *p = '\0';
            return buf;
        }
        p++;
    }
    // 防治循环没执行完的情况此时也需要返回数据
    if (p != buf)
    {
        return buf;
    }
    free(buf);
    return 0;
}

int main(int argc, char *argv[])
{

    // 首先如何处理输入的命令
    if (argc < 2)
    {
        printf("command erroe\n");
        exit(-1);
    }
    // 读取输入
    char *l;           // 用来接收readline的输入
    //注意这里的很关键不能漏掉
    argv++;

    //要跳过第一个参数；
    char *nargv[16];   // 用来复制参数
    char **pna = argv; // 用来指定当前参数位置；
    char **pa = nargv;
    // 首先循环把所有输入都进去
    while (*pna != 0)
    {
        *pa = *pna;
        pa++;
        pna++;
    }
    while ((l = readline()) != 0)
    {
        char *p = l;            // 用来接收l读到的所有参数；
        char *buf = malloc(36); // 为每个参数开辟内存空间
        char *bh = buf;         // 指针指向当前内存空间的起始位置
        int nargc = argc - 1;   // 注意这里nargc相当于复制了argc作为新的参数个数
        while (*p != 0)         // 参数不为空时
        {
            if (*p == ' ' && buf != bh) // 参数遇到空格且当前内存空间有东西时
            {
                // 有参数然后传入第一个参数
                // 首先要把指针给指回去
                *bh = 0;
                nargv[nargc] = buf; // 传入参数；
                buf = malloc(36);
                //注意这里的
                bh = buf;
                //重新分配完后要指向内存要指向当前的位置；
                nargc++;
            }
            else // 如果不是呢那就是一个字符一个字符复制过去就行
            {
                *bh = *p;
                bh++;
            }
            p++;
        } // 可能会有最后一个参数的情况但是
        if (buf != bh)
        {
            nargv[nargc] = buf;
            nargc++;
        }
        free(l);
        nargv[nargc] = 0;
        int pid = fork();
        if (pid == 0)
        {
            //exec(nargv, pna);
            //注意exec的使用，第一个是命令，后面是参数数组
            exec(nargv[0], nargv);
        }
        else
        {
            wait(0);
        }
        
    }
    exit(0);
}