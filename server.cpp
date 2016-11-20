#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <winsock2.h>
#include <time.h>


using namespace std;

struct Log
{
    char User[120];
    int mon;
    int day;
    int year;
    int hour;
    int minute;
    int sec;
    char message[1024];
};

class Server{
private:
    fstream ifof;
    struct sockaddr_in server_addr;
    int sze, clientCount;
    char username[120];
    char extUsername[120];

    WSADATA wsa;
    void InitLog(char*);
	void writeLog(char*, char*);
    void killSock();

public:
    void InitWinsock();
    void CreateSocket();
    void BindSocket();
    void CloseLog();
    void sendRecvMsg();
    void SendUsername();
    void RecvUsername();

    SOCKET client, server;
    int port = 1500;
    int BUFSIZE =  1024;
    bool isExit = false;
    char buffer[1024];

};



void Server::InitWinsock(){
    cout << "[*] Initialising Winsock." << endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout << "[!] Failed. Error Code:" << WSAGetLastError();
        return;
    }

    cout << "[:)] Initialised!" << endl;
}

void Server::writeLog(char* mesg, char* uname)
{
    Log l1;
    strcpy(l1.User, username);
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    l1.year = timeinfo->tm_year + 1900;
    l1.mon = timeinfo->tm_mon + 1;
    l1.day = timeinfo->tm_mday;
    l1.hour = timeinfo->tm_hour;
    l1.minute = timeinfo->tm_min;
    l1.sec = timeinfo->tm_sec;
    strcpy(l1.message, mesg);
    ifof.write((char*)&l1, sizeof(Log));
}

void Server::InitLog(char* name)
{
    char filename[120];
    strcpy(filename, name);
    strcat(filename, ".DAT");
    ifof.open(filename, ios::in|ios::out|ios::binary|ios::ate);
}

void Server::CloseLog()
{
    ifof.close();
}

void Server::CreateSocket(){
    if((client = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cout << "[!] Could not create socket: " << WSAGetLastError();
    }

    cout << "[*] Socket server has been created." << endl;
}

void Server::killSock(){
        CloseLog();
        cout << "[!] Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        closesocket(server);
        WSACleanup();
        isExit = false;
        exit(1);
}

void Server::SendUsername()
{
    cout << "Enter the username you wish to use..." <<endl;
    gets(username);
    char toSend[120];
    strcpy(toSend, username);
    strcat(toSend, "%");
    send(server, toSend, BUFSIZE, 0);
}

void Server::RecvUsername()
{
    int flag = 0;
    do {
        recv(server, buffer, BUFSIZE, 0);
        if(buffer[strlen(buffer)-1]=='%'){
            strcpy(extUsername, buffer);
            extUsername[strlen(extUsername)-1] = '\0';
            flag = 1;
        }
    }while (!flag);
}

void Server::BindSocket(){
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY); // any address can attempt to connect
    server_addr.sin_port = htons(port);
    if ((bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
    {
        cout << "[!] Error binding connection, the socket has already been established..." << endl;
        return;
    }
    sze = sizeof(server_addr);
    cout << "[*] Looking for clients." << endl;
    listen(client, 1);
    clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&sze);
    // first check if it is valid or not
    if (server < 0)
        cout << "[!] Error accepting client connection." << endl;
}

void Server::sendRecvMsg()
{
    do {
        strcpy(buffer, "[:)] Server connected!");
        send(server, buffer, BUFSIZE, 0);
        cout << "[*] Connected with the client. " << clientCount << endl;
        cout << "[*] Waiting for username exchange...please hold on." <<endl;
        RecvUsername();
        SendUsername();
        cout << "[:)] Username received! " <<endl;
        InitLog(extUsername);
        cout << "[*] Enter # (i.e. Shift + 3), to end the connection." << endl;
        cout<<"Client's message goes first. Wait for a message..."<<endl;
       do {
            cout << endl << extUsername <<": ";
             do {
                 recv(server, buffer, BUFSIZE, 0);
                 cout << buffer << " ";
                 writeLog(username, buffer);
                 if (*buffer == '#') {
                     *buffer == '*';
                     isExit = true;
                 }
             } while (*buffer != '*');
             cout << endl <<"You: ";
             do {
                 cin >> buffer;
                 send(server, buffer, BUFSIZE, 0);
                 writeLog(username, buffer);
                 if (*buffer == '#') {
                     send(server, buffer, BUFSIZE, 0);
                     *buffer = '*';
                     isExit = true;
                 }
             } while (*buffer != '*');


         } while (!isExit);
    killSock();
    }while(!isExit);
}

int main(){
    Server s;
    s.InitWinsock();
    s.CreateSocket();
    s.BindSocket();
    s.sendRecvMsg();
    return 0;
}
