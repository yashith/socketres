#include <pthread.h>
#include <regex.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <io.h>
#elif defined(__linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#define BUFFER_SIZE 1024
#define PORT 1234
void *handle_client(void *);
void build_http_response(const char *, const char *, char *, size_t *);
char *get_mime_type(char *);
char* url_decode(const char* );

int server_fd;
struct sockaddr_in server_addr;
int main(int argc, char const *argv[])
{
    printf("starting");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed soocket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(PORT);

    int b = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (b < 0) {
        perror("Bind fialed");
        exit(1);
    }

    printf("Hit here1");
    if(listen(server_fd,10)<0){
        perror("listen failed");
        exit(1);
    }

    printf("Hit here3");
    while (1) {

        printf("Hit here2");
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("%d",*client_fd);
        if (*client_fd < 0)
        {
            perror("Failed accepting client");
            continue;
        }
        pthread_t thread;

        printf("Hit here1");
        pthread_create(&thread, NULL, handle_client, (void *)client_fd);
        pthread_detach(thread);
    }
    return 0;
}
void *handle_client(void *args)
{
    int client_fd = *((int *)args);
    char *buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    ssize_t bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);
    char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
    size_t response_len;
    //just test
    response_len = sizeof(char)* BUFFER_SIZE;
    regex_t regex;
    regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
    regmatch_t matches[2];
    if(bytes>0){
        response = "more than 0 bytes reveiced";
        if (regexec(&regex, buffer, 2, matches, 0) == 0) {
            response = "<html><body>Something something darkside</body></html>";
        }
    }
    send(client_fd, response, response_len, 0);
    close(client_fd);
    free(args);
    free(buffer);
    return NULL;
}