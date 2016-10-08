#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

struct Chat
{
    char username[20];
    char message[50]; //note to self: make this a pointer
};

void msgToFile(Chat sendmsg)
{
    // writing message to file
    ofstream tofile;
    tofile.open("chat.dat", ios::binary | ios::out);
    tofile.write((char*)&sendmsg.username, sizeof(sendmsg.username));
    tofile.write((char*)&sendmsg.message, sizeof(sendmsg.message));
    tofile.close();
}

void getMsg()
{
    Chat getMessage;

    ifstream fromfile;
    fromfile.open("chat.dat", ios::binary | ios::in);
    // honestly, I don't know how to do this reading crap
    fromfile.close();
}

int main()
{
    Chat userEntry;
    std::cout << "\t\t\t\tFileChat\n\n";
    std::cout << "\t\tGrade 12 Semester 2 Project\n\n\n";
    std::cout << "Enter the username you wish to use: ";
    gets(userEntry.username);
    // need to do some sort of loop her;
    std::cout << "Message: ";
    gets(userEntry.message);

    msgToFile(userEntry);
    return 0;
}


