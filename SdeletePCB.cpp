#include "Segment.h"
#include "PCB.h"
/*		int deletePCB(int outChoi,int id);		//**�ͷ��ڴ�		*/
// outChoi = 1ʱ��ɾ����ѯ id �Ķα��Լ����ڴ����Ŀռ䣬
// outChoi = 1ʱ���Ѳ�ѯ id �Ķα���뵽�α������β��state״̬�� 0 �Լ� ���ڴ����Ŀռ䣬
int MemorySegment::deletePCB(int outChoi,int id)
{
	cout<<"Segment����in deletePCB"<<endl;
    SegTableP p;
    SegTableP currentP=PCBSegP;

    if(PCBSegP==NULL)
    {
        cout<<"process"<<id<<" is not exist!"<<endl;
        return 0;
    }
    else
    {
        int find=0; //**��ʾ�ڽ����������Ƿ��ҵ���Ӧ���̣�1��ʾ�ҵ���0��ʾû�У��ҵ���ɾ����Ӧ�α�
        int pro = PCBSegP->getProcessid();
        
        //�ҵ���Ӧ���̺ŵĶα� ����ֵ�� p ���������PCBSegP�α�����ɾ��
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
        
        
        if(0 == find)		//**��� û���ҵ� ��Ӧ���̵Ķα�
        {
           cout<<"process"<<id<<" is not exist!"<<endl;
            return 0;
        }
        else			//**����ҵ��˶�Ӧ���̵Ķα�
        {
            //��ռ�ö�����ɾ���ý���ռ�õ��ڴ��
            for(int i=0;i<=2;i++)
            {
                MemoryManageP currentUseP=useP;
                if(currentUseP==NULL)
                {
                    cout<<"useP segment error!"<<endl;
                    return 0;
                }
                else
                {	//�ҵ� ʹ�öΣ����� delP ɾ�� 
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
            }	//end of for() ��ռ�ö�����ɾ���ý���ռ�õ��ڴ�� 
            
            
            //�ڿ��жβ����ͷŵ��ڴ� ����ȷ��ģʽ������β���
			 
            //�������䷨������̣����� ��ַ��С����˳�� ����������� 
            if(mode==1)
            {
                MemoryManageP currentFreeP=freeP;
                for(int i=0;i<=2;i++)
                {
                    //���ж�ͷָ��
                    if(freeP->startAddr >= p->startAddr[i])
                    {	//��ͷָ��ǰ������ �� �����ţ� 
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
                    {	// ��ͷָ��� �Ұ��� 
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
                    //���ж�ͷָ���Ľڵ�
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
                
                
				//���� ���жοռ��С �Լ� ռ�öοռ��С
                int k=p->size[0]+p->size[1]+p->size[2];
                setFreeSize(freeSize+k);
                setUseSize(useSize-k);
            }//end of mode 1

            //����䷨������ �ռ��ɴ�С˳�� �����������  
            else if(mode==2)
            {
                MemoryManageP currentFreeP=useP;
                for(int i=0;i<=2;i++)
                {
                    MemoryManageP m=new MemoryManage;
                    m->startAddr=p->startAddr[i];
                    m->length=p->size[i];
                    m->next=NULL;
                    
                    //���ж�ͷָ���Ƿ�Ϊ��
                    if(freeP==NULL)
                    {
                        freeP=m;
                    }
                    else
                    {
                        //�ж�ͷ����Ƿ���Ժϲ����п�
                        if(freeP->startAddr == m->startAddr+m->length || freeP->startAddr+freeP->length == m->startAddr)
                        {
                            if(freeP->startAddr == m->startAddr+m->length)
                            {	//���һ�� ǰ��ϲ� 
                                m->length=m->length+freeP->length;
                                m->next=NULL;
                                MemoryManageP ptr=freeP;
                                freeP=freeP->next;
                                delete ptr;
                                ptr=NULL;
                            }
                            else
                            {	// ���һ�� ����ϲ� 
                                m->length=m->length+freeP->length;
                                m->startAddr=freeP->startAddr;
                                m->next=NULL;
                                MemoryManageP ptr=freeP;
                                freeP=freeP->next;
                                delete ptr;
                                ptr=NULL;
                            }
                            
                            if(freeP != NULL){
                            	//ͷ���ϲ����п�� �Ƿ���Ժϲ����µ�ͷ��㣨���ڶ��飩 
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
	                            //��ǰͷ��㣨���ڶ��飩���ܺϲ����п飬���ں���Ľ����
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
	                        } //end of if()ͷ���ϲ����п�� �Ƿ���Ժϲ����µ�ͷ��㣨���ڶ��飩  
	                        
	                    }	//end  of if() ͷ�����Ժϲ����п� 
	                    else
	                    {	//ͷ��㲻�ܺϲ����п�
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
                   	 	//end of �ϲ����п� 
                        
	                    //�Ѻϲ���Ŀ��п���뵽���ж�����
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
	                    //��� �Ѻϲ���Ŀ� ���� ���� 
	                }//end  of if() freeP ==��NULL 
	            }//end of for(0~2)
	                
	            //���� ���жοռ��С �Լ� ռ�öοռ��С
	            int k=p->size[0]+p->size[1]+p->size[2];
	            setFreeSize(freeSize+k);
	            setUseSize(useSize-k);
	        }//end of mode 2

            //������䷨�����ѿ��п� ��С����˳�� ���� 
            else if(mode==3)
            {
                MemoryManageP currentFreeP=freeP;
                for(int i=0;i<=2;i++)
                {
                    MemoryManageP m=new MemoryManage;
                    m->startAddr=p->startAddr[i];
                    m->length=p->size[i];
                    m->next=NULL;
                    
                    //���ж�ͷָ���Ƿ�Ϊ��
                    if(freeP==NULL)
                    {
                        //freeP=m;
                    }
                    else
                    {
                        //�ж�ͷ����Ƿ���Ժϲ����п�
                        if(freeP->startAddr == m->startAddr+m->length || freeP->startAddr+freeP->length == m->startAddr)
                        {
                            if(freeP->startAddr == m->startAddr+m->length)
                            {	//��ͷ���ǰ�� �ϲ� 
                                m->length=m->length+freeP->length;
                                m->next=NULL;
                                MemoryManageP ptr=freeP;
                                freeP=freeP->next;
                                delete ptr;
                                ptr=NULL;
                            }
                            else
                            {	//�� ͷ������ �ϲ� 
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
                                //ͷ���ϲ����п�� �Ƿ���Ժϲ����µ�ͷ��㣨���ڶ��飩 
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
                                //��ǰͷ��㲻�ܺϲ����п飬���ں���Ľ����
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
                            }//end of if �ڶ��� freeP ������ 
                        }
                        else
                        { 	//ͷ��㲻�ܺϲ����п�
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
                        
                        //�Ѻϲ���Ŀ��п���뵽���ж�����
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
                        //��� �Ѻϲ���� ���п���� ���� 
                    }//end of else (freeP ==NULL)
                }//end  of  for (0~2)
                
                //���� ���жοռ��С �Լ� ռ�öοռ��С
                int k=p->size[0]+p->size[1]+p->size[2];
                setFreeSize(freeSize+k);
                setUseSize(useSize-k);
            }
            else
            {
				//ֻҪ mode ��ֵ��ȷ����ִ�д˿� 
            }
            //���� PCBNum ������ 
            setPCBNum(PCBNum-1);
            
            //�����ɾ����������ɾ����PCB�α� 
            if(1 == outChoi)
            	delete(p);
            else if(2 == outChoi){ //����ǻ�������������µĶα���ڶα���β 
            
            	p->setState(0);		//��־�ñ�δ���� 
            	//**����α����� 
				if(PCBSegP==NULL)	//���̶� ����ͷָ��Ϊ�գ��½� 
				{
		            PCBSegP=p;
				}
				else				//�����̶�����ͷ��Ϊ�� �����ҵ�ĩβ�������¶α� s �ڽ��̶����� 
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
    
    cout<<"Segment����delete pcb"<<p->getProcessid();
    
    //�����ڴ����ļ� 
    monitor();
    monitorPCB(p);
    return 1;
}
