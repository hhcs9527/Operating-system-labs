#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

/*
[stuck] => how to capture the input
1. Use fork and exec to invoke the command on each line of input. Use wait in the parent to wait for the child to complete the command.
2. To read individual lines of input, read a character at a time until a newline ('\n') appears.
    kernel/param.h declares MAXARG, which may be useful if you need to declare an argv array.
3. Add the program to UPROGS in Makefile.
4. Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.

find . b | xargs grep hello => grep hello in find . b
command must be the first element in the command.
*/

/*
    The readline() function in the code you provided reads a line of input from the standard input (stdin).
    It reads characters one by one until it encounters a newline character ('\n') or a null character ('\0'),
    indicating the end of a line.
*/
char* readline() {
    char* buf = malloc(MAXARG);
    char* pos = buf;
    while (read(0, pos, sizeof(char)) != 0) {
        if ((*pos == '\n') || (*pos == '\0')) {
            // *pos to '\0' ensures that the line read by readline()
            *pos = '\0';
            return buf;
        }
        pos++;
    }
    if (buf != pos) {
        return buf;
    }
    free(buf);
    return 0;
}


int
main(int argc, char* argv[]) {
    // not enough command
    if (argc < 2) {
        fprintf(2, "Usage: xargs [command]\n");
        exit(1);
    }

    char* command;
    char* temp;
    int pid;

    command = argv[1];
    /*
        echo 'a/nb' | xarg mkdir
        => xarg mkdir => argc = 2
        => mkdir a => we need extra one to call it.
        => mkdir b
    */
    char *temp_argv[MAXARG];
    for (int i = 0; i < argc - 1; i++) {
        temp_argv[i] = argv[i+1];
    }

    // somehow read previous argument
    while ((temp = readline()) != 0) {
        pid = fork();
        if (pid == 0) {
            temp_argv[argc-1] = temp;

            // exec with the command and the output of the input
            // why does we need the command
            exec(command, temp_argv);
        } else {
            wait(0);
        }
        free(temp);
    }

    exit(0);
}