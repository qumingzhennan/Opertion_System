#include "memory_share.h"
#include<QFile>
#include<QTextStream>
/*
	Memory_share::Memory_share() //根据配置文件初始化 
	int Memory_share::change_page(int id,string data) //页面更新，用提供的字符串覆盖该ID对应的原有的数据段
		//id--进程号 data--需要存储的代码 
		//返回-1/0/1/2 -1--没有足够空间完成数据段更新 0--数据段将没有足够空间继续更新 1--允许继续数据段更新 2--删除失败 
	bool Memory_share::Deleteall(int id) //删除内存函数，根据给定的PCBID删除所有为其分配的内存
		//id--进程号 
		//返回true/false --删除成功与否	
	bool Memory_share::Delete(int logic_address) //根据逻辑地址删除内存
		//logic_address--想删除帧所在的逻辑地址 
		//返回true/false --删除成功与否	
	void Memory_share::print(int type) //日志文件输出
		//根据要进行的操作进行分类，对应不同的type类型
	string Memory_share::read(int id,int start,int length,int type1) //从内存中读取数据 
		//id--进程号 start--内存起始地址 length--数据长度 type1--1为访问页表对应内存，2为访问内存内容 	
		//调用replace函数，最后返回要读取的数据 
	string Memory_share::replace(int pcblogicaddress,int start ,int end) //访问函数，根据逻辑地址，起始和结束位置返回逻辑地址指向的内容从起始开始到结束位置结束的内容	
		//pcblogicaddress--程序控制块所在逻辑地址 start--起始位置 end--结束位置
		//返回要读取的数据  
	string Memory_share::Replace_page(int number_frame,int type,int start,int length) //虚存调用页面置换算法换页访问函数
		//number_frame--新的物理帧号 type--页面置换算法，1为FIFO，2为LRU 	 	
		//返回v_realframe[vn].substr(start,length) --记录的虚存页帧中从start开始长度为length的字符串 
	int Memory_share::Store(string code) //内存的设计存储：存储提供的字符串，返回逻辑地址 
		//code--要存储的字符串 
		//返回first_address--字符串在内存中存储的逻辑地址 	
	bool Memory_share::Storejudge(string code) //内存分配判断函数
		//code--要存储的字符串 
		//返回true/false  在空闲帧数量足够分配给存储内容的帧和存储页表的帧的情况下，返回true，否则返回false
	int Memory_share::num_Storejudge(string code) //计算分配帧数
		//code--要存储的字符串
		//返回(num_frame+length_page) --总帧数 
	int  Memory_share::storein(int id,string code) //PCB初始化申请内存分配函数
		//id--进程号，code--该PCB的代码段 
		//返回1/0--分配成功与否
	int Memory_share::getroom_physicmemory() //获取物理内存总数
		//返回物理内存大小	
	int Memory_share::getusedroom_physicframe() //获取已使用的物理内存大小
		//返回已使用物理内存大小 
*/ 

Memory_share::Memory_share()
{
    //根据配置文件初始化内存系统
    ifstream fin("Initialpage.txt");
    //QTextStream in(&fin);
    /*  内存大小:20000
	    帧大小:200
	    虚存大小:1000
	    页面置换算法（1为FIFO 2为LRU）:1  */
    string s[4];
    string buffer;
    int cs,pd,i,st;
    int ini[4];//配置文件中4个参数存入数组 
    cs=0;

    //逐行读取配置文件内容至缓冲区
    while( getline(fin,buffer) )
    {
        i=0;
        pd=0;

        //以：为分隔符，获取：后的数据
        while(buffer[i])
        {
            if(buffer[i]==':')
            {
                pd=1;
                break;
            }
            else
                i++;
        }
        //配置文件出错（没有出现：）
        if(pd==0)
        {
            cout<<"error"<<endl;

        }
        else
        {
            //获取：之后的字符串长度
            st=i+1;
            i++;
            while(buffer[i])
            {	
				pd++;
                i++;
			}
            pd--;
            //获取：之后的内容，并将字符串转换成int型存储进数组
            ini[cs]=atoi((buffer.substr(st,pd)).c_str());
            cs++;
        }
        
    }
	

    //将配置文件提取获得的数据赋值给内存系统配置参数
    //物理内存大小
    //room_physicmemory=ini[0];
    room_physicmemory=20000;
    cout<<"000000物理内存大小为："<<room_physicmemory<<endl;
	
    //帧大小 
    //room_physicframe=ini[1];
    room_physicframe = 200;
   cout<<"1111111111帧大小为："<<room_physicframe<<endl;
    
    //物理帧帧数
    number_physicframe=room_physicmemory/room_physicframe;
    
    //空闲帧数量，初始就是物理帧总数
    free_physicframe=number_physicframe;
    
    //虚存大小
    //v_memory=ini[2];
    v_memory = 1000;
    cout<<"22222222虚存大小为："<<v_memory<<endl;
    
    //虚存数量
    num_pageframe=v_memory/room_physicframe;
    
    //页式置换算法类型，1为FIFO，2为LRU
    //pagereplacetype=ini[3]; //由配置文件读入置换算法类型
    pagereplacetype = 1;
    if(pagereplacetype==1){
        cout<<"3333页式置换算法类型为："<<"先入先出（FIFO）"<<endl;
    }
    else{
        cout<<"3333333页式置换算法类型为："<<"近期最少使用（LRU）"<<endl;
	} 

    //动态数组，数量为物理帧数量
    state_physicframe=new int[number_physicframe];
    
    //此处记录物理帧状态，初始化为0表示帧内没有记录任何信息
    for(int i=0;i<number_physicframe;i++)
    {
        state_physicframe[i]=0;
    }

    //二维数组记录物理层内记录的内容，第一维为物理帧数量
    frame = new int*[number_physicframe];
    for (int a = 0; a != number_physicframe; ++a)
    {
        frame[a] = new int[room_physicframe];
    }
    //第二维为物理帧大小，初始化为-1
    for(int i=0;i<number_physicframe;i++)
        for(int n=0;n<room_physicframe;n++)
        {
            frame[i][n]=-1;
        }

    //虚存页帧转态，第一维为虚存页帧数量
    state_pageframe=new int*[num_pageframe];
    for(int m=0;m<num_pageframe;m++)
    {
        state_pageframe[m]=new int[2];
    }

    //第二维大小为2，分别记录该帧对应的物理内存的帧号以及在虚存中的存在时间
    for(int p=0;p<num_pageframe;p++)
    {
        state_pageframe[p][0]=-1;
        state_pageframe[p][1]=0;
    }

    //物理内存实际内容存储，以字符串来保存
    realframe=new string[number_physicframe];
    for(int i=0;i<number_physicframe;i++)
    {
        realframe[i]="";
    }

    //虚存实际内容存储，以字符串来保存
    v_realframe=new string[num_pageframe];
    for(int i=0;i<num_pageframe;i++)
    {
        v_realframe[i]="";
    }

    /*二维数组，作用是将PCB（进程控制块）ID和该PCB的数据段和代码段的逻辑地址一一对应
    内存和该PCB的交互全部以ID为关键字，逻辑地址由内存自己记录 */   
    //初始化 
    onebyone=new int*[number_physicframe];
    for(int q=0;q<number_physicframe;q++)
    {
        onebyone[q]=new int[3];
    }
    
    //初始化全部位-1，第二维的第一位记录ID，第二第三位记录数据段和代码段的逻辑地址
    for(int w=0;w<number_physicframe;w++)
    {
        onebyone[w][0]=-1;
        onebyone[w][1]=-1;
        onebyone[w][2]=-1;
    }
		
    //日志文件清空函数
    print(0);
}



