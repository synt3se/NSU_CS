#include <stdio.h>

int* return_local_address() {
    int local = 13;
    int* local_p = &local;
    return local_p;
}

int main(void) {
    int* local_address = return_local_address();
    printf("Address of a local variable out of function: %p\n", (void*)local_address);
    printf("Variable value out of function: %d\n", *local_address);
    return 0;
}
