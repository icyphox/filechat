#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>


using namespace std;

int main()
{
    int client, server;
    int port = 1500;
    int BUFSIZE =  1024;
    bool isExit = false;
    char buffer[BUFSIZE];

    struct sockaddr_in server_addr;
    socklen_t size;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        cout << "[!]Error establishing socket." << endl;
        exit(1);
    }

    

    cout << "[*] Socket server has been created." << endl;


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);


    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
    {
        cout << "[!] Error binding connection, the socket has already been established..." << endl;
        return -1;
    }


    size = sizeof(server_addr);
    cout << "[*] Looking for clients." << endl;

    listen(client, 1);


    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    // first check if it is valid or not
    if (server < 0)
        cout << "[!] Error accepting client connection." << endl;

    while (server > 0)
    {
        strcpy(buffer, "[:)] Server connected!");
        send(server, buffer, BUFSIZE, 0);
        cout << "[*] Connected with the client. " << clientCount << endl;
        cout << "[*] Enter ~ to end the connection." << endl;

      

        cout << "Client: ";
        do {
            recv(server, buffer, BUFSIZE, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
                send(server, buffer, BUFSIZE, 0);
                if (*buffer == '#') {
                    send(server, buffer, BUFSIZE, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
                recv(server, buffer, BUFSIZE, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '8';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

        cout << "[!] Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        isExit = false;
        exit(1);
    }

    close(client);
    return 0;
}