//内存存储函数
int Memory_share::change_page(int id,string data) //页面更新，用提供的字符串覆盖该ID对应的原有的数据段
{
	cout<<"进入函数--"<<"Memory_share::change_page()"<<endl;
    //根据给定的PCBID在内存中找到对应的数据段逻辑地址
    int i,a,b,logicaddress1;
    for(i=0;i<number_physicframe;i++)
    {
        if(onebyone[i][0]==id)
          break;
    }

    a=1;
    b=1;
    logicaddress1=onebyone[i][1];

    //计算该PCB在内存中已分配的内存大小
    i=0;
    while(i<room_physicframe)
    {
            if(frame[logicaddress1][i]!=-1)
            {
                a++;
                 if(i==(room_physicframe-1))
                 {
                     if(state_physicframe[frame[logicaddress1][i]]==2)
                     break;
                     else
                     {
                         logicaddress1=frame[logicaddress1][i];
                         i=0;
                     }
                 }
                 else
                     i++;
            }
            else
                break;
    }
    
	//计算需要存储的代码长度
    string pcbcode=data;
    int length_code=pcbcode.size();

    //计算需要存储这段代码会生成的页表的长度，也就是存储代码段内容需要的页帧数量
    int length_page;
    length_page=length_code/room_physicframe;
    if((length_code)%room_physicframe!= 0)
    length_page++;

	//计算需要用来存放页表的总帧数
    int num_frame;
    //如果页表长度为0，则需要0个页帧来存储，长度为1，则需要1个帧来存储
    if(length_page==0)
    num_frame=0;
    else if(length_page==1)
    num_frame=1;
   //计算其他情况下存放页表需要的总帧数
    else if((length_page%(room_physicframe-1))<2)
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }
     //需要用来存放页表的总帧数（包括存储页表的帧）
    int need=num_frame+length_page;


	//如果空闲帧数量不足够存储这段内容，则返回-1表示没有足够空间完成数据段更新
     if((free_physicframe+a)>=need)
     {
         //删除原有数据段，如果删除失败返回2表示
        if(Delete(logicaddress1)==true)
         {
            //删除成功的情况下，为新的内容分配内存
             int i;
             for(i=0;i<number_physicframe;i++)
             {
                 if(onebyone[i][0]==id)
                   break;
             }
             //记录新存储的数据段的逻辑地址
             onebyone[i][1]=Store(data);

             //如果更新数据段之后还有40个以上字节（为下一秒钟的输入更新预留）
             //并且还有两个以上空闲帧的情况下（防止当前页帧为该页表倒数第二帧）
             //返回1表示允许继续数据段更新，否则返回0表示数据段将没有足够空间继续更新
             if((free_physicframe*room_physicframe>=40)&&(free_physicframe>1))
             {
                 return 1;
             }
             else
             {
                 return 0;
             }

         }
         else
         {
             return 2;
         }
     }
     else
         return -1;

}



//删除内存函数，根据给定的PCBID删除所有为其分配的内存
bool Memory_share::Deleteall(int id)
{
	cout<<"进入函数--"<<"Memory_share::Deleteall()"<<endl;
    int s=0;
    int p=0;
    //在内存中找到与给定的ID对应的ID
    for(s=0;s<number_physicframe;s++)
    {
        if(onebyone[s][0]==id)
        {
            p=1;
            break;
        }
    }
    //如果寻找不到，则返回删除失败
    if(p==0)
    {
        return false;
    }
    else
    {
        //找到对应的ID，则可以得到对应的数据段和代码段的逻辑地址
        //根据数据段和代码段逻辑地址删除数据段和代码段分配的内存
        Delete(onebyone[s][1]);
        Delete(onebyone[s][2]);
        //删除与给定ID一一对应的内容并且返回删除成功
        onebyone[s][0]=-1;
        onebyone[s][1]=-1;
        onebyone[s][2]=-1;
        return true;
    }

}

