#include <stdio.h>
#include <stdlib.h>

void environment_test() {
    if (setenv("NEW_VAR", "initial", 1) != 0) {
        perror("Error setenv");
        exit(EXIT_FAILURE);
    }
    printf("NEW_VAR = %s\n", getenv("NEW_VAR"));
    if (setenv("NEW_VAR", "changed", 1) != 0) {
        perror("Error setenv changing");
        exit(EXIT_FAILURE);
    }
    printf("NEW_VAR changed = %s\n", getenv("NEW_VAR"));
}

int main(void) {
    environment_test();
    return 0;
}
