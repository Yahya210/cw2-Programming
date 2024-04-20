#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string>

using namespace std;

// Function prototypes
void startListeningAndPrintMessages(int socketFD); // Start listening for incoming messages and print them
string decryptCaesar(const string &ciphertext);    // Decrypt a message encrypted using Caesar cipher
string encryptCaesar(const string &plaintext);     // Encrypt a message using Caesar cipher
void *listenAndPrint(void *socketFD);              // Thread function to continuously listen for incoming messages

// Client function to send and receive messages
int client(string username)
{
        // Creating socket
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        // Specifying server address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // Attempting to connect
        int result = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

        // Checking if connection was successful
        if (result == 0)
        {
                cout << "Connection was Successful\n";
        }
        else
        {
                // Displaying error message and retrying connection
                cout << "Failed to connect to the server. Retrying...\n";
                // Closing the socket to attempt reconnection
                close(clientSocket);
                // Attempting reconnection (recursively or in a loop)
                client(username);
        }

        // Sending and receiving data
        char *line = NULL;
        size_t lineSize = 0;
        cout << "Hello " << username << " Enter your Message: ";

        // Creating a thread to listen for incoming messages
        pthread_t id;
        pthread_create(&id, NULL, listenAndPrint, (void *)(intptr_t)clientSocket);

        char buffer[1024];

        while (true)
        {
                string encryptedMessage = "";

                // Reading user input
                ssize_t charCount = getline(&line, &lineSize, stdin);
                if (charCount > 0)
                {
                        if (strcmp(line, "exit\n") == 0)
                                break;
                        snprintf(buffer, sizeof(buffer), "%s:%s", username.c_str(), line);
                        string encryptedMessage = encryptCaesar(buffer);
                        ssize_t amountWasSent = send(clientSocket, encryptedMessage.c_str(), strlen(encryptedMessage.c_str()), 0);
                }
        }

        // Closing the socket
        close(clientSocket);
        return 0;
}

// Function to start listening for incoming messages
void startListeningAndPrintMessages(int socketFD)
{
        pthread_t id;
        pthread_create(&id, NULL, listenAndPrint, (void *)(intptr_t)socketFD);
}

// Thread function to continuously listen for incoming messages
void *listenAndPrint(void *socketFD)
{
        int fd = (intptr_t)socketFD;
        char buffer[1024];
        while (true)
        {
                // Receiving data
                ssize_t amountReceived = recv(fd, buffer, sizeof(buffer), 0);
                if (amountReceived > 0)
                {
                        buffer[amountReceived] = '\0';
                        // Decrypting and printing the received message
                        string decryptedMessage = decryptCaesar(buffer);
                        cout << decryptedMessage << endl;
                }
                if (amountReceived == 0)
                        break;
        }
        close(fd);
        return NULL;
}

// Function to encrypt a message using Caesar cipher
string encryptCaesar(const string &plaintext)
{
        string ciphertext = "";
        for (int i = 0; i < plaintext.size(); i++)
        {
                char e = plaintext[i] + 3; // Shifting each character by 3
                ciphertext += e;
        }
        return ciphertext;
}

// Function to decrypt a message encrypted using Caesar cipher
string decryptCaesar(const string &ciphertext)
{
        string plaintext = "";
        for (int i = 0; i < ciphertext.size(); i++)
        {
                char e = ciphertext[i] - 3; // Shifting each character back by 3
                plaintext += e;
        }
        return plaintext;
}
