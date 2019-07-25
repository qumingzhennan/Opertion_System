#include "Segment.h"
#include "head.h"
#include <string>
/*		int FitBest(int id,int size);		//������䷨�����ڴ�		
		int FitFirst(int id,int size);		//�������䷨�����ڴ�		
		int FitWorst(int id,int size);		//����䷨�����ڴ�
			//��������� id = (���̺�)   size = (��Ҫд������ݴ�С ) 
				
		void fileInit();	 				//monitor.txt �� monitorPCB.txt�ļ���ʼ������ 		
		int store(int id,int size);			//�������Ƿ���ڣ����½����̷����ڴ�		
		
        void monitor();						//��������ڴ�ʹ�����������¼���ļ��� 
		void monitorPCB(SegTableP p);		//���������ڴ��еķ�����ͷŹ��̲���¼���ļ���
			//���������	p = (�½����߸��µ� PCB ��) 


		string visitData(int id,int offsetAddress,int length);	//�������ݶ�
        string visitCode(int id,int offsetAddress,int length);	//�ڴ����ȡָ��ִ��
        	//���������id = (���̺�)   offsetAddress = (���ʵ���ʼ��ַ��Ĭ��Ϊ 0 )   
						//  length = (���ʵĳ��� , ����ͨ����ȡ�α��е����ݳ��Ȼ��)
        int getDataLength(int id);			//������ݶγ���
		int getDataAddr(int id);			//������ݶ��׵�ַ 
		int getCodeAddr(int id);			//��ô�����׵�ַ 
			//���������id = (���̺�)
		
		int writeData(int id,int offsetAddress,string s);		//�����ݶ�д����
        int writeCode(int id,int offsetAddress,string s);		//�ڴ���δ���� 
        	//���������id = (���̺�)   offsetAddress = (д�����ݵ���ʼ��ַ��Ĭ��Ϊ 0 )   s = (д�������) 

		int deletePCB(int outChoi,int id);		//�ͷ��ڴ�	
			//���������  outChoi = (1-ɾ������,2-��������)	  id = (���̺�) 

		int cleanDebris();			//��ʱ������Ƭ 		

*/



/*		int FitBest(int id,int size);//**������䷨�����ڴ�		*/
//	best fit�������Ӧ�����㷨�������������� ��С���� ����
//ռ�ö����� ����ʹ�ö���ʼ��ַ ��С�����˳������

