#include "memory_share.h"
#include<QFile>
#include<QTextStream>
/*
	Memory_share::Memory_share() //���������ļ���ʼ�� 
	int Memory_share::change_page(int id,string data) //ҳ����£����ṩ���ַ������Ǹ�ID��Ӧ��ԭ�е����ݶ�
		//id--���̺� data--��Ҫ�洢�Ĵ��� 
		//����-1/0/1/2 -1--û���㹻�ռ�������ݶθ��� 0--���ݶν�û���㹻�ռ�������� 1--����������ݶθ��� 2--ɾ��ʧ�� 
	bool Memory_share::Deleteall(int id) //ɾ���ڴ溯�������ݸ�����PCBIDɾ������Ϊ�������ڴ�
		//id--���̺� 
		//����true/false --ɾ���ɹ����	
	bool Memory_share::Delete(int logic_address) //�����߼���ַɾ���ڴ�
		//logic_address--��ɾ��֡���ڵ��߼���ַ 
		//����true/false --ɾ���ɹ����	
	void Memory_share::print(int type) //��־�ļ����
		//����Ҫ���еĲ������з��࣬��Ӧ��ͬ��type����
	string Memory_share::read(int id,int start,int length,int type1) //���ڴ��ж�ȡ���� 
		//id--���̺� start--�ڴ���ʼ��ַ length--���ݳ��� type1--1Ϊ����ҳ���Ӧ�ڴ棬2Ϊ�����ڴ����� 	
		//����replace��������󷵻�Ҫ��ȡ������ 
	string Memory_share::replace(int pcblogicaddress,int start ,int end) //���ʺ����������߼���ַ����ʼ�ͽ���λ�÷����߼���ַָ������ݴ���ʼ��ʼ������λ�ý���������	
		//pcblogicaddress--������ƿ������߼���ַ start--��ʼλ�� end--����λ��
		//����Ҫ��ȡ������  
	string Memory_share::Replace_page(int number_frame,int type,int start,int length) //������ҳ���û��㷨��ҳ���ʺ���
		//number_frame--�µ�����֡�� type--ҳ���û��㷨��1ΪFIFO��2ΪLRU 	 	
		//����v_realframe[vn].substr(start,length) --��¼�����ҳ֡�д�start��ʼ����Ϊlength���ַ��� 
	int Memory_share::Store(string code) //�ڴ����ƴ洢���洢�ṩ���ַ����������߼���ַ 
		//code--Ҫ�洢���ַ��� 
		//����first_address--�ַ������ڴ��д洢���߼���ַ 	
	bool Memory_share::Storejudge(string code) //�ڴ�����жϺ���
		//code--Ҫ�洢���ַ��� 
		//����true/false  �ڿ���֡�����㹻������洢���ݵ�֡�ʹ洢ҳ���֡������£�����true�����򷵻�false
	int Memory_share::num_Storejudge(string code) //�������֡��
		//code--Ҫ�洢���ַ���
		//����(num_frame+length_page) --��֡�� 
	int  Memory_share::storein(int id,string code) //PCB��ʼ�������ڴ���亯��
		//id--���̺ţ�code--��PCB�Ĵ���� 
		//����1/0--����ɹ����
	int Memory_share::getroom_physicmemory() //��ȡ�����ڴ�����
		//���������ڴ��С	
	int Memory_share::getusedroom_physicframe() //��ȡ��ʹ�õ������ڴ��С
		//������ʹ�������ڴ��С 
*/ 

