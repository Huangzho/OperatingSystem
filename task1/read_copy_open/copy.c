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
void P(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num=index;
	sem.sem_op=-1;
	sem.sem_flg=0;
	semop(semid,&sem,1);
}
void V(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num=index;
	sem.sem_op=1;
	sem.sem_flg=0;
	semop(semid,&sem,1);
}

int main(int argv,char *argc[])
{
	int N,size;
	int shmid1,semid1;
	int shmid2,semid2;
	int i;
	char *virtaddr1,*virtaddr2;
    unsigned int count=0;

	N=atoi(argc[0]);//参数转化
	size=atoi(argc[1]);
	shmid1=atoi(argc[2]);
	semid1=atoi(argc[3]);
	shmid2=atoi(argc[4]);
	semid2=atoi(argc[5]);
	virtaddr1=shmat(shmid1,NULL,0);
    virtaddr2=shmat(shmid2,NULL,0);

	char *a[N],*b[N];
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
		if(i==N) i=0;
	}
	exit(0);
}
