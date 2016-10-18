#include <iostream>
#include <fstream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

#define BUFSIZE 1024

using namespace std;

int main()
{
    int client;
    char username[30];
    int port = 1337;
    int isExit = 0;
    char buffer[BUFSIZE];
    char* ip = ""; /* fill this in later */

    struct sockaddr_in serverAddr; /* something for the server address AFAIK :P */

    // the socket gets created here
    client = socket(AF_INET, SOCK_STREAM, 0) /* AF_INET is ipv4 protocol,
                                             and SOCK_STREAM is for sending TCP packets */

    if(client < 0) // since socket() will return -1 if the call fails
    {
        cout << "[!] Error establishing socket." << endl; /* metasploit style :) */
    }

    cout << "[*] Socket client has been created succesfully." << endl;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // htons() converts port number to network byte order from host byte order

    // connecting to the socket happens here
    if(connect(client, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 0)
    {
        cout << "[*] Connection to the server port number " << port << endl;
    }

    // the receiving happens here
    cout << "[*] Waiting for confirmation from server" << endl;
    recv(client, buffer, BUFSIZE, 0);
    cout << "[:)] Connection confirmed!" << endl;
    cout << "[*] Enter your username" << endl;
    cout << "Username: " << endl;
    gets(username);
    cout <<"[!] Enter @ (i.e., Shift + 2), to end this connection" << endll;

    // client's message goes first

    do{
        cout << username << ": " << endl;
        do{
            cin >> buffer;
            send(client, buffer, BUFSIZE, 0);
            if(*buffer = '@'){
                send(client)
            }
        }
    }




}
