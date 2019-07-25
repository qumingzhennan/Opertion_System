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

//const int FCBCOUNT = BLOCKSIZE/sizeof(FCB);	//计算单页可以存储的FCB个数
//加一个文件控制PCB，储存当前的文件信息，时间，大小，类型，修改权限

class File
{
private:

    string path;
    string name;
public:
    //FCB fcb;

    //创建文件对象

    File(string path, string name);
    //读取文件路径
    string getPath(){ return path; }
    //读取文件名字
    string getName(){ return name; }
    //将insert_content的字符串数组插入到该文件尾，每个string占一行，若成功则返回true
    //若插入失败则返回false
//	bool insertToFile(string* insert_content, int content_size);

    File();
    ~File();
};
