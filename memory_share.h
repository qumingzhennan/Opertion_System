#ifndef MEMORY_SHARE
#define MEMORY_SHARE
#include <iostream>
#include<fstream>
#include <sstream>
#include <stdlib.h>
#include <ctime>
#include <iomanip>

#include "Segment.h"

using namespace std;

class Memory_share
{
private:

    //�洢ÿ��֡��״̬��0���У�1�洢ҳ��2�洢����
    int *state_physicframe;

    //�ڴ���䷽ʽ
    int type_memoryshare;

    //�����ڴ��С
    int room_physicmemory;

    //֡��С
    int room_physicframe;

    //����֡����
    int number_physicframe;

    //����֡����
    int free_physicframe;

    //�ڴ�ÿ��֡��״̬
    int **frame;

    //����С
    int v_memory;

    //���ҳ֡����
    int  num_pageframe;

    //���ҳ֡״̬
    int **state_pageframe;

    //ҳ�û��㷨��1Ϊfifo
    int pagereplacetype;

    //ÿ������֡�洢������
    string *realframe;

    //���ÿ��֡�洢������
    string *v_realframe;

    //��id�����ݶκʹ�����׵�ַһһ��Ӧ
    int **onebyone;
    
public:

    //���캯��
    Memory_share();

    //ҳ���û���������������֡�š�ҳ���û��㷨���͡���ʼλ�á�������Ѱ������֡���������з���
    string Replace_page(int number_frame,int type,int start,int length);

    //���ݸ������߼���ַ���ʴ�start��ʼ�ĳ���Ϊlength������
    string replace(int pcblogicaddress,int start ,int end);

    //�洢�ṩ���ַ����������߼���ַ
    int Store(string code);

    //�洢�жϣ��ж��Ƿ����㹻�Ŀռ����洢�������Ǹ��ַ���
    bool Storejudge(string code);

    //���ݸ������߼���ַ��ɾ�����߼���ַָ��������֡������
    bool Delete(int logic_address);

    //������������ڴ���Ϣ�������Ϣ������־�ļ�
    void print(int type);

    //��ȡ�����ڴ��С
    int getroom_physicmemory();

    //��ȡ��ʹ�������ڴ��С
    int getusedroom_physicframe();

    //ΪPCB�ṩID�ʹ���Σ�Ϊ��PCB�����ڴ沢���߼���ַ��IDһһ��Ӧ�洢
    int storein(int id,string code);

    //ɾ������������IDɾ��Ϊ�������ڴ�ռ�
    bool Deleteall(int id);

    //���ʺ��������ݸ������id�ͷ��ʵĶ����ͼ����߼���ַ��Ȼ����ݿ�ʼλ�úͳ�������ȡ�ö�����
    string read(int id,int start,int length,int type);
  	//string read(int id,int start,int length);

    //ҳ����£����ṩ���ַ������Ǹ�ID��Ӧ��ԭ�е����ݶ�
    int change_page(int id,string data);

    //��ȡĳ���ַ�����ռ��֡��
    int num_Storejudge(string code);

};

class Memory{
private :

    //static Memory* instance; 
    //Memory(int memorytype);

    //�ڴ���䷽ʽ��1Ϊҳʽ2Ϊ��ʽ
    int type;

    //�ڴ��С
    int size;

    //ҳʽ����
    Memory_share a;
    
    //��ʽ����
    MemorySegment b;


public:

  	//static Memory* SetMemoryType(memorytype);
    //	SetMemoryType(int memorytype);
    int MemoryInit(int memorytype);

    //�����ڴ��С
    void setSize(int i);

    //��ȡ�ڴ��С
    int getSize();

    //�����ڴ���䷽ʽ
    void setType(int t);

    //��ȡ�ڴ���䷽ʽ
    int getType();

    //��ʼ�������ڴ溯��
    int Initize(int id,string code);

     //���ʺ��������ݸ������id�ͷ��ʵĶ����ͼ����߼���ַ��Ȼ����ݿ�ʼλ�úͳ�������ȡ�ö�����
    string Read(int id, int start, int length, int type1);
  //	string Read(int id, int start, int length);

    //ɾ������������IDɾ��Ϊ�������ڴ�ռ�
    int Delete(int id);

    //ҳ����£����ṩ���ַ������Ǹ�ID��Ӧ��ԭ�е����ݶ�
    int change(int id,string data);

    //��ȡ�ڴ�������
    float getusedmemory();
    
    int cleanDebris(){
    	b.cleanDebris();
    	
	}			//��ʱ������Ƭ 

};
#endif // PCB