int MemorySegment::FitBest(int id,int size)	//size	������Ҫ���ڴ�(���̴�С)
{
    cout<<"Segment����in best"<<endl;
    int seg[3];//**���������Σ������ÿ����������Ķηֳ�3�Σ��ֱ��ʾ���ݶΣ�����Σ�������
	int store=0; //store=1��ʾ����ɹ���store=0��ʾ����ʧ�� 
	
    seg[0]=40*size+100;
    seg[1]=size;
    seg[2]=size;

    SegTableP s=new SegmentTable();		//����һ���µĶα� 
	// ���½��α�ֵ 
    s->setProcessid(id);		// ���öα���̺� 
	s->setSegNum(3);			// ���ö��� 
	s->setState(1);				// ��־�öα��ѷ��� 
	s->distribute();			// ����̬�������ռ� 
	s->next=NULL;				//�α� next ָ���ÿ� 

	int ok=0;
	MemoryManageP currentP = freeP;
	//**�ж��ڴ��Ƿ��㹻������� 
	while(currentP != NULL && ok<3)
	{
		int manageSize1=0,manageSize2=0;
		int m,n;
		for( m=ok;m<=2;m++)
		{
			manageSize1=manageSize1+seg[m];
		}
		for(n=ok;n<=1;n++ )
		{
			manageSize2=manageSize2+seg[n];
		}
		if(currentP->length >= manageSize1)
		{
			ok=m+1;		// 4 
		}	
		else if(currentP->length >= manageSize2)
		{
			ok=n+1;		// 3
			currentP=currentP->next;
		}
		else if(currentP->length >= seg[ok])
		{
			ok++;		//2
			currentP=currentP->next;
		}
        else
            currentP=currentP->next;
	}
	
	//**�ڴ治�� ����ӡ�ڴ����ʧ�� 
	if(ok<3)
	{
        store=0;
        cout<<"process"<<id<<"memory store failed!"<<endl;
        return 0;
	}
	 
	//�ڴ治������ѯ���Լ����ȼ����͵Ľ��̣�ִ�� deletePCB ********************************************************** 
	
	//**	�ڴ��㹻�������ڴ� --������䷨ 
	if(3==mode)
	{
		for(int i=0;i<=2;i++)
		{
			currentP = freeP;		//currentP ָ����ж� 
			
			if(freeP->length >= seg[i])	//**********����һ����п�������� 
			{
				MemoryManageP useseg=new MemoryManage;
				MemoryManageP freeseg=new MemoryManage;
				MemoryManageP ptr=freeP;
				
				//�� seg[i] ����ʹ�� ������ʹ�öα� 
				useseg->length=seg[i];
				useseg->startAddr=currentP->startAddr;
				useseg->next=NULL;
				//���� ������ʣ����ж� �Ŀ��жα� 
				freeseg->length=currentP->length-useseg->length;
				freeseg->startAddr=currentP->startAddr+useseg->length;
				freeseg->next=NULL;
				//���¿��ж�����ָ�� ��ָ����һ���ж� 
				freeP=freeP->next; 
				currentP=freeP;
				//ɾ��ԭ���ж�����ָ�� 
				delete ptr;
				ptr=NULL;
				
				//** �ڿ��ж��������ڵ� ����ѭ����С�Ŀ��п��������ͷ�� 
				MemoryManageP currentP1=useP,currentP2=freeP;
				if(freeP==NULL)
				{
					freeP=freeseg;
				}
				else{
					if(freeseg->length != 0){//��ʹ�õĿ��жθպ����꣬��freeP�ж���飬��ɾ���ýڵ� 	
						delete freeseg;
					}
					else{	//ʹ�ù��Ŀ����в��� ������С����˳����� 
						if(freeP->length >= freeseg->length)
						{
							freeseg->next=freeP;
							freeP=freeseg;
							currentP2=freeP;
						}
						else
						{
							int find=0;
			                while(currentP2->next!=NULL && find==0)
							{
								if(currentP2->next->length >= freeseg->length)
								{
									freeseg->next=currentP2->next;
									currentP2->next=freeseg->next;
									find=1;
		
								}
								else
			                        currentP2=currentP2->next;
							}
							if(currentP2->next==NULL)
								currentP2->next=freeseg;
						}
					} 
				}
					
				//**��ռ�ö������в���ڵ� ����ʹ�öε�ַ ��С�����˳������ (currentP1=useP)
				if(currentP1==NULL)
				{
					useP = useseg;
					s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
					s->startAddr[i]=useseg->startAddr;
				}
				else if(currentP1->startAddr >= useseg->startAddr){
					useseg->next = currentP1;
					currentP1 = useseg;
					useP = useseg;
					s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
					s->startAddr[i]=useseg->startAddr;
				}
				else{
					int okin = 0 ;
					while((currentP1->next !=NULL) &&(okin != 1)){
						if(currentP1->next->startAddr >= useseg->startAddr){
							//������ currentP1->next ǰ�� 
							useseg->next = currentP1->next;
							currentP1->next = useseg;
							s->size[i]=seg[i];
							s->startAddr[i]=useseg->startAddr;
							okin = 1;
						}
						else{
							currentP1=currentP1->next;
						}
					}
					if(currentP1->next ==NULL){
						currentP1->next=useseg;
						s->size[i]=seg[i];
						s->startAddr[i]=useseg->startAddr;
					}
					/* 
					while(currentP1->next!=NULL)
					{
						currentP1=currentP1->next;
					}
					currentP1->next=useseg;
					s->size[i]=seg[i];
					s->startAddr[i]=useseg->startAddr
					*/
				} 
				
			}
			else	//*********************��һ����п鲻�������ʱ 
			{
				while(currentP->next!=NULL)	
				{
					if(currentP->next->length >= seg[i])	//������һ�飬ֱ���ҵ��������Ŀ��п� 
					{
						MemoryManageP useseg=new MemoryManage;
						MemoryManageP freeseg=new MemoryManage;
						MemoryManageP ptr=currentP->next;
						useseg->length=seg[i];
						useseg->startAddr=currentP->next->startAddr;
						useseg->next=NULL;
						
						freeseg->length=currentP->next->length-useseg->length;
						freeseg->startAddr=currentP->next->startAddr+useseg->length;
						freeseg->next=NULL;
						
						//ɾ��ʹ�õĿ��жα� 
						currentP->next=currentP->next->next;
						delete ptr;
						ptr=NULL;
						
						MemoryManageP currentP1=useP,currentP2=freeP;
						//**��ռ�ö������в���ڵ� ����ʹ�öε�ַ ��С�����˳������ (currentP1=useP)
						if(currentP1==NULL)
						{
							useP = useseg;
							s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
							s->startAddr[i]=useseg->startAddr;
						}
						else if(currentP1->startAddr >= useseg->startAddr){
							useseg->next = currentP1;
							currentP1 = useseg;
							useP = useseg;
							s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
							s->startAddr[i]=useseg->startAddr;
						}
						else{
							int okin = 0 ;
							while((currentP1->next !=NULL) &&(okin != 1)){
								if(currentP1->next->startAddr >= useseg->startAddr){
									//������ currentP1->next ǰ�� 
									useseg->next = currentP1->next;
									currentP1->next = useseg;
									s->size[i]=seg[i];
									s->startAddr[i]=useseg->startAddr;
									okin = 1;
								}
								else{
									currentP1=currentP1->next;
								}
							}
							if(currentP1->next ==NULL){
								currentP1->next=useseg;
								s->size[i]=seg[i];
								s->startAddr[i]=useseg->startAddr;
							}
							/* 
							while(currentP1->next!=NULL)
							{
								currentP1=currentP1->next;
							}
							currentP1->next=useseg;
							s->size[i]=seg[i];
							s->startAddr[i]=useseg->startAddr
							*/
						} 
						
						//**�ڿ��жβ���ڵ� 
						if(currentP2==NULL)
						{
							freeP=freeseg;
							currentP=freeP;
						}
						else{
							if(freeseg->length == 0){	//��ʹ�õĿ��жθպ����꣬��ɾ���ýڵ�
								delete  freeseg;
							}
							else{ //ʹ�ù��Ŀ����в���
								
								if(freeP->length >= freeseg->length)
								{
									freeseg->next=freeP;
									freeP=freeseg;
									currentP2=freeP;
								}
								else
								{
									int find=0;
									while(currentP2->next!=NULL && find==0)
									{
										if(currentP2->next->length >= freeseg->length)
										{
											freeseg->next=currentP2->next;
											currentP2->next=freeseg;
											find=1;
										}	
										else
										{
											currentP2=currentP2->next;
										}
									}
		                            if(currentP2->next==NULL)
									{
										currentP2->next= freeseg;
									}
								}
							} 
						}
						
                        break;
					}
					else
					{
						currentP=currentP->next;
					}
					
				}
			}
			
			
		}
       
		//**����α����� 
		if(PCBSegP==NULL)	//���̶� ����ͷָ��Ϊ�գ��½� 
		{
            PCBSegP=s;
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;
		}
		else				//�����̶�����ͷ��Ϊ�� �����ҵ�ĩβ�������¶α� s �ڽ��̶����� 
		{
			SegTableP newCurrentP=PCBSegP;
			while(newCurrentP->next!=NULL)
			{
				newCurrentP=newCurrentP->next;
			}
			newCurrentP->next=s;
			
			//���¿��ж� �Լ� ʹ�öδ�С 
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;		//PCB �������� 
		}
		store=1;
		//�����ڴ����ļ����Լ��ڴ���PCB�ļ� 
		monitor();
		monitorPCB(s);
        return 1;

	}
	
}
 


