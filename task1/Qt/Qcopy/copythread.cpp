#include "copythread.h"
void P(int semid,unsigned short index)//PV操作
{
    struct sembuf sem;
    sem.sem_num=index;
    sem.sem_op=-1;
    sem.sem_flg=0;
    semop(semid,&sem,1);
}
void V(int semid,unsigned short index)
{
    struct sembuf sem;
    sem.sem_num=index;
    sem.sem_op=1;
    sem.sem_flg=0;
    semop(semid,&sem,1);
}
copyThread::copyThread(int a,int b,int sh1,int se1,int sh2,int se2,long &nr,bool &s):n(nr),is_over(s)
{
    N=a;size=b;
    shmid1=sh1;semid1=se1;
    shmid2=sh2;semid2=se2;


}
void copyThread::run()
{

    int i;
    char *virtaddr1,*virtaddr2;
    unsigned int count=0;


    virtaddr1=(char*)shmat(shmid1,NULL,0);
    virtaddr2=(char*)shmat(shmid2,NULL,0);

    char **a=new char*[N];
    char **b=new char*[N];
    for(i=0;i<N;i++) a[i]=virtaddr1+i*size;//循环缓冲区构建
    for(i=0;i<N;i++) b[i]=virtaddr2+i*size;

    i=0;
    while(1){
        P(semid1,1);
        P(semid2,0);

        memcpy(b[i],a[i],size);
        count=*(unsigned int*)a[i];


        V(semid2,1);
        V(semid1,0);

        if(count<size-4) break;
        i++;
        n++;//all capacity
        if(i==N) i=0;//a loop
    }
    is_over=true;
    delete [] a;
    delete [] b;
    exit(0);

}
