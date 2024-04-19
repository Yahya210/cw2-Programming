#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string>

using namespace std;

void startListeningAndPrintMessages(int socketFD);
string decryptCaesar(const string &ciphertext);
string encryptCaesar(const string &plaintext);
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

        pthread_t id;
        pthread_create(&id, NULL, listenAndPrint, (void *)(intptr_t)clientSocket); // Moved here

        char buffer[1024];

        while (true)
        {
                string encryptedMessage = "";

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
                        buffer[amountRecieved] = '\0';
                        string decryptedMessage = decryptCaesar(buffer);
                        cout << decryptedMessage << endl;
                }
                if (amountRecieved == 0)
                        break;
        }
        close(fd);
        return NULL;
}

string encryptCaesar(const string &plaintext)
{
        string ciphertext = "";
        for (int i = 0; i < plaintext.size(); i++)
        {
                char e = plaintext[i] + 3; // Shift each character by 3
                ciphertext += e;
        }
        return ciphertext;
}

string decryptCaesar(const string &ciphertext)
{
        string plaintext = "";
        for (int i = 0; i < ciphertext.size(); i++)
        {
                char e = ciphertext[i] - 3; // Shift each character back by 3
                plaintext += e;
        }
        return plaintext;
}