/*		int FitFirst(int id,int size);//**�������䷨�����ڴ�		*/
//		first fit��������Ӧ�����㷨������ַ�������� 
//ռ�ö����� ����ʹ�ö���ʼ��ַ ��С�����˳������

int MemorySegment::FitFirst(int id,int size)	//size	������Ҫ���ڴ�(���̴�С)
{	cout<<"in first"<<endl;
	int seg[3];	//**���������Σ������ÿ����������Ķηֳ�3�Σ���һ��ռ20%���ڶ���ռ60%��������ռ20%
	int store=0; //store=1��ʾ����ɹ���store=0��ʾ����ʧ�� 
	
    seg[0]=size*40+100;
    seg[1]=size;
    seg[2]=size;
    
	SegTableP s=new SegmentTable;		//����һ���µĶα� 
	// ���½��α�ֵ 
    s->setProcessid(id);		// ���öα���̺� 
	s->setSegNum(3);			// ���ö��� 
	s->setState(1);				// ��־�öα��ѷ��� 
	s->distribute();			// ����̬�������ռ� 
	s->next=NULL;				//�α� next ָ���ÿ� 
	
	int ok=0;
	MemoryManageP currentP=freeP;
	//**�ж��ڴ��Ƿ��㹻������� 
	while(currentP != NULL && ok<3)
	{
		int manageSize1=0,manageSize2=0;
		int m,n;
		for( m=ok;m<=2;m++)
		{
			manageSize1=manageSize1+seg[m];
		}
		for(n=ok;n<=1;n++ )
		{
			manageSize2=manageSize2+seg[n];
		}
		if(currentP->length >= manageSize1)
		{
			ok=m+1;
		}	
		else if(currentP->length >= manageSize2)
		{
			ok=n+1;
			currentP=currentP->next;
		}
		else if(currentP->length >= seg[ok])
		{
			ok++;
			currentP=currentP->next;
		}
        else
            currentP=currentP->next;
	}
	//**�ڴ治�� 
	if(ok<3)
	{
		store=0;
        cout<<"process"<<id<<"memory store failed!"<<endl;
		return 0;
	}
	
	//**	�ڴ��㹻�������ڴ� --������Ӧ��	
	if(1 == mode)
	{ 
		currentP=freeP;
		for(int i=0;i<=2;i++)
		{
			currentP=freeP;
			
			if(freeP->length >= seg[i])		//**********����һ����п��������
			{
				MemoryManageP useseg=new MemoryManage;
				MemoryManageP freeseg=new MemoryManage;
				MemoryManageP ptr=freeP;
				
				//�� seg[i] ����ʹ�� ������ʹ�öα� 
				useseg->length=seg[i];
				useseg->startAddr=currentP->startAddr;
				useseg->next=NULL;
				//���� ������ʣ����ж� �Ŀ��жα� 
				freeseg->length=currentP->length-useseg->length;
				freeseg->startAddr=currentP->startAddr+useseg->length;
				freeseg->next=NULL;
				//���¿��ж�����ָ�� ��ָ����һ���ж� 
				freeP=freeP->next; 
				currentP=freeP;
				//ɾ��ԭ���ж�����ָ�� 
				delete ptr;
				ptr=NULL;
				
				if(freeP==NULL)
				{
					freeP = freeseg;
				}
				else{	
					 if(freeseg->length != 0){	//��ʣ��Ŀ��п� �����������ͷ 
					 	freeseg->next=freeP;
						freeP=freeseg;
						currentP=freeP;
					 }
					 //freeseg->length == 0 ʱ��ɶ������ 
				}
				
					
				MemoryManageP currentP1=useP,currentP2=freeP;
				//**��ռ�ö������в���ڵ� ����ʹ�öε�ַ ��С�����˳������ (currentP1=useP)
				if(currentP1==NULL)
				{
					useP = useseg;
					s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
					s->startAddr[i]=useseg->startAddr;
				}
				else if(currentP1->startAddr >= useseg->startAddr){
					useseg->next = currentP1;
					currentP1 = useseg;
					useP = useseg;
					s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
					s->startAddr[i]=useseg->startAddr;
				}
				else{
					int okin = 0 ;
					while((currentP1->next !=NULL) &&(okin != 1)){
						if(currentP1->next->startAddr >= useseg->startAddr){
							//������ currentP1->next ǰ�� 
							useseg->next = currentP1->next;
							currentP1->next = useseg;
							s->size[i]=seg[i];
							s->startAddr[i]=useseg->startAddr;
							okin = 1;
						}
						else{
							currentP1=currentP1->next;
						}
					}
					if(currentP1->next ==NULL){
						currentP1->next=useseg;
						s->size[i]=seg[i];
						s->startAddr[i]=useseg->startAddr;
					}
					/* 
					while(currentP1->next!=NULL)
					{
						currentP1=currentP1->next;
					}
					currentP1->next=useseg;
					s->size[i]=seg[i];
					s->startAddr[i]=useseg->startAddr
					*/
				}  
			}
			else			//*********************��һ����п鲻�������ʱ 
			{
				int find=0;
				while(currentP->next!=NULL && find==0) //������һ�飬ֱ���ҵ��������Ŀ��п�
				{
					if(currentP->next->length >= seg[i])
					{
						MemoryManageP useseg=new MemoryManage;
						MemoryManageP freeseg=new MemoryManage;
						MemoryManageP tempseg=new MemoryManage;
						useseg->length=seg[i];
						useseg->startAddr=currentP->next->startAddr;
						useseg->next=NULL;
						freeseg->length=currentP->next->length-useseg->length;
						freeseg->startAddr=currentP->next->startAddr+useseg->length;
						
						
						//**�ڿ��жβ������� 	
						if( (freeseg->length == 0)&&(currentP->next->next != NULL) ){
							//�պ�����  currentP->next ��һ���п� 
							//�ѿ��п�ָ��ָ����һ�����п� 
							tempseg = currentP->next;
							currentP->next = currentP->next->next;
							delete tempseg;
						} 
						else{	//��ʹ�õĿ��п�� ��ʼ��ַ�����д�С��ֵ�޸�
							currentP->next->length=freeseg->length;
							currentP->next->startAddr=freeseg->startAddr;
							delete freeseg;
							freeseg=NULL;						
						}
						
						
						MemoryManageP currentP1=useP,currentP2=freeP;
						//**��ռ�ö������в���ڵ� ����ʹ�öε�ַ ��С�����˳������ (currentP1=useP)
						if(currentP1==NULL)
						{
							useP = useseg;
							s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
							s->startAddr[i]=useseg->startAddr;
						}
						else if(currentP1->startAddr >= useseg->startAddr){
							useseg->next = currentP1;
							currentP1 = useseg;
							useP = useseg;
							s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
							s->startAddr[i]=useseg->startAddr;
						}
						else{
							int okin = 0 ;
							while((currentP1->next !=NULL) &&(okin != 1)){
								if(currentP1->next->startAddr >= useseg->startAddr){
									//������ currentP1->next ǰ�� 
									useseg->next = currentP1->next;
									currentP1->next = useseg;
									s->size[i]=seg[i];
									s->startAddr[i]=useseg->startAddr;
									okin = 1;
								}
								else{
									currentP1=currentP1->next;
								}
							}
							if(currentP1->next ==NULL){
								currentP1->next=useseg;
								s->size[i]=seg[i];
								s->startAddr[i]=useseg->startAddr;
							}
							/* 
							while(currentP1->next!=NULL)
							{
								currentP1=currentP1->next;
							}
							currentP1->next=useseg;
							s->size[i]=seg[i];
							s->startAddr[i]=useseg->startAddr
							*/
						} 
						find=1;
					}
					else	//if(currentP->next->length >= seg[i])
					{
						currentP=currentP->next;
					}
				}
			}
		}
		
		//**����α����� 
		if(PCBSegP==NULL)	//���̶� ����ͷָ��Ϊ�գ��½� 
		{
			PCBSegP=s;
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;
		}
		else				//�����̶�����ͷ��Ϊ�� �����ҵ�ĩβ�������¶α� s �ڽ��̶����� 
		{
			SegTableP newCurrentP=PCBSegP;
			while(newCurrentP->next!=NULL)
			{
				newCurrentP=newCurrentP->next;
			}
			newCurrentP->next=s;
			
			//���¿��ж� �Լ� ʹ�öδ�С 
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;			//PCB �������� 
		}
		
		store=1;
		//�����ڴ����ļ����Լ��ڴ���PCB�ļ� 
		monitor();
		monitorPCB(s);
		return 1;
	}
}


