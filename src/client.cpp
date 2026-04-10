#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 5000

void put_data(int sock, const string &msg)
{
    send(sock, msg.c_str(), msg.length(), 0);
}

int main()
{
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        cout << "Socket creation failed\n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        cout << "Invalid address\n";
        return -1;
    }

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "Connection failed\n";
        return -1;
    }

    cout << "Connected to Server\n";

    int temperature = 50;

    while(1)
    {
        sleep(5);

        temperature += 10;

        cout << "[TEMP] Engine Temperature = " << temperature << "°C\n";

        if(temperature >= 150)
        {
            cout << "THRESHOLD EXCEEDED!\n";
            put_data(sock, "TEMP_OVERHEAT");
        }
        else
        {
            put_data(sock, "TEMP_NORMAL");
        }

        if(temperature > 180)
        {
            cout << "[TEMP] Resetting...\n";
            temperature = 50;
        }
    }

    close(sock);
    return 0;
}