Memory_share::Memory_share()
{
    //���������ļ���ʼ���ڴ�ϵͳ
    ifstream fin("Initialpage.txt");
    //QTextStream in(&fin);
    /*  �ڴ��С:20000
	    ֡��С:200
	    ����С:1000
	    ҳ���û��㷨��1ΪFIFO 2ΪLRU��:1  */
    string s[4];
    string buffer;
    int cs,pd,i,st;
    int ini[4];//�����ļ���4�������������� 
    cs=0;

    //���ж�ȡ�����ļ�������������
    while( getline(fin,buffer) )
    {
        i=0;
        pd=0;

        //�ԣ�Ϊ�ָ�������ȡ���������
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
        //�����ļ�����û�г��֣���
        if(pd==0)
        {
            cout<<"error"<<endl;

        }
        else
        {
            //��ȡ��֮����ַ�������
            st=i+1;
            i++;
            while(buffer[i])
            {	
				pd++;
                i++;
			}
            pd--;
            //��ȡ��֮������ݣ������ַ���ת����int�ʹ洢������
            ini[cs]=atoi((buffer.substr(st,pd)).c_str());
            cs++;
        }
        
    }
	

    //�������ļ���ȡ��õ����ݸ�ֵ���ڴ�ϵͳ���ò���
    //�����ڴ��С
    //room_physicmemory=ini[0];
    room_physicmemory=20000;
    cout<<"000000�����ڴ��СΪ��"<<room_physicmemory<<endl;
	
    //֡��С 
    //room_physicframe=ini[1];
    room_physicframe = 200;
   cout<<"1111111111֡��СΪ��"<<room_physicframe<<endl;
    
    //����֡֡��
    number_physicframe=room_physicmemory/room_physicframe;
    
    //����֡��������ʼ��������֡����
    free_physicframe=number_physicframe;
    
    //����С
    //v_memory=ini[2];
    v_memory = 1000;
    cout<<"22222222����СΪ��"<<v_memory<<endl;
    
    //�������
    num_pageframe=v_memory/room_physicframe;
    
    //ҳʽ�û��㷨���ͣ�1ΪFIFO��2ΪLRU
    //pagereplacetype=ini[3]; //�������ļ������û��㷨����
    pagereplacetype = 1;
    if(pagereplacetype==1){
        cout<<"3333ҳʽ�û��㷨����Ϊ��"<<"�����ȳ���FIFO��"<<endl;
    }
    else{
        cout<<"3333333ҳʽ�û��㷨����Ϊ��"<<"��������ʹ�ã�LRU��"<<endl;
	} 

    //��̬���飬����Ϊ����֡����
    state_physicframe=new int[number_physicframe];
    
    //�˴���¼����֡״̬����ʼ��Ϊ0��ʾ֡��û�м�¼�κ���Ϣ
    for(int i=0;i<number_physicframe;i++)
    {
        state_physicframe[i]=0;
    }

    //��ά�����¼������ڼ�¼�����ݣ���һάΪ����֡����
    frame = new int*[number_physicframe];
    for (int a = 0; a != number_physicframe; ++a)
    {
        frame[a] = new int[room_physicframe];
    }
    //�ڶ�άΪ����֡��С����ʼ��Ϊ-1
    for(int i=0;i<number_physicframe;i++)
        for(int n=0;n<room_physicframe;n++)
        {
            frame[i][n]=-1;
        }

    //���ҳ֡ת̬����һάΪ���ҳ֡����
    state_pageframe=new int*[num_pageframe];
    for(int m=0;m<num_pageframe;m++)
    {
        state_pageframe[m]=new int[2];
    }

    //�ڶ�ά��СΪ2���ֱ��¼��֡��Ӧ�������ڴ��֡���Լ�������еĴ���ʱ��
    for(int p=0;p<num_pageframe;p++)
    {
        state_pageframe[p][0]=-1;
        state_pageframe[p][1]=0;
    }

    //�����ڴ�ʵ�����ݴ洢�����ַ���������
    realframe=new string[number_physicframe];
    for(int i=0;i<number_physicframe;i++)
    {
        realframe[i]="";
    }

    //���ʵ�����ݴ洢�����ַ���������
    v_realframe=new string[num_pageframe];
    for(int i=0;i<num_pageframe;i++)
    {
        v_realframe[i]="";
    }

    /*��ά���飬�����ǽ�PCB�����̿��ƿ飩ID�͸�PCB�����ݶκʹ���ε��߼���ַһһ��Ӧ
    �ڴ�͸�PCB�Ľ���ȫ����IDΪ�ؼ��֣��߼���ַ���ڴ��Լ���¼ */   
    //��ʼ�� 
    onebyone=new int*[number_physicframe];
    for(int q=0;q<number_physicframe;q++)
    {
        onebyone[q]=new int[3];
    }
    
    //��ʼ��ȫ��λ-1���ڶ�ά�ĵ�һλ��¼ID���ڶ�����λ��¼���ݶκʹ���ε��߼���ַ
    for(int w=0;w<number_physicframe;w++)
    {
        onebyone[w][0]=-1;
        onebyone[w][1]=-1;
        onebyone[w][2]=-1;
    }
		
    //��־�ļ���պ���
    print(0);
}



