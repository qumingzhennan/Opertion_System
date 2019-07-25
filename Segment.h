#ifndef Segment_H
#define Segment_H
#include <iostream>
#include <fstream>
#include "head.h"

//**#include<str>
using namespace std; 
//**空闲段和使用段类定义，即段式分配空闲段链表和使用段链表节点的结构 
typedef class MemoryManage 
{
	public: 
		int startAddr;//**该段的起始地址 
		int length;//**该段的长度
		MemoryManage* next; 
		MemoryManage(){}
		~MemoryManage(){}
}* MemoryManageP;

//**段表定义
 class SegmentTable
{
	private:
		int processid;//**进程号 
		int segNum;//**进程的段数，即把进程分成几段
		int state;//**分配状态，1表示给进程分配内存，0表示释放 
		 
	public:
		int * size;//**每一段的大小，储存在size数组中
        int * startAddr;//**每一段的起始地址，存在startAddr数组中
		SegmentTable * next;
		
		SegmentTable()
		{
			segNum=0;
			state=0; 
			next=NULL;
		} 
		~SegmentTable()
		{
			delete [] size;
			delete [] startAddr;
		}
		
		void setProcessid(int n){processid=n;}
		int getProcessid(){return processid;}
		
		void setState(int n){state=n;}
		int getState(){return state;}
		
		void setSegNum(int n){segNum=n;}
		int getSegNum(){return segNum;}
		
		//**给动态数组分配空间 
		void distribute() 
		{

            //delete [] size;

            //delete [] startAddr;

			size=new int [segNum];
			startAddr=new int [segNum];
		}
} ;

typedef class SegmentTable *SegTableP;

//**内存段式分配定义 
class MemorySegment
{
	private:
		int size;//**内存大小
		int type;//**分配方式，1表示页式，2表示段式
		int freeSize;//**内存空闲大小
		int useSize;//**内存占用大小
		int mode;//**段式分配方式，1表示最先适配法，2表示最坏适配法，3表示最佳适配法,4表示下次适配法 
		int PCBNum;//**内存中进程个数 
	public:
		char*  memContent;
		MemoryManageP freeP;	//**空闲段链表头指针
		MemoryManageP useP;		//**使用段链表头指针 
		SegTableP PCBSegP;		//**进程段表头指针 
		 
        MemorySegment(){}

        MemorySegment(int memorySize,int managemode)
		{
			mode=managemode; 
			size=memorySize;
            type=2;
			freeSize=size;
			useSize=0;
			MemoryManageP m=new MemoryManage;
			m->startAddr=0;
			m->length=size;
			m->next=NULL;
			freeP=m;
			useP=NULL;
			PCBSegP=NULL;
			PCBNum=0;
			memContent=new char [size];
		}
		~MemorySegment(){delete [] memContent;}
		
		int getSize(){return size;}
		void setSize(int s){size=s;}
		
		int getType(){return type;}
		void setType(int t){type=t;}
		
		int getFreeSize(){return freeSize;}
		void setFreeSize(int f){freeSize=f;}
		
		int getUseSize(){return useSize;}
		void setUseSize(int u){useSize=u;}
		
		int getMode(){return mode;}
		void setMode(int s){mode=s;}
		
		int getPCBNum(){return PCBNum;}
		void setPCBNum(int s){PCBNum=s;}
		
		void fileInit();//**文件初始化函数 
        int store(int id,int size);//**给进程分配内存函数
        int FitFirst(int id,int size);//**最先适配法分配内存
        int FitWorst(int id,int size);//**最坏适配法分配内存
        int FitBest(int id,int size);//**最佳适配法分配内存
        int deletePCB(int outChoi,int id);		//**释放内存
		void monitor();				//**函数监测内存使用情况，并记录在文件中 
		void monitorPCB(SegTableP p);//**监测进程在内存中的分配和释放过程并记录在文件中
		int writeData(int id,int offsetAddress,string s);//**在数据段写数据
        int writeCode(int id,int offsetAddress,string s);//**在代码段存代码 
        string visitData(int id,int offsetAddress,int length);//**访问数据段
        string visitCode(int id,int offsetAddress,int length);//**在代码段取指令执行
        int getDataLength(int id);	//**获得数据段长度
		int getDataAddr(int id);	//**获得数据段首地址 
		int getCodeAddr(int id);	//**获得代码段首地址 
		int cleanDebris();			//定时清理碎片 
} ;
#endif

