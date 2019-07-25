#include "process.h"
#include<iostream>
#include<QMessageBox>
using namespace std;
process::process() //构造函数，初始化
{
    runhead=new PPCB;   //运行队列的表头和表尾 及初始化
    runrear=new PPCB;
    runhead->next=runrear;
    runrear->pre=runhead;



    waithead=new PPCB;   //等待队列的表头和表尾  及初始化
    waitrear=new PPCB;
    waithead->next=waitrear;
    waitrear->pre=waithead;
    current=runhead;
    processinit();

}

void process::newprocess()
{
    int b,c;
    if(run_count<10)
        {
            if(this->pri_flag==1)
            {
				 b=10000;
				 c=5;
            }
            else
            {
				 b=(rand()%10+1); //随机产生CPU占用时间和优先级
				 c=(rand()%5+1);
            }
            temp1=runrear->pre;
            insertrun(temp1,runrear,b,c);
        }
    else
    {
        if(this->pri_flag==1)
        {
			b=10000;
			c=5;
        }
        else
        {
			b=(rand()%10+1); //随机产生CPU占用时间和优先级
			c=(rand()%5+1);
        }
            temp2=waitrear->pre;
            insertwait(temp2,waitrear,b,c);
    }
    this->pri_flag==0;
}
void process::processinit()
{
    for(int a=1;a<=10;a++)   //模拟10个进程 先放进去两个队列中 已便演示
    {
        int b=((rand()%50+1)+1000); //随机产生CPU占用时间和优先级
        int c=(rand()%5+1);
        if(run_count<5)
        {
            temp1=runrear->pre;
            insertrun(temp1,runrear,b,c);
        }
        else
        {
            temp2=waitrear->pre;
            insertwait(temp2,waitrear,b,c);
        }
    }
    showprocess(runhead,runrear,1);
    showprocess(waithead,waitrear,2);
}
void process::insertrun(PPCB *p,PPCB *q,int cputime,int priority)//将进程插入运行队列
{
    processid++;   //进程编号
    runcount++;		
	//运行中的进程数量
    run_count++;
    //flag_count++;
    PPCB *newp=new PPCB;
    newp->id=processid;
    newp->cputime=cputime;
    newp->lefttime=cputime;
    newp->state=1;
    newp->priority=priority;
    newp->pre=p;
    newp->next=q;
    q->pre=newp;
    p->next=newp;   //将新进程插入p，q之间 
}

void process::insertwait(PPCB *p,PPCB *q,int cputime,int priority) //p为之前等待队列的最后一个进程，q为等待队列的尾指针？？？这句话怎么理解呢？
{
    processid++;
    waitcount++;
    wait_count++;
    //flag_count++;
    PPCB *newp=new PPCB;
    newp->id=processid;
    newp->cputime=cputime;
    newp->lefttime=cputime;  //进程尚未执行，CPU时间尚未减少
    newp->state=2;
    newp->priority=priority;
    newp->pre=p;
    newp->next=q;		//q依然是尾指针
    q->pre=newp;		//newp变成了新的最后一个进程
    p->next=newp;
}