/*		int FitWorst(int id,int size);	//**����䷨�����ڴ�		*/
// ���Ӧ�����㷨�������������ȴӴ�С����
//ռ�ö����� ����ʹ�ö���ʼ��ַ ��С�����˳������

int MemorySegment::FitWorst(int id,int size)	//size	������Ҫ���ڴ�(���̴�С)
{
	int seg[3];//**���������Σ������ÿ����������Ķηֳ�3�Σ���һ��ռ20%���ڶ���ռ60%��������ռ20%
	int store=0; //store=1��ʾ����ɹ���store=0��ʾ����ʧ�� 
	
    seg[0]=size*40+100;
    seg[1]=size;
    seg[2]=size;
    
	SegTableP s=new SegmentTable;		//����һ���µĶα� 
	// ���½��α�ֵ 
    s->setProcessid(id);		// ���öα���̺� 
	s->setSegNum(3);			// ���ö��� 
	s->setState(1);				// ��־�öα��ѷ��� 
	s->distribute();			// ����̬�������ռ� 
	s->next=NULL;				//�α� next ָ���ÿ�
	
	int ok=0;
	MemoryManageP currentP=freeP;
	//**	�ж��ڴ��Ƿ��㹻������� 
	while(currentP != NULL && ok<3)
	{
		int manageSize1=0,manageSize2=0;
		int m,n;
		for( m=ok;m<=2;m++)
		{
			manageSize1=manageSize1+seg[m];
		}
		for(n=ok;n<=1;n++ )
		{
			manageSize2=manageSize2+seg[n];
		}
		
		if(currentP->length >= manageSize1)
		{
			ok=m+1;	//4
		}	
		else if(currentP->length >= manageSize2)
		{
			ok=n+1;	//3
			currentP=currentP->next;
		}
		else if(currentP->length >= seg[ok])
		{
			ok++;	//2
			currentP=currentP->next;
		}
        else
            currentP=currentP->next;
	}
	//**�ڴ治�� 
	if(ok<3)
	{
		store=0;
        cout<<"process"<<id<<"memory store failed!"<<endl;
		return 0;
	}
	
	//**	�ڴ��㹻�������ڴ� --����䷨ 
	if(2==mode)
	{
		for(int i=0;i<=2;i++)
		{
			currentP=freeP;
			
			if(freeP->length >= seg[i])		//**********����һ����п��������
			{
				MemoryManageP useseg=new MemoryManage;
				MemoryManageP freeseg=new MemoryManage;
				MemoryManageP ptr=freeP;
				useseg->length=seg[i];
				useseg->startAddr=currentP->startAddr;
				useseg->next=NULL;
				freeseg->length=currentP->length-useseg->length;
				freeseg->startAddr=currentP->startAddr+useseg->length;
				freeseg->next=NULL;
				freeP=freeP->next; 
				currentP=freeP;
				delete ptr;
				ptr=NULL;
				
				//**�ڿ��ж��������ڵ� 
				MemoryManageP currentP1=useP,currentP2=freeP;
				if(freeP==NULL)
				{
					freeP=freeseg;
				}
				else if(freeP->length <= freeseg->length)
				{
					freeseg->next=freeP;
					freeP=freeseg;
					currentP2=freeP;
				}
				else
				{
					int find=0;
					while(currentP2->next!=NULL && find==0)
					{
						if(currentP2->next->length <= freeseg->length)
						{
							freeseg->next=currentP2->next;
							currentP2->next=freeseg->next;
							find=1;
						}
						else
                            currentP2=currentP2->next;
					}
					if(currentP2->next==NULL)
						currentP2->next=freeseg;
				}
					
					
				//**��ռ�ö������в���ڵ� ����ʹ�öε�ַ ��С�����˳������ (currentP1=useP)
				if(currentP1==NULL)
				{
					useP = useseg;
					s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
					s->startAddr[i]=useseg->startAddr;
				}
				else if(currentP1->startAddr >= useseg->startAddr){
					useseg->next = currentP1;
					currentP1 = useseg;
					useP = useseg;
					s->size[i]=seg[i];					//�α�ֵ�ĸ��� 
					s->startAddr[i]=useseg->startAddr;
				}
				else{
					int okin = 0 ;
					while((currentP1->next !=NULL) &&(okin != 1)){
						if(currentP1->next->startAddr >= useseg->startAddr){
							//������ currentP1->next ǰ�� 
							useseg->next = currentP1->next;
							currentP1->next = useseg;
							s->size[i]=seg[i];
							s->startAddr[i]=useseg->startAddr;
							okin = 1;
						}
						else{
							currentP1=currentP1->next;
						}
					}
					if(currentP1->next ==NULL){
						currentP1->next=useseg;
						s->size[i]=seg[i];
						s->startAddr[i]=useseg->startAddr;
					}
					
				} 
				
			}
		
		}
		//**����α����� 
		if(PCBSegP==NULL)
		{
			PCBSegP=s;
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;
		}
		else
		{
			SegTableP newCurrentP=PCBSegP;
			while(newCurrentP->next!=NULL)
			{
				newCurrentP=newCurrentP->next;
			}
            newCurrentP->next=s;
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;
		}
		store=1;
        cout<<"in store2";
		monitor();
		monitorPCB(s);
		return 1;
	}
}
 


