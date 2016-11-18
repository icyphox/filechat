
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
<<<<<<< HEAD
#include <cstdlib>
#define BUFSIZE 1024
=======
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>



>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
using namespace std;
int main()
{
    int client;
    int port = 1500; 
    int BUFSIZE = 1024;
    bool isExit = false;
    char buffer[BUFSIZE];
<<<<<<< HEAD
    char* ip = "127.0.0.1"; /* fill this in later */
    struct sockaddr_in serverAddr; /* something for the server address AFAIK :P */
    // the socket gets created here
    client = socket(AF_INET, SOCK_STREAM, 0); /* AF_INET is ipv4 protocol,
                                             and SOCK_STREAM is for sending TCP packets */
    if(client < 0) // since socket() will return -1 if the call fails
=======
    char* ip = "127.0.0.1";

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
    {
        cout << "[!] Error creating socket." << endl;
        exit(1);
    }
<<<<<<< HEAD
    else
        cout<< "[*] Socket client has been created successfully." << endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr=inet_addr(ip);
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
    cout <<"[!] Enter ~ (i.e., Shift + 2), to end this connection" << endl;
    // client's message goes first
    do{
        cout << username << ": ";
        do{
=======

    cout << "[*] Socket client has been created." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "[*] Connection to the server port number: " << port << endl;

    cout << "[*] Awaiting confirmation from the server." << endl; 
    recv(client, buffer, BUFSIZE, 0);
    cout << "[:)] Connection confirmed!" << endl;;

    cout << "[*] Enter ~ to end this connection." << endl;


    do {
        cout << "Client: "; // We'll figure out the sending of client and server username later.
        do {
>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
            cin >> buffer;
            send(client, buffer, BUFSIZE, 0);
            if (*buffer == '#') {
                send(client, buffer, BUFSIZE, 0);
                *buffer = '*'; // Have to end each message with this character. Can't seem to figure out a workaround.
                isExit = true;
            }
<<<<<<< HEAD
        }while(*buffer != 0);
    }while(isExit == 0);
    cout << "[*] Connection terminated." << endl;
=======
        } while (*buffer != 42);

        cout << "Server: ";
        do {
            recv(client, buffer, BUFSIZE, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*'; // Same here.
                isExit = true;
            }

        } while (*buffer != 42);
        cout << endl;

    } while (!isExit);


    cout << "[!] Connection terminated with server.";

>>>>>>> ca6cc969428af0360ddce0fa63820b10a715e9c7
    close(client);
    return 0;
}