//根据逻辑地址删除内存
bool Memory_share::Delete(int logic_address)
{
	cout<<"进入函数--"<<"Memory_share::Delete"<<endl;
    int logic_address2;
    logic_address2=logic_address;
    //将逻辑地址所在的帧内容删除状态改为空闲
    state_physicframe[logic_address2]=0;
    realframe[logic_address2]="";
    free_physicframe++;
    int i=0;

    //根据逻辑地址，将逻辑地址指向的所有存储有内容数据的帧全部删除内容，存储内容改为空字符串状态改为空闲
    while(i<room_physicframe)
    {
        int a=frame[logic_address2][i];
        if(a==-1)
            break;
        else if(state_physicframe[a]==2)
        {
            state_physicframe[a]=0;
            realframe[a]="";
            free_physicframe++;
            frame[logic_address2][i]=-1;
            i++;
        }
        //当页表存储在多个帧的情况下，更新逻辑地址，并删除新的逻辑地址所在的帧的内容和状态
        else if(state_physicframe[a]==1)
        {
            logic_address2=frame[logic_address2][i];
            realframe[a]="";
            state_physicframe[a]=0;
            free_physicframe++;
            i=0;
        }
    }
    //将内存的改变输出到日志文件并返回删除成功
    print(1);
    print(2);
    print(5);
    return true;
}



//日志文件输出
void Memory_share::print(int type)
{
    //cout<<"Memory_Dialog out"<<endl;
    //时间函数，在记录内存和虚存发生变化的时候同时记录发生时间
    time_t t = time(0);
    char tmp[64];
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X",localtime(&t) );


    if(type==1)
    {
        //日志状态记录文件
        //ofstream outfile("memorystate_logfile.txt",ios::app);//内存状态日志文件 //每个帧的状态：0空闲，1存储页表，2存储数据
        QFile outfile("memorystate_logfile.txt");
        QTextStream in(&outfile);
        if(!outfile.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //cout<<"1.Memory_Dialog out"<<endl;
            //逐行记录下内存每个帧的状态
            //每次调用这个函数依次往下记录
            in<<tmp<<"\n";
            in<<"帧号"<<"    "<<"状态"<<"\n";
            for(int i=0;i<number_physicframe;i++)
            {
               // in.setFieldWidth(9);
				 //QString str="123";
				 in<<i<<state_physicframe[i]<<"\n"; //in<<setiosflags(ios::left)<<setw(9)<<i<<state_physicframe[i]<<endl;
            }
            outfile.close();
        }
    }
    else if(type==2)
    {
        //cout<<"2.Memory_Dialog out"<<endl;
        //日志内容记录文件
        //ofstream outfile1("memorycontent_logfile.txt",ios::app);//内存内容日志文件
        QFile outfile1("memorycontent_logfile.txt");
        QTextStream in(&outfile1);
        if(!outfile1.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //逐行记录每个帧内存储的内容
            //每次记录依次往下记录
            in<<tmp<<"\n";
            in<<"帧号"<<"    "<<"内容"<<endl;
            for(int i=0;i<number_physicframe;i++)
            {
                //in.setFieldWidth(9);
                QString s=QString::number(i,10)+"    ";
                s.append(QString::fromStdString(realframe[i]));
               in<<s<<"\n"; //outfile1<<setiosflags(ios::left)<<setw(8)<<i<<realframe[i]<<endl;
            }
            outfile1.close();
        }
    }
    else if(type==3)
    {
        //cout<<"3.Memory_Dialog out"<<endl;
        //虚存状态记录文件
        //ofstream outfile2("virtualstate_logfile.txt",ios::app);//虚存状态日志文件
            QFile outfile2("virtualstate_logfile.txt");
            QTextStream in(&outfile2);
        if(!outfile2.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //依次记录虚存每个帧的帧号和存在时间
            //每次调用依次向下记录
            in<<tmp<<"\n";
            in<<"页帧号"<<"   "<<"页帧内容"<<"  "<<"时长"<<endl;
            for(int i=0;i<num_pageframe;i++)
            {
                //in.setFieldWidth(10);
                QString s=QString::number(i,10)+"    ";
                s.append(QString::fromStdString(realframe[i]));
                in<<s<<"\n";//outfile2<<setiosflags(ios::left)<<setw(10)<<i<<setw(10)<<state_pageframe[i][0]<<state_pageframe[i][1]<<endl;
            }
                outfile2.close();
        }
    }
    else if(type==4)
    {
        //cout<<"4.Memory_Dialog out"<<endl;
        //虚存内容记录文件
        QFile outfile3("virtualcontent_logfile.txt");
        QTextStream in(&outfile3);
        //ofstream outfile3("virtualcontent_logfile.txt",ios::app);//虚存内容日志文件
        if(!outfile3.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //依次记录虚存每个帧的帧号和存储内容
            //每次调用依次向下记录
            in<<tmp<<"\n";
            in<<"页帧号"<<"   "<<"页帧存储内容"<<endl;
            for(int i=0;i<num_pageframe;i++)
            {
                QString s=QString::number(i,10)+"    ";
                s.append(QString::fromStdString(v_realframe[i]));
                in<<s<<"\n";//outfile3<<setiosflags(ios::left)<<setw(10)<<i<<v_realframe[i]<<endl;
            }
            outfile3.close();
        }
    }
    else if (type==5)
    {

        //内存当前内容记录文件
        //每次调用清空原有内容，用新内容覆盖
        FILE *fp1;
        fp1=fopen("memeory_instant.txt","w");
        fclose(fp1);

        //ofstream outfile5("memeory_instant.txt",ios::app);//即时内存文状态件
        QFile outfile5("virtualcontent_logfile.txt");
        QTextStream in(&outfile5);
        if(!outfile5.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //cout<<"5.Memory_Dialog out"<<endl;
            //依次记录内存每个帧的帧号和存储内容
            in<<tmp<<endl;
            in<<"帧号"<<"    "<<"内容"<<"\n";
            for(int i=0;i<number_physicframe;i++)
            {
                QString s=QString::number(i,10)+"    ";
                s.append(QString::fromStdString(realframe[i]));
                in<<s<<"\n";//outfile5<<setiosflags(ios::left)<<setw(8)<<i<<realframe[i]<<endl;
            }
            outfile5.close();
        }
    }
    else if(type==6)
    {
        //虚存当前内容记录文件
        //每次调用清空原有内容，用新内容覆盖
        FILE *fp;
        fp=fopen("virtual_instant.txt","w");
        fclose(fp);

        QFile outfile4("virtual_instant.txt");
        QTextStream in(&outfile4);
        //ofstream outfile4("virtual_instant.txt",ios::app);//即时虚存状态文件
        if(!outfile4.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //cout<<"6.Memory_Dialog out"<<endl;
            //依次记录虚存每个帧的帧号和存储内容
           in<<tmp<<"\n";
            in<<"页帧号"<<"   "<<"页帧存储内容"<<endl;
            for(int i=0;i<num_pageframe;i++)
            {
                QString s=QString::number(i,10)+"    ";
                s.append(QString::fromStdString(v_realframe[i]));
                in<<s<<"\n";//outfile4<<setiosflags(ios::left)<<setw(10)<<i<<v_realframe[i]<<endl;
            }
            outfile4.close();
        }
    }
    else if(type==0)
    {
        //cout<<"0.Memory_Dialog out"<<endl;
        //内存初始化，清空所有记录文件
        FILE *fp2;
        fp2=fopen("memorystate_logfile.txt","w");
        fclose(fp2);

        FILE *fp3;
        fp3=fopen("memorycontent_logfile.txt","w");
        fclose(fp3);

        FILE *fp4;
        fp4=fopen("virtualstate_logfile.txt","w");
        fclose(fp4);

        FILE *fp5;
        fp5=fopen("virtualcontent_logfile.txt","w");
        fclose(fp5);

        FILE *fp6;
        fp6=fopen("memeory_instant.txt","w");
        fclose(fp6);

        FILE *fp7;
        fp7=fopen("virtual_instant.txt","w");
        fclose(fp7);
    }
}



