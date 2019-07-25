#include "file_control.h"
#include<QMessageBox>
file_control::file_control()
{
    cout<<"HOME:"<<FileManager::getIncetence()->setPresentPath(home);
}

void file_control::change_cur_address()
{
    //temppath为路径全称
    int state;//1为成功 0为失败
    state = FileManager::getIncetence()->setPresentPath(temppath);
    //cout<<"******11"<<state<<endl;
}
void file_control::new_file(int temp_right,string filename)//0 为只读，1 为可读可写，2为可读可删除 (目前默认为 0 )
{
    // cout<<"get name  "<<filename<<endl;

    if(!filename.length())
       QMessageBox::about(NULL,"Wrong Opt","The name can't be NULL");
    else{
       FCB *ne_fcb ;
       ne_fcb = new FCB;
       int file_right=1;
       if(temp_right==1||temp_right==0||temp_right==2)
       {
          file_right=temp_right;
       }

       int state = FileManager::getIncetence()->createDirectory(filename,0,temp_right);
       if(SUCCESS==state){
          //cout<<"New file success！"<<endl;
          //cout<<curpath;
          ne_fcb= FileManager::getIncetence()->FindFCB(curpath+filename);
          if(ne_fcb!=NULL)
             cout<<"attribute of this file is："<<endl<<" name: "<<ne_fcb->name<<" size："<<ne_fcb->size<<" right："<<ne_fcb->right<<" type: "<<ne_fcb->type<<" data："<<ne_fcb->time<<endl;
       }
    }
}
void file_control::new_file_folder(int temp_right,string filename)//在当前目录下新建文件夹  temp_right 0 为只读，1 为可读可写，2为可读可删除 (目前默认为 0 )
{
    //cout<<"(new_file_folder)当前路径为："<<curpath<<endl;
    if(!filename.length()){
        QMessageBox::about(NULL,"Wrong Opt","The name of folder can't be NULL");
        //cout<<"文件夹名不可为空！"<<endl;
    }
    else{
        int file_right=0;
        if(temp_right==1||temp_right==0||temp_right==2)
            file_right=temp_right;
        int state = FileManager::getIncetence()->createDirectory(filename,0,file_right);
        if(SUCCESS==state){
            //cout<<"新建文件成功！"<<endl;
            FCB *ne_fcb ;
            ne_fcb = new FCB;
            ne_fcb=FileManager::getIncetence()->FindFCB(curpath+filename);
            if(ne_fcb!=NULL)
     cout<<"Find FCB："<<endl<<" name: "<<ne_fcb->name<<" size："<<ne_fcb->size<<" priority："<<ne_fcb->right<<" type: "<<ne_fcb->type<<" data："<<ne_fcb->time<<endl;
       }
    }
}

void file_control::del_file(string filename)
{

    //cout<<"deletefile!!!lookat"<<endl;
    //cout<<"删除文件夹/文件----请输入文件夹名/文件名，文件夹名后请后缀/！"<<endl;
    if(!FileManager::getIncetence()->isFileOpened(this->fm->present_path, filename))
    {
        //cout<<"This file is  not opened!"<<endl;
       FCB * show_fcb;
       cout<<"(dele_file)"<<this->fm->present_path<<"+"<<filename<<endl;
       show_fcb= FileManager::getIncetence()->FindFCB(this->fm->present_path+filename);
       if(show_fcb!=NULL)
       {
          cout<<"Find FCB："<<endl<<" name: "<<show_fcb->name<<" size："<<show_fcb->size<<" priority："<<show_fcb->right<<" type: "<<show_fcb->type<<" data："<<show_fcb->time<<endl;
          if(show_fcb->right!=-1){
             if(FileManager::getIncetence()->deleteDirectory(filename))//从文本框读出来
                cout<<"delete success!"<<endl;
             else
                 QMessageBox::about(NULL,"Wrong Opt","can not delete");
                //cout<<"can not delete"<<endl;
          }
          else
               QMessageBox::about(NULL,"Wrong Opt","This file can only be read");
             //cout<<"This file can only be read!"<<endl;
        }
        else
           QMessageBox::about(NULL,"Wrong Opt","Have no priority!");
            //cout<<"Have no priority!"<<endl;
    }
    else
        QMessageBox::about(NULL,"Wrong Opt","File is open! can not delete");
        //cout<<"File is open! can not delete!"<<endl;
}


string file_control::open_file(string filename)
{


    temppath=curpath;
    //temppath.append(filename);// /aa

    string tempdir="";

    string currentpath=curpath.substr(0,curpath.size()-filename.size());

    cout<<FileManager::getIncetence()->setPresentPath(currentpath);
     //cout<<"look at the currentpath"<<currentpath<<endl;

    //cout<<"open_file_start(flag)="<<temppath<<"--"<<filename<<endl;

    //FileManager::getIncetence()->setPresentPath("/");
    //temppath="/aa";
    //filename="aa";

    if(FileManager::getIncetence()->openFile(filename)){
        tempdir = FileManager::getIncetence()->showContent(temppath);//打开文件的内容
         cout<<"file open success"<<endl;
         if(FileManager::getIncetence()->setPresentPath(temppath))
         {
             //cout<<"******33"<<endl;
             curpath=temppath;
             //cout<<"change path success"<<endl;
         }

    }
    else
        QMessageBox::about(NULL,"Tip","File can not open");
        //cout<<"can not open"<<endl;
    //cout<<"open_file_end(tempdir------2)"<<tempdir<<endl;
    return tempdir;
}

