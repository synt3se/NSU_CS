#include "dir_process.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Use: %s <src_dir>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* src_rel_path = argv[1];
    char dest_dir_path[PATH_MAX];

    int return_code = get_root_dest_dir(src_rel_path, dest_dir_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return_code = copy_reverse_dir(src_rel_path, dest_dir_path);
    return return_code;
}
