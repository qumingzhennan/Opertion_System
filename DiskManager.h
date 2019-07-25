#pragma once
#include"head.h"
using namespace std;
typedef struct FCB
{
    char name[25];
    int size;
    int right;
    int type;//0��ʾĿ¼,1��ʾ�����ļ�
    char time[25];//����ʱ��
}FCB;
typedef struct directoryNode directoryNode;
typedef directoryNode* directoryList;
class DiskManager
{
private:
    //������Ŀ¼��Ϣ����λ��
    const int DIRECTORY_INFO =0;
    //�������ļ���������λ��
    const int FILE_INDEX = 1;
    //������λͼ����λ��
    const int BIT_MAP = 2;

//	const int FCB_LOC=3;

    //�������ļ����ݿ�ʼλ��
    const int FILE_CONTENT = 3;

    //���������̴�С  ��λΪB
    const int DISK_SIZE = 1024 * 1024;
    //��������Ĵ�С  ��λΪB
    const int BLOCK_SIZE = 128;
    //�������������
    const int BLOCK_NUM = DISK_SIZE / BLOCK_SIZE;
    //����������·��
    const string DISK = "Disk.txt";
    //Ŀ¼�����ͷ�ڵ�
    directoryList directory_list;


    static DiskManager *incetence;

    vector<FCB*> fcbInfo;

    //���Ŀ¼����ֻ�����ڵ㣩
    void clearDirectory();

    void readFCB();

    //�Ӵ����ж�ȡ�ַ������д���vector
    vector<string> readStream();

    vector<string> readPCBStream();
    //��vector���ַ������������������



    void writeStream(vector<string> buffer);

    string getTime(void);



    void Strcpy(char *p2,string p1);

    //string���͵�int����ת��


    int stringToInt(string temp);
    //int���͵�string����ת��
    string intToString(int temp);

    //��������ڵ��ȡȫ·��
    string getFullPathByNode(directoryList node);

    //�ж����ļ��Ƿ�����
    bool isSameName(string name1, string name2);
    //�ж���·�����ļ�����Ŀ¼����ΪĿ¼����false
    bool isFile(string name);
    //��ȡĿ¼����ȥ��/�����֣�
    string getDirectorySelfName(string name);
    //�������е�Ŀ¼��Ϣ��ȡ��Ŀ¼������
    void loadDirectoryToList();
    //��Ŀ¼�����������������
    void loadDirectoryToDisk();
    //����ȫ·�������ļ�
    void createFileInDisk(string full_name);
    //ɾ���ļ�
    int deleteFile(string input_path);
    //ɾ��Ŀ¼
    int deleteDirectory(string input_path);
    //���̹���Ĺ��캯��  ���ݴ������ݳ�ʼ��Ŀ¼����
    DiskManager();
    //���̹������������  �ͷ�Ŀ¼����Ŀռ�
    ~DiskManager();
public:

    //��ǰ·����Ŀ¼�ڵ�
    directoryList current_path;

    //���������·�������/���ԣ��õ�Ŀ�����·������·���������򷵻�NULL
   string getFullPathByInput(string input_path);
    /*
    ���ܣ��ж�input_path·���Ƿ����
    ���룺����·�������·��
    �������·������--true  ��·��������--false
    */
    static DiskManager *getIncetence();

    void initialFCB();

    vector<FCB *> getfcbInfo(){ return fcbInfo ;}

    FCB* FindFCBinfo(string full_path);

    bool dele_fCB(string d_name);

    bool add_fCB(string a_name,int a_type,int a_right);

    bool update_fCB(string u_name,int u_size);


    void LoadIntoFCB();


    bool isInputPathExisting(string input_path);
    /*
    ���ܣ���ȡinput_path·���µ������ļ�����Ŀ¼����
    ���룺����·�������·��
    �������·���µ������ļ�����Ŀ¼������  ������·�������ڷ��ش�СΪ0��vector�����·������������Ŀ¼���ļ��ķ���ֵ��ͬ��
    ע�����øú���֮ǰ�������isInputPathExisting(string)�����жϸ�·���Ƿ����
    */




