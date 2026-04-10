#ifndef ALARM_H
#define ALARM_H

#include <pthread.h>
#include <time.h>

#define MAX_ALARMS 10

typedef struct
{
    int id, seconds, periodic;

    timer_t timer_id;
} Alarm;

extern Alarm alarms[MAX_ALARMS];
extern int alarmcount;

extern pthread_mutex_t alarmmutex;
extern pthread_mutex_t printmutex;

void createalarm(int chid, int seconds, int periodic);
void listalarm();
void removealarm(int chid);

void *notificationtask(void *arg);
void *cli_loop(void *arg);

#endif
