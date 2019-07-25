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

    //存储每个帧的状态：0空闲，1存储页表，2存储数据
    int *state_physicframe;

    //内存分配方式
    int type_memoryshare;

    //物理内存大小
    int room_physicmemory;

    //帧大小
    int room_physicframe;

    //物理帧数量
    int number_physicframe;

    //空闲帧数量
    int free_physicframe;

    //内存每个帧的状态
    int **frame;

    //虚存大小
    int v_memory;

    //虚存页帧数量
    int  num_pageframe;

    //虚存页帧状态
    int **state_pageframe;

    //页置换算法，1为fifo
    int pagereplacetype;

    //每个物理帧存储的内容
    string *realframe;

    //虚存每个帧存储的内容
    string *v_realframe;

    //将id和数据段和代码段首地址一一对应
    int **onebyone;
    
public:

    //构造函数
    Memory_share();

    //页面置换函数，根据物理帧号、页面置换算法类型、开始位置、长度来寻找物理帧传入虚存进行访问
    string Replace_page(int number_frame,int type,int start,int length);

    //根据给出的逻辑地址访问从start开始的长度为length的内容
    string replace(int pcblogicaddress,int start ,int end);

    //存储提供的字符串，返回逻辑地址
    int Store(string code);

    //存储判断，判断是否有足够的空间来存储给定的那个字符串
    bool Storejudge(string code);

    //根据给定的逻辑地址，删除该逻辑地址指引的所有帧的内容
    bool Delete(int logic_address);

    //输出函数，将内存信息和虚存信息记入日志文件
    void print(int type);

    //获取物理内存大小
    int getroom_physicmemory();

    //获取已使用物理内存大小
    int getusedroom_physicframe();

    //为PCB提供ID和代码段，为该PCB分配内存并把逻辑地址和ID一一对应存储
    int storein(int id,string code);

    //删除函数，根据ID删除为其分配的内存空间
    bool Deleteall(int id);

    //访问函数，根据给定你的id和访问的段类型计算逻辑地址，然后根据开始位置和长度来获取该段数据
    string read(int id,int start,int length,int type);
  	//string read(int id,int start,int length);

    //页面更新，用提供的字符串覆盖该ID对应的原有的数据段
    int change_page(int id,string data);

    //获取某段字符串所占的帧数
    int num_Storejudge(string code);

};

class Memory{
private :

    //static Memory* instance; 
    //Memory(int memorytype);

    //内存分配方式：1为页式2为段式
    int type;

    //内存大小
    int size;

    //页式分配
    Memory_share a;
    
    //段式分配
    MemorySegment b;


public:

  	//static Memory* SetMemoryType(memorytype);
    //	SetMemoryType(int memorytype);
    int MemoryInit(int memorytype);

    //设置内存大小
    void setSize(int i);

    //获取内存大小
    int getSize();

    //设置内存分配方式
    void setType(int t);

    //获取内存分配方式
    int getType();

    //初始化分配内存函数
    int Initize(int id,string code);

     //访问函数，根据给定你的id和访问的段类型计算逻辑地址，然后根据开始位置和长度来获取该段数据
    string Read(int id, int start, int length, int type1);
  //	string Read(int id, int start, int length);

    //删除函数，根据ID删除为其分配的内存空间
    int Delete(int id);

    //页面更新，用提供的字符串覆盖该ID对应的原有的数据段
    int change(int id,string data);

    //获取内存利用率
    float getusedmemory();
    
    int cleanDebris(){
    	b.cleanDebris();
    	
	}			//定时清理碎片 

};
#endif // PCB

