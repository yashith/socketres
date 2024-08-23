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
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8087);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket,(sockaddr *)&serverAddress,sizeof(serverAddress));
    listen(serverSocket, 5);

    int clientSocket = accept(serverSocket,nullptr,nullptr);
    char buffer[1024] = {0};
    recv(clientSocket,buffer,sizeof(buffer),0);
    std::cout << "Message : " << buffer << std::abort;

    close(serverSocket);

    return 0;
}
