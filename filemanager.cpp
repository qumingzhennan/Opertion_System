#pragma once
#include"head.h"
#include<QMessageBox>
FileManager::FileManager()
{
    openedFileList = new OpenedFile();
    dm = DiskManager::getIncetence();
}
FileManager::~FileManager()
{
}

FileManager *FileManager::incetence = new FileManager();

FileManager *FileManager::getIncetence()
{
    if (incetence == NULL)
    {
        incetence = new FileManager();
    }
    return incetence;
}

FCB* FileManager::FindFCB(string full_path){
    return dm->FindFCBinfo(full_path);
}

bool FileManager::isFileOpened(string file_path, string file_name)//前面是完整的路径 后面是文件名
{
    OpenedFile *temp = openedFileList->next;
    //file_path_flag+=file_name;
    //遍历链表中的所有结点，判断该文件是否打开
    while (temp != NULL)
    {
        /*cout<<"(isFileOpened):1.temp->getFile()->getPath()"<<temp->getFile()->getPath()<<endl;
        cout<<"(isFileOpened):2.file_path"<<file_path<<endl;
        cout<<"(isFileOpened):3.temp->getFile()->getName()"<<temp->getFile()->getName()<<endl;
        cout<<"(isFileOpened):4.file_name"<<file_name<<endl;*/

        if (temp->getFile()->getPath()+file_name == file_path && temp->getFile()->getName() == file_name)
        {
            cout << "find" << endl;
            return 1;
        }
        temp = temp->next;
    }
    cout << "not find" << endl;
    return 0;
}

int FileManager::createDirectory(string directory_name,int file_type,int file_right)
{



    //string directory_name = directory_name;

    for (int i = 0; i < directory_name.length(); i++)
    {
        string ch = directory_name.substr(i, 1);
        //遍历是否存在非法字符
        if (ch == "\\" || ch == ":" || ch == "*" || ch == "?" || ch == ">" || ch == "<" || ch == "|" || ch == "\"")
        {
            //cout << "error:there is an invalid character" << endl;
            QMessageBox::about(NULL,"ERROR","there is an invalid character");
            return INVALID_CHARACTER;
        }
    }
    directory_name = cutSpace(directory_name);

    string temp_path = dm->getCurrentPath();
    cout<<"look at this !!!"<<temp_path<<' '<<directory_name;
    int state = dm->insertIntoDisk(temp_path, directory_name);
    if (state == SUCCESS)
    {
    //	cout<<"新建文件夹/文件成功！"<<endl;
         // 	cout<<"写入相关文件FCB"<<endl;
    //	cout<<"栗子要睡觉！！！"<<endl;
        QMessageBox::about(NULL,"Tip","New file/folder successful");
        dm->add_fCB(temp_path+directory_name,file_type,file_right);
    }

    else if (state == SAME_NAME_EXISTS_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","The file/folder exists");
        //cout << "error:the " << directory_name << " exists" << endl;
    }
    else if (state == TARGET_NOT_EXIST_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","target does not exist");
        //cout << "error : target does not exist" << endl;
    }
    else if (state == OUT_OF_DISK_CPACITY_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","no enough hard disk space");
        //cout << "error:no enough hard disk space" << endl;
    }
    else
    {
    }

    return state;
}

bool FileManager::setPresentPath(string required_path)
{
    string require_path = required_path;
    //判断该文件夹是否存在
    if (dm->changeCurrentPath(require_path))
    {
        //更改当前文件夹
        present_path = dm->getCurrentPath();
        return true;
    }
    else
    {
         QMessageBox::about(NULL,"Tip","the directory cannot be found");
        //cout << "error: the directory cannot be found" << endl;
        return false;
    }
}

bool FileManager::openFile(string name)
{
    string file_name = name;

    string file_name_temp = present_path + file_name;
    cout<<endl;
    //cout<<"(BUG is There)::"<<file_name_temp<<endl<<endl;
    //该文件不存在
    if (dm->isInputPathExisting(file_name_temp) == false)//isInputPathExisting  这个函数是输入完整路径
    {
        QMessageBox::about(NULL,"Tip","the file do not exist");
        //cout << "error:the file do not exist" << endl;
        return false;
    }
    //该文件存在
    else
    {
        //判断该文件是否已经被打开
        if (isFileOpened(present_path, file_name))//前面是上一级路径
        {
            return false;
        }
        //文件未被打开
        else
        {
            File *temp_file = new File(present_path, file_name);
            //新建打开链表的结点
            OpenedFile *temp = openedFileList;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            //插入到打开文件链表
            OpenedFile *temp_openedfile = new OpenedFile(temp_file);
            temp->next = temp_openedfile;
            return true;
        }
    }
}

