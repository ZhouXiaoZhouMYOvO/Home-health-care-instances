#include<iostream>
#include<vector>
#include<string>
#include<queue>
using namespace std;


struct PCB {//进程控制块
	char name;
	int priority;
	int requir_time;
	int run_time;
	bool operator < (const PCB &a) const{ return priority>a.priority; };
};

const int memory = 5;
PCB pro[20];
vector<PCB> running;
vector<PCB> done;//已经完成的进程数组
queue<PCB> block;//阻塞队列
priority_queue<PCB> ready;
priority_queue<PCB> wait;
priority_queue<PCB> temp_wait;
queue<PCB> temp_block;
priority_queue<PCB> temp_ready;
int flag = 0;  //无程序在执行

void initial()//进程初始化（最早放进来的程序具有优先访问权）
{
	int n;   //总程序数目
	cout << "请输入当前进程数量";
	cout << "please input the num of progress:";
	cin >> n;
	cout << n << " progresses has been load,please initial their status:" << endl;
	cout << "这么多个进程被初始化";
	int i = 0;
	for (int i = 0; i < n; i++)            //最终程序调整为15个
	{
		pro[i].name = 'A' + i;
		pro[i].priority = i;
		pro[i].run_time = 0;
		cout << "please input the requir time of " << pro[i].name << endl;
		cin >> pro[i].requir_time;

		if (i < memory)
			ready.push(pro[i]);
		else
			wait.push(pro[i]);
	}

	return;
}

void output() {

	if (!running.empty())
	{
		cout << "running:" << running[0].name << ":" << endl;
		cout << "requir time " << running[0].requir_time << ",run time " << running[0].run_time << endl;
	}
	cout << "ready queue:" << endl;
	//PCB current;
	temp_ready = ready;
	temp_block = block;
	temp_wait = wait;
	while (!temp_ready.empty())
	{
		cout << temp_ready.top().name << " ";
		//temp_ready.push(ready.top());
		temp_ready.pop();

	}
	cout << endl;
	cout << "block queue:" << endl;
	while (!temp_block.empty())
	{
		cout << temp_block.front().name << " ";
		//temp_block.push(block.front());
		temp_block.pop();
	}
	cout << endl;
	cout << "wait:" << endl;
	while (!temp_wait.empty())
	{
		cout << temp_wait.top().name << " ";
		temp_wait.pop();
	}

	cout << endl;
	return;
}

void done_output(){//已经完成的进程
	cout << "已经完成的进程";
	cout << "done:";

	for(int i=0;i<done.size();i++)
	{
		cout << done[i].name << " ";
	}
	cout << endl;
	return;

}

void ready_to_run()    //二状态必然引起一状态
{
	if (ready.empty())
	{
		cout << "当前没有进程在就绪队列中";
		cout << "no progress in ready queue" << endl;
		return;
	}
	else
	{
		running.push_back(ready.top());
		running[0].run_time++;
		ready.pop();
		output();
		return;
	}
}

void run_to_ready()
{
	if (running[0].run_time == running[0].requir_time)
	{
		cout << "progress \"" << running[0].name << "\" has done!" << endl;
		if (!wait.empty())
		{
			ready.push(wait.top());
			wait.pop();
		}
		done.push_back(running[0]);
		running.pop_back();
		ready_to_run();
		return;
	}
	else
	{
		running[0].priority++;
		ready.push(running[0]);
		running.pop_back();
		ready_to_run();
		return;
	}
}
void run_to_block()     //就绪队列不为空时，三状态必然引起一状态
{
	if (running[0].run_time == running[0].requir_time)
	{
		cout << "progress \"" << running[0].name << "\" has done!" << endl;
		if (!wait.empty())
		{
			ready.push(wait.top());
			wait.pop();
		}
		done.push_back(running[0]);
		running.pop_back();
		ready_to_run();
		return;
	}
	else
	{
		//有问题的地方
		//没有判断就绪队列是否为空
		block.push(running[0]);
		running.pop_back();
		ready_to_run();
		return;
	}
	
}
void block_to_ready()        //CPU空闲 就绪队列为空，四状态必然引起一状态
{
	PCB current;
	current = block.front();
	block.pop();
	ready.push(current);
	if (!running.empty())
	{
		if (running[0].run_time == running[0].requir_time)
		{
			cout << "progress \"" << running[0].name << "\" has done!" << endl;
			if (!wait.empty())
			{
				ready.push(wait.top());
				wait.pop();
			}
			done.push_back(running[0]);
			running.pop_back();
			ready_to_run();
		}
		else
		{
			running[0].run_time++;
			output();
		}
	}
	else
	{
		ready_to_run();
	}

	//output();
	return;
}

int main()
{
	
	initial();        //等待队列和就绪队列已经创建完毕

	output();
	cout << endl;
	cout << "do you want to run first progress?" << endl;
	int cur;
	cin >> cur;
	if (cur == 1)
	{
		system("cls");
		ready_to_run();
	}
	else
		return 0;
	cout << endl;
	cout << "input 2 3 4 to change status,0 to exit" << endl;
	//cin >> cur;
	while (1)
	{
		cout << endl;
		if (ready.empty() && block.empty())
		{
			cout << "no progress is waiting" << endl;
			break;
		}
		cin >> cur;
		if (cur != 0)
		{
			
			switch (cur)
			{
			case 2:{
				if (running.empty())
					cout << "cpu is vacant,please wake blocked progress!" << endl;
				else
					run_to_ready();
				break;
			}
			case 3: {
				if (running.empty())
					cout << "cpu is vacant,please wake blocked progress!" << endl;
				else
					run_to_block();
				break;
			}
			case 4: {
				if (block.empty())
					cout << "no progress is blocked";
				else
					block_to_ready();
				break;
			}
			default: {
				if(running.empty())
					cout << "cpu is vacant,please wake blocked progress!" << endl;
				else
				{
					if (running[0].requir_time == running[0].run_time)
					{
						cout << "progress \"" << running[0].name << "\" has done!" << endl;
						if (!wait.empty())
						{
							ready.push(wait.top());
							wait.pop();
						}
						done.push_back(running[0]);
						running.pop_back();
						ready_to_run();
					}
					else
					{
						running[0].run_time++;
						output();
					}
				}
				break;
			}
			}
		}
		else
			break;
	}

	/*while (running[0].requir_time != running[0].run_time)
	{
		cout << "input 1 to give time" << endl;
		int time;
		cin >> time;
		if(time)
		{
			running[0].run_time++;
			continue;
		}
	}*/

	cout << "all progress have done" << endl;
	done_output();


	system("pause");
	return 0;
}
