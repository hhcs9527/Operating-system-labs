#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void) {
    /*
    1. Use pipe to create a pipe.
    2. Use fork to create a child.
    3. Use read to read from a pipe, and write to write to a pipe.
    4. Use getpid to find the process ID of the calling process.
    */
    int p[2];
    pipe(p);
    int childID;

    if (fork() == 0) {
        close(p[0]);
        int pid = getpid();
        printf("%d: received ping\n", pid);
        write(p[1], &pid, sizeof(int));
        close(p[1]);
    } else {
        close(p[1]);
        read(p[0], &childID, sizeof(int));
        int pid = getpid();
        printf("%d: received pong\n", pid);
        close(p[0]);
    }
    exit(0);
}