#include "dir_process.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Use: %s <src_dir> <dest_dir>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* src_rel_path = argv[1];
    const char* dest_rel_path = argv[2];
    char full_dest_path[PATH_MAX];
    char full_src_path[PATH_MAX];

    int return_code = get_realpath(src_rel_path, full_src_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return_code = get_full_dest_dir_path(full_src_path, dest_rel_path, full_dest_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return_code = copy_reverse_dir(src_rel_path, full_dest_path);
    return return_code;
}
