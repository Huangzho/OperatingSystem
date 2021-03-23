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
	int N,size,shmid,semid;
	int i;
	char *virtaddr;
	int count;

	N=atoi(argc[0]);//参数转化
	size=atoi(argc[1]);
	shmid=atoi(argc[2]);
	semid=atoi(argc[3]);

	virtaddr=shmat(shmid,NULL,0);
	char *a[N];
	for(i=0;i<N;i++) a[i]=virtaddr+i*size;//循环缓冲区构建

	int fp=open(argc[4],O_WRONLY|O_CREAT|O_TRUNC);//只写模式，如无文件创建，如有内容消除
	if(!fp) printf("can't open\n");

	i=0;
	while(1){
		P(semid,1);

		count=write(fp,a[i]+4,*(int*)a[i]);

		V(semid,0);
		if(count<size-4) break;
		i++;
		if(i==N) i=0;
	}
	close(fp);
	chmod(argc[4],0766);
	exit(0);
}
