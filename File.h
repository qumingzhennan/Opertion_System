#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

/*typedef class Adress{
    public:
        int first;
        Adress* next;

        Adress(){}
        ~Adress(){}
}* AdressP;
*/

//const int FCBCOUNT = BLOCKSIZE/sizeof(FCB);	//���㵥ҳ���Դ洢��FCB����
//��һ���ļ�����PCB�����浱ǰ���ļ���Ϣ��ʱ�䣬��С�����ͣ��޸�Ȩ��

class File
{
private:

    string path;
    string name;
public:
    //FCB fcb;

    //�����ļ�����

    File(string path, string name);
    //��ȡ�ļ�·��
    string getPath(){ return path; }
    //��ȡ�ļ�����
    string getName(){ return name; }
    //��insert_content���ַ���������뵽���ļ�β��ÿ��stringռһ�У����ɹ��򷵻�true
    //������ʧ���򷵻�false
//	bool insertToFile(string* insert_content, int content_size);

    File();
    ~File();
};
