#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
int main()
{
    int client;
    int port = 1500;
    int BUFSIZE = 1024;
    bool isExit = false;
    char buffer[BUFSIZE];
    char* ip = "127.0.0.1"; /* fill this in later */
    struct sockaddr_in serverAddr; /* something for the server address AFAIK :P */
    // the socket gets created here
    WSADATA wsa;
    //char username[20];

    cout << "[*] Initialising Winsock." << endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout << "[!] Failed. Error Code:" << WSAGetLastError();
        return 1;
    } /* AF_INET is ipv4 protocol,
                                             and SOCK_STREAM is for sending TCP packets */
     // since socket() will return -1 if the call fails


    cout << "[:)] Initialised!" << endl;



    if((client = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cout << "[!] Could not create socket: " << WSAGetLastError();
    }

    else
        cout<< "[*] Socket client has been created successfully." << endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr=inet_addr(ip);
    serverAddr.sin_port = htons(port); // htons() converts port number to network byte order from host byte order
    // connecting to the socket happens here
    if(connect(client, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << "[!] Failed to connect to " << port << endl;
        return 1;
    }
    // the receiving happens here
    cout << "[*] Waiting for confirmation from server." << endl;
    recv(client, buffer, BUFSIZE, 0);
    cout << "[:)] Connection confirmed!" << endl;
   // cout << "[*] Enter your username" << endl;
   // cout << "Username: " << endl;
    //gets(username);
    cout <<"[!] Enter # (i.e., Shift + 3), to end this connection" << endl;
    // client's message goes first
   do {
        cout << "Client: ";
        do {
            cin >> buffer;
            send(client, buffer, BUFSIZE, 0);
            if (*buffer == '#') {
                send(client, buffer, BUFSIZE, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);

        cout << "Server: ";
        do {
            recv(client, buffer, BUFSIZE, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }

        } while (*buffer != 42);
        cout << endl;

    } while (!isExit);

    cout << "[!] Connection terminated with server.";

    closesocket(client);
    return 0;
}