string Memory_share::read(int id,int start,int length,int type1)
{
	cout<<"进入函数--"<<"Memory_share::read()"<<endl;
    int s=0;
    int a=0;
    //根据提供的PCBID查看是否存在于内存中
    for(s=0;s<number_physicframe;s++)
    {
        if(onebyone[s][0]==id)
        {
            a=1;
            break;
        }
    }
    if(a==0)
    {
        return "";
    }
    else
    {
        //计算访问内容的结束位置
        int end=start+length-1;
        //1为页表，访问页表对应内存，2为内存，访问内存内容
        if(type1==1)
            return replace(onebyone[s][1],start,end);
        else if(type1==2)
            return replace(onebyone[s][2],start,end);
    }


}




//访问函数，根据逻辑地址，起始和结束位置返回逻辑地址指向的内容从起始开始到结束位置结束的内容
string Memory_share::replace(int pcblogicaddress,int start ,int end)
{
	cout<<"进入函数--"<<"Memory_share::replace"<<endl;
    int a,b,c,logicaddress1;
    a=0;
    c=0;
    logicaddress1= pcblogicaddress;
    
    //计算逻辑地址指向内容的总长度
    while(c==0)
    {
        for(b=0;b<room_physicframe;b++)
        {
            if(frame[logicaddress1][b]!=-1) //内存每个帧的状态正常 
            {
                if(state_physicframe[frame[logicaddress1][b]]==2) //存储数据 
                {
                    a=a+realframe[frame[logicaddress1][b]].size(); //帧内存储的内容大小 
                }
                else if(state_physicframe[frame[logicaddress1][b]]==1) //存储页表 
                {
                    logicaddress1=frame[logicaddress1][b]; //文件控制块中逻辑地址对应的帧 
                }
            }
            else //出现异常帧（即该帧不为空） 
            {
                c=1;
                break;
            }
        }

    }
    //如果结束位置大于内容总长度，则返回一个空字符串表示访问越界
    if(a<=end)
    {
        string a="";
        cout << "访问越界！";
        return a;
    }
    //访问内容存在的情况下，继续执行
    else
    {
        int p,q,p1,q1;
		int logicaddress,length,length1,pagenumber,pagenumber1,numinpage,numinpage1,framenum;

        logicaddress= pcblogicaddress; //程序控制块逻辑地址 

        p=start/room_physicframe; //计算出访问内容开始位置在从逻辑地址开始的第几个页帧内 
        q=start%room_physicframe; //计算出问内容开始位置在该页帧内的哪一个位置 
        p1=end/room_physicframe; //计算出访问内容结束位置在从逻辑地址开始的第几个页帧内 
        q1=end%room_physicframe; //计算出问内容结束位置在该页帧内的哪一个位置 

        //起始位置和结束位置都在同一个帧内
        if(p==p1)
        {
            //取余运算的差就是访问的内容长度
            length=q1-q+1;
            
            pagenumber=p/(room_physicframe-1); //取整计算起始和结束位置所在帧在第几个存储页表的帧内
            numinpage=p%(room_physicframe-1); //取余运算计算起始和结束位置所在帧在该页表的第几个位置
            
            //目标帧处于索引页表的最后两位
            if(numinpage<=1)
            {
                //不在第一个存储页表的帧内
                if(pagenumber!=0)
                {
                    //寻找到存有目标帧的页表所在的帧，并把当前逻辑地址修改为该帧的帧号
                    pagenumber--;
                    for(int i=0;i<pagenumber;i++)
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                    }
                    //如果该页表帧的最后位存储的是数据内容，则访问内容所在帧就是这个帧

                    if(state_physicframe[frame[logicaddress][room_physicframe-1]]==2)
                    {
                        framenum=frame[logicaddress][room_physicframe-1];
                    }
                    //如果存储的是页表，则访问内容所在帧就是下一个存储页表所在帧的第一位
                    else
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                        framenum=frame[logicaddress][numinpage];
                    }

                }
                //如果是在第一个存储页表的帧内，则取余的值就是存储该段内容所在帧的位置
                else
                {
                    framenum=frame[logicaddress][numinpage];
                }

            }
            else
            {
                //如果目标帧不是在页表后两位，则直接通过取整置可以计算出页表所在帧
                //通过区域置获取在页表内的位置，获取访问内容所在的帧的帧号
                for(int i=0;i<pagenumber;i++)
                {
                    logicaddress=frame[logicaddress][room_physicframe-1];
                }
                framenum=frame[logicaddress][numinpage];
            }

            return Replace_page(framenum,pagereplacetype,q,length);

        }
        else
        {
            //此处p必定小于p1
            string buffer="";
            string buffer1="";

            //将起始位置和结束位置分别和页帧大小取整运算和取余运算
            //取整运算是为了计算出他所在从逻辑地址开始的第几个页帧内
            //取余运算时为了计算出他在该页帧内的哪一个位置
            pagenumber=p/(room_physicframe-1);
            numinpage=p%(room_physicframe-1);
            pagenumber1=p1/(room_physicframe-1);
            numinpage1=p1%(room_physicframe-1);

            //起始位所在帧内容提取
            //访问内容在起始位所在帧中的内容长度
            length=room_physicframe-q+1;
            //目标帧处于索引页表的最后两位
            if(numinpage<=1)
            {//不在第一个帧内
                if(pagenumber!=0)
                {
                    //寻找到存有目标帧的页表所在的帧，并把当前逻辑地址修改为该帧的帧号
                    pagenumber--;
                    for(int i=0;i<pagenumber;i++)
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                    }

                    //如果该页表帧的最后位存储的是数据内容，则访问内容所在帧就是这个帧
                    if(state_physicframe[frame[logicaddress][room_physicframe-1]]==2)
                    {
                        framenum=frame[logicaddress][room_physicframe-1];
                    }
                    //如果存储的是页表，则访问内容所在帧就是下一个存储页表所在帧的第一位
                    else
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                        framenum=frame[logicaddress][numinpage];
                    }

                }
                //如果是在第一个存储页表的帧内，则取余的值就是存储该段内容所在帧的位置
                else
                {
                    framenum=frame[logicaddress][numinpage];
                }

            }
            else
            {
                //如果不是在页表后两位，则直接通过取整就可以计算出页表所在帧
                //通过区域置获取在页表内的位置，获取访问内容所在的帧的帧号
                for(int i=0;i<pagenumber;i++)
                {
                    logicaddress=frame[logicaddress][room_physicframe-1];
                }
                framenum=frame[logicaddress][numinpage];
            }
            //记录从起始位置所在帧中访问获取的内容
            buffer=Replace_page(framenum,pagereplacetype,q,length);
            ///////////////////////////////////////////////////////////
            //下一帧
            p++;
            int pagebuff;
            pagebuff=numinpage+1;
            //如果下一帧不是结束位置所在的帧，则将这一帧的内容全部提取出，连接在起始位置中提取出的内容后面，
            //以此类推，直到到达结束位置所在的帧为止
            while(p<p1)
            {
                //如果遇到页表所在帧的最后一位存储了页表位置，则更新当前逻辑地址，然后再从新的页表从头开始遍历
                if(state_physicframe[frame[logicaddress][pagebuff]]==2)
                {
                    framenum=frame[logicaddress][pagebuff];
                    //将这一帧的内容全部提取出，连接在起始位置中提取出的内容后面
                    buffer+=Replace_page(framenum,pagereplacetype,0,room_physicframe);
                    p++;
                    pagebuff++;
                }
                else
                {
                    logicaddress=frame[logicaddress][room_physicframe-1];
                    pagebuff=0;
                }

            }
            ////////////////////////////////////////////////////////////////////////////
            
            //结束位置所在帧
            length1=q1+1;
            logicaddress=pcblogicaddress;
            //目标帧处于索引页表的最后两位
            if(numinpage1<=1)
            {
                //不在第一个帧内
                if(pagenumber1!=0)
                {
                    //寻找到存有目标帧的页表所在的帧，并把当前逻辑地址修改为该帧的帧号
                    pagenumber1--;
                    for(int i=0;i<pagenumber1;i++)
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                    }
                    //如果该页表帧的最后位存储的是数据内容，则访问内容所在帧就是这个帧
                    if(state_physicframe[frame[logicaddress][room_physicframe-1]]==2)
                    {
                        framenum=frame[logicaddress][room_physicframe-1];
                    }
                    else
                    {
                        //如果存储的是页表，则访问内容所在帧就是下一个存储页表所在帧的第一位
                        logicaddress=frame[logicaddress][room_physicframe-1];
                        framenum=frame[logicaddress][numinpage1];
                    }
                }
                else
                    //如果是在第一个存储页表的帧内，则取余的值就是存储该段内容所在帧的位置
                {
                    framenum=frame[logicaddress][numinpage1];
                }
            }
            else
            {
                //如果不是在页表后两位，则直接通过取整就可以计算出页表所在帧
                //通过区域置获取在页表内的位置，获取访问内容所在的帧的帧号
                for(int i=0;i<pagenumber1;i++)
                {
                    logicaddress=frame[logicaddress][room_physicframe-1];
                }
                framenum=frame[logicaddress][numinpage1];
            }

            //获得结束位置所在帧的从0开始结束位置结束的内容，并连接在之前获得的内容后方
            buffer+= Replace_page(framenum,pagereplacetype,0,length1);
            ////////////////////////////////////////////////////////////////////////////end
        //    cout << "100";
            //将全部内容连接完成后返回该内容
            return buffer;

        }

    }

}




