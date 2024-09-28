#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            if (seed <= 0) { // initialize seed for generating numbers
              printf("seed is a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            if (array_size <= 0) { // check size of array
              printf("array_size is a positive number\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            if (pnum <= 0) { // check size of array
              printf("pnum is a positive number\n");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int chunk_size = array_size / pnum;
  int pipes[pnum][2];
  pid_t pids[pnum];

  if (!with_files){
    // Создаем каналы для каждого дочернего процесса
    for (int i = 0; i < pnum; i++) {
        if (pipe(pipes[i]) == -1) {
            printf("pipe failed, number is %d", i);
            return 1;
        }
    }
  }

  for (int i = 0; i < pnum; i++) {
    pids[i] = fork();
    pid_t child_pid = pids[i];
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        int start = i * chunk_size;
        int end = (i == pnum - 1) ? array_size : start + chunk_size;
        struct MinMax local_min_max = GetMinMax(array, start, end);

        if (with_files) {
          
          char filename[256];
          snprintf(filename, sizeof(filename), "%s%d.txt", "result_", i);

          FILE *file = fopen(filename, "w");
          if (file == NULL) {
              printf("fopen failed, number is %d\n", i);
              return 1;
          }

          fprintf(file, "%d %d\n", local_min_max.min, local_min_max.max);
          fclose(file);

        } else {
          close(pipes[i][0]);
          // Записываем результаты в канал
          write(pipes[i][1], &local_min_max.min, sizeof(int));
          write(pipes[i][1], &local_min_max.max, sizeof(int));

          close(pipes[i][1]);

        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  int status;
  int i = 0;
  while (active_child_processes > 0) {
    waitpid(pids[i], &status, 0);
    if (!WIFEXITED(status)) {
      printf("Child process %d did not exit normally\n", i);
    }

    active_child_processes -= 1;
    i++;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {

      char filename[256];
      snprintf(filename, sizeof(filename), "%s%d.txt", "result_", i);

      FILE *file = fopen(filename, "r");
      if (file == NULL) {
          printf("fopen (r) failed, number is %d\n", i);
          continue;
      }

      fscanf(file, "%d %d", &min, &max);
      fclose(file);
      remove(filename);  // Удаляем файл после чтения

    } else {
      
      close(pipes[i][1]);  // Закрываем запись в канал

      read(pipes[i][0], &min, sizeof(int));
      read(pipes[i][0], &max, sizeof(int));

      close(pipes[i][0]);  // Закрываем чтение из канала

    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
