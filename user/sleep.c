#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(2, "usage: sleep need time...\n");
        exit(1);
    }

    for(int i=1; i<argc; i++)
        if (atoi(argv[i]) < 0) {
            fprintf(2, "usage: sleep time must >= 0...\n");
            exit(1);
        } else {
            sleep(atoi(argv[i]));
        }

    exit(0);
}