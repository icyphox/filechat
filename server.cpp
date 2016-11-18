#include <iostream>
#include <string.h>
<<<<<<< HEAD
#include <stdlib.h>
#include <winsock2.h>
#include <stdio.h>
=======
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7


using namespace std;

int main()
{
<<<<<<< HEAD
    SOCKET client, server;
    WSADATA wsa;
=======
    int client, server;
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
    int port = 1500;
    int BUFSIZE =  1024;
    bool isExit = false;
    char buffer[BUFSIZE];
<<<<<<< HEAD

    struct sockaddr_in server_addr;

    cout<<"[*] Initialising Winsock..."<<endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout<<"[!] Failed. Error Code : "<<WSAGetLastError();
        return 1;
    }

    cout<<"[*] Initialized."<<endl;

    if((client = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cout<<"Could not create socket : "<<WSAGetLastError();
    }

    cout<<"Socket created.\n";

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);


    if ((connect(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
=======

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
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
    {
        cout << "[!] Error binding connection, the socket has already been established..." << endl;
        return -1;
    }


<<<<<<< HEAD
=======
    size = sizeof(server_addr);
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
    cout << "[*] Looking for clients." << endl;

    listen(client, 1);


    int clientCount = 1;
<<<<<<< HEAD
    server = accept(client,(struct sockaddr *)&server_addr, sizeof(&server_addr));
=======
    server = accept(client,(struct sockaddr *)&server_addr,&size);
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7

    // first check if it is valid or not
    if (server < 0)
        cout << "[!] Error accepting client connection." << endl;

    while (server > 0)
    {
        strcpy(buffer, "[:)] Server connected!");
        send(server, buffer, BUFSIZE, 0);
        cout << "[*] Connected with the client. " << clientCount << endl;
        cout << "[*] Enter ~ to end the connection." << endl;

<<<<<<< HEAD

=======
      
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7

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
<<<<<<< HEAD
        closesocket(server);
=======
        close(server);
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
        isExit = false;
        exit(1);
    }

    closesocket(client);
    return 0;
}