void process::showprocess(PPCB *p,PPCB *q,int type)  //p为查询队列的头指针 ,q为查询队列的尾指针
{
    int count=0;
    if(type==1)//运行队列
    {
		if(p->next==q)
		   {
				this->runcount=0;
				this->waitcount=0;
				this->run_count=0;
				this->wait_count=0;
				count=0;
		   }
		else
		{
			int l=0;
			//cout<<"位置\tID\t总时间\t剩余时间\t状态\t优先级"<<endl;
			while(p->next!=q)
			{
				p=p->next;
				run_queue[l].id=p->id;
				run_queue[l].cputime=p->cputime;
				run_queue[l].lefttime=p->lefttime;
				run_queue[l].pre=p->pre;
				run_queue[l].priority=p->priority;
				run_queue[l].state=p->state;

				all_queue[count].id=p->id;
				all_queue[count].cputime=p->cputime;
				all_queue[count].lefttime=p->lefttime;
				all_queue[count].pre=p->pre;
				all_queue[count].priority=p->priority;
				all_queue[count].state=p->state;
				l++;
				count++;
			}
		}
    }
    else//等待队列
    {
        if(p->next==q)
           {
				this->runcount=0;
				this->waitcount=0;
				count=0;
           }
        else
        {
            int l=0;  //数组下标使用
            //cout<<"位置\tID\t总时间\t剩余时间\t状态\t优先级"<<endl;
            while(p->next!=q)
			{
				 p=p->next;
				 wait_queue[l].id=p->id;
				 wait_queue[l].cputime=p->cputime;
				 wait_queue[l].lefttime=p->lefttime;
				 wait_queue[l].pre=p->pre;
				 wait_queue[l].priority=p->priority;
				 wait_queue[l].state=p->state;

				 all_queue[count].id=p->id;
				 all_queue[count].cputime=p->cputime;
				 all_queue[count].lefttime=p->lefttime;
				 all_queue[count].pre=p->pre;
				 all_queue[count].priority=p->priority;
				 all_queue[count].state=p->state;

                 l++;
                 //wait_count=l;
                 count++;
            }
        }
    }

}

void process::backprocess(PPCB *p,PPCB *r,PPCB *q,int ID)  //p为运行队列的头指针，r为运行队列的尾指针
                                        //q为等待队列的最后一个进程
{//将进程从运行队列中放入等待队列
    p=p->next;
    while((p->id!=ID)&&(p!=r))
    {
        p=p->next;
    }
    if(p->id==ID)
    {
        p->pre->next=p->next;
        p->next->pre=p->pre;
        p->pre=q;
        p->next=q->next;
        p->state=2;
        q->next->pre=p;
        q->next=p;
        run_count--;
        wait_count++;
        runcount--;
        waitcount++;
        //cout<<"成功将该进程放入等待队列！"<<endl;
    }
    //showprocess(runhead,runrear,1);
    //showprocess(waithead,waitrear,2);
}

void process::wakeupprocess(int ID,PPCB *p,PPCB *r,PPCB *q)    //p为等待队列的头指针，r为等待队列的尾指针
                                            //q为运行队列的最后一个进程
{//将进程从等待队列放入到运行队列中

    p=p->next;
    while((p->id!=ID)&&(p!=r))
    {
        p=p->next;
    }
    if(p->id==ID)
    {
        p->pre->next=p->next;
        p->next->pre=p->pre;;
        p->pre=q;
        p->next=q->next;
        p->state=1;
        q->next->pre=p;
        q->next=p;
        wait_count--;
        run_count++;
        //cout<<"一个进程加入到运行队列！"<<endl;
    }
    //showprocess(runhead,runrear,1);
    //showprocess(waithead,waitrear,2);
}

void process::deleteprocess(PPCB *p)
{//将进程从运行队列中删除
    p->pre->next=p->next;
    p->next->pre=p->pre;
    run_count--;
    delete(p);
    //showprocess(runhead,runrear,1);
    //showprocess(waithead,waitrear,2);
}

void process::sortprocess(PPCB *p,PPCB *q,int count)  //p为排序队列的头指针，q为排序队列的尾指针 ,count为队列的进程计数
{
    int a=1;
    PPCB *r;
    r=p->next;
    for(a=1;a<=count;a++)
    {
        while(r->next!=q)
        {
            if((r->priority)<(r->next->priority))  //根据优先级进行排序
                swapprocess(r,r->next);
            else
                r=r->next;
        }
        r=p->next;
    }
    //showprocess(runhead,runrear,1);
   // showprocess(waithead,waitrear,2);
}

