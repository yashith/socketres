#include <cstring>
#include <iostream>
#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <io.h>
#elif defined(__linux__)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif




int main(int argc, char const *argv[])
{
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
    const char* message = "Hello server";
    send(clientSocket,message,strlen(message),0);
    
    close(clientSocket);
    return 0;
}
