#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

void FindData(const string& Data, const string& Tag)
{
    cout << Tag << ": ";
    for (auto i = Data.begin() + Data.find(Tag) + Tag.length() + 2; *i != ',' && *i != '}'; i++)
    {
        cout << *i;
    }
    cout << "\n";
}

int main()
{
    WSADATA WSAdata;
    SOCKET Sock = INVALID_SOCKET;
    addrinfo SockAddr;
    addrinfo* Result = NULL;
    addrinfo* ptr = NULL;
    const int SIZE = 1024;
    WSAStartup(MAKEWORD(2, 2), &WSAdata);
    char HOST[SIZE] = "api.openweathermap.org";
    ZeroMemory(&SockAddr, sizeof(SockAddr));
    SockAddr.ai_family = AF_INET;
    SockAddr.ai_socktype = SOCK_STREAM;
    SockAddr.ai_protocol = IPPROTO_TCP;
    getaddrinfo(HOST, "http", &SockAddr, &Result);
    for (ptr = Result; ptr != NULL; ptr = ptr->ai_next)
    {
        Sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        connect(Sock, ptr->ai_addr, (int)ptr->ai_addrlen);
    }
    string URI = "/data/2.5/weather?q=Odessa&appid=75f6e64d49db78658d09cb5ab201e483&mode=JSON";
    string Request = "GET " + URI + " HTTP/1.1" + "\n";
    Request += "Host: " + string(HOST) + "\n";
    Request += "Accept: */*";
    Request += "Accept-Encoding: gzip, deflate, br";
    Request += "Connection: close";
    Request += "\n\n";
    send(Sock, Request.c_str(), Request.length(), NULL);
    string Response;
    char BUFF[SIZE];
    int ResponseLength;
    do
    {
        ResponseLength = recv(Sock, BUFF, SIZE, NULL);
        if (ResponseLength > NULL)
        {
            Response += string(BUFF).substr(NULL, ResponseLength);
        }
    } while (ResponseLength == SIZE);
    FindData(Response, "id");
    FindData(Response, "name");
    FindData(Response, "country");
    cout << "\n";
    FindData(Response, "lon");
    FindData(Response, "lat");
    cout << "\n";
    FindData(Response, "temp_min");
    FindData(Response, "temp_max");
    cout << "\n";
    FindData(Response, "sunset");
    FindData(Response, "sunrise");
    shutdown(Sock, SD_BOTH);
    closesocket(Sock);
    WSACleanup();
    return(NULL);
}