void FileManager::showOpenedFile()
{
    OpenedFile *temp = openedFileList->next;
    //int num = 0;
    while (temp != NULL)
    {
        cout <<"(OPen file :)"<< temp->getFile()->getPath() << temp->getFile()->getName() << endl;
        //num++;
        temp = temp->next;
    }
}


bool FileManager::closeFile(string path)
{
    string file_path = path;
    //cout<<"PATH="<<path<<endl;
    if (file_path == "quit")
    {
        return 0;
    }
    //遍历打开文件链表
    OpenedFile *temp_file = openedFileList->next;
    OpenedFile *last_file = openedFileList;
    string temp;
    while (temp_file != NULL)
    {
        temp = temp_file->getFile()->getPath()+temp_file->getFile()->getName();
        //该文件已被打开
        if(temp == file_path)
        {
            break;
        }
        last_file = temp_file;
        temp_file = temp_file->next;
    }
    if (temp_file == NULL)
    {
        //QMessageBox::about(NULL,"Tip","the file do not open");
        //cout << "error:the file do not open" << endl;
        //return false;
        return true;
    }
    else
    {
        last_file->next = temp_file->next;
        return true;
    }
}

bool FileManager::deleteDirectory(string full_path)
{
    //cout<<full_path<<"full_path"<<endl;
    string full_name = full_path;
    string file_name = full_name;
    //cout<<"present_path"<<present_path<<endl;
    cout<<file_name.substr(file_name.length()) <<endl;
    if (file_name.substr(file_name.length()-1.1) == "/")
    {
        full_name = present_path + full_name;
        int state = dm->deleteFileOrDirectory(full_name);
        if (state == SUCCESS)
        {
            if(dm->dele_fCB(full_name))
                return 1;
            else
                QMessageBox::about(NULL,"Tip","can not delete the fcb");
                //cout << "error:can not delete the fcb " << endl;
                return 0;
        }
        else if (state == RIGHT_PROBLEM)
        {
            QMessageBox::about(NULL,"Tip","error:the operation is denied");
            //cout << "error:the operation is denied" << endl;
            return 0;
        }
        else if (state == TARGET_NOT_EXIST_PROBLEM)
        {
            QMessageBox::about(NULL,"Tip","target does not exist");
            //cout << "error : target does not exist" << endl;
            return 0;
        }
        else if (state == FILES_EXIST_PROBLEM)
        {
            QMessageBox::about(NULL,"Tip","the directory is not empty");
            //cout << "error:the directory is not empty" << endl;
            return 0;
        }
    }
    else
    {
        string file_name_temp = present_path + file_name;
        //该文件不存在
        if (dm->isInputPathExisting(file_name_temp) == false)
        {
            QMessageBox::about(NULL,"Tip","the file do not exist");
            //cout << "error:the file do not exist" << endl;
            return false;
        }
        //该文件存在
        else
        {
            //判断该文件是否已经被打开
            if (isFileOpened(present_path, file_name))
            {
                QMessageBox::about(NULL,"Tip","the file has been opened");
                //cout << "the file has been opened" << endl;
                return false;
            }
            //文件未被打开
            else
            {
                full_name = present_path + file_name;
                int state = dm->deleteFileOrDirectory(full_name);
                if (state == SUCCESS)
                {
                    if(dm->dele_fCB(full_name))
                        return 1;
                    else
                        QMessageBox::about(NULL,"Tip","can not delete the fcb");
                        //cout << "error:can not delete the fcb " << endl;
                    return 0;
                }
                else if (state == RIGHT_PROBLEM)
                {
                    QMessageBox::about(NULL,"Tip","the operation is denied");
                    //cout << "error:the operation is denied" << endl;
                    return 0;
                }
                else if (state == TARGET_NOT_EXIST_PROBLEM)
                {
                    QMessageBox::about(NULL,"Tip","target does not exist");
                   // cout << "error : target does not exist" << endl;
                    return 0;
                }
                else if (state == FILES_EXIST_PROBLEM)
                {
                    QMessageBox::about(NULL,"Tip","the directory is not empty");
                   // cout << "error:the directory is not empty" << endl;
                    return 0;
                }
            }
        }
    }
}

