#include "getthread.h"

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
getThread::getThread(int a,int b,int c,int d,char *e,long &x,bool& y):n(x),is_over(y)
{

    N=a;
    size=b;
    shmid=c;
    semid=d;
    file=e;
}
void getThread::run()
{
    int i;
    char *virtaddr;
    int count;

    virtaddr=(char*)shmat(shmid,NULL,0);//缓冲区引入
    char **a=new char*[N];
    for(i=0;i<N;i++) a[i]=virtaddr+i*size;//环形缓冲区构建
    int fp=open(file,O_RDONLY);

    i=0;
    while(1){
            P(semid,0);

            count=read(fp,a[i]+4,size-4);//写入缓冲区
            *(unsigned int*)a[i]=count;//前四个字节为读取大小
                      //printf("get:i=%d %d\n",i,count);

            V(semid,1);

            if(count<size-4) break;
            n++;
            i++;
            if(i==N) i=0;//loop
    }
    delete [] a;
    is_over=true;
    close(fp);
    exit(0);

}
