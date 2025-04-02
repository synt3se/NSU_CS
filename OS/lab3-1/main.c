#include "dir_process.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Use: %s <src_dir>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* src_rel_path = argv[1];
    char dest_dir_path[PATH_MAX];

    int status = get_dest_dir(src_rel_path, dest_dir_path);
    if (status != EXIT_SUCCESS) {
        return status;
    }

    status = copy_reverse_dir(src_rel_path, dest_dir_path);
    return status;
}
