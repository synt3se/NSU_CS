#include "dir_process.h"
#include "file_process.h"
#include "utils.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int init_entry_paths(const char* src, const char* dest, const char* entry_name, char* e_src_path, char* e_dest_path) {
    int len = snprintf(e_src_path, PATH_MAX, "%s/%s", src, entry_name);
    if (len < 0 || len >= PATH_MAX) {
        fprintf(stderr, "Path construction error for: %s/%s\n", src, entry_name);
        return EXIT_FAILURE;
    }
    len = snprintf(e_dest_path, PATH_MAX, "%s/%s", dest, entry_name);
    if (len < 0 || len >= PATH_MAX) {
        fprintf(stderr, "Path construction error for: %s/%s\n", dest, entry_name);
        return EXIT_FAILURE;
    }

    char* path_to_reverse = e_dest_path + strlen(dest) + 1;
    reverse_string(path_to_reverse, strlen(path_to_reverse));
    return EXIT_SUCCESS;
}

int get_file_type(char* path, mode_t* file_type) {
    struct stat stat_buf;
    if (lstat(path, &stat_buf) == -1) {
        print_error("Error getting file status", path);
        return EXIT_FAILURE;
    }
    *file_type = stat_buf.st_mode;
    return EXIT_SUCCESS;
}

int process_entry(const char* src_path, const char* dest_path, const char* entry_name) {
    char entry_src_path[PATH_MAX];
    char entry_dest_path[PATH_MAX];
    int return_code = init_entry_paths(src_path, dest_path, entry_name, entry_src_path, entry_dest_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    mode_t file_type;
    return_code = get_file_type(entry_src_path, &file_type);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (S_ISDIR(file_type)) {
        return_code = copy_reverse_dir(entry_src_path, entry_dest_path);
    }
    if (S_ISREG(file_type)) {
        return_code = copy_reverse_file(entry_src_path, entry_dest_path);
    }
    return return_code;
}

int create_directory(const char* path, const char* src) {
    struct stat src_stat;
    if (stat(src, &src_stat) == -1) {
        print_error("Error getting source directory info", src);
        return EXIT_FAILURE;
    }
    if (mkdir(path, src_stat.st_mode) == -1) {
        print_error("Error creating directory", path);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int copy_reverse_dir(const char* src_path, const char* dest_path) {
    int return_code = create_directory(dest_path, src_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    DIR* src_dir = opendir(src_path);
    if (src_dir == NULL) {
        print_error("Failed to open directory", src_path);
        return EXIT_FAILURE;
    }

    struct dirent* dir_entry = readdir(src_dir);
    while (dir_entry != NULL) {
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
            dir_entry = readdir(src_dir);
            continue;
        }
        return_code = process_entry(src_path, dest_path, dir_entry->d_name);
        if (return_code == EXIT_FAILURE)
            break;
        dir_entry = readdir(src_dir);
    }

    closedir(src_dir);
    return return_code;
}

int get_root_dest_dir(const char* relative_src_path, char* dest_path) {
    char absolute_src_path[PATH_MAX];
    char* resolved_path = realpath(relative_src_path, absolute_src_path);
    if (resolved_path == NULL) {
        print_error("Error resolving path", relative_src_path);
        return EXIT_FAILURE;
    }

    char* base_name = strrchr(resolved_path, '/');
    if (base_name == NULL) {
        fprintf(stderr, "Invalid path: %s\n", resolved_path);
        return EXIT_FAILURE;
    }

    size_t base_offset = base_name - resolved_path;
    strncpy(dest_path, resolved_path, PATH_MAX);

    char* path_to_reverse = dest_path + base_offset + 1;
    reverse_string(path_to_reverse, strlen(path_to_reverse));
    return EXIT_SUCCESS;
}
