#include <stdio.h>
#include <unistd.h>

int g_initialized = 4;
int g_uninitialized;
const int g_const = 6;

void print_addresses() {
    int local_var = 1;
    static int static_local = 2;
    const int const_local = 3;

    printf("Address of a local variable: %p\n", (void*)&local_var);
    printf("Address of a static variable: %p\n", (void*)&static_local);
    printf("Address of a constant in a function: %p\n", (void*)&const_local);

    printf("Address of the global initialized variable (g_initialized): %p\n", (void*)&g_initialized);
    printf("Address of the global uninitialized variable (g_uninitialized): %p\n", (void*)&g_uninitialized);
    printf("Address of the global constant (g_const): %p\n", (void*)&g_const);
}

int main(void) {
    print_addresses();
    printf("\nPID: %d\n", getpid());
    printf("Write any character to continue: ");
    getchar();
    return 0;
}
