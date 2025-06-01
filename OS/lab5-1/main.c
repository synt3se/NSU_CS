#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ERROR (-1)
#define CHILD_PID 0

int g_var = 13;

void process_child(int* local_var) {
    pid_t pid = getpid();
    pid_t ppid = getppid();

    printf("  PID: %d, Parent PID: %d\n", pid, ppid);
    printf("  [Child] g_var - address: %p, value: %d\n", (void*)&g_var, g_var);
    printf("  [Child] local_var - address: %p, value: %d\n", (void*)&local_var, *local_var);
    getchar();

    g_var = 0;
    *local_var = 1;
    printf("  [Child] Modified g_var - address: %p, value: %d\n", (void*)&g_var, g_var);
    printf("  [Child] Modified local_var - address: %p, value: %d\n", (void*)&local_var, *local_var);
    getchar();
}

void process_parent(int* local_var) {
    printf("[Parent after fork] g_var - address: %p, value: %d\n", (void*)&g_var, g_var);
    printf("[Parent after fork] local_var - address: %p, value: %d\n", (void*)&local_var, *local_var);

    sleep(30);

    printf("[Parent after sleep] g_var - address: %p, value: %d\n", (void*)&g_var, g_var);
    printf("[Parent after sleep] local_var - address: %p, value: %d\n", (void*)&local_var, *local_var);

    int status;
    pid_t wait_pid = wait(&status);
    if (wait_pid == ERROR) {
        perror("wait");
        exit(EXIT_FAILURE);
    }
    printf("[Parent after wait] g_var - address: %p, value: %d\n", (void*)&g_var, g_var);
    printf("[Parent after wait] local_var - address: %p, value: %d\n", (void*)&local_var, *local_var);

    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        printf("Child exited normally, exit code = %d\n", exit_status);
        return;
    }
    if (WIFSIGNALED(status)) {
        int signal_status = WTERMSIG(status);
        printf("Child killed by signal %d\n", signal_status);
        return;
    }
    printf("Child terminated not normally\n");
}

int main(void) {
    int local_var = 31;
    pid_t pid = getpid();

    printf("Parent pid : %d\n", pid);
    printf("[Parent before fork] g_var - address: %p, value: %d\n", (void*)&g_var, g_var);
    printf("[Parent before fork] local_var - address: %p, value: %d\n", (void*)&local_var, local_var);

    pid_t fork_pid = fork();
    switch (fork_pid) {
        case ERROR:
            perror("fork");
            exit(EXIT_FAILURE);
        case CHILD_PID:
            process_child(&local_var);
            _exit(5);
        default:
            process_parent(&local_var);
    }
    return EXIT_SUCCESS;
}