bool FileManager::rename(string full_name, string new_name)
{
    string full_name_temp = full_name;
    string new_name_temp = new_name;
    new_name_temp = cutSpace(new_name_temp);
    if (isFileOpened(present_path, full_name_temp))
    {
        QMessageBox::about(NULL,"Tip","the file is opened");
       // cout << "error:the file is opened" << endl;
        return 0;
    }
    int state = dm->renameFileOrDirectory(full_name_temp, new_name_temp);
    if (state == SUCCESS)
    {
        return 1;
    }
    else if (state == RIGHT_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the operation is denied");
        //cout << "error:the operation is denied" << endl;
        return 0;
    }
    else if (state == TARGET_NOT_EXIST_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","target does not exist");
        //cout << "error : target does not exist" << endl;
        return 0;
    }
    else if (state == SAME_NAME_EXISTS_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the directory(file) exists");
        //cout << "error:the directory(file) exists" << endl;
        return 0;
    }
    else
    {
        return 0;
    }
}
int FileManager::copy(string full_name)
{
    //判断是否有源文件进入复制缓冲
    if(last_path == "")
    {
        return NO_DATA_IN_BUFFER;
    }
    string full_name_temp = full_name;
    //判断源文件是否被打开
    if (isFileOpened(last_path, full_name_temp))
    {
        QMessageBox::about(NULL,"Tip","the file is opened");
        //cout << "error:the file is opened" << endl;
        return FILE_OPENED;
    }
    string sourse, target;

    sourse = last_path + full_name_temp;
    target = present_path;

    int state = dm->copyFileOrDirectory(sourse, target);
    if (state == SUCCESS)
    {
        FCB * copy_fcb;
        copy_fcb = dm->FindFCBinfo(sourse);
        if(!(copy_fcb==NULL)){
            dm->add_fCB(present_path+full_name_temp,copy_fcb->type,copy_fcb->right);
            QMessageBox::about(NULL,"Tip","Copy/Paste successful");
            //cout<<"复制粘贴成功！"<<endl;
        }
        else
            state = FCB_ERROR;
    }
    else if (state == RIGHT_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the operation is denied");
        cout << "error:the operation is denied" << endl;
    }
    else if (state == TARGET_NOT_EXIST_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","target does not exist");
        //cout << "error : target does not exist" << endl;
    }
    else if (state == SAME_NAME_EXISTS_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the directory(file) exists");
        //cout << "error:the directory(file) exists" << endl;
    }
    else if (state == OUT_OF_DISK_CPACITY_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","no enough hard disk space");
        //cout << "error::no enough hard disk space" << endl;
    }
    else if (state == TARGET_DIRECTORY_INVALID) {

    }
    else
    {
    }
    return state;
}

int FileManager::cut(string full_name)
{
    //判断是否有源文件进入剪切缓冲
    if(last_path == "")
    {
        return NO_DATA_IN_BUFFER;
    }
    string full_name_temp = full_name;
    //判断源文件是否被打开
    if (isFileOpened(last_path, full_name_temp))
    {
        QMessageBox::about(NULL,"Tip","the file is opened");
        //cout << "error:the file is opened" << endl;
        return FILE_OPENED;
    }
    string sourse, target;
    sourse = last_path + full_name_temp;
    target = present_path;
    FCB * cut_fcb;
    cut_fcb = dm->FindFCBinfo(sourse);
    int c_type;
    int c_right;
    if(!(cut_fcb==NULL)){
        c_type=cut_fcb->type;
        c_right=cut_fcb->right;
    }
    int state = dm->cutFileOrDirectory(sourse, target);
    if (state == SUCCESS)
    {
        if(!dm->add_fCB(present_path+full_name_temp,c_type,c_right))
            state == FCB_ERROR;
    }
    else if (state == RIGHT_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the operation is denied");
        //cout << "error:the operation is denied" << endl;
    }
    else if (state == TARGET_NOT_EXIST_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","target does not exist");
        //cout << "error : target does not exist" << endl;
    }
    else if (state == SAME_NAME_EXISTS_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the directory(file) exists");
        //cout << "error:the directory(file) exists" << endl;
    }
    else if (state == OUT_OF_DISK_CPACITY_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","no enough hard disk space");
        //cout << "error::no enough hard disk space" << endl;
    }
    else if (state == TARGET_DIRECTORY_INVALID) {

    }
    else
    {
    }
    return state;
    last_path = "";
}

