#ifndef MEM_H
#define MEM_H
#include"memory_share.h"
#include"head.h"
#include"Segment.h"
class mem
{
public:
    mem();
    float mem_uti;
    string code,data;		//将数据写入代码段的 变量 ,将数据写入数据段的 变量(就是要写入的数据)
    int id;					//进程编号
        Memory memoryC;
        int memorytype;			//内存模式的选择  1-页式	2-段式
        int ok;					//检测变量
        int size;
            int page_start,page_length;	//临时变量，访问数据的起始地址，访问数据的长度
            int page_choice,page_i;
        //int  page_choice;//1--内存写入 2--内存读出 3--查看内存使用情况 4--删除PCB  5--段式清理碎片

        void mem_in();

        void mem_out();

        void mem_utilization();

        void del_pcb();

        void clear_seg();
};

#endif // MEM_H
