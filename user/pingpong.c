#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    int p1[2]; // 父进程
    int p2[2]; // 子进程
    // 创建管道
    if (pipe(p1) < 0)
    {
        printf("pipe");
        exit(-1);
    }
    if (pipe(p2) < 0)
    {
        printf("pipe");
        exit(-1);
    }

    // 创建子进程
    int pid = fork();
    if (pid == 0)
    { // child
        char buffer[2];
        read(p1[0], buffer, 2);
        printf("%d : receive ping\n", getpid());
        write(p2[1], "q", 2);
        //printf("%d : receive ping\n", getpid());

        /* printf 的输出是缓冲的，尤其是在标准输出中。
        如果在 write 之后没有立即刷新输出（例如，程序
        因为其他原因退出），那么输出可能不会被及时显示。
        使用 fflush(stdout); 可以强制刷新缓冲区。 */
    }
    else
    {
        // 父进程中应该先写后读,同时读写的话会堵塞。
        write(p1[1], "w", 2);
        char buffer[2];
        read(p2[0], buffer, 2);
        printf("%d : receive pong\n", getpid());
    }
    close(p1[0]);
    close(p1[1]);
    close(p2[1]);
    close(p2[0]);
    exit(0);
    // 死锁，管道两端是不一样的。
}
