#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>

#define BUFSIZE 1024

using namespace std;

int main()
{
    int client, server;
    int port = 1337;
    int isExit = 0;
    char buffer[BUFSIZE];
    char username[30];
    struct sockaddr_in serverAddr;
    socklen_t size;

    if(client < 0)
    {
        cout << "[!] Error creating socket." << endl;
        exit(0);
    }

    else
        cout << "[*] Socket created successfully." << endl;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if((bind(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0)
    {
        cout << "[!] Error binding connection, the socket already exists." << endl;
        return -1;
    }

    size = sizeof(serverAddr);
    cout << "[*] Awaiting client connection." << endl;

    listen(client, 1); // listening for 1 client

    int clientCount = 1;
    server = accept(client, (struct sockaddr*)&serverAddr, &size); // you need to accept a connection, once you get one.

    if(server < 0)
        cout << "[!] Error accepting connection." << endl;

    while(server > 0)
    {
        strcpy(buffer, "[:)] Server connected! \n");
        cout << "[*] Enter your username" << endl;
        gets(username);
        send(server, buffer, BUFSIZE, 0);
        cout << "[*] Connected with client " << clientCount << endl;
        cout << "[*] Enter ~ (Shift + `) to end this connection." << endl;

        cout << "Client: ";  // need to recv the client username prior to this, so as to display it here
        do{
            recv(server, buffer, BUFSIZE, 0);
            cout << buffer << " ";
            if(*buffer == '~'){
                *buffer = ' ';
                isExit = 1;
            }
        }while(*buffer != ' ');

        do{
            cout << "\n" << username << ": ";
            do{
                cin >> buffer;
                send(server, buffer, BUFSIZE, 0);
                if(*buffer == '~'){
                    send(server, buffer, BUFSIZE, 0);
                    *buffer = ' ';
                    isExit = 1;
                }
            }while(*buffer != ' ');

            cout << "Client: "; // need to get the client username here too
            do{
                recv(server, buffer, BUFSIZE, 0);
                cout << buffer << " ";
                if(*buffer == '~'){
                    *buffer == ' ';
                    isExit =  1;
                }
            }while(*buffer != ' ');
        }while(isExit != 0);

        cout << "\n [*] Connection terminated with IP " << inet_ntoa(serverAddr.sin_addr); //inet_ntoa() converts packet data to IP (apparently :D)
        close(server);
        isExit = 0;
        exit(1);
    }

    close(client);
    return 0;
}
