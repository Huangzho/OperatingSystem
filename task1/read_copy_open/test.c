#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/wait.h>
#include<unistd.h>
int N=10,size=1024;


int main(int argv,char *argc[])//argc获取文件名
{
	int shmid1,semid1,shmid2,semid2;
	pid_t p1,p2,p3;

	char str_N[20],str_size[20],str_shmid1[20],str_semid1[20],str_shmid2[20],str_semid2[20];
	shmid1=shmget(IPC_PRIVATE,N*size,IPC_CREAT|0666);//创建缓冲区
	semid1=semget(IPC_PRIVATE,2,0666);//创建信号灯
	shmid2=shmget(IPC_PRIVATE,N*size,IPC_CREAT|0666);//创建缓冲区
	semid2=semget(IPC_PRIVATE,2,0666);//创建信号灯

	sprintf(str_N,"%d",N);//向get与put传递信息
	sprintf(str_size,"%d",size);
	sprintf(str_shmid1,"%d",shmid1);	
	sprintf(str_semid1,"%d",semid1);
	sprintf(str_shmid2,"%d",shmid2);	
	sprintf(str_semid2,"%d",semid2);

	semctl(semid1,0,SETVAL,N);//写信号灯赋值
	semctl(semid1,1,SETVAL,0);//读
	semctl(semid2,0,SETVAL,N);//写信号灯赋值
	semctl(semid2,1,SETVAL,0);//读
	p1=fork();
	if(!p1){
		puts("get created\n");
		execl("./get",str_N,str_size,str_shmid1,str_semid1,argc[1],NULL);
	}
	else{
		p2=fork();
		if(!p2){
			puts("copy created\n");
			execl("./copy",str_N,str_size,str_shmid1,str_semid1,str_shmid2,str_semid2,NULL);
		}
		else{
			p3=fork();
			if(!p3){
				puts("put cireated\n");
				execl("./put",str_N,str_size,str_shmid2,str_semid2,argc[2],NULL);
			}
			else{
				wait(NULL);//等待子进程结束
				wait(NULL);
				wait(NULL);
				semctl(semid1,0,IPC_RMID);//关闭信号灯
				shmctl(shmid1,IPC_RMID,0);//关闭缓冲区
				semctl(semid2,0,IPC_RMID);//关闭信号灯
				shmctl(shmid2,IPC_RMID,0);//关闭缓冲区
				return 0;
			}
		}
	}
}
