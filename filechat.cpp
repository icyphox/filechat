#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <winsock2.h>
#include <time.h>

using namespace std;

fstream ifof;
ofstream cr8;

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

void dispChat(char* name, int ch=0)
{
    Log l1;
    fstream fin;
    char nof[120];
    strcpy(nof, name);
    strcat(nof, ".hist");
    fin.open(nof, ios::in|ios::binary);
    while(fin)
    {
        fin.read((char*)&l1, sizeof(Log));
        cout<<"["<<l1.hour<<":"<<l1.minute<<":"<<l1.sec<<"|";
        cout<<l1.day<<"/"<<l1.mon<<"/"<<l1.year<<"] ";
        cout<<l1.User<<": "<<l1.message<<endl;
    }
    fin.close();
    if(ch==1){
        cout<<endl<<"Press any key to continue..."<<endl;
        cin.sync();
        cin.get();
    }
    // cout<<endl<<"END OF FILE. THANKS"<<endl;
    // cin.get();
}

void deleteChats(char* name, int d=0, int m=0, int y=0)
{
    char nof[120];
    strcpy(nof, name);
    strcat(nof, ".hist");
    if(d==0 && m==0 && y==0)
    {
        remove(nof);
        cout<<"[*] History of chats with "<<name<<" deleted! Press any key to continue..."<<endl;
        cin.sync();
        cin.get();
        return;
    }
    ifstream fin; ofstream fout;
    fin.open(nof, ios::in|ios::binary);
    fout.open("temp.hist", ios::out|ios::binary);
    Log l1;
    while(fin)
    {
        fin.read((char*)&l1, sizeof(Log));
        if(l1.day==d && l1.mon==m && l1.year==y)
            continue;
        fout.write((char*)&l1, sizeof(Log));
    }
    fin.close();
    fout.close();
    remove(nof);
    rename("temp.hist", nof);
    cout<<"[*] Removed all messages conveyed on the date entered! Press any key to continue..."<<endl;
    cin.sync();
    cin.get();
}

void renameChats(char* nuname, char* ouname)
{
    char nof[120], newfile[120];
    strcpy(nof, ouname);
    strcat(nof, ".hist");
    strcpy(newfile, nuname);
    strcat(newfile, ".hist");
    ifstream fin;
    ofstream fout;
    fin.open(nof, ios::in|ios::binary);
    fout.open(newfile, ios::out|ios::binary);
    Log l1;
    while(fin)
    {
        fin.read((char*)&l1, sizeof(Log));
        if(strcmpi(ouname, l1.User)==0)
            strcpy(l1.User, nuname);
        fout.write((char*)&l1, sizeof(Log));
    }
    fin.close();
    fout.close();
    remove(nof);
    cout<<"[*] Username changed!..."<<endl;
    cin.sync();
    cin.get();
}

/* BEGIN SERVER CLASS DEFINITION
 *******************************************
 */

void timeStamp()
{
    int h, m, s, dd, mm, yyyy;
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    yyyy = timeinfo->tm_year + 1900;
    mm = timeinfo->tm_mon + 1;
    dd = timeinfo->tm_mday;
    h = timeinfo->tm_hour;
    m = timeinfo->tm_min;
    s = timeinfo->tm_sec;
    cout<<"["<<h<<":"<<m<<":"<<s<<"|";
    cout<<dd<<"/"<<mm<<"/"<<yyyy<<"] ";
}

 class Server{
private:

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
    int port;
    int BUFSIZE;
    int isExit;
    char buffer[1024];
    Server();

};

Server::Server()
{
    port = 1500;
    BUFSIZE = 1024;
    isExit = 0;
}

void Server::InitWinsock(){
    cout << "[*] Initialising Winsock." << endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout << "[!] Failed. Error Code:" << WSAGetLastError();
        return;
    }

    cout << "[:)] Initialised!" << endl;
}

