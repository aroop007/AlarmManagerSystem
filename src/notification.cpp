#include <iostream>
#include <sys/neutrino.h>
#include <time.h>
#include "Alarm.h"

using namespace std;

void *notificationtask(void *arg)
{
    int chid = *(int*)arg;

    struct _pulse pulse;

    while(1)
    {
        MsgReceive(chid, &pulse, sizeof(pulse), NULL);

        if(pulse.code == 1)
        {
            int id = pulse.value.sival_int;

            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            pthread_mutex_lock(&printmutex);

            cout<<"\n["<<t -> tm_hour<<":"<<t -> tm_min<<":"<<t -> tm_sec<<"] Alarm "<<id<<" Triggered ("<<(alarms[id].periodic ? "Periodic" : "One-Shot")<<")"<<endl;

            cout<<"Enter Command: ";
            fflush(stdout);

            pthread_mutex_unlock(&printmutex);
        }
    }

    return NULL;
}
