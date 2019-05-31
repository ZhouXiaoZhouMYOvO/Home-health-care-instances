#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<iostream>
using namespace std;
int pid1, pid2, pid3;

int main()
{
	int fd[2];		//供进程使用的文件描述符数组
	char outpipe[100], inpipe[100];
	//outpipe存储输出到管道的消息的变量，inpipe存储输出到显示屏消息的变量
	pipe(fd);           //创建一个管道
	if ((pid1 = fork()) == -1) { printf("failed!"); }   //调用fork()，创建子进程1失败
	if (pid1 == 0)
	{	//创建子进程成功，从子进程返回的ID
		lockf(fd[1], 1, 0);       //对管道加锁
		sprintf(outpipe, "child1 process is sending message!");
		write(fd[1], outpipe, 50);        //子进程1向管道写消息
		lockf(fd[1], 0, 0);       //解除管道的锁定
		exit(0);
	}
	else {
		if ((pid2 = fork()) == -1) { printf("failed!"); }     //调用fork(),创建子进程2失败
		if (pid2 == 0)
		{   //创建子进程2成功，从子进程2返回的ID
			lockf(fd[1], 1, 0);       //对管道加锁
			sprintf(outpipe, "child2 process is sending message!");
			write(fd[1], outpipe, 50);  //子进程2向管道写消息
			lockf(fd[1], 0, 0);
			exit(0);
		}
		else {
			if ((pid3 = fork()) == -1) { printf("failed!"); }
			//调用fork(),创建子进程3失败
			if (pid3 == 0)
			{  //创建子进程3成功，从子进程3返回的ID
				lockf(fd[1], 1, 0);          //对管道加锁
				sprintf(outpipe, "child3 process is sending message!");
				write(fd[1], outpipe, 50);   //子进程3向管道写消息
				lockf(fd[1], 0, 0);          //解除管道的锁定
				exit(0);
			}
			else {//输出管道中的消息
				read(fd[0], inpipe, 50);
				printf("%s\n", inpipe);
				read(fd[0], inpipe, 50);
				printf("%s\n", inpipe);
				read(fd[0], inpipe, 50);
				printf("%s\n", inpipe);
				exit(0);
			}
		}
	}
}