/*		void fileInit();	//**monitor.txt �� monitorPCB.txt�ļ���ʼ������ 		*/

void MemorySegment::fileInit()
{
	cout<<"Segment����in fileinit"<<endl;

	//**������򿪼���ļ�,����ʼ�� 
	ofstream monitorfile("monitor.txt");
	
	if(!monitorfile)
	{
        cout<<"file open failed!"<<endl;
	}
	else
	{
		if(1 == type)
		{
            monitorfile<<"memory type:page"<<endl;
		}
		else if(2 == type) 
		{
			monitorfile<<"memory type:segment"<<endl;
		}
		else if(3 == type) 
		{
            monitorfile<<""<<endl;
		}
		else
		{
            cout<<"initial failed!"<<endl;
		}
	} 
	monitorfile.close();
	
	//**��ʼ������ռ���ڴ����ļ� 
	ofstream monitorPCB("monitorPCB.txt");
	if(!monitorPCB)
	{
        cout<<"file open failed!"<<endl;
	}
	else
	{
		if(1 == type)
		{
            monitorPCB<<"memory type:page"<<endl;
		}
		else if(2 == type) 
		{
            monitorPCB<<"memory type:segment"<<endl;
		}
		else if(3 == type) 
		{
            monitorPCB<<""<<endl;
		}
		else
		{
            cout<<"initial failed!"<<endl;
		}
	} 
	monitorPCB.close();
}



/*		int store(int id,int size);	//�������Ƿ���ڣ����½����̷����ڴ�		*/

int MemorySegment::store(int id,int size)
{	
	cout<<"Segment����in store"<<endl;
	SegTableP currentP = PCBSegP;   //currentP ָ����̶α�ͷ 
    int pcbId=id;
    
	//**�жϽ����Ƿ��Ѵ��ڣ�������ڣ�������ڴ�ʧ��. 
	if (currentP == NULL){
		cout<<"currentP(PCBSegP) == NULL"<<endl;
	}
	while(currentP!=NULL)
	{
		if(currentP->getProcessid() == pcbId) //�½����̺��Ѿ����� 
		{
           cout<<"process"<<id<<"has already exist!"<<endl;
           return 0;
		}
		else
			currentP=currentP->next;	 
	}

    cout<<"!!!!!!!!!!!mode!!!!!!"<<mode<<endl<<endl<<endl;
    mode =3;
	//1��ʾ�������䷨��2��ʾ����䷨��3��ʾ������䷨ 
	if(mode == 1) 
    {
        return FitFirst(pcbId,size);
    }
	else if(mode == 2)
    {
        return FitWorst(pcbId,size);
    }
	else if(mode == 3)
    {
        return FitBest(pcbId,size);
    }
	else
    {
        cout<<"store fail";
        return 0;
    }

}



/*		
		void monitor();				//**��������ڴ�ʹ�����������¼���ļ��� 
		void monitorPCB(SegTableP p);//**���������ڴ��еķ�����ͷŹ��̲���¼���ļ���
*/

