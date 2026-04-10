#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <sys/neutrino.h>
#include "Alarm.h"

pthread_mutex_t alarmmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t printmutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_t cli_thread;
    pthread_t notify_thread;
    pthread_t server_thread;
    pthread_t scheduler_thread;

    int chid = ChannelCreate(0);

    pthread_attr_t cli_attr;
    pthread_attr_t notify_attr;

    struct sched_param cli_param;
    struct sched_param notify_param;

    pthread_attr_init(&cli_attr);
    pthread_attr_init(&notify_attr);

    pthread_attr_setschedpolicy(&cli_attr, SCHED_RR);
    pthread_attr_setschedpolicy(&notify_attr, SCHED_RR);

    cli_param.sched_priority = 10;
    notify_param.sched_priority = 20;

    pthread_attr_setschedparam(&cli_attr, &cli_param);
    pthread_attr_setschedparam(&notify_attr, &notify_param);

    pthread_attr_setinheritsched(&cli_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setinheritsched(&notify_attr, PTHREAD_EXPLICIT_SCHED);

    pthread_create(&notify_thread, &notify_attr, notificationtask, &chid);
    pthread_create(&cli_thread, &cli_attr, cli_loop, &chid);
    pthread_create(&server_thread, NULL, tcp_server, &chid);
    pthread_create(&scheduler_thread, NULL, scheduler, &chid);

    pthread_join(cli_thread, NULL);
    pthread_join(notify_thread, NULL);
    pthread_join(server_thread, NULL);
    pthread_join(scheduler_thread, NULL);

    return 0;
}
