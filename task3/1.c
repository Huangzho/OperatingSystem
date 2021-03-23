#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/ioctl.h>
int main()
{
	int testdev;
	int i;
	char buf[10];
	char a[10];
	testdev=open("/dev/dev_T",O_RDWR);
	if(testdev==-1){
		printf("wrong\n");
		return 0;
	}
        for(int i=0;i<10;i++){
		buf[i]=i;
	}
	lseek(testdev, 0, SEEK_SET);//to the begining
	write(testdev,buf,10);//写入0-9

	lseek(testdev, 0, SEEK_SET);//to the begining
	read(testdev,a,4);
	printf("first read\n");
	for(int i=0;i<4;i++)//读出前四个
		printf("%d:%d \n",i,a[i]);

	printf("second read\n");
	read(testdev,a,6);
	for(int i=0;i<6;i++)//读出后6个
		printf("%d:%d\n",i+4,a[i]);

	ioctl(testdev,1);//清空
	lseek(testdev, 0, SEEK_SET);//to the begining
	read(testdev,a,10);
	printf("third read\n");
	for(int i=0;i<10;i++) printf("%d:%d\n",i,a[i]);//全读取
	close(testdev);

	return 0;
}
