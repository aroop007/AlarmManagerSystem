#include <iostream>
#include <unistd.h>
#include "Alarm.h"

using namespace std;

void *scheduler(void *arg)
{
    int chid = *(int*)arg;

    while(1)
    {
        pthread_mutex_lock(&queuemutex);

        if(!requestQueue.empty())
        {
            Request req = requestQueue.top();
            requestQueue.pop();

            pthread_mutex_unlock(&queuemutex);

            handle_event((char*)req.message.c_str(), chid);
        }
        else
        {
            pthread_mutex_unlock(&queuemutex);
        }

        usleep(200000);
    }

    return NULL;
}