//�ڴ�洢����
int Memory_share::change_page(int id,string data) //ҳ����£����ṩ���ַ������Ǹ�ID��Ӧ��ԭ�е����ݶ�
{
	cout<<"���뺯��--"<<"Memory_share::change_page()"<<endl;
    //���ݸ�����PCBID���ڴ����ҵ���Ӧ�����ݶ��߼���ַ
    int i,a,b,logicaddress1;
    for(i=0;i<number_physicframe;i++)
    {
        if(onebyone[i][0]==id)
          break;
    }

    a=1;
    b=1;
    logicaddress1=onebyone[i][1];

    //�����PCB���ڴ����ѷ�����ڴ��С
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
    
	//������Ҫ�洢�Ĵ��볤��
    string pcbcode=data;
    int length_code=pcbcode.size();

    //������Ҫ�洢��δ�������ɵ�ҳ��ĳ��ȣ�Ҳ���Ǵ洢�����������Ҫ��ҳ֡����
    int length_page;
    length_page=length_code/room_physicframe;
    if((length_code)%room_physicframe!= 0)
    length_page++;

	//������Ҫ�������ҳ�����֡��
    int num_frame;
    //���ҳ����Ϊ0������Ҫ0��ҳ֡���洢������Ϊ1������Ҫ1��֡���洢
    if(length_page==0)
    num_frame=0;
    else if(length_page==1)
    num_frame=1;
   //������������´��ҳ����Ҫ����֡��
    else if((length_page%(room_physicframe-1))<2)
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }
     //��Ҫ�������ҳ�����֡���������洢ҳ���֡��
    int need=num_frame+length_page;


	//�������֡�������㹻�洢������ݣ��򷵻�-1��ʾû���㹻�ռ�������ݶθ���
     if((free_physicframe+a)>=need)
     {
         //ɾ��ԭ�����ݶΣ����ɾ��ʧ�ܷ���2��ʾ
        if(Delete(logicaddress1)==true)
         {
            //ɾ���ɹ�������£�Ϊ�µ����ݷ����ڴ�
             int i;
             for(i=0;i<number_physicframe;i++)
             {
                 if(onebyone[i][0]==id)
                   break;
             }
             //��¼�´洢�����ݶε��߼���ַ
             onebyone[i][1]=Store(data);

             //����������ݶ�֮����40�������ֽڣ�Ϊ��һ���ӵ��������Ԥ����
             //���һ����������Ͽ���֡������£���ֹ��ǰҳ֡Ϊ��ҳ�����ڶ�֡��
             //����1��ʾ����������ݶθ��£����򷵻�0��ʾ���ݶν�û���㹻�ռ��������
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



//ɾ���ڴ溯�������ݸ�����PCBIDɾ������Ϊ�������ڴ�
bool Memory_share::Deleteall(int id)
{
	cout<<"���뺯��--"<<"Memory_share::Deleteall()"<<endl;
    int s=0;
    int p=0;
    //���ڴ����ҵ��������ID��Ӧ��ID
    for(s=0;s<number_physicframe;s++)
    {
        if(onebyone[s][0]==id)
        {
            p=1;
            break;
        }
    }
    //���Ѱ�Ҳ������򷵻�ɾ��ʧ��
    if(p==0)
    {
        return false;
    }
    else
    {
        //�ҵ���Ӧ��ID������Եõ���Ӧ�����ݶκʹ���ε��߼���ַ
        //�������ݶκʹ�����߼���ַɾ�����ݶκʹ���η�����ڴ�
        Delete(onebyone[s][1]);
        Delete(onebyone[s][2]);
        //ɾ�������IDһһ��Ӧ�����ݲ��ҷ���ɾ���ɹ�
        onebyone[s][0]=-1;
        onebyone[s][1]=-1;
        onebyone[s][2]=-1;
        return true;
    }

}

//�����߼���ַɾ���ڴ�
bool Memory_share::Delete(int logic_address)
{
	cout<<"���뺯��--"<<"Memory_share::Delete"<<endl;
    int logic_address2;
    logic_address2=logic_address;
    //���߼���ַ���ڵ�֡����ɾ��״̬��Ϊ����
    state_physicframe[logic_address2]=0;
    realframe[logic_address2]="";
    free_physicframe++;
    int i=0;

    //�����߼���ַ�����߼���ַָ������д洢���������ݵ�֡ȫ��ɾ�����ݣ��洢���ݸ�Ϊ���ַ���״̬��Ϊ����
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
        //��ҳ��洢�ڶ��֡������£������߼���ַ����ɾ���µ��߼���ַ���ڵ�֡�����ݺ�״̬
        else if(state_physicframe[a]==1)
        {
            logic_address2=frame[logic_address2][i];
            realframe[a]="";
            state_physicframe[a]=0;
            free_physicframe++;
            i=0;
        }
    }
    //���ڴ�ĸı��������־�ļ�������ɾ���ɹ�
    print(1);
    print(2);
    print(5);
    return true;
}



//��־�ļ����
void Memory_share::print(int type)
{
    //cout<<"Memory_Dialog out"<<endl;
    //ʱ�亯�����ڼ�¼�ڴ����淢���仯��ʱ��ͬʱ��¼����ʱ��
    time_t t = time(0);
    char tmp[64];
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X",localtime(&t) );


    if(type==1)
    {
        //��־״̬��¼�ļ�
        //ofstream outfile("memorystate_logfile.txt",ios::app);//�ڴ�״̬��־�ļ� //ÿ��֡��״̬��0���У�1�洢ҳ��2�洢����
        QFile outfile("memorystate_logfile.txt");
        QTextStream in(&outfile);
        if(!outfile.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //cout<<"1.Memory_Dialog out"<<endl;
            //���м�¼���ڴ�ÿ��֡��״̬
            //ÿ�ε�����������������¼�¼
            in<<tmp<<"\n";
            in<<"֡��"<<"    "<<"״̬"<<"\n";
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
        //��־���ݼ�¼�ļ�
        //ofstream outfile1("memorycontent_logfile.txt",ios::app);//�ڴ�������־�ļ�
        QFile outfile1("memorycontent_logfile.txt");
        QTextStream in(&outfile1);
        if(!outfile1.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //���м�¼ÿ��֡�ڴ洢������
            //ÿ�μ�¼�������¼�¼
            in<<tmp<<"\n";
            in<<"֡��"<<"    "<<"����"<<endl;
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
        //���״̬��¼�ļ�
        //ofstream outfile2("virtualstate_logfile.txt",ios::app);//���״̬��־�ļ�
            QFile outfile2("virtualstate_logfile.txt");
            QTextStream in(&outfile2);
        if(!outfile2.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //���μ�¼���ÿ��֡��֡�źʹ���ʱ��
            //ÿ�ε����������¼�¼
            in<<tmp<<"\n";
            in<<"ҳ֡��"<<"   "<<"ҳ֡����"<<"  "<<"ʱ��"<<endl;
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
        //������ݼ�¼�ļ�
        QFile outfile3("virtualcontent_logfile.txt");
        QTextStream in(&outfile3);
        //ofstream outfile3("virtualcontent_logfile.txt",ios::app);//���������־�ļ�
        if(!outfile3.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //���μ�¼���ÿ��֡��֡�źʹ洢����
            //ÿ�ε����������¼�¼
            in<<tmp<<"\n";
            in<<"ҳ֡��"<<"   "<<"ҳ֡�洢����"<<endl;
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

        //�ڴ浱ǰ���ݼ�¼�ļ�
        //ÿ�ε������ԭ�����ݣ��������ݸ���
        FILE *fp1;
        fp1=fopen("memeory_instant.txt","w");
        fclose(fp1);

        //ofstream outfile5("memeory_instant.txt",ios::app);//��ʱ�ڴ���״̬��
        QFile outfile5("virtualcontent_logfile.txt");
        QTextStream in(&outfile5);
        if(!outfile5.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //cout<<"5.Memory_Dialog out"<<endl;
            //���μ�¼�ڴ�ÿ��֡��֡�źʹ洢����
            in<<tmp<<endl;
            in<<"֡��"<<"    "<<"����"<<"\n";
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
        //��浱ǰ���ݼ�¼�ļ�
        //ÿ�ε������ԭ�����ݣ��������ݸ���
        FILE *fp;
        fp=fopen("virtual_instant.txt","w");
        fclose(fp);

        QFile outfile4("virtual_instant.txt");
        QTextStream in(&outfile4);
        //ofstream outfile4("virtual_instant.txt",ios::app);//��ʱ���״̬�ļ�
        if(!outfile4.open(QIODevice::ReadWrite)){
            cout << "Open the file failure...\n";
        }
        else
        {
            //cout<<"6.Memory_Dialog out"<<endl;
            //���μ�¼���ÿ��֡��֡�źʹ洢����
           in<<tmp<<"\n";
            in<<"ҳ֡��"<<"   "<<"ҳ֡�洢����"<<endl;
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
        //�ڴ��ʼ����������м�¼�ļ�
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
	cout<<"���뺯��--"<<"Memory_share::read()"<<endl;
    int s=0;
    int a=0;
    //�����ṩ��PCBID�鿴�Ƿ�������ڴ���
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
        //����������ݵĽ���λ��
        int end=start+length-1;
        //1Ϊҳ������ҳ���Ӧ�ڴ棬2Ϊ�ڴ棬�����ڴ�����
        if(type1==1)
            return replace(onebyone[s][1],start,end);
        else if(type1==2)
            return replace(onebyone[s][2],start,end);
    }


}




//���ʺ����������߼���ַ����ʼ�ͽ���λ�÷����߼���ַָ������ݴ���ʼ��ʼ������λ�ý���������
string Memory_share::replace(int pcblogicaddress,int start ,int end)
{
	cout<<"���뺯��--"<<"Memory_share::replace"<<endl;
    int a,b,c,logicaddress1;
    a=0;
    c=0;
    logicaddress1= pcblogicaddress;
    
    //�����߼���ַָ�����ݵ��ܳ���
    while(c==0)
    {
        for(b=0;b<room_physicframe;b++)
        {
            if(frame[logicaddress1][b]!=-1) //�ڴ�ÿ��֡��״̬���� 
            {
                if(state_physicframe[frame[logicaddress1][b]]==2) //�洢���� 
                {
                    a=a+realframe[frame[logicaddress1][b]].size(); //֡�ڴ洢�����ݴ�С 
                }
                else if(state_physicframe[frame[logicaddress1][b]]==1) //�洢ҳ�� 
                {
                    logicaddress1=frame[logicaddress1][b]; //�ļ����ƿ����߼���ַ��Ӧ��֡ 
                }
            }
            else //�����쳣֡������֡��Ϊ�գ� 
            {
                c=1;
                break;
            }
        }

    }
    //�������λ�ô��������ܳ��ȣ��򷵻�һ�����ַ�����ʾ����Խ��
    if(a<=end)
    {
        string a="";
        cout << "����Խ�磡";
        return a;
    }
    //�������ݴ��ڵ�����£�����ִ��
    else
    {
        int p,q,p1,q1;
		int logicaddress,length,length1,pagenumber,pagenumber1,numinpage,numinpage1,framenum;

        logicaddress= pcblogicaddress; //������ƿ��߼���ַ 

        p=start/room_physicframe; //������������ݿ�ʼλ���ڴ��߼���ַ��ʼ�ĵڼ���ҳ֡�� 
        q=start%room_physicframe; //����������ݿ�ʼλ���ڸ�ҳ֡�ڵ���һ��λ�� 
        p1=end/room_physicframe; //������������ݽ���λ���ڴ��߼���ַ��ʼ�ĵڼ���ҳ֡�� 
        q1=end%room_physicframe; //����������ݽ���λ���ڸ�ҳ֡�ڵ���һ��λ�� 

        //��ʼλ�úͽ���λ�ö���ͬһ��֡��
        if(p==p1)
        {
            //ȡ������Ĳ���Ƿ��ʵ����ݳ���
            length=q1-q+1;
            
            pagenumber=p/(room_physicframe-1); //ȡ��������ʼ�ͽ���λ������֡�ڵڼ����洢ҳ���֡��
            numinpage=p%(room_physicframe-1); //ȡ�����������ʼ�ͽ���λ������֡�ڸ�ҳ��ĵڼ���λ��
            
            //Ŀ��֡��������ҳ��������λ
            if(numinpage<=1)
            {
                //���ڵ�һ���洢ҳ���֡��
                if(pagenumber!=0)
                {
                    //Ѱ�ҵ�����Ŀ��֡��ҳ�����ڵ�֡�����ѵ�ǰ�߼���ַ�޸�Ϊ��֡��֡��
                    pagenumber--;
                    for(int i=0;i<pagenumber;i++)
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                    }
                    //�����ҳ��֡�����λ�洢�����������ݣ��������������֡�������֡

                    if(state_physicframe[frame[logicaddress][room_physicframe-1]]==2)
                    {
                        framenum=frame[logicaddress][room_physicframe-1];
                    }
                    //����洢����ҳ���������������֡������һ���洢ҳ������֡�ĵ�һλ
                    else
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                        framenum=frame[logicaddress][numinpage];
                    }

                }
                //������ڵ�һ���洢ҳ���֡�ڣ���ȡ���ֵ���Ǵ洢�ö���������֡��λ��
                else
                {
                    framenum=frame[logicaddress][numinpage];
                }

            }
            else
            {
                //���Ŀ��֡������ҳ�����λ����ֱ��ͨ��ȡ���ÿ��Լ����ҳ������֡
                //ͨ�������û�ȡ��ҳ���ڵ�λ�ã���ȡ�����������ڵ�֡��֡��
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
            //�˴�p�ض�С��p1
            string buffer="";
            string buffer1="";

            //����ʼλ�úͽ���λ�÷ֱ��ҳ֡��Сȡ�������ȡ������
            //ȡ��������Ϊ�˼���������ڴ��߼���ַ��ʼ�ĵڼ���ҳ֡��
            //ȡ������ʱΪ�˼�������ڸ�ҳ֡�ڵ���һ��λ��
            pagenumber=p/(room_physicframe-1);
            numinpage=p%(room_physicframe-1);
            pagenumber1=p1/(room_physicframe-1);
            numinpage1=p1%(room_physicframe-1);

            //��ʼλ����֡������ȡ
            //������������ʼλ����֡�е����ݳ���
            length=room_physicframe-q+1;
            //Ŀ��֡��������ҳ��������λ
            if(numinpage<=1)
            {//���ڵ�һ��֡��
                if(pagenumber!=0)
                {
                    //Ѱ�ҵ�����Ŀ��֡��ҳ�����ڵ�֡�����ѵ�ǰ�߼���ַ�޸�Ϊ��֡��֡��
                    pagenumber--;
                    for(int i=0;i<pagenumber;i++)
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                    }

                    //�����ҳ��֡�����λ�洢�����������ݣ��������������֡�������֡
                    if(state_physicframe[frame[logicaddress][room_physicframe-1]]==2)
                    {
                        framenum=frame[logicaddress][room_physicframe-1];
                    }
                    //����洢����ҳ���������������֡������һ���洢ҳ������֡�ĵ�һλ
                    else
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                        framenum=frame[logicaddress][numinpage];
                    }

                }
                //������ڵ�һ���洢ҳ���֡�ڣ���ȡ���ֵ���Ǵ洢�ö���������֡��λ��
                else
                {
                    framenum=frame[logicaddress][numinpage];
                }

            }
            else
            {
                //���������ҳ�����λ����ֱ��ͨ��ȡ���Ϳ��Լ����ҳ������֡
                //ͨ�������û�ȡ��ҳ���ڵ�λ�ã���ȡ�����������ڵ�֡��֡��
                for(int i=0;i<pagenumber;i++)
                {
                    logicaddress=frame[logicaddress][room_physicframe-1];
                }
                framenum=frame[logicaddress][numinpage];
            }
            //��¼����ʼλ������֡�з��ʻ�ȡ������
            buffer=Replace_page(framenum,pagereplacetype,q,length);
            ///////////////////////////////////////////////////////////
            //��һ֡
            p++;
            int pagebuff;
            pagebuff=numinpage+1;
            //�����һ֡���ǽ���λ�����ڵ�֡������һ֡������ȫ����ȡ������������ʼλ������ȡ�������ݺ��棬
            //�Դ����ƣ�ֱ���������λ�����ڵ�֡Ϊֹ
            while(p<p1)
            {
                //�������ҳ������֡�����һλ�洢��ҳ��λ�ã�����µ�ǰ�߼���ַ��Ȼ���ٴ��µ�ҳ���ͷ��ʼ����
                if(state_physicframe[frame[logicaddress][pagebuff]]==2)
                {
                    framenum=frame[logicaddress][pagebuff];
                    //����һ֡������ȫ����ȡ������������ʼλ������ȡ�������ݺ���
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
            
            //����λ������֡
            length1=q1+1;
            logicaddress=pcblogicaddress;
            //Ŀ��֡��������ҳ��������λ
            if(numinpage1<=1)
            {
                //���ڵ�һ��֡��
                if(pagenumber1!=0)
                {
                    //Ѱ�ҵ�����Ŀ��֡��ҳ�����ڵ�֡�����ѵ�ǰ�߼���ַ�޸�Ϊ��֡��֡��
                    pagenumber1--;
                    for(int i=0;i<pagenumber1;i++)
                    {
                        logicaddress=frame[logicaddress][room_physicframe-1];
                    }
                    //�����ҳ��֡�����λ�洢�����������ݣ��������������֡�������֡
                    if(state_physicframe[frame[logicaddress][room_physicframe-1]]==2)
                    {
                        framenum=frame[logicaddress][room_physicframe-1];
                    }
                    else
                    {
                        //����洢����ҳ���������������֡������һ���洢ҳ������֡�ĵ�һλ
                        logicaddress=frame[logicaddress][room_physicframe-1];
                        framenum=frame[logicaddress][numinpage1];
                    }
                }
                else
                    //������ڵ�һ���洢ҳ���֡�ڣ���ȡ���ֵ���Ǵ洢�ö���������֡��λ��
                {
                    framenum=frame[logicaddress][numinpage1];
                }
            }
            else
            {
                //���������ҳ�����λ����ֱ��ͨ��ȡ���Ϳ��Լ����ҳ������֡
                //ͨ�������û�ȡ��ҳ���ڵ�λ�ã���ȡ�����������ڵ�֡��֡��
                for(int i=0;i<pagenumber1;i++)
                {
                    logicaddress=frame[logicaddress][room_physicframe-1];
                }
                framenum=frame[logicaddress][numinpage1];
            }

            //��ý���λ������֡�Ĵ�0��ʼ����λ�ý��������ݣ���������֮ǰ��õ����ݺ�
            buffer+= Replace_page(framenum,pagereplacetype,0,length1);
            ////////////////////////////////////////////////////////////////////////////end
        //    cout << "100";
            //��ȫ������������ɺ󷵻ظ�����
            return buffer;

        }

    }

}




//������ҳ���û��㷨��ҳ���ʺ�����typeΪҳ���û��㷨��1ΪFIFO��2ΪLRU
string Memory_share::Replace_page(int number_frame,int type,int start,int length)
{
	cout<<"���뺯��--"<<"Memory_share::Replace_page()"<<endl;
    int a,b,c,m,n,vn;
    b=0;
    //FIFO�û��㷨
    if(type==1)
    {
        //��������������֡�������Ƿ��Ѿ����ڸ���������֡֡��
        for(a=0;a<num_pageframe;a++)
        {
            if(state_pageframe[a][0]==number_frame)
            {
                //����������֡�Ѿ�����������У���¼�����֡��
                vn=a;
                b=1;
                break;
            }
        }
        //����в������Ѹ���������֡�������
        if(b==0)
        {
            //����Ѱ�ҿ����Ƿ��п��е����ҳ֡��ͬʱ�ѷǿ��е�֡�Ĵ���ʱ���һ
            for(a=0;a<num_pageframe;a++)
            {
                if(b==0)
                {
                    //��������ҵ�����ҳ֡����Ѹ���������֡��֡�š�����֡�洢���ݴ�����沢��¼���֡��
                    //���ҰѸ����ҳ֡�Ĵ���ʱ���Ϊ1
                    if(state_pageframe[a][0]==-1)
                    {
                        state_pageframe[a][0]=number_frame;
                        state_pageframe[a][1]=1;
                        v_realframe[a]=realframe[number_frame];
                        vn=a;

                        b=1;
                    }
                    //�������ǿ���֡�Ĵ���ʱ���һ
                    else
                        state_pageframe[a][1]++;

                }
                else
                {
                    if(state_pageframe[a][0]!=-1)
                        state_pageframe[a][1]++;
                }

            }
            //��������޷��ҵ��ǿ���֡
            if(b==0)
            {
                //���������棬�ҵ�����ʱ�����һ��֡

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
                //Ȼ�����¸���������֡�š�����֡�����滻�����֡��ԭ�ȴ洢�����ݲ���¼���֡��
                state_pageframe[n][0]=number_frame;//�µ�֡��
                state_pageframe[n][1]=1;
                v_realframe[n]=realframe[number_frame];
                vn=n;
            }
        }
        //������д����Ѹ���������֡������£������еķǿ�֡����ʱ���һ
        else
        {
            for(a=0;a<num_pageframe;a++)
            {
                if(state_pageframe[a][0]!=-1)
                    state_pageframe[a][1]++;
            }
        }
    }
    //LRUҳ���û��㷨
    else if(type==2)
    {
        //��������������֡�������Ƿ��Ѿ����ڸ���������֡֡��
        for(a=0;a<num_pageframe;a++)
        {
            if(state_pageframe[a][0]==number_frame)
            {
                //����������֡�Ѿ�����������У���¼�����֡��
                //���ҰѸ�֡�Ĵ���ʱ����Ϊ0
                state_pageframe[a][1]=0;
                vn=a;
                b=1;
                //������д����Ѹ���������֡������£������еķǿ�֡����ʱ���һ
                for(c=0;c<num_pageframe;c++)
                {
                    if(state_pageframe[c][0]!=-1)
                        state_pageframe[c][1]++;
                }

                break;
            }
        }
        //����в������Ѹ���������֡�������
        if(b==0)
        {
            //����Ѱ�ҿ����Ƿ��п��е����ҳ֡��ͬʱ�ѷǿ��е�֡�Ĵ���ʱ���һ
            for(a=0;a<num_pageframe;a++)
            {
                if(b==0)
                {
                    if(state_pageframe[a][0]==-1)
                    {
                        //��������ҵ�����ҳ֡����Ѹ���������֡��֡�š�����֡�洢���ݴ�����沢��¼���֡��
                        //���ҰѸ����ҳ֡�Ĵ���ʱ���Ϊ1
                        state_pageframe[a][0]=number_frame;//�µ�֡��
                        state_pageframe[a][1]=1;
                        v_realframe[a]=realframe[number_frame];
                        vn=a;
                        b=1;
                    }

                    else
                        state_pageframe[a][1]++;
                    //�������ǿ���֡�Ĵ���ʱ���һ
                }
                else
                {
                    if(state_pageframe[a][0]!=-1)
                        state_pageframe[a][1]++;
                }

            }
            //��������޷��ҵ��ǿ���֡
            if(b==0)
            {
                //���������棬�ҵ�����ʱ�����һ��֡
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
                //Ȼ�����¸���������֡�š�����֡�����滻�����֡��ԭ�ȴ洢�����ݲ���¼���֡��
                state_pageframe[n][0]=number_frame;//�µ�֡��
                state_pageframe[n][1]=1;
                v_realframe[n]=realframe[number_frame];
                vn=n;
            }

        }
    }

    //���ڴ�ĸı�д�뵽��־�ļ���
    print(3); //���μ�¼���ÿ��֡��֡�źʹ���ʱ�� 
    print(4); //���μ�¼���ÿ��֡��֡�źʹ洢���ݣ�ÿ�ε����������¼�¼ 
    print(6); //���μ�¼���ÿ��֡��֡�źʹ洢���ݣ�ÿ�ε������ԭ�����ݣ��������ݸ��� 
    
    cout<<"�ڴ�������ݣ�" ; 
  	cout<< v_realframe[vn].substr(start,length) <<endl ;  //���֡�д�start��ʼ����Ϊlength���ַ��� 
    return v_realframe[vn].substr(start,length); //���ؼ�¼�����ҳ֡�д�start��ʼ����Ϊlength���ַ���

}



//�ڴ����ƴ洢
int Memory_share::Store(string code) //�洢�ṩ���ַ����������߼���ַ
{
	cout<<"���뺯��--"<<"Memory_share::Store()"<<endl;
    //����Ļ��ȡ�����ݶ�ת�����ַ�����ʽ
    string pcbcode=code;
    //�ַ�������
    int length_code=pcbcode.size();
    //�ö���������Ҫ��ҳ����
    int length_page;
    length_page=length_code/room_physicframe;

    //�޷������������ҳ���ȼ�һ�����һ֡������ڲ���Ƭ
    if((length_code)%room_physicframe!= 0)
        length_page++;

    //��Ҫ������Ÿö�ҳ������֡������
    int num_frame;
    
    if(length_page==0) //��ҳ����Ϊ0ʱ��ҳ��ռ��֡��Ϊ0
        num_frame=0;
    else if(length_page==1) //ҳ����Ϊ1��ҳ��ռ��֡����Ϊ1
        num_frame=1;
        
    /*��ҳ���ȴ���1��ʱ��
      ͨ��ҳ���Ⱥ�֡��С��һ�ĳ�ȡ������ȡҳ��ռ�ݵ���֡��
      ��֡��С��һȡ����ԭ���ǵ�ҳ����Ҫռ�ݶ��֡��ʱ�����һλΪ����λ */ 
    else if((length_page%(room_physicframe-1))<2) //������Ϊ0����1��ʱ�����һ��ҳ���֡�����һλ���������λ������Ҫ����������һ����ҳ���֡
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else //��ҳ���Ⱥ�֡��С��һȡ��������1��ʱ����Ҫ���һ��ҳ�����洢
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }
    
    //���嵱ǰҳ������֡�ĵ�ַ�͵�һ��ҳ������֡�ĵ�ַ��Ҳ�����߼���ַ
    int logic_address,first_address;
    
    if(length_page<=(free_physicframe-num_frame)) //����֡���㹻�洢����һ������
    {
        int a=0;
        
        while(state_physicframe[a]!=0) //�������ÿ��֡��״̬����Ѱ��һ������֡
            a++;
        
        state_physicframe[a]=1; //�ҵ���һ������֡������һ������֡��״̬�ɿ���ת��Ϊ�洢ҳ��
								//����״̬Ϊ0���洢ҳ��״̬Ϊ1���洢����״̬Ϊ2 
        free_physicframe--; //����һ������֡����        
        logic_address=a; //��֡��������ݵ��߼���ַ
        first_address=a; //Ҳ���׵�ַ 

        int i,m,n,start,length;
        n=0;
        start=0; //��ҳ��ĵ�0λ��ʼ�����ö��ַ�����������֡��С��Ϊ���ֱ�洢������֡
        length=0;
        
        if((length_code-start)>room_physicframe) //��һ������֡���㹻�洢����ҳ�������£��зֳ���Ϊ����֡��С
        {
            length=room_physicframe;
        }
        else
        {
            
            length=length_code-start; //����㹻�洢���򳤶�Ϊ�ַ�������
        }
        for(m=0;m<num_frame;m++) //ÿҳ��Ҫ֡�� 
        {
            for(i=0;i<room_physicframe-1;i++) //����֡�ռ��С 
            {
                if(n<length_page)
                {
                    while(state_physicframe[a]!=0) //���ַ�������֡��С�ֿ������Ϊ��Ѱ�ҿ���״̬��֡
                        a++;
                    frame[logic_address][i]=a; //�������ݴ洢����֡����ҳ��״̬֡�ĵڶ�ά��Ӧλ�õ����ݸ�Ϊ�洢��֡��֡�� 
                    state_physicframe[a]=2; //�ѿ���״̬��Ϊ�洢����  
                    free_physicframe--;
                    n++;
                    
                    //������ǰ������ĸ�֡�洢���ݵĳ������и��ַ������������и����ʼλ��
                    realframe[a]=pcbcode.substr(start,length);
                    
                    start=start+room_physicframe;
                    //������ʼλ��֮���ٴμ�����һ��֡�ڴ洢�����ݵĳ���
                    
                    if((length_code-start)>room_physicframe) //��һ������֡���㹻�洢����ҳ�������£��зֳ���Ϊ����֡��С
                    {
                        length=room_physicframe;
                    }
                    else //����㹻�洢���򳤶�Ϊ�ַ�������
                    {
                        length=length_code-start;
                    }
                }
                
                else //��ҳ֡��ȫ�洢�ɹ�������£�����ѭ��
                    break;
            }
            
            //��ʣ�������ֻ�����һ֡���Դ洢�������
            if((length_page-n)==1)
            {
                //Ѱ�ҿ���֡
                while(state_physicframe[a]!=0)
                    a++;

                //���洢ҳ�������֡�����һλָ��ÿ���֡���ÿ���֡ͬ���洢���ݣ����޸�״̬��ͬ�ϣ� 
                frame[logic_address][i]=a;
                state_physicframe[a]=2;
                free_physicframe--;
                n++;
                
                realframe[a]=pcbcode.substr(start,length); //�и�ʵ�ʴ洢���ݣ��������ڴ�                
                start=start+room_physicframe; //������ʼλ�úͳ���
                
                if((length_code-start)>room_physicframe)
                {
                    length=room_physicframe;
                }
                else
                {
                    length=length_code-start;
                }
                
                //��ҳ���֡�ڴ洢�����ݣ��������֣�ָ��洢���ݵ�֡��֡�ţ�����ת����ַ���������
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
                        //����������Щָ��֡������
                        pnn+=buff;

                    }
                    else
                        break;
                }
                
                //���ȫ��������֮�����Ӷ��ɵ��ַ������Ǵ洢ҳ�������֡�ڴ洢������
                realframe[logic_address]=pnn;
                break;
            }
            
            //���ڴ洢ҳ�������֡�ĵ����ڶ�λ���ȫ���洢�������
            else if((length_page-n)==0)
            {
                //�������Ӵ洢ҳ���֡�ڴ洢��ָ��洢���ݵ�֡��֡�ţ���ת����ַ���
                //�������֮��洢���洢ҳ���֡������
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
            //�ڵ�������֡�޷��洢����ҳ�������£������д洢ҳ���֡�����һλָ��һ������֡
            //�ÿ���֡��Ϊ�����洢ʣ���ҳ���ÿ���֡��״̬����ı�Ϊ�洢ҳ��
            else
            {
                while(state_physicframe[a]!=0)
                    a++;

                //����ǰ�洢ҳ�������֡�����һλָ��ÿ���֡����ʹ�øÿ���֡������¼ҳ��
                //��֡״̬Ϊ��¼ҳ����¼ֵΪ1��������һλ����֡����
                frame[logic_address][i]=a;
                state_physicframe[a]=1;//��һ���߼���ַ

                //�����д洢ҳ���֡��״̬������Ӳ�ת�����ַ����洢����֡������
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
                //�ı����д洢ҳ���֮֡�󣬼���һλ����֡
                realframe[logic_address]=pnn;
                free_physicframe--;
                logic_address=a;
            }

        }
        //���ڴ�ĸı�����������־�ļ�
        print(1); //��¼ÿ��֡��״̬ 
        print(2); //��¼ÿ��֡�洢������ 
        print(5); //��¼�ڴ浱ǰ���� 
        //���ⷵ������ַ������ڴ��д洢���߼���ַ
        return first_address;
    }
    else //�洢��������������֡�����ڿ���֡��������£�����-1��ʾ�洢ʧ��
        return -1;

}

//�ڴ�����жϺ���
bool Memory_share::Storejudge(string code)
{
	cout<<"���뺯��--"<<"Memory_share::Storejudge()"<<endl;
	
    //������ת�����ַ�������ȡ�ַ�������
    string pcbcode=code;
    int length_code=pcbcode.size();

    //��pcb��ҳ���ȣ������ַ������Ⱥ�����֡��С�����
    int length_page;
    length_page=length_code/room_physicframe;
    //ȡ�಻Ϊ0����ҳ����Ҫ����һҳ�����һҳ�����ڲ���Ƭ
    if((length_code)%room_physicframe!= 0)
        length_page++;

    //��Ҫ�������ҳ�����֡��
    int num_frame;
    //ҳ����Ϊ0����洢ҳ���֡������ҲΪ0��ҳ����Ϊ1����洢ҳ���֡����Ϊ1
    if(length_page==0)
        num_frame=0;
    else if(length_page==1)
        num_frame=1;
    //������ҳ����ȡ������֡��С��һ����ȡ�洢ҳ���֡������
    //ȡ���������1���������Ҫ��һ��֡���洢ҳ����֡Ҳ�����ڲ���Ƭ
    else if((length_page%(room_physicframe-1))<2)
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }

    //�ڿ���֡�����㹻������洢���ݵ�֡�ʹ洢ҳ���֡������£�����true�����򷵻�false
    if(length_page<=(free_physicframe-num_frame-1))
    {
        return true;
    }
    else
        return false;

}

int Memory_share::num_Storejudge(string code)
{
	cout<<"���뺯��--"<<"Memory_share::num_Storejudge()"<<endl;
    //������ת�����ַ�������ȡ�ַ�������
    string pcbcode=code;
    int length_code=pcbcode.size();//���볤��
    //��pcb��ҳ���ȣ������ַ������Ⱥ�����֡��С�����
    int length_page;
    length_page=length_code/room_physicframe;
    //ȡ�಻Ϊ0����ҳ����Ҫ����һҳ�����һҳ�����ڲ���Ƭ
    if((length_code)%room_physicframe!= 0)
        length_page++;

    //��Ҫ�������ҳ�����֡��
    int num_frame;
    //ҳ����Ϊ0����洢ҳ���֡������ҲΪ0��ҳ����Ϊ1����洢ҳ���֡����Ϊ1
    if(length_page==0)
        num_frame=0;
    else if(length_page==1)
        num_frame=1;
    //������ҳ����ȡ������֡��С��һ����ȡ�洢ҳ���֡������
    //ȡ���������1���������Ҫ��һ��֡���洢ҳ����֡Ҳ�����ڲ���Ƭ
    else if((length_page%(room_physicframe-1))<2)
    {
        num_frame=length_page/(room_physicframe-1);
    }
    else
    {
        num_frame=length_page/(room_physicframe-1)+1;
    }

    //������֡��
    return (num_frame+length_page);

}



//PCB��ʼ�������ڴ���䣬idΪPCBID��codeΪ��PCB�Ĵ����
int  Memory_share::storein(int id,string code)
{
	cout<<"���뺯��--"<<"Memory_share::storein()"<<endl;
    int i;
    string buffer;
    
    //Ϊ���ݶ�Ԥ����40�ֽڵĿռ䣬ÿ�������õ���������Ϊ20�ֽڣ�Ϊ���������ж���׼��
  //  buffer="****************************************";  //!!!!!!!!!!!!!
    
    //�������κ�Ԥ�����ݶ��ܹ���Ҫ��ҳ֡�������ж��ܷ�Ϊ��PCB�����ڴ�ռ�
    int all=num_Storejudge(code)+num_Storejudge(buffer);

    //����֡�����㹻Ϊ��PCB�����ڴ������£������ڴ沢����2�����򷵻�0��ʾ�����ڴ�ʧ��
    if(free_physicframe>=all)
    {
        //��onebyone������Ѱ��һ����λ
        for(i=0;i<number_physicframe;i++)
        {
            if(onebyone[i][0]==-1)
              break;
        }

        //��һλ��¼PCB��
        onebyone[i][0]=id;
        //�ڶ�λ��¼ҳ���߼���ַ
        onebyone[i][1]=Store(buffer);
        //����λ��¼���ݶ��߼���ַ
        onebyone[i][2]=Store(code);
        return 1;
    }
    else
    {
        return 0;
    }

}


//��ȡ�����ڴ�����
int Memory_share::getroom_physicmemory()
{
    return room_physicmemory;
}

//��ȡ��ʹ�õ������ڴ��С
int Memory_share::getusedroom_physicframe()
{
    int a;
    //���㷽��Ϊ�ܴ�С��ȥ����֡��Ŀ��֡��С�ĳ˻�
    a=room_physicmemory-free_physicframe*room_physicframe;
    return a;
}