    vector<string> getAllFileOrDirectoryOfPath(string input_path);
    /*
    ���ܣ���ȡ��ǰ·���ľ���·��
    ���룺void
    �������ǰ·��current_path�ľ���·��
    */



    //���������·�������/���ԣ��õ���Ӧ�Ľڵ㣬��·�������ڵķ���NULL
    directoryList getNodeByInput(string input_path);


    string getCurrentPath();
    /*
    ���ܣ����ĵ�ǰ·��current_path
    ���룺����·�������·��
    ��������ĳɹ�--true  ����·��������--false
    */
    bool changeCurrentPath(string input_path);
    /*
    ���ܣ���ȡ�ļ�����
    ���룺�ļ��ľ���·�������·��
    ������ļ�������  ·��������--""�����·���������ļ�����Ϊ�յķ���ֵ��ͬ��
    ע�����øú���֮ǰ�������isInputPathExisting(string)�����жϸ�·���Ƿ����
    */
    string readFileContent(string input_path);
    /*
    ���ܣ��½��ļ����ļ���
    ���룺��Ŀ¼�ľ���·�������·�����½��������
    �������Ŀ¼�´�������ļ��������ļ���Ŀ¼--SAME_NAME_EXISTS_PROBLEM  �����ɹ�--SUCCESS
    ·��������--TARGET_NOT_EXIST_PROBLEM  ������������--OUT_OF_DISK_CPACITY
    */
    int insertIntoDisk(string father_path, string name);
    /*
    ���ܣ��޸��ļ�����
    ���룺�ļ��ľ���·�������·�����ļ��޸ĺ�����
    ������޸ĳɹ�--SUCCESS  ������������--OUT_OF_DISK_CPACITY  ·��������--OUT_OF_DISK_CPACITY
    Ȩ������--RIGHT_PROBLEM
    */
    int updateFile(string input_path, string content);
    /*
    ���ܣ�ɾ���ļ���Ŀ¼
    ���룺�ļ�Ŀ¼�ľ���·�������·��
    �����ɾ���ɹ�--SUCCESS Ȩ������--RIGHT_PROBLEM Ŀ¼�´����ļ�����Ŀ¼--FILES_EXIST_PROBLEM
    �ļ���Ŀ¼������--TARGET_NOT_EXIST_PROBLEM
    */
    int deleteFileOrDirectory(string input_path);
    /*
    ���ܣ������ļ���Ŀ¼
    ���룺ԴĿ¼�ľ���·�������·����Ŀ��Ŀ¼�ľ���·�������·��
    ��������Ƴɹ�--SUCCESS Ȩ������--RIGHT_PROBLEM ������������--OUT_OF_DISK_CPACITY
    Ŀ¼������--TARGET_NOT_EXIST_PROBLEM
    */
    int copyFileOrDirectory(string source_path, string target_path);
    /*
    ���ܣ������ļ���Ŀ¼
    ���룺ԴĿ¼�ľ���·�������·����Ŀ��Ŀ¼�ľ���·�������·��
    ��������гɹ�--SUCCESS Ȩ������--RIGHT_PROBLEM ������������--OUT_OF_DISK_CPACITY
    Ŀ¼������--TARGET_NOT_EXIST_PROBLEM
    */
    int cutFileOrDirectory(string source_path, string target_path);
    /*
    ���ܣ��������ļ���Ŀ¼
    ���룺Ŀ¼�ľ���·�������·����Ŀ������
    ������������ɹ�--SUCCESS Ȩ������--RIGHT_PROBLEM ������������--OUT_OF_DISK_CPACITY
    Ŀ¼������--TARGET_NOT_EXIST_PROBLEM  Ŀ¼����--SAME_NAME_EXISTS_PROBLEM
    Ŀ�����Ʊ���Ϸ������û��/��ֻ����Ŀ¼��ʵ�����ƣ�
    */
    int renameFileOrDirectory(string input_path, string target_name);
};

