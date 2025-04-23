#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

#define BUFFER_SIZE 4096
#define MAX_ERROR_MSG 256

void print_error(const char* context, const char* path);
void reverse_string(char* str, size_t len);