void process::sortprocess2(PPCB *p,PPCB *q,int count)  //p为排序队列的头指针，q为排序队列的尾指针 ,count为队列的进程计数
{
    int a=1;
    PPCB *r;
    r=p->next;
    for(a=1;a<=count;a++)
    {
        while(r->next!=q)
        {
            if((r->lefttime)>(r->next->lefttime))			//根据剩余时间进行排序，SJF
                swapprocess(r,r->next);
            else
                r=r->next;
        }
        r=p->next;
    }
    //showprocess(runhead,runrear,1);
    //showprocess(waithead,waitrear,2);
}

void process::swapprocess(PPCB *p,PPCB *q)
{
    p->pre->next=q;
    q->next->pre=p;
    q->pre=p->pre;
    p->next=q->next;
    q->next=p;
    p->pre=q;

}

void process::R_R(int k,int j,int cputime,int priority,int ID)//k 表示选择的是模拟进程还是中断等操作
{
        if(k==1)//模拟进程操作
        {
            if(run_count==0)
             run_count=0;
            else
            {
                while((run_count<5)&&(wait_count>0))
                                {
                                    int ID=waithead->next->id;
                                    temp1=runrear->pre;
                                    wakeupprocess(ID,waithead,waitrear,temp1);
                                }
                current=runhead->next;
                current->lefttime=current->lefttime-timeslip;//时间片的选择暂时设置为1
                if(current->lefttime<=0)//当前进程完成
                {
                    if(wait_count>0)   //等待的进程数大于0，唤醒进程
                    {
                        int ID=waithead->next->id;
                        temp1=runrear->pre;
                        deleteprocess(current);
                        wakeupprocess(ID,waithead,waitrear,temp1);
                    }
                    else
                        deleteprocess(current);
                }
                else//时间片轮转到下一个进程
                {
                    runhead->next=current->next;
                    current->next->pre=runhead;

                    current->pre=runrear->pre;
                    runrear->pre->next=current;
                    runrear->pre=current;
                    current->next=runrear;
                }
            }
        }
        else if(k==2)       //中断操作
        {
            cout<<"interrupt"<<j<<endl;
            switch(j)
            {
                case 1: //添加进程
                    {
                        //cout<<"请输入创建进程的cpu占用时间"<<endl;
                        //cin>>cputime;
                        //cout<<"请输入创建进程的优先级"<<endl;
                        //cin>>priority;
                        if(run_count<20)
                        {
                            temp1=runrear->pre;
                            insertrun(temp1,runrear,cputime,priority);
                        }
                        else
                        {
                            temp2=waitrear->pre;
                            insertwait(temp2,waitrear,cputime,priority);
                        }
                        break;
                    }
                case 2:
                    {
                     ;
                    }
                case 3:			//运行队列中的进程加入到等待队列中，阻塞进程
                    {

                        if(run_count==0)
                        {
                            run_count=0;
                            //cout<<"运行队列为空！无法进行此操作！"<<endl;
                        }
                        else
                        {

                            temp2=waitrear->pre;
                            backprocess(runhead,runrear,temp2,ID);

                        }
                        break;
                    }
                case 4:		//唤醒进程
                    {
                cout<<"case4["<<ID<<"]"<<endl;
                        if(wait_count==0)
                           QMessageBox::about(NULL,"Tip","The wait queue is empty");
                            //cout<<"等待队列为空！无法进行此操作！"<<endl;
                        else if(run_coun2t==20)
                            QMessageBox::about(NULL,"Tip","The run queue is full");
                            //run_count=20;
                            //cout<<"运行队列已满，无法加入更多的进程！"<<endl;
                        else
                        {
                            temp1=runrear->pre;
                            wakeupprocess(ID,waithead,waitrear,temp1);
                        }
                        break;
                    }
                case 5:  //删除进程
                    {
                        int ID;
                        PPCB *p=runhead->next;
                        //cout<<"请输入要删除的进程ID："<<endl;
                        //cin>>ID;
                        while((p->id!=ID)&&(p!=runrear))
                        {
                            p=p->next;
                        }
                        if(p->id==ID)
                        {
                            deleteprocess(p);
                        }
                        else
                            //cout<<"没有找到该进程，请检查是否存在该进程！"<<endl;
                            break;
                    }
                default:
                    break;
            }

        }
        else
        {
            i=0;
        }
        showprocess(runhead,runrear,1);
        showprocess(waithead,waitrear,2);
    }

