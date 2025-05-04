#pragma once

#include "utils.h"

int copy_reverse_dir(const char* src_dir, const char* dest_dir);
int create_directory(const char* path, const char* src);
int get_realpath(const char* relative_path, char* full_path);
int get_full_dest_dir_path(const char *src_rel_path, const char *dest_rel_path, char *out_full_dest);

