#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H
#include"head.h"
class file_control
{
public:
    file_control();
    void change_cur_address();
    void show_cur_folder();//列出当前文件夹的目录信息
    void new_file_folder(int temp_right,string filename);//在当前目录下新建文件夹
    void del_file(string filename);//在当前目录下删除文件夹/文件
    void new_file(int temp_right,string filename);//在当前目录下创建文件
    bool close_file(string filename,string text);//关闭文件
    void save_file(string filename,string text);//保存文件（对已打开文件进行操作）
    string open_file(string filename);//在当前目录下打开文件
    void cut_file(string filename);//剪切
    void copy_file(string filename);//复制
    void paste_file(string filename);//粘贴
    void rename(string filename,string re_name);//重命名
    FileManager *fm = FileManager::getIncetence();
    DiskManager *dm = DiskManager::getIncetence();
    string filename;
    string lastpath="/";
    string home="/";
    string temppath="/";
    string curpath="/";
    int count;//
    FCB* file_fcb[20];
};

#endif // FILE_CONTROL_H
