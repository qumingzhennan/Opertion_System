#ifndef PROCESS_H
#define PROCESS_H
#include<process.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
using namespace std;
struct PPCB{
    int id;     //进程的编号
    int cputime;   //进程的总CPU占用时间
    int lefttime;   //进程剩余的CPU占用时间
    //int possition;	//进程在队列中的位置
    int state;	//进程的状态，在哪一个队列中 1代表在运行队列 2代表在等待队列
    int priority;	//进程的优先级
    PPCB *pre;	//指向进程对象的在队列中的前一个进程
    PPCB *next;	//指向进程对象在队列中的后一个进程
};
struct Block{
    int name;//Match name
    int id;  //Match id
    PPCB *pcb;//Match Node
};
class process
{
	public:
		PPCB *match_pcb;
		int flag_count=0;
		int match_name;
		int match_id;
		int block_count;//count how many blocks
		PPCB all_queue[40];//所有进程
		PPCB run_queue[20];//运行进程
		PPCB wait_queue[20];//等待进程
		int timeslip;//时间片
		int i=1;   //当前选用的调度方式 1：轮转调度算法 2.先来先服务加优先级 3.短作业优先 4.先来先服务
		int j=0;   //选中进程操作 1.插入 2.阻塞 3.唤醒 4.删除
		int k=0; //  1.模拟进程调度 2.中断操作
		int all_count=0;//所有进程数量
		int run_count=0;//运行进程数量
		int wait_count=0;//等待进程数量
		process(); 
		bool findmatpcb(int name);//delete process by id

		void insertrun(PPCB *p,PPCB *q,int cputime,int priority);//将进程插入运行队列
		void insertwait(PPCB *p,PPCB *q,int cputime,int priority);  //将进程插入等待队列
		void showprocess(PPCB *P,PPCB *q,int type); // 显示进程信息   type=1 为运行队列，2为等待队列
		void backprocess(PPCB *p,PPCB *r,PPCB *q,int ID); // 阻塞进程
		void wakeupprocess(int ID,PPCB *p,PPCB *r,PPCB *q); //唤醒进程
		void deleteprocess(PPCB *p); //删除进程
		void sortprocess(PPCB *p,PPCB *q,int count);  //用于给进程按照优先级排序
		void sortprocess2(PPCB *p,PPCB *q,int count);  //用于给进程按照最短作业时间排序
		void swapprocess(PPCB *p,PPCB *q);  //用于交换两个进程的位置
	public:
		void  processinit();
		void R_R(int k,int j,int cputime,int priority,int ID);		//时间片轮转算发
		void FCFS_pro(int k,int j,int cputime,int priority,int ID);//和先来先服务算法一样
		void SJF(int k,int j,int cputime,int priority,int ID);	//段作业有限调度算法
		void FCFS(int k,int j,int cputime,int priority,int ID);	//先来先服务进程调度算法
		void newprocess(); //新建进程
		int pri_flag=1;// priority variant
		int processid=0;  //用于给进程ID赋值
		int runcount=0;   //记录在运行队列中的进程数
		int waitcount=0;  //记录在等待队列中的进程数

		PPCB *runhead;   //运行队列的表头 初始化
		PPCB *runrear;	//运行队列表尾
		PPCB *waithead;   //等待队列的表头  初始化
		PPCB *waitrear;		//等待队列表尾
		PPCB *temp1;  //指示运行队列的最后一个进程
		PPCB *temp2;  //指示等待队列的最后一个进程
		PPCB *current; //用于指示运行的当前进程
};

#endif // PROCESS_H
