#include "kernel/types.h"
#include "user/user.h"

//0是读段
//1是写端

//fd 0是标准输入
//fd 1是标准输出

int main(int argc, char* argv[]) {

    int pipe_1[2];
    int pipe_2[2];

    // 创建两个管道
    pipe(pipe_1);
    pipe(pipe_2);

    // 子进程
    if (fork() == 0) {
        close(pipe_1[1]);  // 关闭不需要的写端
        close(pipe_2[0]);  // 关闭不需要的读端

        int my_pid = getpid();
        char buffer[1];

        // 从管道1中读取数据
        read(pipe_1[0], buffer, 1);
        fprintf(1, "%d: received ping\n", my_pid);

        // 向管道2中写入数据
        write(pipe_2[1], "p", 1);

        close(pipe_1[0]);  // 关闭管道1的读端
        close(pipe_2[1]);  // 关闭管道2的写端
        exit(0);
    }

    // 父进程
    close(pipe_1[0]);  // 关闭不需要的读端
    close(pipe_2[1]);  // 关闭不需要的写端

    int my_pid = getpid();

    // 向管道1中写入数据
    write(pipe_1[1], "p", 1);

    char buffer[1];

    // 从管道2中读取数据
    read(pipe_2[0], buffer, 1);
    fprintf(1, "%d: received pong\n", my_pid);

    close(pipe_1[1]);  // 关闭管道1的写端
    close(pipe_2[0]);  // 关闭管道2的读端
    exit(0);
}
