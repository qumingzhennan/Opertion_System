#include "mem.h"
#include<QMessageBox>
mem::mem()
{ 
}

void mem::mem_in()
{
    size=code.length();
   //存储提供的字符串，返回逻辑地址
    ok = memoryC.Initize(id,code); //0失败  1成功

}

void mem::mem_out()
{
  memoryC.Read(id,page_start,page_length,2);
}

void mem::mem_utilization()
{

    //page_temp = ((float)a.getusedroom_physicframe()/(float)a.getroom_physicmemory() );
    mem_uti= memoryC.getusedmemory();

}

void mem::del_pcb()
{
    ok = 0;
   ok = memoryC.Delete(id);//0失败  1成功
}

void mem::clear_seg()
{
    memoryC.cleanDebris();
}
