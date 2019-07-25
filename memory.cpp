#include"memory_share.h"
#include"Segment.h"
/*  	
	int Memory::MemoryInit(int memorytype)  		//根据选择模式，对Memory变量初始化
		//输入参数：	 memorytype = ( 1-页式	2-段式 ) 
	
	int Memory::Initize(int id,string code);		//初始化分配内存函数
		//输入参数：	id = (进程号)  code = (' ',就一个空格吧) 
		
	
	string Memory::Read(int id, int start, int length, int type1);		//访问函数
		//输入参数：	id = (进程号)  start=(逻辑地址，默认为 0 )  length=(访问数据的长度 )  
					//  type1 = (只有段式才用到，1-数据段  2-代码段 )	

	
	int  Memory::Delete(int id);			//删除函数
		//输入参数：	id = (进程号)
		
	
	int Memory::change(int id,string data);		//改变内存内容函数
		//输入参数：	id = (进程号)   data = ( 输入数据 )
		
	
	int Memory::getusedmemory();		//内存使用率函数
		
	void Memory::setSize(int i);		//设置内存大小 

	int Memory::getSize();				//获取内存大小

	void Memory::setType(int t);		//设置内存分配类型	
		//输入参数： t = ( 1-页式	2-段式 )

	int Memory::getType();				//获取内存分配类型
	  
*/ 


//对Memory变量初始化
int Memory::MemoryInit(int memorytype)
{

    //内存分配方式，只能在初始化的时候选择，之后将不允许修改
    type=memorytype;

    //选择页式分配内存，新建一个页式分配类型的变量
    if(type==1)
    {
        //Memory_share a;
    }
     else if(type==2)
    {
        //type=2;
        	//读取配置文件，初始化 段式类 b 
        ifstream iniMem("memSegSet.txt");
        //iniMem.open("memSegSet.txt");
        /*if(!iniMem)
            cout<<"file can not opened!memory initial failed!"<<endl;
	    else
        {*/
	        cout<<"Success,open memSegSet.txt"<<endl;
			//从配置文件 memSegSet.txt 中读取配置信息 ,并打印到命令行 
			int memSize,memMode;
           // iniMem>>memSize>>memMode;
            memSize= 100000;
            memMode = 3;
            //cout<<"memSize："<<memSize<<"\t";		//输出内存大小
            //cout<<"memMode："<<memMode<<endl;		//输出选择内存分配算法
            //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
	
			//根据配置文件读取的信息，初始化 段类 b  
	        size = memSize;
	        b.setType(2);
	        b.setSize(memSize);
	        b.setMode(memMode);
	        b.setFreeSize(memSize);
	        b.setUseSize(0);
	        MemoryManageP m=new MemoryManage;
	        m->startAddr=0;
	        m->length=size;
	        m->next =NULL;
	        b.freeP =m;
	        b.useP = NULL;
	        b.PCBSegP = NULL;
	        b.setPCBNum(0);
	        b.memContent=new char [memSize];
	        b.fileInit();		// 监测文件初始化 
	        
	        cout<<"fileInit Success,now out "<<endl;
        //}


    }
}

//初始化分配内存函数
int Memory::Initize(int id,string code)
{

           //页式分配内存，调用页式分配的初始化函数
    if(type==1)
    {

        return a.storein(id,code);
        //分配内存并把逻辑地址和ID一一对应存储 
    }
     else if(type==2)
    {


        //string s1 = code.toStdString();
        //size=s1.length();
        size=code.length();


         cout<<"进入函数--"<<"Memory::InitizeSTORE()"<<endl<<endl<<endl<<endl;
        bool ok=b.store(id,size);
        if(ok)
        {
            bool i=b.writeCode(id,0,code);
            //cout<<"进入函数--"<<"Memory::Initize()"<<endl;
            //cout<<code<<endl<<endl;
            if(!i)
                return 0;
            else
                return 1;

        }
        else
            return 0;

    }
}

//访问函数
//根据给定的id和访问的段类型type计算逻辑地址，然后根据开始位置start和长度length来获取该段数据 
string Memory::Read(int id, int start, int length, int type1)
{
    //cout<<"进入函数--"<<"Memory::Read()"<<endl;
   // cout << id << start << length;
    //页式分配内存，调用页式分配的访问函数
    if(type == 1){
         return a.read(id,start,length,type1);
    }
     else if(type==2)
    {
        if(type1==1)
        {
            return b.visitData(id,start,length);
        }
        else if(type1==2)
        {
            return b.visitCode(id,start,length);
        }
        else
        {
            string s="";
            //QString qs = QString::fromStdString(s);
           // return qs;
           return s;
        }
    }
   
}  

//删除函数
int  Memory::Delete(int id)
{
    //cout<<"进入函数--"<<"Memory::Delete()"<<endl;
    //页式分配内存，调用页式分配的删除函数
    if(type==1)
    {
        //页式删除成功返回1，否则返回0
        if(a.Deleteall(id))
        {
            return 1;
        }
        else
            return 0;
    }
    else if(type==2)
    {
        int i=b.deletePCB(1,id);
        if(0==i)
            return 0;
        else
            return 1;
    }
}


//改变内存内容函数
int Memory::change(int id,string data)
{
    //cout<<"进入函数--"<<"Memory::change()"<<endl;
      //cout<<data<<endl<<endl;
    //页式分配内存，调用页式分配的内容改变函数
    if(type==1)
    {
        return a.change_page(id,data);

    }
    else if(type==2)
    {
        //string s = data.toStdString();
        //先存储
        bool i=b.writeData(id,0,data);
        if(!i)
        {
            return -1;
        }
        else
        {
            if(data.length()+20 > b.getDataLength(id))
                return 0;
            else
                return 1;
        }
    }
}

//内存使用率函数
float Memory::getusedmemory()
{
    //cout<<"进入函数--"<<"Memory::getusedmemory()"<<endl;
    //页式分配内存，返回页式分配内存的内存使用率
    if(type==1)
    {
        //cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl<<(float)a.getusedroom_physicframe()/(float)a.getroom_physicmemory()<<endl;
        return (float)a.getusedroom_physicframe()/(float)a.getroom_physicmemory();

    }
    else if(type==2)
    {
        //cout<<"UUUUU"<<(float)b.getUseSize()*100000/(float)b.getSize()<<endl;
        return (float)b.getUseSize()/(float)b.getSize();
    }
}


//设置内存大小
void Memory::setSize(int i){
    size=i;
}

//获取内存大小
int Memory::getSize(){
    return size;
}

//设置内存分配类型
void Memory::setType(int t){
    type=t;
}

//获取内存分配类型
int Memory::getType(){
    return type;
}

