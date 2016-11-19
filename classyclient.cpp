#include <iostream>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>

using namespace std;

class Client{
private:
    SOCKET client;
    int port = 1500;
    char username[120];
    int BUFSIZE = 1024;
    bool isExit = false;
    char buffer[BUFSIZE];
    char ip[12];
    struct sockaddr_in serverAddr; /* something for the server address AFAIK :P */
    // the socket gets created here
    WSADATA wsa;
	
};