void process::FCFS_pro(int k,int j,int cputime,int priority,int ID)//k是1：模拟进程调度  2：中断操作(创建、查询、阻塞、唤醒、删除进程)  3：结束调度"，j是内部操作
{
    //初始化排序优先级
        if(run_count>1)
            sortprocess(runhead,runrear,runcount);
        if(wait_count>1)
            sortprocess(waithead,waitrear,waitcount);
        //选择了优先级调度，已经按照优先级给各个队列排序

        //int k=0;
        //cout<<"请选择执行的操作：\n1：模拟进程调度\n2：中断操作(创建、查询、阻塞、唤醒、删除进程)\n3：结束调度"<<endl;
        //cin>>k;
        if(k==1)
        {
            if(run_count==0)
                run_count=0;
                //cout<<"运行队列为空！"<<endl;
            else
            {
                while((run_count<5)&&(wait_count>0))
                                {
                                    int ID=waithead->next->id;
                                    temp1=runrear->pre;
                                    wakeupprocess(ID,waithead,waitrear,temp1);
                                }
                current=runhead->next;
                current->lefttime=current->lefttime-timeslip;//时间片的选择暂时设置为1
                if(current->lefttime<=0)
                {
                    if(wait_count>0)
                    {
                        int ID=waithead->next->id;
                        temp1=runrear->pre;
						//先删后唤醒
                        deleteprocess(current);
                        wakeupprocess(ID,waithead,waitrear,temp1);
                    }
                    else
                        deleteprocess(current);
                }
            }
        }
        else if(k==2)       //中断操作
        {
            switch(j)
            {
                case 1:
                    {
                        //cout<<"请输入创建进程的cpu占用时间"<<endl;
                        //cin>>cputime;
                        //cout<<"请输入创建进程的优先级"<<endl;
                        //cin>>priority;
                        if(run_count<20)
                        {
                            temp1=runrear->pre;
                            insertrun(temp1,runrear,cputime,priority);
                            if(run_count>1)
                                sortprocess(runhead,runrear,run_count);
                        }
                        else
                        {
                            temp2=waitrear->pre;
                            insertwait(temp2,waitrear,cputime,priority);
                            if(wait_count>1)
                                sortprocess(waithead,waitrear,wait_count);
                        }
                        break;
                    }
                case 2:
                    {
                      ;
                    }
                case 3:		//阻塞
                    {
                        if(run_count==0)
                        {
							run_count=0;   //<<"运行队列为空！无法进行此操作！"<<endl;
                        }
                        else
                        {
                            temp2=waitrear->pre;
                            backprocess(runhead,runrear,temp2,ID);
                            sortprocess(waithead,waitrear,wait_count);
                        }
                        break;
                    }
                case 4:		//唤醒
                    {
                        if(wait_count==0)
                            wait_count=0;//cout<<"等待队列为空！无法进行此操作！"<<endl;
                        else if(run_count==20)
                            run_count=20; //cout<<"运行队列已满，无法加入更多的进程！"<<endl;
                        else
                        {
                            //cout<<"请输入要唤醒的进程ID："<<endl;
                            //cin>>ID;
                            temp1=runrear->pre;
                            wakeupprocess(ID,waithead,waitrear,temp1);
                            sortprocess(runhead,runrear,run_count);
                        }
                        break;
                    }
                case 5:		//删除
                    {
                        PPCB *p=runhead->next;
                        //cout<<"请输入要删除的进程ID："<<endl;
                        //cin>>ID;
                        while((p->id!=ID)&&(p!=runrear))
                        {
                            p=p->next;
                        }
                        if(p->id==ID)
                        {
                            deleteprocess(p);
                        }
                        else
                            //cout<<"没有找到该进程，请检查是否存在该进程！"<<endl;
                            break;
                    }
                default:
                    break;
            }
        }
        else
        {
            i=0;
        }
        showprocess(runhead,runrear,1);
        showprocess(waithead,waitrear,2);
}

