#include "putthread.h"
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
putThread::putThread(int a,int b,int c,int d,char *e,long &x,bool& y):n(x),is_over(y)
{
    N=a;
    size=b;
    shmid=c;
    semid=d;
    file=e;

}
void putThread::run()
{
         int i;
         char *virtaddr;
         int count;


         virtaddr=(char*)shmat(shmid,NULL,0);
         char **a=new char*[N];
         for(i=0;i<N;i++) a[i]=virtaddr+i*size;//循环缓冲区构建

         int fp=open(file,O_WRONLY|O_CREAT|O_TRUNC);//只写模式，如无文件创建，如有内容消除
         if(!fp) printf("can't open\n");

         i=0;
         while(1){
                 P(semid,1);

                 count=write(fp,a[i]+4,*(int*)a[i]);

                 V(semid,0);
                 if(count<size-4) break;
                 n++;
                 i++;
                 if(i==N) i=0;//loop
         }
         close(fp);
         is_over=true;
         delete [] a;
         chmod(file,0766);
         exit(0);


}
