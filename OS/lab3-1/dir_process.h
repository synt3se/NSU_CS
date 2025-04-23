#pragma once

#include "utils.h"

int copy_reverse_dir(const char* src_dir, const char* dest_dir);
int create_directory(const char* path, const char* src);
int get_root_dest_dir(const char* relative_src_path, char* dest_path);

