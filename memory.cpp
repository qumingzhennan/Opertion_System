#include"memory_share.h"
#include"Segment.h"
/*  	
	int Memory::MemoryInit(int memorytype)  		//����ѡ��ģʽ����Memory������ʼ��
		//���������	 memorytype = ( 1-ҳʽ	2-��ʽ ) 
	
	int Memory::Initize(int id,string code);		//��ʼ�������ڴ溯��
		//���������	id = (���̺�)  code = (' ',��һ���ո��) 
		
	
	string Memory::Read(int id, int start, int length, int type1);		//���ʺ���
		//���������	id = (���̺�)  start=(�߼���ַ��Ĭ��Ϊ 0 )  length=(�������ݵĳ��� )  
					//  type1 = (ֻ�ж�ʽ���õ���1-���ݶ�  2-����� )	

	
	int  Memory::Delete(int id);			//ɾ������
		//���������	id = (���̺�)
		
	
	int Memory::change(int id,string data);		//�ı��ڴ����ݺ���
		//���������	id = (���̺�)   data = ( �������� )
		
	
	int Memory::getusedmemory();		//�ڴ�ʹ���ʺ���
		
	void Memory::setSize(int i);		//�����ڴ��С 

	int Memory::getSize();				//��ȡ�ڴ��С

	void Memory::setType(int t);		//�����ڴ��������	
		//��������� t = ( 1-ҳʽ	2-��ʽ )

	int Memory::getType();				//��ȡ�ڴ��������
	  
*/ 


//��Memory������ʼ��
int Memory::MemoryInit(int memorytype)
{

    //�ڴ���䷽ʽ��ֻ���ڳ�ʼ����ʱ��ѡ��֮�󽫲������޸�
    type=memorytype;

    //ѡ��ҳʽ�����ڴ棬�½�һ��ҳʽ�������͵ı���
    if(type==1)
    {
        //Memory_share a;
    }
     else if(type==2)
    {
        //type=2;
        	//��ȡ�����ļ�����ʼ�� ��ʽ�� b 
        ifstream iniMem("memSegSet.txt");
        //iniMem.open("memSegSet.txt");
        /*if(!iniMem)
            cout<<"file can not opened!memory initial failed!"<<endl;
	    else
        {*/
	        cout<<"Success,open memSegSet.txt"<<endl;
			//�������ļ� memSegSet.txt �ж�ȡ������Ϣ ,����ӡ�������� 
			int memSize,memMode;
           // iniMem>>memSize>>memMode;
            memSize= 100000;
            memMode = 3;
            //cout<<"memSize��"<<memSize<<"\t";		//����ڴ��С
            //cout<<"memMode��"<<memMode<<endl;		//���ѡ���ڴ�����㷨
            //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
	
			//���������ļ���ȡ����Ϣ����ʼ�� ���� b  
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
	        b.fileInit();		// ����ļ���ʼ�� 
	        
	        cout<<"fileInit Success,now out "<<endl;
        //}


    }
}

//��ʼ�������ڴ溯��
int Memory::Initize(int id,string code)
{

           //ҳʽ�����ڴ棬����ҳʽ����ĳ�ʼ������
    if(type==1)
    {

        return a.storein(id,code);
        //�����ڴ沢���߼���ַ��IDһһ��Ӧ�洢 
    }
     else if(type==2)
    {


        //string s1 = code.toStdString();
        //size=s1.length();
        size=code.length();


         cout<<"���뺯��--"<<"Memory::InitizeSTORE()"<<endl<<endl<<endl<<endl;
        bool ok=b.store(id,size);
        if(ok)
        {
            bool i=b.writeCode(id,0,code);
            //cout<<"���뺯��--"<<"Memory::Initize()"<<endl;
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

//���ʺ���
//���ݸ�����id�ͷ��ʵĶ�����type�����߼���ַ��Ȼ����ݿ�ʼλ��start�ͳ���length����ȡ�ö����� 
string Memory::Read(int id, int start, int length, int type1)
{
    //cout<<"���뺯��--"<<"Memory::Read()"<<endl;
   // cout << id << start << length;
    //ҳʽ�����ڴ棬����ҳʽ����ķ��ʺ���
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

//ɾ������
int  Memory::Delete(int id)
{
    //cout<<"���뺯��--"<<"Memory::Delete()"<<endl;
    //ҳʽ�����ڴ棬����ҳʽ�����ɾ������
    if(type==1)
    {
        //ҳʽɾ���ɹ�����1�����򷵻�0
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


//�ı��ڴ����ݺ���
int Memory::change(int id,string data)
{
    //cout<<"���뺯��--"<<"Memory::change()"<<endl;
      //cout<<data<<endl<<endl;
    //ҳʽ�����ڴ棬����ҳʽ��������ݸı亯��
    if(type==1)
    {
        return a.change_page(id,data);

    }
    else if(type==2)
    {
        //string s = data.toStdString();
        //�ȴ洢
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

//�ڴ�ʹ���ʺ���
float Memory::getusedmemory()
{
    //cout<<"���뺯��--"<<"Memory::getusedmemory()"<<endl;
    //ҳʽ�����ڴ棬����ҳʽ�����ڴ���ڴ�ʹ����
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


//�����ڴ��С
void Memory::setSize(int i){
    size=i;
}

//��ȡ�ڴ��С
int Memory::getSize(){
    return size;
}

//�����ڴ��������
void Memory::setType(int t){
    type=t;
}

//��ȡ�ڴ��������
int Memory::getType(){
    return type;
}

