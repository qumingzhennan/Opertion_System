#include "Segment.h"
#include "head.h"
#include <string>
/*		int FitBest(int id,int size);		//最佳适配法分配内存		
		int FitFirst(int id,int size);		//最先适配法分配内存		
		int FitWorst(int id,int size);		//最坏适配法分配内存
			//输入参数； id = (进程号)   size = (需要写入的数据大小 ) 
				
		void fileInit();	 				//monitor.txt 和 monitorPCB.txt文件初始化函数 		
		int store(int id,int size);			//检测进程是否存在，给新建进程分配内存		
		
        void monitor();						//函数监测内存使用情况，并记录在文件中 
		void monitorPCB(SegTableP p);		//监测进程在内存中的分配和释放过程并记录在文件中
			//输入参数；	p = (新建或者更新的 PCB 表) 


		string visitData(int id,int offsetAddress,int length);	//访问数据段
        string visitCode(int id,int offsetAddress,int length);	//在代码段取指令执行
        	//输入参数；id = (进程号)   offsetAddress = (访问的起始地址，默认为 0 )   
						//  length = (访问的长度 , 可以通过读取段表中的数据长度获得)
        int getDataLength(int id);			//获得数据段长度
		int getDataAddr(int id);			//获得数据段首地址 
		int getCodeAddr(int id);			//获得代码段首地址 
			//输入参数；id = (进程号)
		
		int writeData(int id,int offsetAddress,string s);		//在数据段写数据
        int writeCode(int id,int offsetAddress,string s);		//在代码段存代码 
        	//输入参数；id = (进程号)   offsetAddress = (写入数据的起始地址，默认为 0 )   s = (写入的数据) 

		int deletePCB(int outChoi,int id);		//释放内存	
			//输入参数；  outChoi = (1-删除操作,2-换出操作)	  id = (进程号) 

		int cleanDebris();			//定时清理碎片 		

*/



/*		int FitBest(int id,int size);//**最佳适配法分配内存		*/
//	best fit，最佳适应分配算法，按空闲区长度 从小到大 排序。
//占用段链表 ，按使用段起始地址 从小到大的顺序排列