//**�ڴ��⺯��,����ڴ�ʹ�����������¼���ļ��� 
void MemorySegment::monitor()
{
	cout<<"Segment����in monitor"<<endl;
	//**�򿪼���ļ�����ʼ���ļ���д��Ϣ��ÿ��д���ļ�ĩβ
	ofstream monitorfile; 
	MemoryManageP currentP; 
	monitorfile.open("monitor.txt",ios::app);
	
	if(!monitorfile)
	{
        cout<<"file can not open!"<<endl;
	}
	else
	{
		if(mode==1) 
            monitorfile<<endl<<endl<<"way:"<<"first"<<endl;
		else if(mode==2) 
            monitorfile<<endl<<endl<<"way:"<<"worst"<<endl;
		else if(mode==3)
            monitorfile<<endl<<endl<<"way:"<<"best"<<endl;
		
        monitorfile<<"memory useP:"<<endl
					<<" size:"<<size<<"  "<<" useP size:"<<useSize<<"  "<<" freeP size:"<<freeSize<<"   "
        			<<" pcb number:"<<PCBNum<<"  "<<" useP:"<<(double)useSize/size<<endl;
        			
        monitorfile<<" freeseg is:";
        // ������ӡ�� ���ж���� 
        currentP=freeP ;
		while(currentP!=NULL) 
		{
            monitorfile<<"-->startaddr:"<<currentP->startAddr<<"  length:"<<currentP->length;
			currentP=currentP->next;
		}
        
        monitorfile<<endl<<" usesegment is:";
		// ������ӡ�� ʹ�ö���� 
		currentP=useP;
		while(currentP!=NULL) 
		{
            monitorfile<<"-->startaddr:"<<currentP->startAddr<<"  length:"<<currentP->length;
			currentP=currentP->next;
		}
		
	}
	
	monitorfile.close();
}


//**����ʹ���ڴ��⺯�� ,���������ڴ��еķ�����ͷŹ��̲���¼���ļ���
void MemorySegment::monitorPCB(SegTableP p)
{
    cout<<"Segment����in monitorPCB"<<endl;
	//**�򿪼���ļ�����ʼ���ļ���д��Ϣ��ÿ��д���ļ�ĩβ
	ofstream monitorPCB; 
	monitorPCB.open("monitorPCB.txt",ios::app);
	
	if(!monitorPCB)
	{
        cout<<"file can not open!"<<endl;
	}
	else
	{
		if(p->getState()==1) 
		{
            monitorPCB<<endl<<"id"<<p->getProcessid()<<" share memory";
            monitorPCB<<"segment number:"<<p->getSegNum()<<endl;
		}
		else
		{
            monitorPCB<<endl<<"id"<<p->getProcessid()<<" freeP memory";
            monitorPCB<<"segment num:"<<p->getSegNum()<<endl;
		}
		 
		 for(int i=0;i<=2;i++)
		 {
            monitorPCB<<"NO."<<i+1<<" start address:"<<p->startAddr[i]<<",size:"<<p->size[i]<<endl;
		 }
		 
		SegTableP currentP=PCBSegP;
        monitorPCB<<"now memory have pcb:";
		while(currentP!=NULL)
		{
            monitorPCB<<"process id:"<<currentP->getProcessid()<<"   ";
			currentP=currentP->next;
		}
		monitorPCB<<endl;
	} 
	
	monitorPCB.close();
}



#include <string>
/*
		string visitData(int id,int offsetAddress,int length);//**�������ݶ�
        string visitCode(int id,int offsetAddress,int length);//**�ڴ����ȡָ��ִ��
        int getDataLength(int id);	//**������ݶγ���
		int getDataAddr(int id);	//**������ݶ��׵�ַ 
		int getCodeAddr(int id);	//**��ô�����׵�ַ 
*/

string MemorySegment::visitData(int id,int offsetAddress,int length)	//**�������ݶ�
{
	cout<<"Segment����in visitData"<<endl;
	string s;

	SegTableP currentP=PCBSegP;
	int find=0;
	while(currentP!=NULL && find==0) 
	{	//���� id ��Ӧ�Ķα� 
		if(currentP->getProcessid()==id)
			find=1;
		else
			currentP=currentP->next;
	}
	
    if(find==0 || currentP==NULL)
	{
        cout<<"process"<<id<<" is not exist!visit memory failed!"<<endl;
	  	s="";
        return s;
	}
	else		// �ҵ��α�֮�󣬿�ʼ�����ݲ��� 
	{
            int address=offsetAddress+currentP->startAddr[0];
		
			if(currentP->startAddr[0] <= address)
			{
				//**�򿪼���ļ�����ʼ���ļ���д��Ϣ��ÿ��д���ļ�ĩβ
				ofstream monitorfile; 
				monitorfile.open("monitorPCB.txt",ios::app);
				
				if(!monitorfile)
				{
                    cout<<"file open failed!"<<endl;
				  	s="";
                    monitorfile.close();
                    return s;
				}
				else if(currentP->startAddr[0]+currentP->size[0] < address+length)
				{
                    cout<<"visit across bound!"<<endl;
                    s="";
                    monitorfile.close();
                    return s;
				}
				else
				{
					
					char * stemp=new char[length+1];
					int k=0;
					for(k=0;k<=length-1;k++)
					{
						stemp[k]=memContent[address+k];
					}
					stemp[k]='\0';
					s=stemp;
					
					//��������� 
					cout<<endl<<"id"<<id<<"visit memory:"<<address<<" get data:"<<s<<endl;
					
                    monitorfile<<endl<<"id"<<id<<"visit memory:"<<address<<" get data:"<<s<<endl;
					monitorfile.close();
                    delete [] stemp;
                    cout<<"Segment����in read data";
                    return s;

				}
				
			}
				
        cout<<"Segment����visit across bound!"<<endl;
		s="";
        return s;
	}
}


