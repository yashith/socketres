#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
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