void process::SJF(int k,int j,int cputime,int priority,int ID)//短作业优先，SJF
{
        if(run_count>1)
            sortprocess2(runhead,runrear,run_count);
        if(wait_count>1)
            sortprocess2(waithead,waitrear,wait_count);
        //cout<<"选择了最短作业优先调度，已经按照作业时间给各个队列排序！"<<endl;
        if(k==1)//模拟进程调度
        {
            if(run_count==0)
                run_count=0;
                //cout<<"运行队列为空！"<<endl;
            else
            {
                while((run_count<5)&&(wait_count>0))
                                {
                                    int ID=waithead->next->id;
                                    temp1=runrear->pre;
                                    wakeupprocess(ID,waithead,waitrear,temp1);
                                }
                current=runhead->next;
                current->lefttime=current->lefttime-timeslip;//时间片的选择暂时设置为1，？？？这个有什么用吗
                if(current->lefttime<=0)
                {
                    if(wait_count>0)
                    {
                        int ID=waithead->next->id;
                        temp1=runrear->pre;
                        deleteprocess(current);
                        wakeupprocess(ID,waithead,waitrear,temp1);
                    }
                    else
                        deleteprocess(current);
                }
            }
        }
        else if(k==2)       //中断操作
        {
            switch(j)
            {
                case 1:
                    {

                        //cout<<"请输入创建进程的cpu占用时间"<<endl;
                        //cin>>cputime;
                        //cout<<"请输入创建进程的优先级"<<endl;
                        //cin>>priority;
                        if(run_count<20)
                        {
                            temp1=runrear->pre;
                            insertrun(temp1,runrear,cputime,priority);
                            if(run_count>1)
                                sortprocess(runhead,runrear,run_count);
                        }
                        else
                        {
                            temp2=waitrear->pre;
                            insertwait(temp2,waitrear,cputime,priority);
                            if(wait_count>1)
                                sortprocess(waithead,waitrear,wait_count);
                        }
                        break;
                    }
                case 2:
                    {
                        ;
                    }
                case 3:
                    {
                        if(run_count==0)
                        {
                            run_count=0;//cout<<"运行队列为空！无法进行此操作！"<<endl;
                        }
                        else
                        {
                            temp2=waitrear->pre;
                            backprocess(runhead,runrear,temp2,ID);
                            sortprocess(waithead,waitrear,wait_count);
                        }
                        break;
                    }
                case 4:
                    {
                        if(wait_count==0)
                           wait_count=0;//cout<<"等待队列为空！无法进行此操作！"<<endl;
                        else if(run_count==20)
                            run_count==20;//cout<<"运行队列已满，无法加入更多的进程！"<<endl;
                        else
                        {
                            //cout<<"请输入要唤醒的进程ID："<<endl;
                            //cin>>ID;
                            temp1=runrear->pre;
                            wakeupprocess(ID,waithead,waitrear,temp1);
                            sortprocess(runhead,runrear,run_count);
                        }
                        break;
                    }
                case 5:
                    {

                        PPCB *p=runhead->next;
                        //cout<<"请输入要删除的进程ID："<<endl;
                        //cin>>ID;
                        while((p->id!=ID)&&(p!=runrear))
                        {
                            p=p->next;
                        }
                        if(p->id==ID)
                        {
                            deleteprocess(p);
                        }
                        else
                            //cout<<"没有找到该进程，请检查是否存在该进程！"<<endl;
                            break;
                    }
                default:
                    break;
            }
        }
        else
        {
            i=0;
        }
        showprocess(runhead,runrear,1);
        showprocess(waithead,waitrear,2);
}