//虚存调用页面置换算法换页访问函数，type为页面置换算法，1为FIFO，2为LRU
string Memory_share::Replace_page(int number_frame,int type,int start,int length)
{
	cout<<"进入函数--"<<"Memory_share::Replace_page()"<<endl;
    int a,b,c,m,n,vn;
    b=0;
    //FIFO置换算法
    if(type==1)
    {
        //在虚存中逐个遍历帧，查找是否已经存在给定的物理帧帧号
        for(a=0;a<num_pageframe;a++)
        {
            if(state_pageframe[a][0]==number_frame)
            {
                //给定的物理帧已经存在于虚存中，记录该虚存帧号
                vn=a;
                b=1;
                break;
            }
        }
        //虚存中不存在已给定的物理帧的情况下
        if(b==0)
        {
            //首先寻找空闲是否有空闲的虚存页帧，同时把非空闲的帧的存在时间加一
            for(a=0;a<num_pageframe;a++)
            {
                if(b==0)
                {
                    //在虚存中找到空闲页帧，则把给定的物理帧的帧号、物理帧存储内容存入虚存并记录虚存帧号
                    //并且把该虚存页帧的存在时间改为1
                    if(state_pageframe[a][0]==-1)
                    {
                        state_pageframe[a][0]=number_frame;
                        state_pageframe[a][1]=1;
                        v_realframe[a]=realframe[number_frame];
                        vn=a;

                        b=1;
                    }
                    //把其他非空闲帧的存在时间加一
                    else
                        state_pageframe[a][1]++;

                }
                else
                {
                    if(state_pageframe[a][0]!=-1)
                        state_pageframe[a][1]++;
                }

            }
            //在虚存中无法找到非空闲帧
            if(b==0)
            {
                //逐个遍历虚存，找到存在时间最长的一个帧

                m=state_pageframe[0][1];
                n=0;
                for(a=1;a<num_pageframe;a++)
                {
                    if(m<state_pageframe[a][1])
                    {
                        m=state_pageframe[a][1];
                        n=a;
                    }
                }
                //然后用新给定的物理帧号、物理帧内容替换该虚存帧中原先存储的内容并记录虚存帧号
                state_pageframe[n][0]=number_frame;//新的帧号
                state_pageframe[n][1]=1;
                v_realframe[n]=realframe[number_frame];
                vn=n;
            }
        }
        //在虚存中存在已给定的物理帧的情况下，将所有的非空帧存在时间加一
        else
        {
            for(a=0;a<num_pageframe;a++)
            {
                if(state_pageframe[a][0]!=-1)
                    state_pageframe[a][1]++;
            }
        }
    }
    //LRU页面置换算法
    else if(type==2)
    {
        //在虚存中逐个遍历帧，查找是否已经存在给定的物理帧帧号
        for(a=0;a<num_pageframe;a++)
        {
            if(state_pageframe[a][0]==number_frame)
            {
                //给定的物理帧已经存在于虚存中，记录该虚存帧号
                //并且把该帧的存在时间设为0
                state_pageframe[a][1]=0;
                vn=a;
                b=1;
                //在虚存中存在已给定的物理帧的情况下，将所有的非空帧存在时间加一
                for(c=0;c<num_pageframe;c++)
                {
                    if(state_pageframe[c][0]!=-1)
                        state_pageframe[c][1]++;
                }

                break;
            }
        }
        //虚存中不存在已给定的物理帧的情况下
        if(b==0)
        {
            //首先寻找空闲是否有空闲的虚存页帧，同时把非空闲的帧的存在时间加一
            for(a=0;a<num_pageframe;a++)
            {
                if(b==0)
                {
                    if(state_pageframe[a][0]==-1)
                    {
                        //在虚存中找到空闲页帧，则把给定的物理帧的帧号、物理帧存储内容存入虚存并记录虚存帧号
                        //并且把该虚存页帧的存在时间改为1
                        state_pageframe[a][0]=number_frame;//新的帧号
                        state_pageframe[a][1]=1;
                        v_realframe[a]=realframe[number_frame];
                        vn=a;
                        b=1;
                    }

                    else
                        state_pageframe[a][1]++;
                    //把其他非空闲帧的存在时间加一
                }
                else
                {
                    if(state_pageframe[a][0]!=-1)
                        state_pageframe[a][1]++;
                }

            }
            //在虚存中无法找到非空闲帧
            if(b==0)
            {
                //逐个遍历虚存，找到存在时间最长的一个帧
                m=state_pageframe[0][1];
                n=0;
                for(a=1;a<num_pageframe;a++)
                {
                    if(m<state_pageframe[a][1])
                    {
                        m=state_pageframe[a][1];
                        n=a;
                    }
                }
                //然后用新给定的物理帧号、物理帧内容替换该虚存帧中原先存储的内容并记录虚存帧号
                state_pageframe[n][0]=number_frame;//新的帧号
                state_pageframe[n][1]=1;
                v_realframe[n]=realframe[number_frame];
                vn=n;
            }

        }
    }

    //将内存的改变写入到日志文件中
    print(3); //依次记录虚存每个帧的帧号和存在时间 
    print(4); //依次记录虚存每个帧的帧号和存储内容，每次调用依次向下记录 
    print(6); //依次记录虚存每个帧的帧号和存储内容，每次调用清空原有内容，用新内容覆盖 
    
    cout<<"内存读出数据：" ; 
  	cout<< v_realframe[vn].substr(start,length) <<endl ;  //输出帧中从start开始长度为length的字符串 
    return v_realframe[vn].substr(start,length); //返回记录的虚存页帧中从start开始长度为length的字符串

}



