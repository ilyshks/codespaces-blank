#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <pthread.h>
#include "common.h"

struct Server {
  char ip[255];
  int port;
};

// Структура для передачи данных в поток
struct ThreadData {
    struct Server server;
    uint64_t begin;
    uint64_t end;
    uint64_t mod;
    uint64_t result;
};


// Функция, которая будет выполняться в потоке
void* ServerThread(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    struct Server server = data->server;
    uint64_t begin = data->begin;
    uint64_t end = data->end;
    uint64_t mod = data->mod;

    struct hostent *hostname = gethostbyname(server.ip);
    //printf("%s\n", server.ip);
    if (hostname == NULL) {
        fprintf(stderr, "gethostbyname failed with %s\n", server.ip);
        data->result = 0;
        return NULL;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server.port);
    server_addr.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
        fprintf(stderr, "Socket creation failed!\n");
        data->result = 0;
        return NULL;
    }

    if (connect(sck, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Connection failed\n");
        data->result = 0;
        close(sck);
        return NULL;
    }

    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0) {
        fprintf(stderr, "Send failed\n");
        data->result = 0;
        close(sck);
        return NULL;
    }

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
        fprintf(stderr, "Receive failed\n");
        data->result = 0;
        close(sck);
        return NULL;
    }

    memcpy(&data->result, response, sizeof(uint64_t));
    close(sck);
    return NULL;
}

// uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
//   uint64_t result = 0;
//   a = a % mod;
//   while (b > 0) {
//     if (b % 2 == 1)
//       result = (result + a) % mod;
//     a = (a * 2) % mod;
//     b /= 2;
//   }

//   return result % mod;
// }

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255
  unsigned int servers_num = 0;
  struct Server *to;
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);
    bool flag = true;
    
    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        flag = ConvertStringToUI64(optarg, &k);
        if (flag && k < 0){
            printf("k should be >= 0\n");
            return 1;
        }
        break;
      case 1:
        flag = ConvertStringToUI64(optarg, &mod);
        if (flag && mod <= 0){
            printf("mod should be > 0\n");
            return 1;
        }
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg));
        if (!strlen(servers)){
          printf("servers shold be the path to file\n");
          return 1;
        }else{
          FILE *file = fopen(servers, "r");
          if (file == NULL) {
              perror("Ошибка при открытии файла");
              return 1;
          }
          printf("Файл '%s' успешно открыт.\n", servers);
          char line[255];
          if (fgets(line, sizeof(line), file) != NULL) {
            servers_num = atoi(line);
          } else {
            fprintf(stderr, "Ошибка при чтении первой строки.\n");
            fclose(file);
            return 1;
          }
          to = malloc(sizeof(struct Server) * servers_num);

          for (int i = 0; i < servers_num; i++) {
            if (fgets(line, sizeof(line), file) != NULL) {
                char *ip = strtok(line, ":");
                char *port_str = strtok(NULL, ":");
                //printf("%s %s", ip, port_str);

                if (ip != NULL && port_str != NULL) {
                    //memcpy(to[i].ip, ip, sizeof(ip));
                    strncpy(to[i].ip, ip, sizeof(to[i].ip) - 1);
                    to[i].ip[sizeof(to[i].ip) - 1] = '\0';
                    to[i].port = atoi(port_str);
                } else {
                    fprintf(stderr, "Ошибка парсинга строки %d.\n", i + 1);
                    //free(addresses);
                    fclose(file);
                    return 1;
                }
            } else {
                fprintf(stderr, "Ошибка при чтении строки %d.\n", i + 1);
                //free(addresses);
                fclose(file);
                return 1;
            }
        }
          fclose(file);
        }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  //unsigned int servers_num = 1;
  //struct Server *to = malloc(sizeof(struct Server) * servers_num);
  // TODO: delete this and parallel work between servers

 // Создаем массив для хранения данных для каждого потока
  struct ThreadData* thread_data = malloc(sizeof(struct ThreadData) * servers_num);
  pthread_t* threads = malloc(sizeof(pthread_t) * servers_num);

  // Распределяем задачи между серверами
  uint64_t range_per_server = k / servers_num;
  for (int i = 0; i < servers_num; i++) {
      thread_data[i].server = to[i];
      //printf("%s\n", to[i].ip);
      thread_data[i].begin = i * range_per_server + 1;
      thread_data[i].end = (i == servers_num - 1) ? k : (i + 1) * range_per_server;
      thread_data[i].mod = mod;
      thread_data[i].result = 0;

      if (pthread_create(&threads[i], NULL, ServerThread, &thread_data[i]) != 0) {
          fprintf(stderr, "Failed to create thread %d\n", i);
          free(thread_data);
          free(threads);
          free(to);
          return 1;
      }
  }

  // Ожидаем завершения всех потоков
  for (int i = 0; i < servers_num; i++) {
      pthread_join(threads[i], NULL);
  }

  // Суммируем результаты
  uint64_t total_result = 1;
  for (int i = 0; i < servers_num; i++) {
    total_result = MultModulo(total_result, thread_data[i].result, mod);
    //total_result *= thread_data[i].result;
  }

  printf("Total result: %lu\n", total_result);

  free(thread_data);
  free(threads);
  free(to);

  return 0;
}