void process::FCFS(int k,int j,int cputime,int priority,int ID)
{
        if(k==1)
        {
            if(run_count==0)
                run_count=0;//cout<<"运行队列为空！"<<endl;
            else
            {
                while((run_count<5)&&(wait_count>0))
                                {
                                    int ID=waithead->next->id;
                                    temp1=runrear->pre;
                                    wakeupprocess(ID,waithead,waitrear,temp1);
                                }

                current=runhead->next;
                current->lefttime=current->lefttime-timeslip;//时间片的选择暂时设置为1
                if(current->lefttime<=0)
                {
                    if(wait_count>0)
                    {
                        int ID=waithead->next->id;
                        temp1=runrear->pre;
                        deleteprocess(current);
                        wakeupprocess(ID,waithead,waitrear,temp1);
                    }
                    else
                        deleteprocess(current);
                }
            }
        }
        else if(k==2)       //中断操作
        {
            //cout<<"请选择执行的操作：\n1：创建进程\n2：查询进程\n3：阻塞进程\n4：唤醒进程\n5：删除进程"<<endl;
            //cin>>j;
            switch(j)
            {
                case 1:
                    {

                        //cout<<"请输入创建进程的cpu占用时间"<<endl;
                        //cin>>cputime;
                        //cout<<"请输入创建进程的优先级"<<endl;
                        //cin>>priority;
                        if(run_count<20)
                        {
                            temp1=runrear->pre;
                            insertrun(temp1,runrear,cputime,priority);
                        }
                        else
                        {
                            temp2=waitrear->pre;
                            insertwait(temp2,waitrear,cputime,priority);
                        }
                        break;
                    }
                case 2://这个可以去掉
                    {
                        ;
                    }
                case 3:
                    {
                        if(run_count==0)
                        {
                            run_count=0; //运行队列为空，无法进行此操作
                        }
                        else
                        {
                            temp2=waitrear->pre;
                            backprocess(runhead,runrear,temp2,ID);
                        }
                        break;
                    }
                case 4:
                    {
                        if(wait_count==0)
                            wait_count=0;//cout<<"等待队列为空！无法进行此操作！"<<endl;
                        else if(run_count==20)
                            run_count=20;//cout<<"运行队列已满，无法加入更多的进程！"<<endl;
                        else
                        {
                            ;//cout<<"请输入要唤醒的进程ID："<<endl;
                            ;//cin>>ID;
                            temp1=runrear->pre;
                            wakeupprocess(ID,waithead,waitrear,temp1);
                        }
                        break;
                    }
                case 5:
                    {
                        PPCB *p=runhead->next;
                        //cout<<"请输入要删除的进程ID："<<endl;
                        //cin>>ID;
                        while((p->id!=ID)&&(p!=runrear))
                        {
                            p=p->next;
                        }
                        if(p->id==ID)
                        {
                            deleteprocess(p);
                        }
                        else
                            //cout<<"没有找到该进程，请检查是否存在该进程！"<<endl;
                            break;
                    }
                default:
                    break;
            }

        }
        else
        {
            i=0;
        }
        showprocess(runhead,runrear,1);
        showprocess(waithead,waitrear,2);
}

bool process::findmatpcb(int name) //查找进程
{
      for(int i=0;i<this->run_count;i++)
      {
          if(this->run_queue[i].id==name)
          {
              PPCB *p=runhead->next;
              while((p->id!=name)&&(p!=runrear))
              {
                  p=p->next;
              }
              if(p->id==name)
              {
                  deleteprocess(p);
              }
              this->run_count--;
              return true;
          }
      }
      for(int i=0;i<this->wait_count;i++)
      {
          if(wait_queue[i].id==name)
          {
              PPCB *p=waithead->next;
              while((p->id!=name)&&(p!=waitrear))
              {
                  p=p->next;
              }
              if(p->id==name)
              {
                  deleteprocess(p);
              }
              return true;
          }
      }
      return false;
}
