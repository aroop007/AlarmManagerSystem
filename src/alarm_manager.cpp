#include <iostream>
#include <signal.h>
#include <sys/neutrino.h>
#include "Alarm.h"
#include <cstring>
#include <unistd.h>

using namespace std;

Alarm alarms[MAX_ALARMS];
int alarmcount = 0;

void createalarm(int chid, int seconds, int periodic)
{
    pthread_mutex_lock(&alarmmutex);

    if(alarmcount >= MAX_ALARMS)
    {
        pthread_mutex_lock(&printmutex);

        cout<<"Maximum alarms reached  :("<<endl;

        pthread_mutex_unlock(&printmutex);
        pthread_mutex_unlock(&alarmmutex);

        return;
    }

    Alarm *alarm = &alarms[alarmcount];

    alarm -> id = alarmcount;
    alarm -> seconds = seconds;
    alarm -> periodic = periodic;

    struct sigevent event;

    int coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);

    if(coid == -1)
    {
        perror("ConnectAttach failed");   // DEBUG
        return;
    }

    SIGEV_PULSE_INIT(&event, coid,
                     SIGEV_PULSE_PRIO_INHERIT, 1, alarm->id);

    timer_create(CLOCK_REALTIME, &event, &alarm -> timer_id);

    struct itimerspec itime;
    itime.it_value.tv_sec = seconds;
    itime.it_value.tv_nsec = 0;

    if(periodic)
        itime.it_interval.tv_sec = seconds;
    else
        itime.it_interval.tv_sec =  0;

    itime.it_interval.tv_nsec = 0;

    timer_settime(alarm -> timer_id, 0, &itime, NULL);

    pthread_mutex_lock(&printmutex);
    cout<<"Alarm "<<alarm -> id<<" set for "<<seconds<<" ("<<(periodic ? "periodic" : "One-shot")<<")"<<endl;

    pthread_mutex_unlock(&printmutex);

    alarmcount++;

    pthread_mutex_unlock(&alarmmutex);
}

void listalarm()
{
    pthread_mutex_lock(&printmutex);

    cout<<"\nActive Alarms"<<endl;
    cout<<"---------------------"<<endl;

    for(int i = 0; i < alarmcount; i++)
    {
        cout<<"ID: "<<alarms[i].id<<" Time: "<<alarms[i].seconds<<" Type: "<<(alarms[i].periodic ? "periodic" : "One-shot")<<endl;
    }

    pthread_mutex_unlock(&printmutex);
}

void removealarm(int id)
{
	pthread_mutex_lock(&alarmmutex);

	if(id >= alarmcount)
    {
		pthread_mutex_lock(&printmutex);

        cout<<"Invalid alarm ID :()"<<endl;

        pthread_mutex_unlock(&printmutex);
        pthread_mutex_unlock(&alarmmutex);

        return;
    }

	timer_delete(alarms[id].timer_id);

	for(int i = id; i < alarmcount - 1; i++)
	{
		alarms[i] = alarms[i + 1];
		alarms[i].id = i;
	}

	alarmcount--;

	pthread_mutex_lock(&printmutex);

	cout<<"Alarm "<<id<<" Removed"<<endl;

	pthread_mutex_unlock(&printmutex);

	pthread_mutex_unlock(&alarmmutex);
}

void handle_event(char *msg, int chid)
{
    cout<<"[PROCESSING] "<<msg<<endl;

    if(strcmp(msg, "Temp_Overheat") == 0)
    {
        createalarm(chid, 2, 0);
        sleep(3);
        removealarm(chid);
    }
    else if(strcmp(msg, "Fuel_Low") == 0)
    {
        createalarm(chid, 5, 0);
        sleep(6);
        removealarm(chid);
    }
    else if(strcmp(msg, "Door_Open") == 0)
    {
        createalarm(chid, 3, 0);
        sleep(4);
        removealarm(chid);

    }
    /*else if(strcmp(msg, "Seatbelt_Not_Fastened") == 0)
    {
        createalarm(chid, 4, 0);
        sleep(5);
        removealarm(chid);
    }*/
    else
    {
        cout<<"[INFO] Normal condition: "<<msg<<endl;
    }
}
