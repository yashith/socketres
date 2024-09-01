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
void *handle_client(void *);
void build_http_response(const char *, const char *, char *, size_t *);
char *get_mime_type(char *);
char* url_decode(const char* );

int server_fd;
struct sockaddr_in server_addr;
int main(int argc, char const *argv[])
{
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed soocket");
        exit(1);
    }

    int b = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (b < 0)
    {
        perror("Bind fialed");
        exit(1);
    }
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (*client_fd < 0)
        {
            perror("Failed accepting client");
            continue;
        }
        pthread_t thread;
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

    if (bytes > 0)
    {
        regex_t regex;
        // todo change the regex
        regcomp(&regex, "^GET /([^])", REG_EXTENDED);
        regmatch_t matches[2];
        if (regexec(&regex, buffer, 2, matches, 0) == 0)
        {
            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            // todo what is url_decode?
            /*
            char *file_name = url_decode(url_encoded_file_name);
            char file_ext[32];
            // todo what's get_file_extention
            strcpy(file_ext, get_file_extention(file_name));

            char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
            size_t response_len;
            build_http_response(file_name, file_ext, response, &response_len);
            send(client_fd, response, response_len, 0);
            free(response);
            free(file_name);
            */
        }
        regfree(&regex);
    }
    close(client_fd);
    free(args);
    free(buffer);
    retrun NULL;
}

void build_http_response(const char *file_name, const char *file_ext, char *response, size_t *response_len) {
    // const char* mime_type = get_mime_type(file_ext);
    char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
    snprint(header, BUFFER_SIZE, "TODO send header response");
    int file_fd = open(file_name, O_RDONLY);

    if (file_fd == -1) {
        sprintf(response, BUFFER_SIZE, "TODO not found response");
        *response_len = strlen(response);
        return;
    }

    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t file_size = file_stat.st_size;

    *response_len = 0;
    memcpy(response, header, strlen(header));
    *response_len += strlen(header);

    ssize_t byte_read;
    while ((byte_read = read(file_fd, response + *response_len, BUFFER_SIZE - *response_len)) > 0) {
        *response_len += byte_read;
    }
    free(header);
    close(file_fd);
}
// char *get_mime_type(char *file_ext) {
// }
char* get_file_extention(char* file_name){
    //split the file name by . and retrun last * of the starting char
    return NULL;
}