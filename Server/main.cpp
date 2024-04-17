#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

struct AcceptedSocket
{
        int acceptedSocketFD;
        struct sockaddr_in address;
        int error;
        bool acceptedSuccessfully;
};

AcceptedSocket *acceptingIncomingConnection(int serverSocket);
void *receiveAndPrintIncomingData(void *socketFD);
void acceptNewConnectionAndReceiveAndPrintData(int serverSocket);
void startAcceptingIncomingConnections(int serverSocket);
void sendReceivedMessageToOtherClients(char *buffer, void *socketFD);

struct AcceptedSocket accepetedSockets[10];
int acceptedSocketsCount = 0;

int server()
{
        // creating socket
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        // specifying the address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        // automatically be filled with current host's IP address
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // binding socket.
        bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

        // listening to the assigned socket
        listen(serverSocket, 5);

        // Start accepting incoming connections
        startAcceptingIncomingConnections(serverSocket);

        // Wait for any keypress to stop the server
        cin.get();

        // closing the socket.
        shutdown(serverSocket, SHUT_RDWR);

        return 0;
}

void *receiveAndPrintIncomingData(void *socketFD)
{
        int fd = *((int *)socketFD);
        // recieving data
        char buffer[1024];
        while (true)
        {
                ssize_t amountRecieved = recv(fd, buffer, sizeof(buffer), 0);

                if (amountRecieved > 0)
                {
                        buffer[amountRecieved] = 0;
                        printf("%s\n", buffer);
                        sendReceivedMessageToOtherClients(buffer, socketFD);
                }

                if (amountRecieved == 0)
                        break;
        }
        close(fd);
        return NULL;
}
void sendReceivedMessageToOtherClients(char *buffer, void *socketFD)
{
        int senderSocketFD = *((int *)socketFD); // Cast socketFD to int pointer and dereference

        for (int i = 0; i < acceptedSocketsCount; i++)
        {
                if (accepetedSockets[i].acceptedSocketFD != senderSocketFD)
                {
                        send(accepetedSockets[i].acceptedSocketFD, buffer, strlen(buffer), 0);
                }
        }
}

AcceptedSocket *acceptingIncomingConnection(int serverSocket)
{
        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(struct sockaddr_in);
        int clientSocketFD = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddressSize);

        AcceptedSocket *acceptedSocket = new AcceptedSocket();
        acceptedSocket->address = clientAddress;
        acceptedSocket->acceptedSocketFD = clientSocketFD;
        acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;

        if (!acceptedSocket->acceptedSuccessfully)
        {
                acceptedSocket->error = clientSocketFD;
        }
        return acceptedSocket;
}

void startAcceptingIncomingConnections(int serverSocket)
{
        pthread_t id;
        pthread_create(&id, NULL, (void *(*)(void *))acceptNewConnectionAndReceiveAndPrintData, (void *)serverSocket);
}

void acceptNewConnectionAndReceiveAndPrintData(int serverSocket)
{
        while (true)
        {
                struct AcceptedSocket *clientSocket = acceptingIncomingConnection(serverSocket);

                accepetedSockets[acceptedSocketsCount++] = *clientSocket;

                pthread_t id;
                pthread_create(&id, NULL, receiveAndPrintIncomingData, (void *)&clientSocket->acceptedSocketFD);
        }
}