string MemorySegment::visitCode(int id,int offsetAddress,int length)	//**�ڴ����ȡָ��ִ��
{
    cout<<"Segment����in visitCode"<<endl;
	string s;
	
	SegTableP currentP=PCBSegP;
	int find=0;
	while(currentP!=NULL && find==0) 
	{
		if(currentP->getProcessid()==id)
			find=1;
		else
			currentP=currentP->next;
	}
	
    if(find==0 || currentP==NULL)
	{
        cout<<"process"<<id<<" is not exist!visit memory failed!"<<endl;
		s="";
        return s;
	}
	else
	{
		
            int address=currentP->startAddr[1]+offsetAddress;
		
			if(currentP->startAddr[1] <= address)
			{
				//**�򿪼���ļ�����ʼ���ļ���д��Ϣ��ÿ��д���ļ�ĩβ
				ofstream monitorfile; 
				monitorfile.open("monitorPCB.txt",ios::app);
				
				if(!monitorfile)
				{
                    cout<<"file open failed!"<<endl;
                    s="";
                    monitorfile.close();

                    return s;
				}
				else if(currentP->startAddr[1] + currentP->size[1] < address+length)
				{
                    cout<<"visit across bound!"<<endl;
                    s="";
                    monitorfile.close();

                    return s;
				}
				else
				{
					char * stemp=new char[length+1];
					int k=0;
					for(k=0;k<=length-1;k++)
					{
						stemp[k]=memContent[address+k];
					}
					stemp[k] = '\0';
					s=stemp;
					
					//��������� 
					cout<<endl<<"id"<<id<<"visit code:"<<address<<" get code:"<<s<<endl;
                    monitorfile<<endl<<"id"<<id<<"visit code:"<<address<<" get code:"<<s<<endl;
					monitorfile.close();
					delete [] stemp;
                    cout<<"in read code";
                    return s;
				}
			}	
			
        cout<<"visit across bound!"<<endl;
		s="";
        return s;
	}
} 


int MemorySegment::getDataLength(int id)	//**������ݶγ���
{
	cout<<"Segment����in getDataLength"<<endl;
    SegTableP currentP=PCBSegP;
    int find=0;
    while(currentP!=NULL && find==0)
    {
        if(currentP->getProcessid()==id)
            find=1;
        else
            currentP=currentP->next;
    }

    if(find==0 || currentP==NULL)
    {
        cout<<"process"<<id<<" is not exist!visit memory failed!"<<endl;
        return size;
    }
    else
    {
        cout<<"get data length";
        return currentP->size[0];		//******�������ݶγ��� 
    }
}

int MemorySegment::getDataAddr(int id)		//**������ݶ��׵�ַ
{	
	cout<<"Segment����in getDataAddr"<<endl;
	SegTableP currentP=PCBSegP;
	int find=0;
	while(currentP!=NULL && find==0) 
	{
		if(currentP->getProcessid()==id)
			find=1;
		else
			currentP=currentP->next;
	}
	
    if(find==0 || currentP==NULL)
	{
        cout<<"process"<<id<<" is not exist!visit memory failed!"<<endl;
		return -1;
	}
	else 
    {
        cout<<"get address"<<currentP->startAddr[0];
        return currentP->startAddr[0];		// �������ݶ��׵�ַ 
    }


}

int MemorySegment::getCodeAddr(int id)	//**��ô�����׵�ַ 
{	
	cout<<"Segment����in getcodeaddr"<<endl;
	SegTableP currentP=PCBSegP;
	int find=0;
	while(currentP!=NULL && find==0) 
	{
		if(currentP->getProcessid()==id)
			find=1;
		else
			currentP=currentP->next;
	}
	
    if(find==0 || currentP==NULL)
	{
        cout<<"process"<<id<<" is not exist!visit memory failed!"<<endl;
		return -1;
	}
	else 
    {
        cout<<"get code addr"<<currentP->startAddr[1];
        return currentP->startAddr[1];
    }

}



/*		
		int writeData(int id,int offsetAddress,string s);//**�����ݶ�д����
        int writeCode(int id,int offsetAddress,string s);//**�ڴ���δ���� 
*/

int MemorySegment::writeData(int id,int offsetAddress,string s)	//**�����ݶ�д����
{
    cout<<"Segment����in writedata"<<endl;
	
	SegTableP currentP=PCBSegP;
	int find=0;
	while(currentP!=NULL && find==0)	// �ҵ� id ��Ӧ�Ķα� 
	{
		if(currentP->getProcessid()==id)
			find=1;
		else
			currentP=currentP->next;
	}	
    if(0 == find)						// ���� id ʧ�� 
	{
        cout<<"process"<<id<<" is not exist!write memory failed!"<<endl;
        cout<<"writedatat end"<<endl;
		return 0;
	}
	else
    {									// ���� id �ɹ���ִ��д���� 
    		// adress ָ�� ��ʼд���� ��ʵ�����ݶε�ַ 
            int address=currentP->startAddr[0]+offsetAddress;
			if(currentP->startAddr[0] <= address)
			{
				//**�򿪼���ļ�����ʼ���ļ���д��Ϣ��ÿ��д���ļ�ĩβ
				ofstream monitorfile; 
				monitorfile.open("monitorPCB.txt",ios::app);
				
				if(!monitorfile)	//��PCB ����ļ�����ʧ�� 
				{
                    cout<<"file open failed!"<<endl;
                    monitorfile.close();
                    cout<<"writedatat end"<<endl;
					return 0;
				}//string���size()/length()�������ص����ֽ����������Ƿ��к��֡� 
				else if(currentP->startAddr[0]+currentP->size[0] < address+s.length())
				{					//д������� ������ַ��Χ 
                    cout<<"write across bound!"<<endl;
                    monitorfile.close();
                    cout<<"writedatat end"<<endl;
					return 0; 
				}
				else
				{					// ִ�� д����� 
                    int k=0,j=s.length();
                    
                    for(k=0;k<j;k++)
                    {
                        memContent[address+k]=s[k];
                    }
                    memContent[address+k]='\0';
                    visitData(1,0,s.length());
                    
                    
                    monitorfile<<endl<<"id"<<id<<"write data"<<" address:"<<address<<" write:"<<s<<endl;
					monitorfile.close();
                    return 1;
				}
				
			} 
		
        cout<<"Segment����write across bound!"<<endl;
        cout<<"Segment����writedatat end"<<endl;
		return 0;
	}
}


