#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<ctime>
#include<algorithm>  //包含sort函数
using namespace std;
static int frame_num = 3;
int pid1, pid2;
float yes = 0, no = 0;
int S;//随机序列数
int N;//内存页面数
int judge;//判断是否命中

struct one_frame
{//记录内存页框的使用情况
	int page_no;      //页面号
	int num;          //记录页面进入内存的时间，值越小表示进内存时间越长
};


bool cmp(one_frame a, one_frame b)
{
	return a.num<b.num;
}

int main()
{
	printf("请输入访问序列的页面数:");
	//printf("Acess_Series_number:");
	scanf("%d", &S);
	printf("请输入为一个进程分配的最大的内存页面数:");
	scanf("%d", &N);
	struct one_frame M_Frame[N];    //结构体数组记录为进程分配的内存页面的使用情况
	struct one_frame Copy_Frame[N];
	int ran_num;
	int j = 0;
	int Acess_Series[S];    //存放页面访问序列的数组
	srand((unsigned)time(NULL));
	printf("页面访问序列为：");
	for (int s = 0; s<S; s++)
	{
		Acess_Series[s] = rand() % 5;
		cout << Acess_Series[s] << " ";
	}
	cout << endl;
	for (int s = 0; s<N; s++)
	{//初始化结构体数组
		M_Frame[s].page_no = 8;
		M_Frame[s].num = 8;
	}
	if ((pid1 = fork()) == -1) { printf("failed!"); } //创建子进程1
	if (pid1 == 0)
	{//此为子进程1
		printf("这里是LRU算法的执行结果！\n");
		for (int i = 0; i<S; i++)
		{
			if (j<N)
			{//内存页框未占满
				judge = 0;
				for (int a = 0; a<N; a++)
				{//命中，退出循环，继续访问下一个页面
					if (M_Frame[a].page_no == Acess_Series[i])
					{
						judge = 1; cout << "当前访问页面" << Acess_Series[i] << "命中" << endl; yes = yes + 1;
					}
				}
				if (judge == 0)
				{
					M_Frame[j].page_no = Acess_Series[i];//未命中，调入缺页的页面号
					M_Frame[j].num = N;                  //初始化模拟寄存器的值
					for (int k = 0; k<j; k++) { M_Frame[k].num--; }//内存中其它页面的寄存器值减一
					cout << "当前访问页面" << Acess_Series[i] << "未命中，调入内存" << endl;
					j++;
				}
			}
			else
			{//内存页框被占满
				judge = 0;
				for (int a = 0; a<N; a++)
				{
					Copy_Frame[a].page_no = M_Frame[a].page_no;
					Copy_Frame[a].num = M_Frame[a].num;
					if (M_Frame[a].page_no == Acess_Series[i])
					{//命中
						M_Frame[a].num = N;//LRU算法，命中时对应页面置初值，更新状态
						cout << "当前访问页面" << M_Frame[a].page_no << "命中，更新寄存器的值" << endl;
						for (int i = 0; i<N; i++)
						{//内存中其他页面的寄存器值减一
							if (M_Frame[i].page_no != M_Frame[a].page_no) { M_Frame[i].num--; }
						}
						yes = yes + 1;//命中次数加一
						judge = 1;   //置命中标志
					}
				}
				if (judge == 0)
				{//未命中
					int tihuan;
					sort(Copy_Frame, Copy_Frame + N, cmp);//对内存中页面的寄存器值进行排序，最小的为最近最久未使用页面，淘汰
					tihuan = Copy_Frame[0].page_no;      //应该被替换的页面号
					for (int b = 0; b<N; b++)
					{
						if (M_Frame[b].page_no == tihuan)
						{//最近最久未使用页面替换为当前访问的页面
							M_Frame[b].page_no = Acess_Series[i];
							M_Frame[b].num = N;
						}
						else { M_Frame[b].num--; }//其他页面寄存器值减一
					}
					cout << "当前访问页面" << Acess_Series[i] << ",缺页，将页面" << tihuan << "置换出内存" << endl;
				}
			}
			for (int i = 0; i<j; i++)
			{//输出内存中页面的实时情况：页面号--寄存器值
				printf("%d--%d  ", M_Frame[i].page_no, M_Frame[i].num);
			}
			printf("\n");
		}
		printf("命中页面数:%f 未命中页面数:%f\n", yes, S - yes);
		printf("命中率:%f\n", yes / S);
		printf("缺页率:%f\n", 1 - yes / S);
	}

	else {
		if ((pid2 = fork()) == -1) { printf("failed!"); }//创建子进程2
		if (pid2 == 0)
		{ //此为子进程2
			printf("这里是FIFO算法的执行结果!\n");
			for (int i = 0; i<S; i++)
			{
				if (j<N)
				{//内存页框未占满
					judge = 0;
					for (int a = 0; a<N; a++)
					{//命中，退出循环，继续访问下一个页面
						if (M_Frame[a].page_no == Acess_Series[i])
						{
							judge = 1; cout << "当前访问页面" << Acess_Series[i] << "命中" << endl; yes = yes + 1;
						}
					}
					if (judge == 0)
					{
						M_Frame[j].page_no = Acess_Series[i];//未命中，调入缺页的页面号
						M_Frame[j].num = N;                   //初始化模拟寄存器的值
						for (int k = 0; k<j; k++) { M_Frame[k].num--; }//内存中其它页面的寄存器值减一
						cout << "当前访问页面" << Acess_Series[i] << "未命中，调入内存" << endl;
						j++;
					}
				}
				else
				{//内存页框被占满
					judge = 0;
					for (int a = 0; a<N; a++)
					{
						Copy_Frame[a].page_no = M_Frame[a].page_no;
						Copy_Frame[a].num = M_Frame[a].num;
						if (M_Frame[a].page_no == Acess_Series[i])
						{//命中
							cout << "当前访问页面" << M_Frame[a].page_no << "命中" << endl;
							yes = yes + 1;//命中次数加一
							judge = 1;    //置命中标志
						}
					}
					if (judge == 0)
					{//未命中
						int tihuan;
						sort(Copy_Frame, Copy_Frame + N, cmp);//对内存中页面的寄存器值进行排序，最小的为最近最久未使用页面，淘汰
						tihuan = Copy_Frame[0].page_no;//应该被替换的页面号
						for (int b = 0; b<N; b++)
						{
							if (M_Frame[b].page_no == tihuan)
							{//最近最久未使用页面替换为当前访问的页面
								M_Frame[b].page_no = Acess_Series[i];
								M_Frame[b].num = N;
							}
							else { M_Frame[b].num--; }//其他页面寄存器值减一
						}
						cout << "当前访问页面" << Acess_Series[i] << ",缺页，将页面" << tihuan << "置换出内存" << endl;
					}
				}
				for (int i = 0; i<j; i++)
				{//输出内存中页面的实时情况：页面号--寄存器值
					printf("%d--%d  ", M_Frame[i].page_no, M_Frame[i].num);
				}
				printf("\n");
			}
			printf("命中页面数:%f 未命中页面数:%f\n", yes, S - yes);
			printf("命中率:%f\n", yes / S);
			printf("缺页率:%f\n", 1 - yes / S);

		}
	}
	return 0;
}
