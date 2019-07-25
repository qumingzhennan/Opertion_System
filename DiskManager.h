#pragma once
#include"head.h"
using namespace std;
typedef struct FCB
{
    char name[25];
    int size;
    int right;
    int type;//0表示目录,1表示数据文件
    char time[25];//创建时间
}FCB;
typedef struct directoryNode directoryNode;
typedef directoryNode* directoryList;
class DiskManager
{
private:
    //磁盘中目录信息所在位置
    const int DIRECTORY_INFO =0;
    //磁盘中文件索引所在位置
    const int FILE_INDEX = 1;
    //磁盘中位图所在位置
    const int BIT_MAP = 2;

//	const int FCB_LOC=3;

    //磁盘中文件内容开始位置
    const int FILE_CONTENT = 3;

    //常量：磁盘大小  单位为B
    const int DISK_SIZE = 1024 * 1024;
    //常量：块的大小  单位为B
    const int BLOCK_SIZE = 128;
    //常量：块的数量
    const int BLOCK_NUM = DISK_SIZE / BLOCK_SIZE;
    //常量：磁盘路径
    const string DISK = "Disk.txt";
    //目录链表的头节点
    directoryList directory_list;


    static DiskManager *incetence;

    vector<FCB*> fcbInfo;

    //清空目录链表（只留根节点）
    void clearDirectory();

    void readFCB();

    //从磁盘中读取字符串按行存入vector
    vector<string> readStream();

    vector<string> readPCBStream();
    //将vector中字符串按行输出到磁盘中



    void writeStream(vector<string> buffer);

    string getTime(void);



    void Strcpy(char *p2,string p1);

    //string类型到int类型转换


    int stringToInt(string temp);
    //int类型到string类型转换
    string intToString(int temp);

    //根据输入节点获取全路径
    string getFullPathByNode(directoryList node);

    //判断两文件是否重名
    bool isSameName(string name1, string name2);
    //判断子路径是文件还是目录，若为目录返回false
    bool isFile(string name);
    //获取目录名（去掉/的名字）
    string getDirectorySelfName(string name);
    //将磁盘中的目录信息读取到目录链表中
    void loadDirectoryToList();
    //将目录链表中输出到磁盘中
    void loadDirectoryToDisk();
    //根据全路径创建文件
    void createFileInDisk(string full_name);
    //删除文件
    int deleteFile(string input_path);
    //删除目录
    int deleteDirectory(string input_path);
    //磁盘管理的构造函数  根据磁盘内容初始化目录链表
    DiskManager();
    //磁盘管理的析构函数  释放目录链表的空间
    ~DiskManager();
public:

    //当前路径的目录节点
    directoryList current_path;

    //根据输入的路径（相对/绝对）得到目标绝对路径，若路径不存在则返回NULL
   string getFullPathByInput(string input_path);
    /*
    功能：判断input_path路径是否存在
    输入：绝对路径或相对路径
    输出：该路径存在--true  该路径不存在--false
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
    功能：获取input_path路径下的所有文件和子目录名称
    输入：绝对路径或相对路径
    输出：该路径下的所有文件和子目录的名称  若输入路径不存在返回大小为0的vector（与该路径存在且无子目录与文件的返回值相同）
    注：调用该函数之前建议调用isInputPathExisting(string)函数判断该路径是否存在
    */




    vector<string> getAllFileOrDirectoryOfPath(string input_path);
    /*
    功能：获取当前路径的绝对路径
    输入：void
    输出：当前路径current_path的绝对路径
    */



    //根据输入的路径（相对/绝对）得到对应的节点，若路径不存在的返回NULL
    directoryList getNodeByInput(string input_path);


    string getCurrentPath();
    /*
    功能：更改当前路径current_path
    输入：绝对路径或相对路径
    输出：更改成功--true  输入路径不存在--false
    */
    bool changeCurrentPath(string input_path);
    /*
    功能：读取文件内容
    输入：文件的绝对路径或相对路径
    输出：文件的内容  路径不存在--""（与该路径存在且文件内容为空的返回值相同）
    注：调用该函数之前建议调用isInputPathExisting(string)函数判断该路径是否存在
    */
    string readFileContent(string input_path);
    /*
    功能：新建文件或文件夹
    输入：父目录的绝对路径或相对路径，新建项的名称
    输出：该目录下存在与该文件重名的文件或目录--SAME_NAME_EXISTS_PROBLEM  创建成功--SUCCESS
    路径不存在--TARGET_NOT_EXIST_PROBLEM  超出磁盘容量--OUT_OF_DISK_CPACITY
    */
    int insertIntoDisk(string father_path, string name);
    /*
    功能：修改文件内容
    输入：文件的绝对路径或相对路径，文件修改后内容
    输出：修改成功--SUCCESS  超出磁盘容量--OUT_OF_DISK_CPACITY  路径不存在--OUT_OF_DISK_CPACITY
    权限问题--RIGHT_PROBLEM
    */
    int updateFile(string input_path, string content);
    /*
    功能：删除文件或目录
    输入：文件目录的绝对路径或相对路径
    输出：删除成功--SUCCESS 权限问题--RIGHT_PROBLEM 目录下存在文件或子目录--FILES_EXIST_PROBLEM
    文件或目录不存在--TARGET_NOT_EXIST_PROBLEM
    */
    int deleteFileOrDirectory(string input_path);
    /*
    功能：复制文件或目录
    输入：源目录的绝对路径或相对路径，目的目录的绝对路径或相对路径
    输出：复制成功--SUCCESS 权限问题--RIGHT_PROBLEM 超出磁盘容量--OUT_OF_DISK_CPACITY
    目录不存在--TARGET_NOT_EXIST_PROBLEM
    */
    int copyFileOrDirectory(string source_path, string target_path);
    /*
    功能：剪切文件或目录
    输入：源目录的绝对路径或相对路径，目的目录的绝对路径或相对路径
    输出：剪切成功--SUCCESS 权限问题--RIGHT_PROBLEM 超出磁盘容量--OUT_OF_DISK_CPACITY
    目录不存在--TARGET_NOT_EXIST_PROBLEM
    */
    int cutFileOrDirectory(string source_path, string target_path);
    /*
    功能：重命名文件或目录
    输入：目录的绝对路径或相对路径，目标名称
    输出：重命名成功--SUCCESS 权限问题--RIGHT_PROBLEM 超出磁盘容量--OUT_OF_DISK_CPACITY
    目录不存在--TARGET_NOT_EXIST_PROBLEM  目录重名--SAME_NAME_EXISTS_PROBLEM
    目标名称必须合法，最后没有/（只输入目录的实际名称）
    */
    int renameFileOrDirectory(string input_path, string target_name);
};

