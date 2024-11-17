#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void match(const char *path, const char *name)
{
    // printf("%s %s", path, name);
    int pp = 0;
    int pa = 0;
    while (path[pp] != 0)
    {
        pa = 0;
        int np = pp;
        while (name[pa] != 0)
        {
            if (name[pa++] == path[np++])
            {
                // pa++;
                // np++;
            }
            else
                break;
        }
        if (name[pa] == 0)
        {
            printf("%s\n", path);
            return;
        }
        pp++;
    }
}
void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    // 打开指定路径的文件或目录
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls, cannot open %s\n", path);
        return;
    }

    // 获取文件或目录的状态信息
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls : cannot stat %s\n", path);
        close(fd);
        return;
    }

    // 根据文件类型处理不同情况
    switch (st.type)
    {
    case T_FILE:
        //printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
        match(path, name);
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls : path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
            {
                continue;
            }
            
            if (de.name[0] == '.' && de.name[1] == 0)
            {
                continue;
            }
            if (de.name[0] == '.' && de.name[1] == '.' && de.name[2] == 0)
            {
                continue;
            }
            
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (stat(buf, &st) < 0)
            {
                printf("ls : cannot stat %s\n");
                continue;
            }

            //printf("f%s %d %d %d\n", match(buf), st.type, st.ino, st.size);
            find(buf, name);
        }
        break;
    }
    close(fd);
}
int main(int argc, char *argv[])
{
    // 注意这里的argc的意义，argv[0]：程序名。
    // argv[1]：用户提供的第一个参数（在此示例中是路径）
    // argv[2]：用户提供的第二个参数（在此示例中是文件名）。
    if (argc < 3)
    {
        printf("find  error \n");
        exit(-1);
    }
    find(argv[1], argv[2]);
    exit(0);
}