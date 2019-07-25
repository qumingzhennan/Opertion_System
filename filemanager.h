#pragma once
#include "head.h"
#include "OpenedFile.h"
using namespace std;
class FileManager
{
private:
    static FileManager *incetence;
    //磁盘管理器
    DiskManager *dm;
    //打开文件列表
    OpenedFile *openedFileList;

    //根路径（已设定）
    const string home_path = "D:";
    //原先的路径（仅剪切和复制需要用到）
    string last_path = "/";
    string copyOrcut_name = "";
    //当前路径
    //string present_path = "/";
    //根据文件在打开文件链表中的序号获得该文件的File对象
    //若number超过打开文件链表的最大序号则返回NULL
    File* getFileFromOpenFileList(int number);


    //记录发生剪切/粘贴操作
    int flag;
    //文件管理初始化
    FileManager();
    ~FileManager();
public:

    string present_path = "/";
    FCB* FindFCB(string full_path);
    //查看当前路径下名称为file_name的文件是否被打开，若已被打开则返回true,否则返回false(改)
    bool isFileOpened(string file_path,string file_name);

    void setFlagCopy(string file_name){flag = 0;setLastPath();copyOrcut_name=file_name;}
    void setFlagCut(string file_name){flag = 1;setLastPath();copyOrcut_name=file_name;}
    int copyOrCut(string file_name);
    //返回磁盘管理器
    DiskManager *getDm(){return dm;}
    static FileManager *getIncetence();
    string getHomePath(){ return home_path; }
    //获取当前路径
    string getPresentPath(){ return present_path; }

    //显示当前打开文件链表中的所有信息，输出到屏幕上
    void describeOpenFileList();
    //bool deleleFileinOpenedFile(string filename);
    //设置当前路径（进入子路径[..]/返回上一路径），若设置成功则返回true
    //若required_path的某一路径不存在，则当前路径不变，返回false
    //若当前路径已经是根路径，且required_path为..，则当前路径不变，返回true
    bool setPresentPath(string required_path);
    //在当前路径下创建名称为directory_name的文件夹，若创建成功则返回true
    //若当前路径下已存在名称为directory_name的文件夹，则不做任何操作并返回false
    //在实际磁盘的当前路径下创建文件夹
    int createDirectory(string directory_path,int file_type,int file_right);
    //在当前路径下创建名称为file_name的文件，若创建成功则返回true
    //若当前路径下已存在名称为file_name的文件，则不做任何操作并返回false
    //文件的访问权限默认为“可读可写”
    bool createFile(string file_name);
    //打开当前路径下名称为file_name的文件，若打开成功则输出该文件的内容、创建并返回该文件的File对象
    //将该文件File对象加入到openedFileList中
    //若当前路径下不存在名称为file_name的文件，则不做任何操作并返回NULL
    bool openFile(string name);
    //调用getFileFromOpenFileList方法，获得file文件
    //修改file文件，若修改成功则返回true
    //若file文件不存在，则不做任何操作并返回false
    //修改文件目前只支持对文件的尾插入（即新开启一行将用户的输入插入到文件尾）
    //在该方法中用户输入要插入的值，调用File对象的insertToFile方法
    //bool updateFile(int number);
    //调用getFileFromOpenFileList方法，获得file文件
    //关闭file文件，将该文件从打开文件列表中移除，若关闭成功则返回true
    //若file文件不存在，则不做任何操作并返回false
    bool closeFile(string path);
    //删除当前路径下名称为file_name的文件
    //若删除成功则返回true
    //调用isFileOpened方法，判断该文件是否被打开，若已被打开则输出提示并返回false
    //若该文件不存在，则输出提示并返回false
    bool deleteDirectory(string full_path);
    //显示被打开文件内容
    string showContent(string full_path);
    //显示当前已被打开的文件
    void showOpenedFile();
    //重命名文件或文件夹
    //输入：full_name为当前路径下被需要修改的文件夹或文件，new_name为目标名称
    bool rename(string full_name, string new_name);
    //将文件夹或文件复制到目标文件夹
    int copy(string full_name);
    //将文件夹或文件移动到目标文件夹 剪切操作
    int cut(string full_name);
    //获取当前路径，为复制/移动做准备
    void setLastPath();
    //获取要修改的文件的名称
    string getcopyOrcut_name(){return copyOrcut_name;}

    //修改文件信息
    bool changeContent(string full_path, string content);
    //获取当前路径下的子路径
    vector<string> getFileAndDirectory(string full_path);
    //剪掉字符串首尾空格
    string cutSpace(string name);


};