void FileManager::setLastPath()
{
    last_path = dm->getCurrentPath();
}
string FileManager::showContent(string full_path)
{
    string full_name = full_path;
    OpenedFile *temp = openedFileList->next;
    string temp_full_path;
    while (temp != NULL)
    {
        temp_full_path = temp->getFile()->getPath()+ temp->getFile()->getName();
        if(temp_full_path == full_name)
        {
            break;
        }
        temp = temp->next;
    }
    if (temp == NULL)
    {
        QMessageBox::about(NULL,"Tip","the file is not opened");
        //cout << "error:the file is not opened" << endl;
        return "";
    }
    string content = dm->readFileContent(full_name);

    for(int i = 0;i < (int)content.length()-1;i++)
    {
        //将回车符变为"\n"正常显示
        if(content.substr(i,1) == "\\" && content.substr(i+1,1) == "n")
            {
                content = content.substr(0,i)+"\n"+content.substr(i+2,content.length()-i-2);
            }
    }


    string str;
    str = content;

 //   cout<<str<<"显示文件内容"<<endl;
    return str;

}

bool FileManager::changeContent(string full_path, string content)
{
    string full_path_temp = full_path;
    string content_temp = content;
    OpenedFile *temp = openedFileList->next;
    string tmp;
    while (temp != NULL)
    {
        tmp = temp->getFile()->getPath()+temp->getFile()->getName();
        if(tmp == full_path_temp)
        {
            break;
        }
        temp = temp->next;
    }
    if (temp == NULL)
    {
        QMessageBox::about(NULL,"Tip","the file is not opened");
       // cout << "error:the file is not opened" << endl;
        return 0;
    }
    //将存入磁盘的回车符变为“\n”存储
    for(int i = 0;i<content_temp.length();i++)
    {
        if(content_temp.substr(i,1) == "\n")
        {
            content_temp = content_temp.substr(0,i)+"\\n"+content_temp.substr(i+1,content_temp.length()-i-1);
        }
    }

    int state = dm->updateFile(full_path_temp,content_temp);
    if (state == SUCCESS)
    {
        return 1;
    }
    else if (state == RIGHT_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the operation is denied");
        //cout << "error:the operation is denied" << endl;
        return 0;
    }
    else if (state == TARGET_NOT_EXIST_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","target does not exist");
        //cout << "error : target does not exist" << endl;
        return 0;
    }
    else if (state == OUT_OF_DISK_CPACITY_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","no enough hard disk space");
        //cout << "error::no enough hard disk space" << endl;
        return 0;
    }
    else
    {
        return 0;
    }
}

vector<string> FileManager::getFileAndDirectory(string full_path)
{
    vector<string> temp_ret;
    string full_path_temp = full_path;
    vector<string> string_temp = dm->getAllFileOrDirectoryOfPath(full_path_temp);

    vector<string> sort;
    for(int i = 0;i<string_temp.size();i++)
    {
        string temp_name = string_temp[i];
        if(temp_name.substr(temp_name.length()-1,1) == "/")
        {
            sort.push_back(temp_name);
        }
    }
    for(int i = 0;i<string_temp.size();i++)
    {
        string temp_name = string_temp[i];
        if(temp_name.substr(temp_name.length()-1,1) != "/")
        {
            sort.push_back(temp_name);
        }
    }
    string_temp = sort;
    for(int i = 0;i<string_temp.size();i++)
    {
        temp_ret.push_back(string_temp[i]);
    }

    return temp_ret;
}

int FileManager::copyOrCut(string file_name)
{
    //copy
    if(flag == 0)
    {
        return copy(file_name);
    }
    //move
    else if(flag == 1)
    {
        return cut(file_name);
    }
    else
    {
        return 0;
    }
}

string FileManager::cutSpace(string name)
{
    //剪去字符串开头的空格
    while(name.substr(0,1) == " ")
    {
        name = name.substr(1,name.length()-1);
    }
    //判断该字符串是文件夹名还是文件名
    //删去末尾空格
    if(name.substr(name.length()-1,1) == "/")
    {
        while(name.substr(name.length()-2,1) == " ")
        {
            name = name.substr(0,name.length()-2)+"/";
        }
    }
    else
    {
        while(name.substr(name.length()-1,1) == " ")
        {
            name = name.substr(0,name.length()-1);
        }
    }
    return name;
}
