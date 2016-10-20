
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>



using namespace std;

int main()
{
    int client;
    int port = 1500; 
    int BUFSIZE = 1024;
    bool isExit = false;
    char buffer[BUFSIZE];
    char* ip = "127.0.0.1";

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "[!] Error creating socket." << endl;
        exit(1);
    }

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
            cin >> buffer;
            send(client, buffer, BUFSIZE, 0);
            if (*buffer == '#') {
                send(client, buffer, BUFSIZE, 0);
                *buffer = '*'; // Have to end each message with this character. Can't seem to figure out a workaround.
                isExit = true;
            }
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

    close(client);
    return 0;
}
