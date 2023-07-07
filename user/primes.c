#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define WRITE_ENTRY 1
#define READ_ENTRY 0

void
generate_primes(int read_fd) {
    int prime;
    if (read(read_fd, &prime, sizeof(int)) == sizeof(int)) { // exit condition
        int p[2];
        pipe(p);

        printf("prime %d\n", prime);

        if (fork() == 0) {
            close(0);
            dup(p[READ_ENTRY]);
            close(p[READ_ENTRY]);
            close(p[WRITE_ENTRY]);
            generate_primes(READ_ENTRY);
            exit(0);
        } else {
            close(p[READ_ENTRY]);
            int number;
            while (read(read_fd, &number, sizeof(int)) == sizeof(int)) {
                if (number % prime != 0) {
                    write(p[WRITE_ENTRY], &number, sizeof(int));
                }
            }
            close(p[WRITE_ENTRY]);
        }
    }
}


int
main(void) {
    /*
    Write a program that uses pipe and fork to find the prime number.

    1. fork a child process to write the primes, parent write the number into the pipe.
    2. let the child process do the same thing like step 1
        2-1. print the first element as primes
        2-2. do the same like step 1, but filter by the child
    */

    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(0);
        dup(p[READ_ENTRY]);
        close(p[READ_ENTRY]);
        close(p[WRITE_ENTRY]);
        generate_primes(READ_ENTRY);
    } else {
        close(p[READ_ENTRY]);
        for (int i = 2; i <= 35; i++) {
            write(p[WRITE_ENTRY], &i, sizeof(int));
        }
        close(p[WRITE_ENTRY]);
    }
    exit(0);
}