void Server::InitLog(char* name)
{
    dispChat(extUsername);
    char filename[120];
    strcpy(filename, name);
    strcat(filename, ".hist");
    cr8.open(filename, ios::out|ios::app);
    cr8.close();
}

void Server::writeLog(char* mesg, char* uname)
{
    char filename[120];
    strcpy(filename, extUsername);
    strcat(filename, ".hist");
    ifof.open(filename, ios::in|ios::out|ios::binary|ios::app);
    Log l1;
    strcpy(l1.User, uname);
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    l1.year = timeinfo->tm_year + 1900;
    l1.mon = timeinfo->tm_mon + 1;
    l1.day = timeinfo->tm_mday;
    l1.hour = timeinfo->tm_hour;
    l1.minute = timeinfo->tm_min;
    l1.sec = timeinfo->tm_sec;
    strcpy(l1.message, mesg);
    ifof.write((char*)&l1, sizeof(Log));
    ifof.close();
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
    cout << "[*] Enter the username you wish to use..." <<endl;
    cin>>username;
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
        cout << "[*] Enter # (i.e. Shift + 3), to end the connection." << endl;
        cout<<"[*] Client's message goes first. Wait for a message..."<<endl<<endl;
        char msg[1024];
        InitLog(extUsername);
        do {
            fflush(stdin);
            timeStamp();
            cout << extUsername <<": ";
            msg[0]='\0';
             do {
                 recv(server, buffer, BUFSIZE, 0);
                 cout << buffer;
                 strcat(msg, buffer);
                 if (*buffer == '#') {
                     *buffer == '*';
                     isExit = true;
                 }
             } while (*buffer != 10 && *buffer != 13);
             if(strcmp(msg, "")==0)
             writeLog(msg, extUsername);
             msg[0] = '\0';
             timeStamp();
             cout  << "You: ";
            fflush(stdin);
            do {
                 fgets(buffer, 1024, stdin);
                 send(server, buffer, BUFSIZE, 0);
                 strcat(msg, buffer);
                 if (*buffer == '#') {
                     send(server, "#", BUFSIZE, 0);
                     *buffer = '*';
                     isExit = true;
                 }
             } while (*buffer != 10 && *buffer != 13);
             if(strcmp(msg, "")==0)
              writeLog(msg, "You");
         } while (!isExit);
    killSock();
    }while(!isExit);
}
 /* END SERVER CLASS DEFINITION
  *********************************************************************************
  */

/* BEGIN CLIENT CLASS DEFINITION
 **********************************************************************************
 */

class Client{
private:
    fstream ifof;
    SOCKET client;
    int port;
    char username[120];
    char extUsername[120];
    int BUFSIZE;
    int isExit;
    char buffer[1024];
    char ip[12];
    struct sockaddr_in serverAddr;
    WSADATA wsa;
    void KillSock();

public:
	int InitWinsock();
	void CreateSocket();
	void SendUsername();
	void RecvUsername();
	void CloseLog();
	void InitLog(char*);
	void writeLog(char*, char*);
	int Connekt();
	void sendRecvMsg();
	Client();

};
Client::Client()
{
    port  = 1500;
    BUFSIZE = 1024;
    isExit = 0;
}
void Client::InitLog(char* name)
{
    dispChat(extUsername);
    char filename[120];
    strcpy(filename, name);
    strcat(filename, ".hist");
    cr8.open(filename, ios::out|ios::app);
    cr8.close();
}

void Client::CloseLog()
{
    ifof.close();
}
int Client::InitWinsock()
{
	cout << "[*] Initialising Winsock." << endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout << "[!] Failed. Error Code:" << WSAGetLastError();
        return 1;
    }
}

