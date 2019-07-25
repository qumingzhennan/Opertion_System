#include "Segment.h"
#include "PCB.h"
/*		int deletePCB(int outChoi,int id);		//**释放内存		*/
// outChoi = 1时，删除查询 id 的段表以及在内存分配的空间，
// outChoi = 1时，把查询 id 的段表插入到段表链表队尾，state状态置 0 以及 在内存分配的空间，
int MemorySegment::deletePCB(int outChoi,int id)
{
	cout<<"Segment：：in deletePCB"<<endl;
    SegTableP p;
    SegTableP currentP=PCBSegP;

    if(PCBSegP==NULL)
    {
        cout<<"process"<<id<<" is not exist!"<<endl;
        return 0;
    }
    else
    {
        int find=0; //**表示在进程链表中是否找到对应进程，1表示找到，0表示没有，找到了删除对应段表
        int pro = PCBSegP->getProcessid();
        
        //找到对应进程号的段表 （赋值给 p ），把其从PCBSegP段表链中删除
        if(pro == id)
        {
            p=PCBSegP;
            PCBSegP=PCBSegP->next;
            currentP=PCBSegP;
            find = 1;
        }
        else
        {
            find = 0;
            while(currentP->next!=NULL && find==0)
            {
                int pro=currentP->next->getProcessid();
                
                if(pro == id)
                {
                    p=currentP->next;
                    currentP->next=currentP->next->next;
                    find = 1;
                }
                else
                {
                    currentP=currentP->next;
                }
            }

        }	//end of else-if(pro == id) 
        
        
        if(0 == find)		//**如果 没有找到 对应进程的段表
        {
           cout<<"process"<<id<<" is not exist!"<<endl;
            return 0;
        }
        else			//**如果找到了对应进程的段表
        {
            //在占用段链表删除该进程占用的内存段
            for(int i=0;i<=2;i++)
            {
                MemoryManageP currentUseP=useP;
                if(currentUseP==NULL)
                {
                    cout<<"useP segment error!"<<endl;
                    return 0;
                }
                else
                {	//找到 使用段，并用 delP 删除 
                    if(useP->startAddr==p->startAddr[i] && useP->length==p->size[i])
                    {
                        MemoryManageP delP;
                        delP=useP;
                        useP=useP->next;
                        currentUseP=useP;
                        delete delP;
                        delP=NULL;
                    }
                    else
                    {
                        int find1=0;
                        while(currentUseP->next!=NULL && find1==0)
                        {
                            if(currentUseP->next->startAddr==p->startAddr[i] && currentUseP->next->length==p->size[i] )
                            {
                                MemoryManageP delP;
                                delP=currentUseP->next;
                                currentUseP->next=currentUseP->next->next;
                                find1=1;
                                delete delP;
                                delP=NULL;
                            }
                            else
                                currentUseP=currentUseP->next;
                        }
                        if(0==find1)
                        {
                           cout<<"process"<<id<<" delete failed!"<<endl;
                           return 0;
                        }
                    }
                }
            }	//end of for() 在占用段链表删除该进程占用的内存段 
            
            
            //在空闲段插入释放的内存 ，先确认模式，在逐段插入
			 
            //最先适配法插入过程：：按 地址从小到大顺序 插入空闲链表 
            if(mode==1)
            {
                MemoryManageP currentFreeP=freeP;
                for(int i=0;i<=2;i++)
                {
                    //先判断头指针
                    if(freeP->startAddr >= p->startAddr[i])
                    {	//在头指针前（挨着 或 不挨着） 
                        MemoryManageP m=new MemoryManage;
                        if(freeP->startAddr == p->startAddr[i]+p->size[i] || freeP->startAddr+freeP->length==p->startAddr[i])
                        {
                            freeP->startAddr=p->startAddr[i];
                            freeP->length=freeP->length+ p->size[i];
                        }
                        else
                        {
                            m->startAddr=p->startAddr[i];
                            m->length=p->size[i];
                            m->next=freeP;
                            freeP=m;
                            currentFreeP=freeP;
                        }

                    }
                    else if(freeP->startAddr+freeP->length == p->startAddr[i] )
                    {	// 在头指针后 且挨着 
                        MemoryManageP m=new MemoryManage;
                        if(freeP->next == NULL)
                        {
                            freeP->length=freeP->length+p->size[i];
                        }
                        else if(freeP->next->startAddr == p->startAddr[i]+p->size[i])
                        {
                            freeP->length=freeP->length+p->size[i]+freeP->next->length;
                            MemoryManageP ptr;
                            ptr=freeP->next;
                            freeP->next=freeP->next->next;
                            delete ptr;
                            ptr=NULL;
                        }
                        else
                        {
                            freeP->length=freeP->length+p->size[i];
                        }
                    }
                    //再判断头指针后的节点
                    else
                    {	// currentFreeP=freeP 
                        int find=0;
                        while(currentFreeP->next!=NULL && find==0)
                        {
                            if(currentFreeP->next->startAddr >= p->startAddr[i])
                            {
                                MemoryManageP m=new MemoryManage;
                                if(currentFreeP->startAddr+currentFreeP->length == p->startAddr[i] || currentFreeP->startAddr==p->startAddr[i]+p->size[i])
                                {
                                    currentFreeP->length=currentFreeP->length+p->size[i];
                                    if(currentFreeP->next->startAddr == p->startAddr[i]+p->size[i])
                                    {
                                        MemoryManageP ptr;
                                        ptr=currentFreeP->next;
                                        currentFreeP->length=currentFreeP->length+currentFreeP->next->length;
                                        currentFreeP->next=currentFreeP->next->next;
                                        delete ptr;
                                        ptr=NULL;
                                    }
                                }
                                else
                                {
                                    if(currentFreeP->next->startAddr == p->startAddr[i]+p->size[i] || currentFreeP->next->startAddr+currentFreeP->next->length==p->startAddr[i])
                                    {
                                        currentFreeP->next->startAddr=p->startAddr[i];
                                        currentFreeP->next->length=currentFreeP->next->length+p->size[i];
                                    }
                                    else
                                    {
                                        m->startAddr=p->startAddr[i];
                                        m->length=p->size[i];
                                        m->next=currentFreeP->next;
                                        currentFreeP->next=m;
                                    }
                                }
                                find=1;
                            }
                            else
                            {
                                currentFreeP=currentFreeP->next;
                            }
                        }
                        if(find==0)
                        {
                            if(currentFreeP->startAddr+currentFreeP->length == p->startAddr[i] || currentFreeP->startAddr == p->startAddr[i]+p->size[i])
                            {
                                currentFreeP->length=currentFreeP->length+p->size[i];
                            }
                            else
                            {
                                MemoryManageP m=new MemoryManage;
                                m->startAddr=p->startAddr[i];
                                m->length=p->size[i];
                                m->next=NULL;
                                currentFreeP->next=m;
                            }
                        }
                    }
                } 	//end of for(i=0~2)
                
                
				//更新 空闲段空间大小 以及 占用段空间大小
                int k=p->size[0]+p->size[1]+p->size[2];
                setFreeSize(freeSize+k);
                setUseSize(useSize-k);
            }//end of mode 1

            //最坏适配法：：按 空间由大到小顺序 插入空闲链表  
            else if(mode==2)
            {
                MemoryManageP currentFreeP=useP;
                for(int i=0;i<=2;i++)
                {
                    MemoryManageP m=new MemoryManage;
                    m->startAddr=p->startAddr[i];
                    m->length=p->size[i];
                    m->next=NULL;
                    
                    //先判断头指针是否为空
                    if(freeP==NULL)
                    {
                        freeP=m;
                    }
                    else
                    {
                        //判断头结点是否可以合并空闲块
                        if(freeP->startAddr == m->startAddr+m->length || freeP->startAddr+freeP->length == m->startAddr)
                        {
                            if(freeP->startAddr == m->startAddr+m->length)
                            {	//与第一块 前向合并 
                                m->length=m->length+freeP->length;
                                m->next=NULL;
                                MemoryManageP ptr=freeP;
                                freeP=freeP->next;
                                delete ptr;
                                ptr=NULL;
                            }
                            else
                            {	// 与第一块 后向合并 
                                m->length=m->length+freeP->length;
                                m->startAddr=freeP->startAddr;
                                m->next=NULL;
                                MemoryManageP ptr=freeP;
                                freeP=freeP->next;
                                delete ptr;
                                ptr=NULL;
                            }
                            
                            if(freeP != NULL){
                            	//头结点合并空闲块后 是否可以合并更新的头结点（即第二块） 
	                            if(freeP->startAddr == m->startAddr+m->length || freeP->startAddr+freeP->length == m->startAddr)
	                            {
	                                if(freeP->startAddr == m->startAddr+m->length)
	                                {
	                                    m->length=m->length+freeP->length;
	                                    m->next=NULL;
	                                    MemoryManageP ptr=freeP;
	                                    freeP=freeP->next;
	                                    delete ptr;
	                                    ptr=NULL;
	                                }
	                                else
	                                {
	                                    m->length=m->length+freeP->length;
	                                    m->startAddr=freeP->startAddr;
	                                    m->next=NULL;
	                                    MemoryManageP ptr=freeP;
	                                    freeP=freeP->next;
	                                    delete ptr;
	                                    ptr=NULL;
	                                }
	                            }
	                            //当前头结点（即第二块）不能合并空闲块，再在后面的结点找
	                            else
	                            {
	                                currentFreeP=freeP;
	                                while(currentFreeP->next!=NULL)
	                                {
	                                    if(currentFreeP->next->startAddr == m->startAddr+m->length)
	                                    {
	                                        m->length=m->length+currentFreeP->next->length;
	                                        m->next=NULL;
	                                        MemoryManageP ptr=currentFreeP->next;
	                                        currentFreeP->next=currentFreeP->next->next;
	                                        delete ptr;
	                                        ptr=NULL;
	                                    }
	                                    else if(currentFreeP->next->startAddr+currentFreeP->next->length == m->startAddr)
	                                    {
	                                        m->length=m->length+currentFreeP->next->length;
	                                        m->startAddr=currentFreeP->next->startAddr;
	                                        m->next=NULL;
	                                        MemoryManageP ptr=currentFreeP->next;
	                                        currentFreeP->next=currentFreeP->next->next;
	                                        delete ptr;
	                                        ptr=NULL;
	                                    }
	                                    else
	                                    {
	                                        currentFreeP=currentFreeP->next;
	                                    }
	                                }
	                            }
	                        } //end of if()头结点合并空闲块后 是否可以合并更新的头结点（即第二块）  
	                        
	                    }	//end  of if() 头结点可以合并空闲块 
	                    else
	                    {	//头结点不能合并空闲块
	                        currentFreeP=freeP;
	                        while(currentFreeP->next != NULL)
	                        {
	                            if(currentFreeP->next->startAddr == m->startAddr+m->length)
	                            {
	                                m->length=m->length+currentFreeP->next->length;
	                                m->next=NULL;
	                                MemoryManageP ptr=currentFreeP->next;
	                                currentFreeP->next=currentFreeP->next->next;
	                                delete ptr;
	                                ptr=NULL;
	                            }
	                            else if(currentFreeP->next->startAddr+currentFreeP->next->length == m->startAddr)
	                            {
	                                m->length=m->length+currentFreeP->next->length;
	                                m->startAddr=currentFreeP->next->startAddr;
	                                m->next=NULL;
	                                MemoryManageP ptr=currentFreeP->next;
	                                currentFreeP->next=currentFreeP->next->next;
	                                delete ptr;
	                                ptr=NULL;
	                            }
	                            else
	                            {
	                                currentFreeP=currentFreeP->next;
	                            }
	                        }
                        } 
                   	 	//end of 合并空闲块 
                        
	                    //把合并后的空闲块插入到空闲段链表
	                    MemoryManageP currentFreeP=freeP;
	                    if(freeP==NULL)
	                    {
	                        freeP=m;
	                    }
	                    else if(freeP->length <= m->length)
	                    {
	                        m->next=freeP;
	                        freeP=m;
	                        currentFreeP=freeP;
	                    }
	                    else
	                    {
	                        while(currentFreeP->next != NULL)
	                        {
	                            if(currentFreeP->next->length <= m->length)
	                            {
	                                m->next=currentFreeP->next;
	                                currentFreeP->next=m;
	                                break;
	                            }
	                            else
	                            {
	                                currentFreeP=currentFreeP->next;
	                            }
	                        }
	                        if(currentFreeP->next == NULL)
	                            currentFreeP->next=m;
	                    }
	                    //完成 把合并后的块 插入 链表 
	                }//end  of if() freeP ==　NULL 
	            }//end of for(0~2)
	                
	            //更新 空闲段空间大小 以及 占用段空间大小
	            int k=p->size[0]+p->size[1]+p->size[2];
	            setFreeSize(freeSize+k);
	            setUseSize(useSize-k);
	        }//end of mode 2

            //最佳适配法：：把空闲块 从小到大顺序 排列 
            else if(mode==3)
            {
                MemoryManageP currentFreeP=freeP;
                for(int i=0;i<=2;i++)
                {
                    MemoryManageP m=new MemoryManage;
                    m->startAddr=p->startAddr[i];
                    m->length=p->size[i];
                    m->next=NULL;
                    
                    //先判断头指针是否为空
                    if(freeP==NULL)
                    {
                        //freeP=m;
                    }
                    else
                    {
                        //判断头结点是否可以合并空闲块
                        if(freeP->startAddr == m->startAddr+m->length || freeP->startAddr+freeP->length == m->startAddr)
                        {
                            if(freeP->startAddr == m->startAddr+m->length)
                            {	//和头结点前向 合并 
                                m->length=m->length+freeP->length;
                                m->next=NULL;
                                MemoryManageP ptr=freeP;
                                freeP=freeP->next;
                                delete ptr;
                                ptr=NULL;
                            }
                            else
                            {	//和 头结点后向 合并 
                                m->length=m->length+freeP->length;
                                m->startAddr=freeP->startAddr;
                                m->next=NULL;
                                MemoryManageP ptr=freeP;
                                freeP=freeP->next;
                                delete ptr;
                                ptr=NULL;
                            }
                            
                            
                            if(freeP!=NULL)
                            {
                                //头结点合并空闲块后 是否可以合并更新的头结点（即第二块） 
                                if(freeP->startAddr == m->startAddr+m->length || freeP->startAddr+freeP->length == m->startAddr)
                                {
                                    if(freeP->startAddr == m->startAddr+m->length)
                                    {
                                        m->length=m->length+freeP->length;
                                        m->next=NULL;
                                        MemoryManageP ptr=freeP;
                                        freeP=freeP->next;
                                        delete ptr;
                                        ptr=NULL;
                                    }
                                    else
                                    {
                                        m->length=m->length+freeP->length;
                                        m->startAddr=freeP->startAddr;
                                        m->next=NULL;
                                        MemoryManageP ptr=freeP;
                                        freeP=freeP->next;
                                        delete ptr;
                                        ptr=NULL;
                                    }
                                }
                                //当前头结点不能合并空闲块，再在后面的结点找
                                else
                                {
                                    //int findP=0;
                                    currentFreeP=freeP;
                                    while(currentFreeP->next!=NULL)
                                    {
                                        if(currentFreeP->next->startAddr == m->startAddr+m->length)
                                        {
                                            m->length=m->length+currentFreeP->next->length;
                                            m->next=NULL;
                                            MemoryManageP ptr=currentFreeP->next;
                                            currentFreeP->next=currentFreeP->next->next;
                                            delete ptr;
                                            ptr=NULL;
                                        }
                                        else if(currentFreeP->next->startAddr+currentFreeP->next->length == m->startAddr)
                                        {
                                            m->length=m->length+currentFreeP->next->length;
                                            m->startAddr=currentFreeP->next->startAddr;
                                            m->next=NULL;
                                            MemoryManageP ptr=currentFreeP->next;
                                            currentFreeP->next=currentFreeP->next->next;
                                            delete ptr;
                                            ptr=NULL;
                                        }
                                        else
                                        {
                                            currentFreeP=currentFreeP->next;
                                        }
                                    }
                                }
                            }//end of if 第二块 freeP 不存在 
                        }
                        else
                        { 	//头结点不能合并空闲块
                            currentFreeP=freeP;
                            while(currentFreeP->next != NULL)
                            {
                                if(currentFreeP->next->startAddr == m->startAddr+m->length)
                                {
                                    m->length=m->length+currentFreeP->next->length;
                                    m->next=NULL;
                                    MemoryManageP ptr=currentFreeP->next;
                                    currentFreeP->next=currentFreeP->next->next;
                                    delete ptr;
                                    ptr=NULL;
                                }
                                else if(currentFreeP->next->startAddr+currentFreeP->next->length == m->startAddr)
                                {
                                    m->length=m->length+currentFreeP->next->length;
                                    m->startAddr=currentFreeP->next->startAddr;
                                    m->next=NULL;
                                    MemoryManageP ptr=currentFreeP->next;
                                    currentFreeP->next=currentFreeP->next->next;
                                    delete ptr;
                                    ptr=NULL;
                                }
                                else
                                {
                                    currentFreeP=currentFreeP->next;
                                }
                            }
                        }
                        
                        //把合并后的空闲块插入到空闲段链表
                        currentFreeP=freeP;
                        if(freeP==NULL)
                        {
                            freeP=m;
                            currentFreeP=freeP;
                        }
                        else if(freeP->length >= m->length)
                        {
                            m->next=freeP;
                            freeP=m;
                            currentFreeP=freeP;
                        }
                        else
                        {
                            int find1=0;
                            while(currentFreeP->next != NULL)
                            {

                                if(currentFreeP->next->length >= m->length)
                                {
                                    m->next=currentFreeP->next;
                                    currentFreeP->next=m;
                                    find1=1;
                                    break;
                                }
                                else
                                {
                                    currentFreeP=currentFreeP->next;
                                }
                            }
                            if(currentFreeP->next == NULL && find1==0)
                                currentFreeP->next=m;
                        }
                        //完成 把合并后的 空闲块插入 链表 
                    }//end of else (freeP ==NULL)
                }//end  of  for (0~2)
                
                //更新 空闲段空间大小 以及 占用段空间大小
                int k=p->size[0]+p->size[1]+p->size[2];
                setFreeSize(freeSize+k);
                setUseSize(useSize-k);
            }
            else
            {
				//只要 mode 赋值正确，则不执行此块 
            }
            //更新 PCBNum 的数量 
            setPCBNum(PCBNum-1);
            
            //如果是删除操作，则删除该PCB段表 
            if(1 == outChoi)
            	delete(p);
            else if(2 == outChoi){ //如果是换出操作，则把新的段表插在段表链尾 
            
            	p->setState(0);		//标志该表未分配 
            	//**插入段表链表 
				if(PCBSegP==NULL)	//进程段 链表头指针为空，新建 
				{
		            PCBSegP=p;
				}
				else				//若进程段链表头不为空 ，则找到末尾，插入新段表 s 在进程段链表 
				{
					SegTableP newCurrentP=PCBSegP;
					while(newCurrentP->next!=NULL)
					{
						newCurrentP=newCurrentP->next;
					}
					newCurrentP->next=p;
				}
            }

        }// end of else-if(0 == find) 
    }
    
    cout<<"Segment：：delete pcb"<<p->getProcessid();
    
    //更新内存检测文件 
    monitor();
    monitorPCB(p);
    return 1;
}
