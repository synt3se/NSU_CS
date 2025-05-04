#include <stdio.h>
#include <stdlib.h>

int environment_test() {
    char* new_var_value = getenv("NEW_VAR");
    if (new_var_value == NULL) {
        perror("Error geting env variable");
        return EXIT_FAILURE;
    }
    printf("NEW_VAR = %s\n", new_var_value);

    int return_code = setenv("NEW_VAR", "changed", 1);
    if (return_code == -1) {
        perror("Error setenv changing");
        return EXIT_FAILURE;
    }
    new_var_value = getenv("NEW_VAR");
    if (new_var_value == NULL) {
        perror("Error geting env variable");
        return EXIT_FAILURE;
    }
    printf("NEW_VAR changed = %s\n", new_var_value);
    return EXIT_SUCCESS;
}

int main(void) {
    int return_code = environment_test();
    return return_code;
}