void Client::SendUsername()
{
    cout << "[*] Enter the username you wish to use..." <<endl;
    cin>>username;
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
    cin>>ip;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr=inet_addr(ip);
    serverAddr.sin_port = htons(port); // htons() converts port number to network byte order from host byte order
    // connecting to the socket happens here
    if(connect(client, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout<<"[!] connect function failed with error: \n"<<WSAGetLastError();
        cout << "[!] Failed to connect to port " << port << " on " << ip << endl;
        return 1;
    }
}

void Client::writeLog(char* mesg, char* uname)
{
    char filename[120];
    strcpy(filename, extUsername);
    strcat(filename, ".hist");
    ifof.open(filename, ios::in|ios::out|ios::binary|ios::app);
    Log l1;
    strcpy(l1.User, uname);
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
    ifof.close();
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
    cout<<"[*] Client's message goes first. Say hi!"<<endl<<endl;
    InitLog(extUsername);
    char msg[1024];
    do {
        timeStamp();
        cout <<"You: ";
        fflush(stdin);
        msg[0]='\0';
        do {
            fgets(buffer, 1024, stdin);
            strcat(msg, buffer);
            send(client, buffer, BUFSIZE, 0);
            if (*buffer == '#') {
                send(client, buffer, BUFSIZE, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 10 && *buffer != 13);
        if(strcmp(msg, "")==0)
        writeLog(msg, "You");
        timeStamp();
        cout << extUsername << ": ";
        msg[0]='\0';
        do {
            recv(client, buffer, BUFSIZE, 0);
            cout << buffer;
            strcat(msg, buffer);
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 10 && *buffer != 13);
        if(strcmp(msg, "")==0)
        writeLog(msg, extUsername);
    } while (!isExit);

    KillSock();
}

void Client::KillSock()
{
        CloseLog();
        closesocket(client);
	    WSACleanup();
        cout << "[!] Connection terminated with server.";
}

/* END CLIENT CLASS DEFINITION
 ****************************************************************************
 */

int main()
{
    int ch, flag = 0;
    while(1){
    do{
       if(flag){
           system("cls");
           cout<<"[!] Please enter a valid choice..."<<endl;
       }
       cout<<"-------------------------FILECHAT - C++ Chat Using Sockets -------------------------"<<endl;
       cout<<"[1] Host a server"<<endl;
       cout<<"[2] Join a server"<<endl;
       cout<<"[3] Display history of a particular chat"<<endl;
       cout<<"[4] Delete messages selectively"<<endl;
       cout<<"[5] Delete all messages from a particular user"<<endl;
       cout<<"[6] Change username of existing chat history"<<endl;
       cout<<"[7] Exit"<<endl;
       cout<<"[*] Please enter your choice"<<endl;
       cin>>ch;
       if(ch<1 || ch>7)
           flag = 1;
    }while(ch<1 || ch>7);
    Server s; Client clnt;
    char nop[120], nonp[120];
    int d, m, y;
    switch(ch)
    {
        case 1:
               s.InitWinsock();
               s.CreateSocket();
               s.BindSocket();
               s.sendRecvMsg();
        break;
        case 2:
                clnt.InitWinsock();
                clnt.CreateSocket();
                clnt.Connekt();
                clnt.sendRecvMsg();
        break;
        case 3: cout<<"[*] Enter name of the person whose chats you want to view..."<<endl;
                cin>>nop;
                dispChat(nop, 1);
        break;
        case 4: cout<<"[*] Enter name of the person whose chats you want to delete..."<<endl;
                cin>>nop;
                cout<<"[*] Enter the date of the messages you want to delete in the format DD MM YYYY"<<endl;
                cin>>d>>m>>y;
                deleteChats(nop, d, m, y);
        break;
        case 5: cout<<"[*] Enter the name of the person whose chats you want to delete..."<<endl;
                cin>>nop;
                deleteChats(nop);
        break;
        case 6: cout<<"[*] Enter the old username of the person"<<endl;
                cin>>nop;
                cout<<"[*] Enter the new username"<<endl;
                cin>>nonp;
                renameChats(nonp, nop);
        break;
        case 7: cout<<"[:)] Thanks for using! Press any key to exit..."<<endl;
                cin.sync();
                cin.get();
                exit(0);
        break;
    }
        system("cls");
    }
}




