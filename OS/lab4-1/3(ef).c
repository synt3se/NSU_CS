#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void heap_test() {
    char *buf1 = (char*)malloc(100);
    if (buf1 == NULL) {
        perror("Memory allocation error for buf1");
        exit(EXIT_FAILURE);
    }
    strcpy(buf1, "hello world");
    printf("buf1 (before free): %s\n", buf1);
    free(buf1);
    printf("buf1 (after free): %s\n", buf1);

    char *buf2 = (char*)malloc(100);
    if (buf2 == NULL) {
        perror("Memory allocation error for buf2");
        exit(EXIT_FAILURE);
    }
    strcpy(buf2, "hello world");

    printf("buf2 (before pointer shift): %s\n", buf2);
    char *mid_ptr = buf2 + 50;
    free(mid_ptr);
    printf("buf2 (after incorrect free): %s\n", buf2);
}

int main(void) {
    heap_test();
    return 0;
}
