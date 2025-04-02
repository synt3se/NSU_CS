#include "utils.h"
#include <string.h>
#include <unistd.h>

void print_error(const char* context, const char* path) {
    char error_msg[MAX_ERROR_MSG];
    snprintf(error_msg, MAX_ERROR_MSG, "%s: '%s'", context, path);
    perror(error_msg);
}

void safe_close(FILE** file) {
    if (file != NULL && *file != NULL) {
        fclose(*file);
        *file = NULL;
    }
}

void reverse_string(char* str, const size_t len) {
    if (str == NULL) {
        print_error("Null pointer in reverse_string", "");
        return;
    }

    for (size_t i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}