int MemorySegment::writeCode(int id,int offsetAddress,string s)	//**�ڴ���δ����
{
	cout<<"Segment����in writecode"<<endl;

	int j=s.length();
	
	SegTableP currentP=PCBSegP;
	int find=0;
	if (currentP == NULL){
		cout<<"currentP == NULL"<<endl;
	}
	while(currentP!=NULL && find==0) 
	{
		if(currentP->getProcessid()==id)
			find=1;
		else
			currentP=currentP->next;
	}
	
	if(find=0 || currentP==NULL)
	{
        cout<<"pcb not exist!"<<endl;
		return 0;
	}
	else
	{
			// adress ָ�� ��ʼд���� ��ʵ�ʴ���ε�ַ 
            int address=currentP->startAddr[1]+offsetAddress;
			if(currentP->startAddr[1] <= address)
			{
				//**�򿪼���ļ�����ʼ���ļ���д��Ϣ��ÿ��д���ļ�ĩβ
				ofstream monitorfile; 
				monitorfile.open("monitorPCB.txt",ios::app);
				
				if(!monitorfile)
				{				// ����ļ���ʧ�� 
                    cout<<"Segment����writecode file open failed!"<<endl;
                    monitorfile.close();
					return -1;
				}
				else if(currentP->size[1]+currentP->startAddr[1] < address+j)
				{				// Ҫ������ ������Χ 
                    cout<<"Segment����write across bound!"<<endl;
                    monitorfile.close();
					return -1;
				}
				else
				{				// ��ʼ������д�� ����� 
					int k=0;
					for(k=0; k<j; k++)
					{
						memContent[address+k]=s[k];
					}
					memContent[address+k]='\0';
                    
                    monitorfile<<endl<<"id"<<id<<"write code"<<" address"<<address<<" write:"<<s<<endl;
					monitorfile.close();
					
                    return 1;
				}
				
			} 
			
        cout<<"Segment����write across bound!"<<endl;
        cout<<"Segment����end write code"<<id;
		return 0;
	}
}
 

/*		int deletePCB(int outChoi,int id);		//**�ͷ��ڴ�		*/
// outChoi = 1ʱ��ɾ����ѯ id �Ķα��Լ����ڴ����Ŀռ䣬
// outChoi = 2ʱ���Ѳ�ѯ id �Ķα���뵽�α������β��state״̬�� 0 �Լ� ���ڴ����Ŀռ䣬
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


 
 //!!!!!!!!!!!!!!!!		�ӽ��̡���ʱ�� ����ǰʵ��	!!!!!!!!!!!!!!!!!!!!!!! 
 //!!!!!!!!!!!!!!!!		�ж������������ڴ�Ĳ��� 	!!!!!!!!!!!!!!!!!!!!!!! 
/*		int cleanDebris();			//��ʱ������Ƭ 		*/

int MemorySegment::cleanDebris(){
	int i,xfind;
	int find = 0;
	int smallAddr,fAddr,lAddr;
	MemoryManageP tempFreeP= new MemoryManage;;			// tempFreeP ָ������Ŀ��ж� 
	MemoryManageP tempUseP = new MemoryManage;;			// tempUseP ָ������Ŀ��ж� 
 	MemoryManageP currentP = freeP;						//currentP ָ����ж� 
 	SegTableP tempSegP = PCBSegP;						//���̶��������ʱָ�� 
			
	//�ҵ���ַ��С�Ŀ��ж�
	// 1��ʾ�������䷨��2��ʾ����䷨��3��ʾ������䷨
	if(1 == mode){// 1��ʾ�������䷨
		//��һ�� currentP ��Ϊ���� 
		tempFreeP = currentP; 
	} 
	else{//(2 == mode) 2��ʾ����䷨ || (3 == mode)//3��ʾ������䷨
		//���� ���������ҵ���ʼ��ַ��С�Ŀ��ж�,�� tempFreeP ָ�� 
		smallAddr = currentP->startAddr;
		tempFreeP = currentP; 
		
		while(currentP != NULL){
			if(currentP->startAddr < smallAddr){
				smallAddr = currentP->startAddr;
				tempFreeP = currentP; 
			}
			currentP = currentP->next;
		} 
	}
	
	//�ҵ��׵�ַ �� smallAddr �����С��ռ�ö� ���� tempUseP ָ�� 
	currentP = useP;
	while((currentP != NULL) && (find ==0)){
		if(currentP->startAddr > smallAddr){
			tempUseP = currentP;
			find = 1;
		}
		currentP = currentP->next;
	}
	
	if(find == 0){
		return 1;
	} 
	else{ //��ʼŲ���ƻ�
		currentP = tempUseP;
		fAddr = smallAddr;
		while(currentP != NULL){	//currentP ָ�� ��ǰ��ҪŲ����ռ�ÿ� 
			lAddr = currentP->startAddr;
			for(i=0;i<currentP->length;i++){
				memContent[fAddr + i]=memContent[lAddr + i];
			}
			// ���� ���̶α����Ӧ�� ��ʼ��ַ
			tempSegP  = PCBSegP;
			xfind = 0;
			while( (tempSegP != NULL) && (xfind == 0)){
				int xi;
				for(xi =0; (xi<=2)&&(xfind == 0) ;xi++){	//�ҵ�ռ�öζ�Ӧ�Ľ��̶α��޸Ķ�Ӧ��ʼ��ֵַ 
					if(tempSegP->startAddr[xi] == lAddr){
						xfind =1;
						tempSegP->startAddr[xi] = fAddr;
					} 
				}
				tempSegP = tempSegP->next;
			} 
			
			//����ռ�ö�����Ķ�Ӧ��ʼ��ַ 
			currentP->startAddr = fAddr;
			fAddr += currentP->length;	//������ʱ���г�ʼ��ַ 
			currentP = currentP->next;
		}
		
		//���е�ռ�öζ�Ų�꣬��ʼ�޸Ŀ��ж�����
		currentP = freeP->next;
		freeP->startAddr = fAddr;
		freeP->next = NULL;
		while(currentP->next != NULL){	//���ɾ��������� 
			tempFreeP = currentP;
			currentP = currentP->next;
			delete tempFreeP;
		} 
		delete currentP;
	}
	
	return 1;
} 
