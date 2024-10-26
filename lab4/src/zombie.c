#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    pid_t child_pid;

    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    // Создаем дочерний процесс
    child_pid = fork();

    if (child_pid > 0) {
        // Родительский процесс
        printf("Родительский процесс (PID: %d) создал дочерний процесс (PID: %d)\n", getpid(), child_pid);
        printf("Родительский процесс спит 20 секунд...\n");

        sleep(20);  // Родительский процесс спит 20 секунд

        printf("Родительский процесс завершается.\n");
    } else if (child_pid == 0) {
        // Дочерний процесс
        sleep(1);
        printf("Дочерний процесс (PID: %d) завершается.\n", getpid());
        exit(0);  // Дочерний процесс завершается
    } else {
        // Ошибка при создании дочернего процесса
        perror("fork");
        return 1;
    }

    return 0;
}