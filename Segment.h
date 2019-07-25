#ifndef Segment_H
#define Segment_H
#include <iostream>
#include <fstream>
#include "head.h"

//**#include<str>
using namespace std; 
//**���жκ�ʹ�ö��ඨ�壬����ʽ������ж������ʹ�ö�����ڵ�Ľṹ 
typedef class MemoryManage 
{
	public: 
		int startAddr;//**�öε���ʼ��ַ 
		int length;//**�öεĳ���
		MemoryManage* next; 
		MemoryManage(){}
		~MemoryManage(){}
}* MemoryManageP;

//**�α���
 class SegmentTable
{
	private:
		int processid;//**���̺� 
		int segNum;//**���̵Ķ��������ѽ��̷ֳɼ���
		int state;//**����״̬��1��ʾ�����̷����ڴ棬0��ʾ�ͷ� 
		 
	public:
		int * size;//**ÿһ�εĴ�С��������size������
        int * startAddr;//**ÿһ�ε���ʼ��ַ������startAddr������
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
		
		//**����̬�������ռ� 
		void distribute() 
		{

            //delete [] size;

            //delete [] startAddr;

			size=new int [segNum];
			startAddr=new int [segNum];
		}
} ;

typedef class SegmentTable *SegTableP;

//**�ڴ��ʽ���䶨�� 
class MemorySegment
{
	private:
		int size;//**�ڴ��С
		int type;//**���䷽ʽ��1��ʾҳʽ��2��ʾ��ʽ
		int freeSize;//**�ڴ���д�С
		int useSize;//**�ڴ�ռ�ô�С
		int mode;//**��ʽ���䷽ʽ��1��ʾ�������䷨��2��ʾ����䷨��3��ʾ������䷨,4��ʾ�´����䷨ 
		int PCBNum;//**�ڴ��н��̸��� 
	public:
		char*  memContent;
		MemoryManageP freeP;	//**���ж�����ͷָ��
		MemoryManageP useP;		//**ʹ�ö�����ͷָ�� 
		SegTableP PCBSegP;		//**���̶α�ͷָ�� 
		 
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
		
		void fileInit();//**�ļ���ʼ������ 
        int store(int id,int size);//**�����̷����ڴ溯��
        int FitFirst(int id,int size);//**�������䷨�����ڴ�
        int FitWorst(int id,int size);//**����䷨�����ڴ�
        int FitBest(int id,int size);//**������䷨�����ڴ�
        int deletePCB(int outChoi,int id);		//**�ͷ��ڴ�
		void monitor();				//**��������ڴ�ʹ�����������¼���ļ��� 
		void monitorPCB(SegTableP p);//**���������ڴ��еķ�����ͷŹ��̲���¼���ļ���
		int writeData(int id,int offsetAddress,string s);//**�����ݶ�д����
        int writeCode(int id,int offsetAddress,string s);//**�ڴ���δ���� 
        string visitData(int id,int offsetAddress,int length);//**�������ݶ�
        string visitCode(int id,int offsetAddress,int length);//**�ڴ����ȡָ��ִ��
        int getDataLength(int id);	//**������ݶγ���
		int getDataAddr(int id);	//**������ݶ��׵�ַ 
		int getCodeAddr(int id);	//**��ô�����׵�ַ 
		int cleanDebris();			//��ʱ������Ƭ 
} ;
#endif

