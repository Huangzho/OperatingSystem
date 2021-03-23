#ifndef COPYTHREAD_H
#define COPYTHREAD_H
#include<QThread>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>


class copyThread:public QThread
{
public:
    int N,size;
    int shmid1,semid1;
    int shmid2,semid2;

    long &n;
    bool &is_over;
    copyThread(int a,int b,int sh1,int se1,int sh2,int se2,long &n,bool &s);
protected:
    void run();
};

#endif // COPYTHREAD_H
