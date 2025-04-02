#include "file_process.h"
#include "utils.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int read_chunk(FILE* src_file, char* buffer, size_t chunk, size_t* total_read, const char* filename) {
    while (*total_read < chunk) {
        size_t read = fread(buffer + *total_read, 1, chunk - *total_read, src_file);
        if (ferror(src_file)) {
            print_error("Error reading file", filename);
            return EXIT_FAILURE;
        }
        *total_read += read;
    }
    return EXIT_SUCCESS;
}

int write_chunk(FILE* dest_file, const char* buffer, size_t chunk, size_t* total_written, const char* filename) {
    while(*total_written < chunk) {
        size_t written = fwrite(buffer + *total_written, 1, chunk - *total_written, dest_file);
        if (ferror(dest_file)) {
            print_error("Error writing to file", filename);
            return EXIT_FAILURE;
        }
        *total_written += written;
    }
    return EXIT_SUCCESS;
}

int process_chunk(FILE* src_file, FILE* dest_file, size_t offset, size_t chunk,
    const char* src_name, const char* dest_name) {
    if (fseek(src_file, (long)offset, SEEK_SET) != 0) {
        print_error("Error seeking in file", src_name);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    size_t total_read = 0;
    int status = read_chunk(src_file, buffer, chunk, &total_read, src_name);
    if (status != EXIT_SUCCESS) {
        return status;
    }

    reverse_string(buffer, total_read);

    size_t total_written = 0;
    status = write_chunk(dest_file, buffer, chunk, &total_written, dest_name);
    if (status != EXIT_SUCCESS) {
        return status;
    }
    if (total_written != total_read) {
        fprintf(stderr, "Write size mismatch for: %s (read: %zu, wrote: %zu)\n",
                dest_name, total_read, total_written);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int init_files(const char* src, const char* dest, FILE** src_file, FILE** dest_file) {
    *src_file = fopen(src, "rb");
    if (*src_file == NULL) {
        print_error("Failed to open source file", src);
        return EXIT_FAILURE;
    }
    struct stat src_stat;
    if (stat(src, &src_stat) != 0) {
        print_error("Error getting file info", src);
        safe_close(src_file);
        return EXIT_FAILURE;
    }
    mode_t mode = src_stat.st_mode;

    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (dest_fd < 0) {
        print_error("Failed to create destination file", dest);
        safe_close(src_file);
        return EXIT_FAILURE;
    }
    *dest_file = fdopen(dest_fd, "wb");
    if (*dest_file == NULL) {
        print_error("Failed to open destination file", dest);
        safe_close(src_file);
        close(dest_fd);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

long get_file_size(FILE* src_file, const char* file_name) {
    if (fseek(src_file, 0, SEEK_END) != 0) {
        print_error("Error seeking in file", file_name);
        return -1;
    }

    long file_size = ftell(src_file);
    if (file_size == -1) {
        print_error("Error getting file size", file_name);
        return -1;
    }
    return file_size;
}

int copy_reverse_file(const char* src, const char* dest) {
    FILE* src_file = NULL;
    FILE* dest_file = NULL;
    int status = init_files(src, dest, &src_file, &dest_file);
    if (status != 0) {
        return status;
    }

    const long file_size = get_file_size(src_file, src);
    if (file_size == -1) {
        fclose(src_file);
        fclose(dest_file);
        return status;
    }

    size_t offset = (size_t)file_size;
    while (offset > 0) {
        size_t chunk = (offset > BUFFER_SIZE) ? BUFFER_SIZE : offset;
        offset -= chunk;
        status = process_chunk(src_file, dest_file, offset, chunk, src, dest);
        if (status != 0) {
            fclose(src_file);
            fclose(dest_file);
            return status;
        }
    }
    fclose(src_file);
    fclose(dest_file);
    return status;
}
