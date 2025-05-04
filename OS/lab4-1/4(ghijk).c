#include <stdio.h>
#include <stdlib.h>

void environment_test() {
    int return_code = setenv("NEW_VAR", "initial", 1);
    if (return_code == -1) {
        perror("Error setenv");
        exit(EXIT_FAILURE);
    }
    printf("NEW_VAR = %s\n", getenv("NEW_VAR"));
    return_code = setenv("NEW_VAR", "changed", 1);
    if (return_code == -1) {
        perror("Error setenv changing");
        exit(EXIT_FAILURE);
    }
    printf("NEW_VAR changed = %s\n", getenv("NEW_VAR"));
}

int main(void) {
    environment_test();
    return 0;
}
