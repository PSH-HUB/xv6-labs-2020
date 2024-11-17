#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void prime(int rd)
{
    int n;
    read(rd, &n, 4); // 这里是因为一开始就是2，2就是筛选素数的基准
    /* 2晒出了不是2的倍数，接下来依次递归，所以这就是为什么管道和进程要在循环里面创建，因为要一直递归产生管道和进程，下一个基准就是3 */
    printf("prime %d\n", n);
    int flag = 0; // 管道创建的标志
    int p[2];
    // 接下来的操作才是开始筛选素数
    // 首先还是读值操作，先从描述符中读取值；
    int num;
    while (read(rd, &num, 4) != 0)
    {
        /* 看一下这个循环，它开始不停的读数，这里有个点，前面的read读了2那么这里还读不读，答案是不读，因为读取数据是连续的。
        所以从这里往后是2之后的所有数据，注意这里的flag因为后面是一直递归的，我们要看什么时候启用管道，当有第二个数的时候就该
        了，然后开始新的进程，注意这里父子进程，这里的父子进程父进程其实不负责东西，子进程负责处理数据。这里两个if第一个是为了创建
        管道进程进行递归用的也就是3后面的数字，然后 */
        if (flag == 0)
        {

            // 顺序读数，然后开始递归函数
            // 这是第二次read所以读到的应该是3这里已经有两个数了创建管道
            pipe(p);
            flag = 1;
            int pid = fork();
            if (pid == 0)
            {
                // 这里说明是子进程，负责读值操作关闭写端然后递归调用prime读取读端的值
                close(p[1]);
                prime(p[0]);
                return;//负责结束当前函数运行。
            }
            else
            {
                close(p[0]);
            }
        }
        // 这里开始进行判断，
        if (num % n != 0)
        {
            write(p[1], &num, 4);
        }
    }
    close(rd);
    close(p[1]);
    wait(0);
}
int main(int argc, char *argv[])
{
    //要深刻理解管道是为了两个进程也就是父子进程通信使用的，
    //一个读那么另一个就负责写，在这个进程中，父进程用于向子进程输送数字所以占写端
    /* 相应的子进程就占用读断，开始的时候不用的描述符需要关掉避免占用资源，其次这个是边读边写的 */
    int num[2];
    pipe(num);
    int pid = fork();
    if (pid == 0)
    {
        close(num[1]);
        prime(num[0]);
        close(num[0]);
    }
    else
    {
        close(num[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(num[1], &i, 4);
        }
        close(num[1]);
        wait(0);
    }
    exit(0);
}