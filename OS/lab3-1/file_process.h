#pragma once

#include "utils.h"

int copy_reverse_file(const char* src, const char* dest);
int init_files(const char* src, const char* dest, FILE** src_file, FILE** dest_file);
long get_file_size(FILE* src_file, const char* file_name);