//内存的设计存储
int Memory_share::Store(string code) //存储提供的字符串，返回逻辑地址
{
	cout<<"进入函数--"<<"Memory_share::Store()"<<endl;
    //将屏幕读取的数据段转换成字符串格式
    string pcbcode=code;
    //字符串长度
    int length_code=pcbcode.size();
    //该段内容所需要的页表长度
    int length_page;
    length_page=length_code/room_physicframe;

    //无法整除的情况下页表长度加一，最后一帧会出现内部碎片
    if((length_code)%room_physicframe!= 0)
        length_page++;

    //需要用来存放该段页表所需帧的总数
    int num_frame;
    
    if(length_page==0) //当页表长度为0时，页表占的帧数为0
        num_frame=0;
    else if(length_page==1) //页表长度为1，页表占的帧总数为1
        num_frame=1;
        
    /*当页表长度大于1的时候
      通过页表长度和帧大小减一的除取整来获取页表占据的总帧数
      与帧大小减一取整的原因是当页表需要占据多个帧的时候，最后一位为索引位 */ 
    else if((length_page%(room_physicframe-1))<2) //若余数为0或者1的时候，最后一张页表的帧的最后一位即编程数据位而不需要继续索引下一个存页表的帧
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else //当页表长度和帧大小减一取余数大于1的时候，需要多加一张页表来存储
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }
    
    //定义当前页表所在帧的地址和第一张页表所在帧的地址，也就是逻辑地址
    int logic_address,first_address;
    
    if(length_page<=(free_physicframe-num_frame)) //空闲帧数足够存储下这一段内容
    {
        int a=0;
        
        while(state_physicframe[a]!=0) //逐个遍历每个帧的状态，找寻第一个空闲帧
            a++;
        
        state_physicframe[a]=1; //找到第一个空闲帧，将第一个空闲帧的状态由空闲转变为存储页表
								//空闲状态为0，存储页表状态为1，存储数据状态为2 
        free_physicframe--; //减少一个空闲帧数量        
        logic_address=a; //该帧是这段内容的逻辑地址
        first_address=a; //也是首地址 

        int i,m,n,start,length;
        n=0;
        start=0; //从页表的第0位开始，将该段字符串按照物理帧大小分为多块分别存储进物理帧
        length=0;
        
        if((length_code-start)>room_physicframe) //当一个物理帧不足够存储整个页表的情况下，切分长度为物理帧大小
        {
            length=room_physicframe;
        }
        else
        {
            
            length=length_code-start; //如果足够存储，则长度为字符串长度
        }
        for(m=0;m<num_frame;m++) //每页需要帧数 
        {
            for(i=0;i<room_physicframe-1;i++) //物理帧空间大小 
            {
                if(n<length_page)
                {
                    while(state_physicframe[a]!=0) //将字符串按照帧大小分开后，逐个为其寻找空闲状态的帧
                        a++;
                    frame[logic_address][i]=a; //将其内容存储进该帧，将页表状态帧的第二维对应位置的内容改为存储该帧的帧号 
                    state_physicframe[a]=2; //把空闲状态改为存储数据  
                    free_physicframe--;
                    n++;
                    
                    //根据先前计算出的该帧存储的容的长度来切割字符串，并更新切割的起始位置
                    realframe[a]=pcbcode.substr(start,length);
                    
                    start=start+room_physicframe;
                    //更新起始位置之后，再次计算下一个帧内存储的内容的长度
                    
                    if((length_code-start)>room_physicframe) //当一个物理帧不足够存储整个页表的情况下，切分长度为物理帧大小
                    {
                        length=room_physicframe;
                    }
                    else //如果足够存储，则长度为字符串长度
                    {
                        length=length_code-start;
                    }
                }
                
                else //当页帧完全存储成功的情况下，跳出循环
                    break;
            }
            
            //当剩余的内容只需最后一帧可以存储的情况下
            if((length_page-n)==1)
            {
                //寻找空闲帧
                while(state_physicframe[a]!=0)
                    a++;

                //将存储页表的物理帧的最后一位指向该空闲帧，该空闲帧同样存储内容，并修改状态（同上） 
                frame[logic_address][i]=a;
                state_physicframe[a]=2;
                free_physicframe--;
                n++;
                
                realframe[a]=pcbcode.substr(start,length); //切割实际存储内容，并存入内存                
                start=start+room_physicframe; //更新起始位置和长度
                
                if((length_code-start)>room_physicframe)
                {
                    length=room_physicframe;
                }
                else
                {
                    length=length_code-start;
                }
                
                //将页表的帧内存储的内容（都是数字，指向存储内容的帧的帧号）依次转变成字符串并连接
                string pnn="";
                int j,pn;
                for(j=0;j<room_physicframe;j++)
                {
                    if(frame[logic_address][j]!=-1)
                    {

                        pn=frame[logic_address][j];
                        stringstream ss;
                        string buff="";
                        ss<<pn;
                        ss>>buff;
                        //依次连接这些指向帧的数字
                        pnn+=buff;

                    }
                    else
                        break;
                }
                
                //完成全部的连接之后，连接而成的字符串就是存储页表的物理帧内存储的内容
                realframe[logic_address]=pnn;
                break;
            }
            
            //当在存储页表的物理帧的倒数第二位完成全部存储的情况下
            else if((length_page-n)==0)
            {
                //依次连接存储页表的帧内存储的指向存储内容的帧的帧号，并转变成字符串
                //连接完成之后存储进存储页表的帧的内容
                string pnn="";
                int j,pn;
                for(j=0;j<room_physicframe;j++)
                {
                    if(frame[logic_address][j]!=-1)
                    {
                        pn=frame[logic_address][j];
                        stringstream ss;
                        string buff="";
                        ss<<pn;
                        ss>>buff;
                        pnn+=buff;
                    }
                    else
                        break;
                }
                realframe[logic_address]=pnn;
                break;
            }
            //在单个物理帧无法存储整个页表的情况下，将现有存储页表的帧的最后一位指向一个空闲帧
            //该空闲帧将为继续存储剩余的页表，该空闲帧的状态将会改变为存储页表
            else
            {
                while(state_physicframe[a]!=0)
                    a++;

                //将当前存储页表的物理帧的最后一位指向该空闲帧，并使用该空闲帧继续记录页表
                //该帧状态为记录页表，记录值为1，并减少一位空闲帧数量
                frame[logic_address][i]=a;
                state_physicframe[a]=1;//下一个逻辑地址

                //将现有存储页表的帧的状态逐个连接并转换成字符串存储进该帧的内容
                string pnn="";
                int j,pn;
                for(j=0;j<room_physicframe;j++)
                {
                    if(frame[logic_address][j]!=-1)
                    {
                        pn=frame[logic_address][j];
                        stringstream ss;
                        string buff="";
                        ss<<pn;
                        ss>>buff;

                        pnn+=buff;

                    }
                    else
                        break;
                }
                //改变现有存储页表的帧之后，减少一位空闲帧
                realframe[logic_address]=pnn;
                free_physicframe--;
                logic_address=a;
            }

        }
        //将内存的改变情况输出到日志文件
        print(1); //记录每个帧的状态 
        print(2); //记录每个帧存储的内容 
        print(5); //记录内存当前内容 
        //向外返回这段字符串在内存中存储的逻辑地址
        return first_address;
    }
    else //存储这段内容所需的总帧数大于空闲帧数的情况下，返回-1表示存储失败
        return -1;

}

