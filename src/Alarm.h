#ifndef ALARM_H
#define ALARM_H

#include <pthread.h>
#include <time.h>
#include <string>
#include <queue>

#define MAX_ALARMS 10

using namespace std;

typedef struct
{
    int id, seconds, periodic;
    timer_t timer_id;
} Alarm;

typedef struct Request
{
    int priority;
    string message;

    bool operator<(const Request &other) const
    {
        return priority > other.priority;
    }
} Request;

extern priority_queue<Request> requestQueue;
extern pthread_mutex_t queuemutex;

extern Alarm alarms[MAX_ALARMS];
extern int alarmcount;

extern pthread_mutex_t alarmmutex;
extern pthread_mutex_t printmutex;
extern pthread_mutex_t queuemutex;

void createalarm(int chid, int seconds, int periodic);
void listalarm();
void removealarm(int chid);

void *notificationtask(void *arg);
void *cli_loop(void *arg);
void *tcp_server(void *arg);
void *scheduler(void *arg);

void handle_event(char *msg, int chid);

#endif
