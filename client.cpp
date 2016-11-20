#include <iostream>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>

using namespace std;

struct Log
{
    char User[120];
    char Date[120];
    char message[1024];
};

class Client{
private:
    SOCKET client;
    int port = 1500;
    char username[120];
    char extUsername[120];
    int BUFSIZE = 1024;
    bool isExit = false;
    char buffer[1024];
    char ip[12];
    struct sockaddr_in serverAddr;
    WSADATA wsa;
    void KillSock();
    char* getTime();

public:
	int InitWinsock();
	void CreateSocket();
	void SendUsername();
	void RecvUsername();
	int Connekt();
	// void InitLog();
	void sendRecvMsg();

};
/*
void Client::InitLog()
{
    fstream ifof;
    ifof.open("Chatlog.DAT", ios::in|ios::out|ios::binary|ios::nocreate|ios::ate);
}
*/

int Client::InitWinsock()
{
	cout << "[*] Initialising Winsock." << endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout << "[!] Failed. Error Code:" << WSAGetLastError();
        return 1;
    } /* AF_INET is ipv4 protocol,
        and SOCK_STREAM is for sending TCP packets */
     // since socket() will return -1 if the call fails


    cout << "[:)] Initialised!" << endl;
}
char* Client::getTime()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return asctime(timeinfo);
}

void Client::SendUsername()
{
    cout << "Enter the username you wish to use..." <<endl;
    gets(username);
    char toSend[120];
    strcpy(toSend, username);
    strcat(toSend, "%");
    send(client, toSend, BUFSIZE, 0);
}

void Client::RecvUsername()
{
    int flag = 0;
    do {
        recv(client, buffer, BUFSIZE, 0);
        if(buffer[strlen(buffer)-1]=='%'){
            strcpy(extUsername, buffer);
            extUsername[strlen(extUsername)-1] = '\0';
            flag = 1;
        }
    }while (!flag);
}

void Client::CreateSocket()
{
	if((client = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cout << "[!] Could not create socket: " << WSAGetLastError();
    }

    else
        cout << "[*] Socket client has been created successfully." << endl;
}

int Client::Connekt()
{
	cout <<"[*] Enter the IP address of the recipient: "<< endl;
    gets(ip);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr=inet_addr(ip);
    serverAddr.sin_port = htons(port); // htons() converts port number to network byte order from host byte order
    // connecting to the socket happens here
    if(connect(client, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout<<"connect function failed with error: \n"<<WSAGetLastError();
        cout << "[!] Failed to connect to port " << port << " on " << ip << endl;
        return 1;
    }
}

void Client::sendRecvMsg()
{
	cout << "[*] Waiting for confirmation from server." << endl;
    recv(client, buffer, BUFSIZE, 0);
    cout << "[:)] Connection confirmed!" << endl;
    SendUsername();
    cout << "[*] Waiting for username exchange...please hold on." <<endl;
    RecvUsername();
    cout << "[:)] Username received! " <<endl;
    cout <<"[*] Please append all your messages with a *" << endl;
    cout<<"[*] Client's message goes first. Say hi!"<<endl;
    do {
        cout << endl <<"You: ";
        do {
            cin >> buffer;
            send(client, buffer, BUFSIZE, 0);
            if (*buffer == '#') {
                send(client, buffer, BUFSIZE, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);
        cout << extUsername << ": ";
        do {
            recv(client, buffer, BUFSIZE, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);
    } while (!isExit);

    KillSock();
}

void Client::KillSock()
{
        closesocket(client);
	    WSACleanup();
        cout << "[!] Connection terminated with server.";
}

int main()
{
    Client clnt;
    clnt.InitWinsock();
    clnt.CreateSocket();
    clnt.Connekt();
    clnt.sendRecvMsg();
}