void file_control::save_file(string filename,string text)
{
    cout<<"save file"<<endl;
    if(FileManager::getIncetence()->isFileOpened(curpath, filename)){
        FCB * show_fcb;
        show_fcb= FileManager::getIncetence()->FindFCB(curpath+filename);
        if(show_fcb!=NULL){
            cout<<"找到的文件fcb块的内容为："<<endl<<" 名称: "<<show_fcb->name<<" 文件大小："<<show_fcb->size<<" 权限："<<show_fcb->right<<" 类型: "<<show_fcb->type<<" 创建时间："<<show_fcb->time<<endl;
            if(show_fcb->right!=0){
                //cout<<"请输入文件内容"<<endl;
                //string text;
                //cin>>text;
                if(FileManager::getIncetence()->changeContent(temppath,text)) //从文本框读出来
                    cout<<"save success！"<<endl;
                else
                    QMessageBox::about(NULL,"Tip","This file save fail");//cout<<"save fail！"<<endl;
                }
             else
                QMessageBox::about(NULL,"Tip","This file can only be read");//cout<<"only can be read！"<<endl;
             }
        else
            QMessageBox::about(NULL,"Tip","This file can't' be read");//cout<<"can not read！！"<<endl;
        }
    else
        QMessageBox::about(NULL,"Tip","This file is not opened");//cout<<"file is not open！"<<endl;
}

bool file_control::close_file(string filename,string text)//这一块有问题  参数
{
    //cout<<"(close_file)"<<curpath<<endl;
    string t=curpath+filename;

    //if(FileManager::getIncetence()->isFileOpened(curpath, filename)){
        //cout<<"(close_file)"<<t<<endl;
        FCB * show_fcb;
        show_fcb= FileManager::getIncetence()->FindFCB(t);
        if(show_fcb!=NULL){
 cout<<"find fcb："<<endl<<" name: "<<show_fcb->name<<" size："<<show_fcb->size<<" priority："<<show_fcb->right<<" type: "<<show_fcb->type<<" data："<<show_fcb->time<<endl;
            if(show_fcb->right!=0){
                //cout<<"(close_file2)"<<t<<endl;
                if(FileManager::getIncetence()->changeContent(temppath,text)&&FileManager::getIncetence()->closeFile(curpath)) //从文本框读出来  原本是curpath+filename
                    return true;
                else
                    return false;
                }
             else
                return false;
        //}
        //else
            //return false;
    }
    else
       return false;
}

void file_control::copy_file(string filename)
{
    FileManager::getIncetence()->setFlagCopy(filename);
}

void file_control::cut_file(string filename)
{
    FileManager::getIncetence()->setFlagCut(filename);
}

void file_control::paste_file(string filename)
{
    int state = FileManager::getIncetence()->copyOrCut(FileManager::getIncetence()->getcopyOrcut_name());
    //cout<<state<<"粘贴操作"<<endl;
    if (state == SUCCESS)
    {
       // cout<<"剪切粘贴成功！"<<endl;
    }
    else if (state == RIGHT_PROBLEM)
    {
       QMessageBox::about(NULL,"Tip","the operation is denied");// cout << "error:the operation is denied" << endl;
    }
    else if (state == TARGET_NOT_EXIST_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","target does not exist");//cout << "error : target does not exist" << endl;
    }
    else if (state == SAME_NAME_EXISTS_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the directory(file) exists");//cout << "error:the directory(file) exists" << endl;
    }
}

void file_control::rename(string filename,string re_name)
{
    int state = FileManager::getIncetence()->rename(curpath+filename,re_name);
    cout<<state<<"重命名操作"<<endl;
    if (state == SUCCESS)
    {
        cout<<"重命名操作成功！"<<endl;
    }
    else if (state == RIGHT_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the operation is denied");//cout << "error:the operation is denied" << endl;
    }
    else if (state == TARGET_NOT_EXIST_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","target does not exist");//cout << "error : target does not exist" << endl;
    }
    else if (state == SAME_NAME_EXISTS_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","the directory(file) exists");//cout << "error:the directory(file) exists" << endl;
    }
    else if (state == OUT_OF_DISK_CPACITY_PROBLEM)
    {
        QMessageBox::about(NULL,"Tip","no enough hard disk space");//cout << "error::no enough hard disk space" << endl;
    }
    else if (state == TARGET_DIRECTORY_INVALID) {
        QMessageBox::about(NULL,"Tip","Target_Directory_Invalid");//cout << "error: TARGET_DIRECTORY_INVALID" << endl;
    }
    else if (state == FCB_ERROR) {
        QMessageBox::about(NULL,"Tip","Fcb Error!");//cout << "error: FCB_ERROR" << endl;
    }

}

void file_control::show_cur_folder()
{
    this->count=0;
    //cout<<"(show_folder)"<<curpath<<endl;
    vector<string> all_file = FileManager::getIncetence()->getFileAndDirectory(curpath);
    for(vector<string>::iterator iter =all_file.begin(); iter !=all_file.end(); iter++){
        //cout<<"filenames in this folder："<<*iter<<endl;
        FCB * show_fcb;
        show_fcb= FileManager::getIncetence()->FindFCB(curpath+(*iter));
        if(show_fcb!=NULL){
            file_fcb[this->count]=show_fcb;
            this->count++;
            //cout<<"the fcb's message which was found is："<<endl<<" name: "<<show_fcb->name<<" size："<<show_fcb->size<<" priority："<<show_fcb->right<<" type: "<<show_fcb->type<<" data："<<show_fcb->time<<endl;
        }
        else
            cout<<"Find nothing！"<<endl;
        }
}
