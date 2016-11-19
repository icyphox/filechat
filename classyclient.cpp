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
    cout << "[*] Enter your username" << endl;
    cout << "Username: " << endl;
    gets(username);
    cout <<"[*] Please append all your messages with a *" << endl;
    // We need to replace ^ with enter
    // client's message goes first
    cout<<"[*] Client's message goes first. Say hi!"<<endl;
    int sentUserName = 0, recvusername = 0;
    char toSend[120];
    strcat(toSend, "%");
    strcat(toSend, username);
    do {
        if(sentUserName == 0)
        {
            send(client, toSend, BUFSIZE, 0);
            sentUserName = 1;
            continue;
        }
        cout <<"You: ";
        do {
            cin >> buffer;
            send(client, buffer, BUFSIZE, 0);
            if (*buffer == '#') {
                send(client, buffer, BUFSIZE, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);
        if(recvusername == 0)
        {
            recv(client, buffer, BUFSIZE, 0);
            if(buffer[0]=='%')
            {
                for(int i=1; i<strlen(buffer); i++)
                {
                    extUsername[i-1]=buffer[i];
                }
                extUsername[strlen(buffer)]='\0';
            }
            recvusername = 1;
            continue;
        }
        cout <<extUsername<<": ";
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

