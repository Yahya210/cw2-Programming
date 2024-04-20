#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

// Structure to store information about each accepted socket
struct AcceptedSocket
{
        int acceptedSocketFD;       // File descriptor for the accepted socket
        struct sockaddr_in address; // Address information of the client
        int error;                  // Error code if acceptedSuccessfully is false
        bool acceptedSuccessfully;  // Flag indicating if the socket was accepted successfully
};

// Function prototypes
AcceptedSocket *acceptingIncomingConnection(int serverSocket);        // Accepts incoming connections
void *receiveAndPrintIncomingData(void *socketFD);                    // Receives and prints incoming data
void acceptNewConnectionAndReceiveAndPrintData(int serverSocket);     // Accepts new connections and handles data reception
void startAcceptingIncomingConnections(int serverSocket);             // Starts accepting incoming connections
void sendReceivedMessageToOtherClients(char *buffer, void *socketFD); // Sends received messages to other clients

// Array to store information about accepted sockets
struct AcceptedSocket accepetedSockets[10];
int acceptedSocketsCount = 0; // Counter to keep track of the number of accepted sockets

// Main server function
int server()
{
        // Creating the socket
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        // Specifying the server address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY; // Automatically filled with current host's IP address

        // Binding the socket
        int binding = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (binding != 0)
        {
                perror("Error Binding Port");
                abort();
        }

        // Listening to the assigned socket
        listen(serverSocket, 5);

        // Start accepting incoming connections
        startAcceptingIncomingConnections(serverSocket);

        // Wait for any keypress to stop the server
        cin.get();

        // Closing the socket
        shutdown(serverSocket, SHUT_RDWR);

        return 0;
}

// Thread function to receive and print incoming data
void *receiveAndPrintIncomingData(void *socketFD)
{
        int fd = *((int *)socketFD); // Get the socket file descriptor from the void pointer
        char buffer[1024];
        while (true)
        {
                // Receiving data
                ssize_t amountReceived = recv(fd, buffer, sizeof(buffer), 0);
                if (amountReceived > 0)
                {
                        buffer[amountReceived] = 0;                          // Null-terminate the received data
                        printf("%s\n", buffer);                              // Print the received data
                        sendReceivedMessageToOtherClients(buffer, socketFD); // Forward the message to other clients
                }
                if (amountReceived == 0) // If the connection is closed by the client
                        break;
        }
        close(fd); // Close the socket
        return NULL;
}

// Function to send received messages to other clients
void sendReceivedMessageToOtherClients(char *buffer, void *socketFD)
{
        int senderSocketFD = *((int *)socketFD); // Get the socket file descriptor from the void pointer

        // Iterate through accepted sockets
        for (int i = 0; i < acceptedSocketsCount; i++)
        {
                // Check if the socket is not the sender's socket
                if (accepetedSockets[i].acceptedSocketFD != senderSocketFD)
                {
                        // Send the message to other clients
                        send(accepetedSockets[i].acceptedSocketFD, buffer, strlen(buffer), 0);
                }
        }
}

// Function to accept incoming connections
AcceptedSocket *acceptingIncomingConnection(int serverSocket)
{
        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(struct sockaddr_in);
        int clientSocketFD = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddressSize);

        // Create a new AcceptedSocket structure to store information about the accepted socket
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

// Function to start accepting incoming connections
void startAcceptingIncomingConnections(int serverSocket)
{
        pthread_t id;
        pthread_create(&id, NULL, (void *(*)(void *))acceptNewConnectionAndReceiveAndPrintData, (void *)serverSocket);
}

// Function to accept new connections and handle data reception
void acceptNewConnectionAndReceiveAndPrintData(int serverSocket)
{
        while (true)
        {
                // Accept incoming connection and get the client socket information
                struct AcceptedSocket *clientSocket = acceptingIncomingConnection(serverSocket);

                // Add the accepted socket information to the array
                accepetedSockets[acceptedSocketsCount++] = *clientSocket;

                // Start a new thread to handle data reception for the client
                pthread_t id;
                pthread_create(&id, NULL, receiveAndPrintIncomingData, (void *)&clientSocket->acceptedSocketFD);
        }
}
