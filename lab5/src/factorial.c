#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>

typedef struct {
    int start;
    int end;
    int mod;
    int *result;
    pthread_mutex_t *mutex;
} thread_data_t;

void *compute_factorial_part(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int start = data->start;
    int end = data->end;
    int mod = data->mod;
    int *result = data->result;
    pthread_mutex_t *mutex = data->mutex;

    int local_result = 1;
    for (int i = start; i <= end; i++) {
        local_result = (local_result * i) % mod;
    }

    pthread_mutex_lock(mutex);
    *result = (*result * local_result) % mod;
    pthread_mutex_unlock(mutex);
}

int main(int argc, char *argv[]) {
    int k = -1, pnum = -1, mod = -1;

    // Обработка аргументов командной строки
    static struct option options[] = {
        {"k", required_argument, 0, 'k'},
        {"pnum", required_argument, 0, 'p'},
        {"mod", required_argument, 0, 'm'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;
    while ((c = getopt_long(argc, argv, "k:p:m:", options, &option_index)) != -1) {
        switch (c) {
            case 'k':
                k = atoi(optarg);
                if (k <= 0) {
                    printf("k is a positive number\n");
                    return 1;
                }
                break;
            case 'p':
                pnum = atoi(optarg);
                if (pnum <= 0) {
                    printf("pnum is a positive number\n");
                    return 1;
                }
                break;
            case 'm':
                mod = atoi(optarg);
                if (mod <= 0) {
                    printf("mod is a positive number\n");
                    return 1;
                }
                break;
            default:
                printf("Index %d is out of options\n", option_index);
                exit(EXIT_FAILURE);
        }
    }

    if (k == -1 || pnum == -1 || mod == -1) {
        printf("Usage: %s -k <number> --pnum=<threads> --mod=<mod>\n", argv[0]);
        return 1;
    }

    // Инициализация мьютекса
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // Инициализация результата
    int result = 1;

    // Создание потоков
    pthread_t threads[pnum];
    thread_data_t thread_data[pnum];

    int chunk_size = k / pnum;
    int remainder = k % pnum;

    for (int i = 0; i < pnum; i++) {
        thread_data[i].start = i * chunk_size + 1;
        thread_data[i].end = (i + 1) * chunk_size;
        if (i == pnum - 1) {
            thread_data[i].end += remainder;
        }
        thread_data[i].mod = mod;
        thread_data[i].result = &result;
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, compute_factorial_part, &thread_data[i]);
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    // Вывод результата
    printf("Factorial of %d modulo %d is %d\n", k, mod, result);

    // Освобождение ресурсов
    pthread_mutex_destroy(&mutex);

    return 0;
}