//内存分配判断函数
bool Memory_share::Storejudge(string code)
{
	cout<<"进入函数--"<<"Memory_share::Storejudge()"<<endl;
	
    //将内容转换成字符串并获取字符串长度
    string pcbcode=code;
    int length_code=pcbcode.size();

    //该pcb的页表长度，根据字符串长度和物理帧大小计算出
    int length_page;
    length_page=length_code/room_physicframe;
    //取余不为0，则页表需要增加一页，最后一页出现内部碎片
    if((length_code)%room_physicframe!= 0)
        length_page++;

    //需要用来存放页表的总帧数
    int num_frame;
    //页表长度为0，则存储页表的帧的数量也为0，页表长度为1，则存储页表的帧数量为1
    if(length_page==0)
        num_frame=0;
    else if(length_page==1)
        num_frame=1;
    //否则按照页表长度取整物理帧大小减一来获取存储页表的帧的数量
    //取余如果大于1，则额外需要多一个帧来存储页表，该帧也会有内部碎片
    else if((length_page%(room_physicframe-1))<2)
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }

    //在空闲帧数量足够分配给存储内容的帧和存储页表的帧的情况下，返回true，否则返回false
    if(length_page<=(free_physicframe-num_frame-1))
    {
        return true;
    }
    else
        return false;

}

