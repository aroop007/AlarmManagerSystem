#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Alarm.h"

using namespace std;

#define PORT 5000

priority_queue<Request> requestQueue;
pthread_mutex_t queuemutex = PTHREAD_MUTEX_INITIALIZER;

int get_priority(const char *msg)
{
    if(strcmp(msg, "TEMP_OVERHEAT") == 0) return 1;
    if(strcmp(msg, "FUEL_LOW") == 0) return 2;
    if(strcmp(msg, "DOOR_OPEN") == 0) return 3;
    if(strcmp(msg, "SEATBELT_NOT_FASTENED") == 0) return 4;
    return 5;
}

void *tcp_server(void *arg)
{
    int chid = *(int*)arg;

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server running on port " << PORT << endl;

    while(1)
    {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if(new_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        while(1)
        {
            memset(buffer, 0, sizeof(buffer));
            int valread = read(new_socket, buffer, sizeof(buffer));

            if(valread <= 0)
                break;

            cout << "[RECEIVED] " << buffer << endl;

            int priority = get_priority(buffer);

            pthread_mutex_lock(&queuemutex);
            requestQueue.push({priority, buffer});
            pthread_mutex_unlock(&queuemutex);
        }

        close(new_socket);
    }

    return NULL;
}
