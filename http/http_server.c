#include <pthread.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <io.h>
#elif defined(__linux__)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

#define BUFFER_SIZE 1024
void* handle_client(void*);


int server_fd;
struct sockaddr_in server_addr;
int main(int argc, char const *argv[]) {
    if((server_fd = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("Failed soocket");
        exit(1);
    }

    int b = bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));

    if(b <0){
        perror("Bind fialed");
        exit(1);
    }
    while(1){
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int* client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd,(struct sockaddr*)&client_addr,client_addr_len);
        if(*client_fd < 0 ){
            perror("Failed accepting client");
            continue;
        }
        pthread_t thread;
        pthread_create(&thread,NULL,handle_client,(void *)client_fd);
        pthread_detach(thread);
    }    
    return 0;
}

void* handle_client(void* args){
    int client_fd = *((int*)args);
    char* buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    ssize_t bytes = recv(client_fd,buffer,BUFFER_SIZE,0);
    //todo 

}