int Memory_share::num_Storejudge(string code)
{
	cout<<"进入函数--"<<"Memory_share::num_Storejudge()"<<endl;
    //将内容转换成字符串并获取字符串长度
    string pcbcode=code;
    int length_code=pcbcode.size();//代码长度
    //该pcb的页表长度，根据字符串长度和物理帧大小计算出
    int length_page;
    length_page=length_code/room_physicframe;
    //取余不为0，则页表需要增加一页，最后一页出现内部碎片
    if((length_code)%room_physicframe!= 0)
        length_page++;

    //需要用来存放页表的总帧数
    int num_frame;
    //页表长度为0，则存储页表的帧的数量也为0，页表长度为1，则存储页表的帧数量为1
    if(length_page==0)
        num_frame=0;
    else if(length_page==1)
        num_frame=1;
    //否则按照页表长度取整物理帧大小减一来获取存储页表的帧的数量
    //取余如果大于1，则额外需要多一个帧来存储页表，该帧也会有内部碎片
    else if((length_page%(room_physicframe-1))<2)
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }

    //返回总帧数
    return (num_frame+length_page);

}



//PCB初始化申请内存分配，id为PCBID，code为该PCB的代码段
int  Memory_share::storein(int id,string code)
{
	cout<<"进入函数--"<<"Memory_share::storein()"<<endl;
    int i;
    string buffer;
    
    //为数据段预申请40字节的空间，每秒钟设置的输入上限为20字节，为后续输入判断做准备
  //  buffer="****************************************";  //!!!!!!!!!!!!!
    
    //计算代码段和预留数据段总共需要的页帧数量，判断能否为该PCB分配内存空间
    int all=num_Storejudge(code)+num_Storejudge(buffer);

    //空闲帧数量足够为该PCB分配内存的情况下，分配内存并返回2，否则返回0表示分配内存失败
    if(free_physicframe>=all)
    {
        //在onebyone数组中寻找一个空位
        for(i=0;i<number_physicframe;i++)
        {
            if(onebyone[i][0]==-1)
              break;
        }

        //第一位记录PCB号
        onebyone[i][0]=id;
        //第二位记录页表逻辑地址
        onebyone[i][1]=Store(buffer);
        //第三位记录数据段逻辑地址
        onebyone[i][2]=Store(code);
        return 1;
    }
    else
    {
        return 0;
    }

}


//获取物理内存总数
int Memory_share::getroom_physicmemory()
{
    return room_physicmemory;
}

//获取已使用的物理内存大小
int Memory_share::getusedroom_physicframe()
{
    int a;
    //计算方法为总大小减去空闲帧数目和帧大小的乘积
    a=room_physicmemory-free_physicframe*room_physicframe;
    return a;
}


