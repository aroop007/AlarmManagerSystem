#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Alarm.h"

using namespace std;

void *cli_loop(void *arg)
{
    int chid = *(int*)arg;

    char input[100];

    pthread_mutex_lock(&printmutex);

    cout<<"Alarm Manager Running"<<endl;

    pthread_mutex_unlock(&printmutex);

    while(1)
    {
        pthread_mutex_lock(&printmutex);

        cout<<"Commands: \n1.add \n2.list \n3.remove \n4.exit"<<endl;

        cout<<"Enter Command: ";

        fflush(stdout);

        pthread_mutex_unlock(&printmutex);

        if(fgets(input, sizeof(input), stdin) == NULL)
            continue;

        input[strcspn(input, "\n")] = 0;

        if(strcmp(input, "add") == 0)
        {
            int sec, periodic;

            pthread_mutex_lock(&printmutex);

            cout<<"Seconds: ";

            fflush(stdout);
            cin>>sec;

            cout<<"Periodic (Yes(1) or No(0)): ";
            fflush(stdout);
            cin>>periodic;

            getchar();

            pthread_mutex_unlock(&printmutex);

            createalarm(chid, sec, periodic);
        }

        else
            if(strcmp(input, "list") == 0)
                listalarm();

            else
                if(strcmp(input, "remove") == 0)
                {
                    pthread_mutex_lock(&printmutex);

                    int id;

                    cout<<"Enter Alarm id: ";
                    fflush(stdout);
                    cin>>id;

                    getchar();

                    pthread_mutex_unlock(&printmutex);

                    removealarm(id);
                }

                else
                    if(strcmp(input, "exit") == 0)
                    {
                        pthread_mutex_lock(&printmutex);

                        cout<<"Exiting..."<<endl;

                        pthread_mutex_unlock(&printmutex);

                        exit(0);
                    }
    }

    return 0;

}
