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
void P(int semid,int index)//PV操作
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
	int N,size,shmid,semid;
	int i;
	char *virtaddr;
	unsigned int count;

	N=atoi(argc[0]);//参数读取
	size=atoi(argc[1]);
	shmid=atoi(argc[2]);
	semid=atoi(argc[3]);
	virtaddr=shmat(shmid,NULL,0);//缓冲区引入
	char *a[N];
	for(i=0;i<N;i++) a[i]=virtaddr+i*size;//环形缓冲区构建
	int fp=open(argc[4],O_RDONLY);

	i=0;
	while(1){
		P(semid,0);

		count=read(fp,a[i]+4,size-4);//写入缓冲区
		*(unsigned int*)a[i]=count;//前四个字节为读取大小
                          //printf("get:i=%d %d\n",i,count);

		V(semid,1);

		if(count<size-4) break;
		i++;
		if(i==N) i=0;
	}
	close(fp);
	exit(0);
}
