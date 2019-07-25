#pragma once
#include "head.h"
#include "OpenedFile.h"
using namespace std;
class FileManager
{
private:
    static FileManager *incetence;
    //���̹�����
    DiskManager *dm;
    //���ļ��б�
    OpenedFile *openedFileList;

    //��·�������趨��
    const string home_path = "D:";
    //ԭ�ȵ�·���������к͸�����Ҫ�õ���
    string last_path = "/";
    string copyOrcut_name = "";
    //��ǰ·��
    //string present_path = "/";
    //�����ļ��ڴ��ļ������е���Ż�ø��ļ���File����
    //��number�������ļ�������������򷵻�NULL
    File* getFileFromOpenFileList(int number);


    //��¼��������/ճ������
    int flag;
    //�ļ������ʼ��
    FileManager();
    ~FileManager();
public:

    string present_path = "/";
    FCB* FindFCB(string full_path);
    //�鿴��ǰ·��������Ϊfile_name���ļ��Ƿ񱻴򿪣����ѱ����򷵻�true,���򷵻�false(��)
    bool isFileOpened(string file_path,string file_name);

    void setFlagCopy(string file_name){flag = 0;setLastPath();copyOrcut_name=file_name;}
    void setFlagCut(string file_name){flag = 1;setLastPath();copyOrcut_name=file_name;}
    int copyOrCut(string file_name);
    //���ش��̹�����
    DiskManager *getDm(){return dm;}
    static FileManager *getIncetence();
    string getHomePath(){ return home_path; }
    //��ȡ��ǰ·��
    string getPresentPath(){ return present_path; }

    //��ʾ��ǰ���ļ������е�������Ϣ���������Ļ��
    void describeOpenFileList();
    //bool deleleFileinOpenedFile(string filename);
    //���õ�ǰ·����������·��[..]/������һ·�����������óɹ��򷵻�true
    //��required_path��ĳһ·�������ڣ���ǰ·�����䣬����false
    //����ǰ·���Ѿ��Ǹ�·������required_pathΪ..����ǰ·�����䣬����true
    bool setPresentPath(string required_path);
    //�ڵ�ǰ·���´�������Ϊdirectory_name���ļ��У��������ɹ��򷵻�true
    //����ǰ·�����Ѵ�������Ϊdirectory_name���ļ��У������κβ���������false
    //��ʵ�ʴ��̵ĵ�ǰ·���´����ļ���
    int createDirectory(string directory_path,int file_type,int file_right);
    //�ڵ�ǰ·���´�������Ϊfile_name���ļ����������ɹ��򷵻�true
    //����ǰ·�����Ѵ�������Ϊfile_name���ļ��������κβ���������false
    //�ļ��ķ���Ȩ��Ĭ��Ϊ���ɶ���д��
    bool createFile(string file_name);
    //�򿪵�ǰ·��������Ϊfile_name���ļ������򿪳ɹ���������ļ������ݡ����������ظ��ļ���File����
    //�����ļ�File������뵽openedFileList��
    //����ǰ·���²���������Ϊfile_name���ļ��������κβ���������NULL
    bool openFile(string name);
    //����getFileFromOpenFileList���������file�ļ�
    //�޸�file�ļ������޸ĳɹ��򷵻�true
    //��file�ļ������ڣ������κβ���������false
    //�޸��ļ�Ŀǰֻ֧�ֶ��ļ���β���루���¿���һ�н��û���������뵽�ļ�β��
    //�ڸ÷������û�����Ҫ�����ֵ������File�����insertToFile����
    //bool updateFile(int number);
    //����getFileFromOpenFileList���������file�ļ�
    //�ر�file�ļ��������ļ��Ӵ��ļ��б����Ƴ������رճɹ��򷵻�true
    //��file�ļ������ڣ������κβ���������false
    bool closeFile(string path);
    //ɾ����ǰ·��������Ϊfile_name���ļ�
    //��ɾ���ɹ��򷵻�true
    //����isFileOpened�������жϸ��ļ��Ƿ񱻴򿪣����ѱ����������ʾ������false
    //�����ļ������ڣ��������ʾ������false
    bool deleteDirectory(string full_path);
    //��ʾ�����ļ�����
    string showContent(string full_path);
    //��ʾ��ǰ�ѱ��򿪵��ļ�
    void showOpenedFile();
    //�������ļ����ļ���
    //���룺full_nameΪ��ǰ·���±���Ҫ�޸ĵ��ļ��л��ļ���new_nameΪĿ������
    bool rename(string full_name, string new_name);
    //���ļ��л��ļ����Ƶ�Ŀ���ļ���
    int copy(string full_name);
    //���ļ��л��ļ��ƶ���Ŀ���ļ��� ���в���
    int cut(string full_name);
    //��ȡ��ǰ·����Ϊ����/�ƶ���׼��
    void setLastPath();
    //��ȡҪ�޸ĵ��ļ�������
    string getcopyOrcut_name(){return copyOrcut_name;}

    //�޸��ļ���Ϣ
    bool changeContent(string full_path, string content);
    //��ȡ��ǰ·���µ���·��
    vector<string> getFileAndDirectory(string full_path);
    //�����ַ�����β�ո�
    string cutSpace(string name);


};
