#include "dir_process.h"
#include "file_process.h"
#include "utils.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int build_path(const char* left_part, const char* right_part, char* out_path) {
    int len = snprintf(out_path, PATH_MAX, "%s/%s", left_part, right_part);
    if (len < 0 || len >= PATH_MAX) {
        fprintf(stderr, "Path construction error for: %s/%s\n", left_part, right_part);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int init_entry_paths(const char* src, const char* dest, const char* entry_name,
                        char* entry_src_path, char* entry_dest_path) {
    int return_code = build_path(src, entry_name, entry_src_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return_code = build_path(dest, entry_name, entry_dest_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    char* path_to_reverse = entry_dest_path + strlen(dest) + 1;
    reverse_string(path_to_reverse, strlen(path_to_reverse));
    return EXIT_SUCCESS;
}

int get_file_type(char* path, mode_t* file_type) {
    struct stat stat_buf;
    int return_code = lstat(path, &stat_buf);
    if (return_code == -1) {
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
    int return_code = stat(src, &src_stat);
    if (return_code == -1) {
        print_error("Error getting source directory info", src);
        return EXIT_FAILURE;
    }
    return_code = mkdir(path, src_stat.st_mode);
    if (return_code == -1) {
        print_error("Error creating directory", path);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

inline bool is_service_entry(const char* entry_name) {
    return strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0;
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

    struct dirent* curr_dir_entry = readdir(src_dir);
    while (curr_dir_entry != NULL) {
        if (is_service_entry(curr_dir_entry->d_name)) {
            curr_dir_entry = readdir(src_dir);
            continue;
        }
        return_code = process_entry(src_path, dest_path, curr_dir_entry->d_name);
        if (return_code == EXIT_FAILURE)
            break;
        curr_dir_entry = readdir(src_dir);
    }

    closedir(src_dir);
    return return_code;
}

int get_realpath(const char* relative_path, char* full_path) {
    char* resolved_path = realpath(relative_path, full_path);
    if (resolved_path == NULL) {
        print_error("Error resolving path", relative_path);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int get_dir_name(const char* path, char* dir_name) {
    path = strrchr(path, '/');
    if (dir_name == NULL) {
        fprintf(stderr, "Invalid path: %s\n", path);
        return EXIT_FAILURE;
    }
    path = path + 1; // shift from '/'
    memcpy(dir_name, path, strlen(path));
    return EXIT_SUCCESS;
}

int get_full_dest_dir_path(const char *src_path, const char *dest_relative_path, char *out_full_dest_path) {
    char reversed_src_dir_name[PATH_MAX];
    int return_code = get_dir_name(src_path, reversed_src_dir_name);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    size_t name_len = strlen(reversed_src_dir_name);
    reverse_string(reversed_src_dir_name, name_len);

    char real_dest_path[PATH_MAX];
    char* resolved_path = realpath(dest_relative_path, real_dest_path);
    if (resolved_path == NULL) {
        perror("Error resolving destination path");
        return EXIT_FAILURE;
    }
    return_code = build_path(real_dest_path, reversed_src_dir_name, out_full_dest_path);
    if (return_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