int MemorySegment::FitBest(int id,int size)	//size	进程需要的内存(进程大小)
{
    cout<<"Segment：：in best"<<endl;
    int seg[3];//**定义三个段，假设把每个进程所需的段分成3段，分别表示数据段，代码段，其他段
	int store=0; //store=1表示分配成功，store=0表示分配失败 
	
    seg[0]=40*size+100;
    seg[1]=size;
    seg[2]=size;

    SegTableP s=new SegmentTable();		//创建一个新的段表 
	// 给新建段表赋值 
    s->setProcessid(id);		// 设置段表进程号 
	s->setSegNum(3);			// 设置段数 
	s->setState(1);				// 标志该段表已分配 
	s->distribute();			// 给动态数组分配空间 
	s->next=NULL;				//段表 next 指针置空 

	int ok=0;
	MemoryManageP currentP = freeP;
	//**判断内存是否足够满足进程 
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
	
	//**内存不足 ，打印内存分配失败 
	if(ok<3)
	{
        store=0;
        cout<<"process"<<id<<"memory store failed!"<<endl;
        return 0;
	}
	 
	//内存不够，查询比自己优先级更低的进程，执行 deletePCB ********************************************************** 
	
	//**	内存足够，分配内存 --最佳适配法 
	if(3==mode)
	{
		for(int i=0;i<=2;i++)
		{
			currentP = freeP;		//currentP 指向空闲段 
			
			if(freeP->length >= seg[i])	//**********若第一块空闲块满足分配 
			{
				MemoryManageP useseg=new MemoryManage;
				MemoryManageP freeseg=new MemoryManage;
				MemoryManageP ptr=freeP;
				
				//给 seg[i] 分配使用 ，定义使用段表 
				useseg->length=seg[i];
				useseg->startAddr=currentP->startAddr;
				useseg->next=NULL;
				//更新 分配后的剩余空闲段 的空闲段表 
				freeseg->length=currentP->length-useseg->length;
				freeseg->startAddr=currentP->startAddr+useseg->length;
				freeseg->next=NULL;
				//更新空闲段链表指针 ，指向下一空闲段 
				freeP=freeP->next; 
				currentP=freeP;
				//删除原空闲段链表指针 
				delete ptr;
				ptr=NULL;
				
				//** 在空闲段链表插入节点 （遵循把最小的空闲块放在链表头） 
				MemoryManageP currentP1=useP,currentP2=freeP;
				if(freeP==NULL)
				{
					freeP=freeseg;
				}
				else{
					if(freeseg->length != 0){//若使用的空闲段刚好用完，且freeP有多个块，则删除该节点 	
						delete freeseg;
					}
					else{	//使用过的块仍有残余 ，按从小到大顺序插入 
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
					
				//**在占用段链表中插入节点 ，按使用段地址 从小到大的顺序排列 (currentP1=useP)
				if(currentP1==NULL)
				{
					useP = useseg;
					s->size[i]=seg[i];					//段表值的更新 
					s->startAddr[i]=useseg->startAddr;
				}
				else if(currentP1->startAddr >= useseg->startAddr){
					useseg->next = currentP1;
					currentP1 = useseg;
					useP = useseg;
					s->size[i]=seg[i];					//段表值的更新 
					s->startAddr[i]=useseg->startAddr;
				}
				else{
					int okin = 0 ;
					while((currentP1->next !=NULL) &&(okin != 1)){
						if(currentP1->next->startAddr >= useseg->startAddr){
							//插入在 currentP1->next 前面 
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
			else	//*********************第一块空闲块不满足分配时 
			{
				while(currentP->next!=NULL)	
				{
					if(currentP->next->length >= seg[i])	//查找下一块，直至找到满足分配的空闲块 
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
						
						//删除使用的空闲段表 
						currentP->next=currentP->next->next;
						delete ptr;
						ptr=NULL;
						
						MemoryManageP currentP1=useP,currentP2=freeP;
						//**在占用段链表中插入节点 ，按使用段地址 从小到大的顺序排列 (currentP1=useP)
						if(currentP1==NULL)
						{
							useP = useseg;
							s->size[i]=seg[i];					//段表值的更新 
							s->startAddr[i]=useseg->startAddr;
						}
						else if(currentP1->startAddr >= useseg->startAddr){
							useseg->next = currentP1;
							currentP1 = useseg;
							useP = useseg;
							s->size[i]=seg[i];					//段表值的更新 
							s->startAddr[i]=useseg->startAddr;
						}
						else{
							int okin = 0 ;
							while((currentP1->next !=NULL) &&(okin != 1)){
								if(currentP1->next->startAddr >= useseg->startAddr){
									//插入在 currentP1->next 前面 
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
						
						//**在空闲段插入节点 
						if(currentP2==NULL)
						{
							freeP=freeseg;
							currentP=freeP;
						}
						else{
							if(freeseg->length == 0){	//若使用的空闲段刚好用完，则删除该节点
								delete  freeseg;
							}
							else{ //使用过的块仍有残余
								
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
       
		//**插入段表链表 
		if(PCBSegP==NULL)	//进程段 链表头指针为空，新建 
		{
            PCBSegP=s;
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;
		}
		else				//若进程段链表头不为空 ，则找到末尾，插入新段表 s 在进程段链表 
		{
			SegTableP newCurrentP=PCBSegP;
			while(newCurrentP->next!=NULL)
			{
				newCurrentP=newCurrentP->next;
			}
			newCurrentP->next=s;
			
			//更新空闲段 以及 使用段大小 
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;		//PCB 数量更新 
		}
		store=1;
		//更新内存检测文件，以及内存检测PCB文件 
		monitor();
		monitorPCB(s);
        return 1;

	}
	
}
 


/*		int FitFirst(int id,int size);//**最先适配法分配内存		*/
//		first fit，最先适应分配算法，按地址递增排序。 
//占用段链表 ，按使用段起始地址 从小到大的顺序排列

int MemorySegment::FitFirst(int id,int size)	//size	进程需要的内存(进程大小)
{	cout<<"in first"<<endl;
	int seg[3];	//**定义三个段，假设把每个进程所需的段分成3段，第一段占20%，第二段占60%，第三段占20%
	int store=0; //store=1表示分配成功，store=0表示分配失败 
	
    seg[0]=size*40+100;
    seg[1]=size;
    seg[2]=size;
    
	SegTableP s=new SegmentTable;		//创建一个新的段表 
	// 给新建段表赋值 
    s->setProcessid(id);		// 设置段表进程号 
	s->setSegNum(3);			// 设置段数 
	s->setState(1);				// 标志该段表已分配 
	s->distribute();			// 给动态数组分配空间 
	s->next=NULL;				//段表 next 指针置空 
	
	int ok=0;
	MemoryManageP currentP=freeP;
	//**判断内存是否足够满足进程 
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
	//**内存不足 
	if(ok<3)
	{
		store=0;
        cout<<"process"<<id<<"memory store failed!"<<endl;
		return 0;
	}
	
	//**	内存足够，分配内存 --最先适应法	
	if(1 == mode)
	{ 
		currentP=freeP;
		for(int i=0;i<=2;i++)
		{
			currentP=freeP;
			
			if(freeP->length >= seg[i])		//**********若第一块空闲块满足分配
			{
				MemoryManageP useseg=new MemoryManage;
				MemoryManageP freeseg=new MemoryManage;
				MemoryManageP ptr=freeP;
				
				//给 seg[i] 分配使用 ，定义使用段表 
				useseg->length=seg[i];
				useseg->startAddr=currentP->startAddr;
				useseg->next=NULL;
				//更新 分配后的剩余空闲段 的空闲段表 
				freeseg->length=currentP->length-useseg->length;
				freeseg->startAddr=currentP->startAddr+useseg->length;
				freeseg->next=NULL;
				//更新空闲段链表指针 ，指向下一空闲段 
				freeP=freeP->next; 
				currentP=freeP;
				//删除原空闲段链表指针 
				delete ptr;
				ptr=NULL;
				
				if(freeP==NULL)
				{
					freeP = freeseg;
				}
				else{	
					 if(freeseg->length != 0){	//把剩余的空闲块 插入空闲链表头 
					 	freeseg->next=freeP;
						freeP=freeseg;
						currentP=freeP;
					 }
					 //freeseg->length == 0 时，啥都不干 
				}
				
					
				MemoryManageP currentP1=useP,currentP2=freeP;
				//**在占用段链表中插入节点 ，按使用段地址 从小到大的顺序排列 (currentP1=useP)
				if(currentP1==NULL)
				{
					useP = useseg;
					s->size[i]=seg[i];					//段表值的更新 
					s->startAddr[i]=useseg->startAddr;
				}
				else if(currentP1->startAddr >= useseg->startAddr){
					useseg->next = currentP1;
					currentP1 = useseg;
					useP = useseg;
					s->size[i]=seg[i];					//段表值的更新 
					s->startAddr[i]=useseg->startAddr;
				}
				else{
					int okin = 0 ;
					while((currentP1->next !=NULL) &&(okin != 1)){
						if(currentP1->next->startAddr >= useseg->startAddr){
							//插入在 currentP1->next 前面 
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
			else			//*********************第一块空闲块不满足分配时 
			{
				int find=0;
				while(currentP->next!=NULL && find==0) //查找下一块，直至找到满足分配的空闲块
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
						
						
						//**在空闲段插入链表 	
						if( (freeseg->length == 0)&&(currentP->next->next != NULL) ){
							//刚好用完  currentP->next 这一空闲块 
							//把空闲块指针指向下一个空闲块 
							tempseg = currentP->next;
							currentP->next = currentP->next->next;
							delete tempseg;
						} 
						else{	//把使用的空闲块的 起始地址、空闲大小的值修改
							currentP->next->length=freeseg->length;
							currentP->next->startAddr=freeseg->startAddr;
							delete freeseg;
							freeseg=NULL;						
						}
						
						
						MemoryManageP currentP1=useP,currentP2=freeP;
						//**在占用段链表中插入节点 ，按使用段地址 从小到大的顺序排列 (currentP1=useP)
						if(currentP1==NULL)
						{
							useP = useseg;
							s->size[i]=seg[i];					//段表值的更新 
							s->startAddr[i]=useseg->startAddr;
						}
						else if(currentP1->startAddr >= useseg->startAddr){
							useseg->next = currentP1;
							currentP1 = useseg;
							useP = useseg;
							s->size[i]=seg[i];					//段表值的更新 
							s->startAddr[i]=useseg->startAddr;
						}
						else{
							int okin = 0 ;
							while((currentP1->next !=NULL) &&(okin != 1)){
								if(currentP1->next->startAddr >= useseg->startAddr){
									//插入在 currentP1->next 前面 
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
		
		//**插入段表链表 
		if(PCBSegP==NULL)	//进程段 链表头指针为空，新建 
		{
			PCBSegP=s;
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;
		}
		else				//若进程段链表头不为空 ，则找到末尾，插入新段表 s 在进程段链表 
		{
			SegTableP newCurrentP=PCBSegP;
			while(newCurrentP->next!=NULL)
			{
				newCurrentP=newCurrentP->next;
			}
			newCurrentP->next=s;
			
			//更新空闲段 以及 使用段大小 
            int k=seg[0]+seg[1]+seg[2];
            setFreeSize(freeSize-k);
            setUseSize(useSize+k);
			PCBNum++;			//PCB 数量更新 
		}
		
		store=1;
		//更新内存检测文件，以及内存检测PCB文件 
		monitor();
		monitorPCB(s);
		return 1;
	}
}


/*		int FitWorst(int id,int size);	//**最坏适配法分配内存		*/
// 最坏适应分配算法，按空闲区长度从大到小排序
//占用段链表 ，按使用段起始地址 从小到大的顺序排列

int MemorySegment::FitWorst(int id,int size)	//size	进程需要的内存(进程大小)
{
	int seg[3];//**定义三个段，假设把每个进程所需的段分成3段，第一段占20%，第二段占60%，第三段占20%
	int store=0; //store=1表示分配成功，store=0表示分配失败 
	
    seg[0]=size*40+100;
    seg[1]=size;
    seg[2]=size;
    
	SegTableP s=new SegmentTable;		//创建一个新的段表 
	// 给新建段表赋值 
    s->setProcessid(id);		// 设置段表进程号 
	s->setSegNum(3);			// 设置段数 
	s->setState(1);				// 标志该段表已分配 
	s->distribute();			// 给动态数组分配空间 
	s->next=NULL;				//段表 next 指针置空
	
	int ok=0;
	MemoryManageP currentP=freeP;
	//**	判断内存是否足够满足进程 
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
	//**内存不足 
	if(ok<3)
	{
		store=0;
        cout<<"process"<<id<<"memory store failed!"<<endl;
		return 0;
	}
	
	//**	内存足够，分配内存 --最坏适配法 
	if(2==mode)
	{
		for(int i=0;i<=2;i++)
		{
			currentP=freeP;
			
			if(freeP->length >= seg[i])		//**********若第一块空闲块满足分配
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
				
				//**在空闲段链表插入节点 
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
					
					
				//**在占用段链表中插入节点 ，按使用段地址 从小到大的顺序排列 (currentP1=useP)
				if(currentP1==NULL)
				{
					useP = useseg;
					s->size[i]=seg[i];					//段表值的更新 
					s->startAddr[i]=useseg->startAddr;
				}
				else if(currentP1->startAddr >= useseg->startAddr){
					useseg->next = currentP1;
					currentP1 = useseg;
					useP = useseg;
					s->size[i]=seg[i];					//段表值的更新 
					s->startAddr[i]=useseg->startAddr;
				}
				else{
					int okin = 0 ;
					while((currentP1->next !=NULL) &&(okin != 1)){
						if(currentP1->next->startAddr >= useseg->startAddr){
							//插入在 currentP1->next 前面 
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
		//**插入段表链表 
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
 


/*		void fileInit();	//**monitor.txt 和 monitorPCB.txt文件初始化函数 		*/

void MemorySegment::fileInit()
{
	cout<<"Segment：：in fileinit"<<endl;

	//**创建或打开监测文件,并初始化 
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
	
	//**初始化进程占用内存监测文件 
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



/*		int store(int id,int size);	//检测进程是否存在，给新建进程分配内存		*/

int MemorySegment::store(int id,int size)
{	
	cout<<"Segment：：in store"<<endl;
	SegTableP currentP = PCBSegP;   //currentP 指向进程段表头 
    int pcbId=id;
    
	//**判断进程是否已存在，如果存在，则分配内存失败. 
	if (currentP == NULL){
		cout<<"currentP(PCBSegP) == NULL"<<endl;
	}
	while(currentP!=NULL)
	{
		if(currentP->getProcessid() == pcbId) //新建进程号已经存在 
		{
           cout<<"process"<<id<<"has already exist!"<<endl;
           return 0;
		}
		else
			currentP=currentP->next;	 
	}

    cout<<"!!!!!!!!!!!mode!!!!!!"<<mode<<endl<<endl<<endl;
    mode =3;
	//1表示最先适配法，2表示最坏适配法，3表示最佳适配法 
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
		void monitor();				//**函数监测内存使用情况，并记录在文件中 
		void monitorPCB(SegTableP p);//**监测进程在内存中的分配和释放过程并记录在文件中
*/

//**内存监测函数,监测内存使用情况，并记录在文件中 
void MemorySegment::monitor()
{
	cout<<"Segment：：in monitor"<<endl;
	//**打开监测文件，开始向文件内写信息，每次写在文件末尾
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
        // 逐条打印出 空闲段情况 
        currentP=freeP ;
		while(currentP!=NULL) 
		{
            monitorfile<<"-->startaddr:"<<currentP->startAddr<<"  length:"<<currentP->length;
			currentP=currentP->next;
		}
        
        monitorfile<<endl<<" usesegment is:";
		// 逐条打印出 使用段情况 
		currentP=useP;
		while(currentP!=NULL) 
		{
            monitorfile<<"-->startaddr:"<<currentP->startAddr<<"  length:"<<currentP->length;
			currentP=currentP->next;
		}
		
	}
	
	monitorfile.close();
}


//**进程使用内存监测函数 ,监测进程在内存中的分配和释放过程并记录在文件中
void MemorySegment::monitorPCB(SegTableP p)
{
    cout<<"Segment：：in monitorPCB"<<endl;
	//**打开监测文件，开始向文件内写信息，每次写在文件末尾
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
		string visitData(int id,int offsetAddress,int length);//**访问数据段
        string visitCode(int id,int offsetAddress,int length);//**在代码段取指令执行
        int getDataLength(int id);	//**获得数据段长度
		int getDataAddr(int id);	//**获得数据段首地址 
		int getCodeAddr(int id);	//**获得代码段首地址 
*/

string MemorySegment::visitData(int id,int offsetAddress,int length)	//**访问数据段
{
	cout<<"Segment：：in visitData"<<endl;
	string s;

	SegTableP currentP=PCBSegP;
	int find=0;
	while(currentP!=NULL && find==0) 
	{	//查找 id 对应的段表 
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
	else		// 找到段表之后，开始读数据操作 
	{
            int address=offsetAddress+currentP->startAddr[0];
		
			if(currentP->startAddr[0] <= address)
			{
				//**打开监测文件，开始向文件内写信息，每次写在文件末尾
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
					
					//命令行输出 
					cout<<endl<<"id"<<id<<"visit memory:"<<address<<" get data:"<<s<<endl;
					
                    monitorfile<<endl<<"id"<<id<<"visit memory:"<<address<<" get data:"<<s<<endl;
					monitorfile.close();
                    delete [] stemp;
                    cout<<"Segment：：in read data";
                    return s;

				}
				
			}
				
        cout<<"Segment：：visit across bound!"<<endl;
		s="";
        return s;
	}
}


string MemorySegment::visitCode(int id,int offsetAddress,int length)	//**在代码段取指令执行
{
    cout<<"Segment：：in visitCode"<<endl;
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
				//**打开监测文件，开始向文件内写信息，每次写在文件末尾
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
					
					//命令行输出 
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


int MemorySegment::getDataLength(int id)	//**获得数据段长度
{
	cout<<"Segment：：in getDataLength"<<endl;
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
        return currentP->size[0];		//******返回数据段长度 
    }
}

int MemorySegment::getDataAddr(int id)		//**获得数据段首地址
{	
	cout<<"Segment：：in getDataAddr"<<endl;
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
        return currentP->startAddr[0];		// 返回数据段首地址 
    }


}

int MemorySegment::getCodeAddr(int id)	//**获得代码段首地址 
{	
	cout<<"Segment：：in getcodeaddr"<<endl;
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
		int writeData(int id,int offsetAddress,string s);//**在数据段写数据
        int writeCode(int id,int offsetAddress,string s);//**在代码段存代码 
*/

int MemorySegment::writeData(int id,int offsetAddress,string s)	//**在数据段写数据
{
    cout<<"Segment：：in writedata"<<endl;
	
	SegTableP currentP=PCBSegP;
	int find=0;
	while(currentP!=NULL && find==0)	// 找到 id 对应的段表 
	{
		if(currentP->getProcessid()==id)
			find=1;
		else
			currentP=currentP->next;
	}	
    if(0 == find)						// 查找 id 失败 
	{
        cout<<"process"<<id<<" is not exist!write memory failed!"<<endl;
        cout<<"writedatat end"<<endl;
		return 0;
	}
	else
    {									// 查找 id 成功，执行写操作 
    		// adress 指向 开始写操作 的实际数据段地址 
            int address=currentP->startAddr[0]+offsetAddress;
			if(currentP->startAddr[0] <= address)
			{
				//**打开监测文件，开始向文件内写信息，每次写在文件末尾
				ofstream monitorfile; 
				monitorfile.open("monitorPCB.txt",ios::app);
				
				if(!monitorfile)	//打开PCB 监测文件操作失败 
				{
                    cout<<"file open failed!"<<endl;
                    monitorfile.close();
                    cout<<"writedatat end"<<endl;
					return 0;
				}//string类的size()/length()方法返回的是字节数，不管是否有汉字。 
				else if(currentP->startAddr[0]+currentP->size[0] < address+s.length())
				{					//写入操作后 超出地址范围 
                    cout<<"write across bound!"<<endl;
                    monitorfile.close();
                    cout<<"writedatat end"<<endl;
					return 0; 
				}
				else
				{					// 执行 写入操作 
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
		
        cout<<"Segment：：write across bound!"<<endl;
        cout<<"Segment：：writedatat end"<<endl;
		return 0;
	}
}


int MemorySegment::writeCode(int id,int offsetAddress,string s)	//**在代码段存代码
{
	cout<<"Segment：：in writecode"<<endl;

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
			// adress 指向 开始写操作 的实际代码段地址 
            int address=currentP->startAddr[1]+offsetAddress;
			if(currentP->startAddr[1] <= address)
			{
				//**打开监测文件，开始向文件内写信息，每次写在文件末尾
				ofstream monitorfile; 
				monitorfile.open("monitorPCB.txt",ios::app);
				
				if(!monitorfile)
				{				// 监测文件打开失败 
                    cout<<"Segment：：writecode file open failed!"<<endl;
                    monitorfile.close();
					return -1;
				}
				else if(currentP->size[1]+currentP->startAddr[1] < address+j)
				{				// 要求输入 超出范围 
                    cout<<"Segment：：write across bound!"<<endl;
                    monitorfile.close();
					return -1;
				}
				else
				{				// 开始把数据写入 代码段 
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
			
        cout<<"Segment：：write across bound!"<<endl;
        cout<<"Segment：：end write code"<<id;
		return 0;
	}
}
 

/*		int deletePCB(int outChoi,int id);		//**释放内存		*/
// outChoi = 1时，删除查询 id 的段表以及在内存分配的空间，
// outChoi = 2时，把查询 id 的段表插入到段表链表队尾，state状态置 0 以及 在内存分配的空间，
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


 
 //!!!!!!!!!!!!!!!!		加进程　定时在 调用前实现	!!!!!!!!!!!!!!!!!!!!!!! 
 //!!!!!!!!!!!!!!!!		中断所有其他对内存的操作 	!!!!!!!!!!!!!!!!!!!!!!! 
/*		int cleanDebris();			//定时清理碎片 		*/

int MemorySegment::cleanDebris(){
	int i,xfind;
	int find = 0;
	int smallAddr,fAddr,lAddr;
	MemoryManageP tempFreeP= new MemoryManage;;			// tempFreeP 指向所求的空闲段 
	MemoryManageP tempUseP = new MemoryManage;;			// tempUseP 指向所求的空闲段 
 	MemoryManageP currentP = freeP;						//currentP 指向空闲段 
 	SegTableP tempSegP = PCBSegP;						//进程段链表的临时指针 
			
	//找到地址最小的空闲段
	// 1表示最先适配法，2表示最坏适配法，3表示最佳适配法
	if(1 == mode){// 1表示最先适配法
		//第一个 currentP 即为所求 
		tempFreeP = currentP; 
	} 
	else{//(2 == mode) 2表示最坏适配法 || (3 == mode)//3表示最佳适配法
		//遍历 空闲链表，找到起始地址最小的空闲段,用 tempFreeP 指向 
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
	
	//找到首地址 比 smallAddr 大的最小的占用段 ，用 tempUseP 指向 
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
	else{ //开始挪动计划
		currentP = tempUseP;
		fAddr = smallAddr;
		while(currentP != NULL){	//currentP 指向 当前需要挪动的占用块 
			lAddr = currentP->startAddr;
			for(i=0;i<currentP->length;i++){
				memContent[fAddr + i]=memContent[lAddr + i];
			}
			// 更新 进程段表里对应的 起始地址
			tempSegP  = PCBSegP;
			xfind = 0;
			while( (tempSegP != NULL) && (xfind == 0)){
				int xi;
				for(xi =0; (xi<=2)&&(xfind == 0) ;xi++){	//找到占用段对应的进程段表，修改对应起始地址值 
					if(tempSegP->startAddr[xi] == lAddr){
						xfind =1;
						tempSegP->startAddr[xi] = fAddr;
					} 
				}
				tempSegP = tempSegP->next;
			} 
			
			//更新占用段链表的对应起始地址 
			currentP->startAddr = fAddr;
			fAddr += currentP->length;	//更新临时空闲初始地址 
			currentP = currentP->next;
		}
		
		//所有的占用段都挪完，开始修改空闲段链表
		currentP = freeP->next;
		freeP->startAddr = fAddr;
		freeP->next = NULL;
		while(currentP->next != NULL){	//逐个删除后续结点 
			tempFreeP = currentP;
			currentP = currentP->next;
			delete tempFreeP;
		} 
		delete currentP;
	}
	
	return 1;
} 
