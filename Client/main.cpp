#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string>

using namespace std;

void startListeningAndPrintMessages(int socketFD);

void *listenAndPrint(void *socketFD);

int client(string username)
{
        // creating socket
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        // specifying address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // Attempt to connect
        int result = connect(clientSocket, (struct sockaddr *)&serverAddress,
                             sizeof(serverAddress));

        // Check if connection was successful
        if (result == 0)
        {
                cout << "Connection was Successful\n";
        }
        else
        {
                // Display error message and retry connection
                cout << "Failed to connect to the server. Retrying...\n";
                // Close the socket to attempt reconnection
                close(clientSocket);
                // Attempt reconnection (recursively or in a loop)
                client(username);
        }

        // sending data
        char *line = NULL;
        size_t lineSize = 0;
        cout << "Hello " << username << " Enter your Message: ";

        startListeningAndPrintMessages(clientSocket);

        char buffer[1024];

        while (true)
        {

                ssize_t charCount = getline(&line, &lineSize, stdin);
                sprintf(buffer, "%s:%s", username.c_str(), line);

                if (charCount > 0)
                {
                        if (strcmp(line, "exit\n") == 0)
                                break;
                        ssize_t amountWasSent = send(clientSocket, buffer, strlen(buffer), 0);
                }
        }

        // closing socket
        close(clientSocket);
        return 0;
}

void startListeningAndPrintMessages(int socketFD)
{
        pthread_t id;
        pthread_create(&id, NULL, listenAndPrint, (void *)(intptr_t)socketFD);
}

void *listenAndPrint(void *socketFD)
{
        int fd = (intptr_t)socketFD;
        char buffer[1024];
        while (true)
        {
                ssize_t amountRecieved = recv(fd, buffer, sizeof(buffer), 0);

                if (amountRecieved > 0)
                {
                        buffer[amountRecieved] = 0;
                        cout << buffer << endl;
                }

                if (amountRecieved == 0)
                        break;
        }
        close(fd);
        return NULL;
}