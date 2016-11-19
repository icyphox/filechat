#include <iostream>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>


using namespace std;

int main()
{
    SOCKET client, server;
    int port = 1500;
    int BUFSIZE =  1024;
    bool isExit = false;
    char buffer[BUFSIZE];

    struct sockaddr_in server_addr;
    int sze;

    WSADATA wsa;

    cout << "[*] Initialising Winsock." << endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout << "[!] Failed. Error Code:" << WSAGetLastError();
        return 1;
    }

    cout << "[:)] Initialised!" << endl;



    if((client = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cout << "[!] Could not create socket: " << WSAGetLastError();
    }

    cout << "[*] Socket server has been created.\n" << endl;


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY); // any address can attempt to connect
    server_addr.sin_port = htons(port);


    if ((bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
    {
        cout << "[!] Error binding connection, the socket has already been established..." << endl;
        return -1;
    }


    sze = sizeof(server_addr);
    cout << "[*] Looking for clients." << endl;

    listen(client, 1);


    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&sze);

    // first check if it is valid or not
    if (server < 0)
        cout << "[!] Error accepting client connection." << endl;

    while (server > 0)
    {
        strcpy(buffer, "[:)] Server connected!");
        send(server, buffer, BUFSIZE, 0);
        cout << "[*] Connected with the client. " << clientCount << endl;
        cout << "[*] Enter # (i.e. Shift + 3), to end the connection." << endl;



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
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

        cout << "[!] Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        closesocket(server);
        WSACleanup();
        isExit = false;
        exit(1);
    }

    closesocket(client);
    return 0;
}
