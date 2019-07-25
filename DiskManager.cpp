#pragma once
#include"head.h"
#include"File.h"
DiskManager *DiskManager::incetence = new DiskManager();
DiskManager::DiskManager() {
    directory_list = new directoryNode;
    directory_list->name = "/";
    directory_list->brother = NULL;
    directory_list->child = NULL;
    directory_list->father = NULL;
    current_path = directory_list;
    initialFCB();//初始化
    loadDirectoryToList();//初始化
}
DiskManager *DiskManager::getIncetence()
{
    if(incetence == NULL)
    {
        incetence = new DiskManager();
    }
    return incetence;
}

void DiskManager::initialFCB(){
    readFCB();
    vector<FCB *>::iterator iter;
    for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++)
        cout<<"初始化FCB中存储的fcb信息："<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;
}


string DiskManager::getTime(void)
{
    struct tm *local,*ptr; //定义tm结构指针存储时间信息
    time_t t; //时间结构或者对象
    t=time(NULL); //获取当前系统的日历时间
    local=localtime(&t);//localtime()函数是将日历时间转化为本地时间
    ptr=gmtime(&t);//将日历时间转化为世界标准时间
    //printf("The UTC time is %s/n",asctime(ptr)); //格式化输出世界标准时间
    //printf("The local time is %s/n",ctime(&t));//输出本地时间
    /*asctime()函数(参数为tm结构指针)和ctime()函数(参数为time_t结构)将时间以固定的格式显示出来，两者的返回值都是char*型的字符串。返回的时间格式为：星期几 月份 日期 时:分:秒 年/n/0 */
    return ctime(&t);
}
void DiskManager::Strcpy(char *p2,string p1){
    int i=0;
    for(;i<p1.length();i++){
        p2[i]=p1[i];
    }
    p2[i]='\0';
}
//从磁盘中读出所有FCB信息，以vector形式返回
void DiskManager::readFCB()
{
    fcbInfo.clear();
//  std::ifstream file1("FCB.dat",std::ios::binary);
//    long l,m;
//	  file1.seekg(0, ios::beg);
//    l = file1.tellg();
//    cout<<"文件头"<<l<<endl;
//    file1.seekg(0, ios::end);
//    cout<<"文件尾"<<m<<endl;
//	  cout <<"size of ";
//    cout <<" is "<< (m-l)<<" bytes.\n";
//    file1.close();
    std::ifstream file("FCB.dat",std::ios::binary);
    file.seekg(0,ios::end);
    long m = file.tellg();
//	cout<<"m的值"<<m<<endl;
    file.seekg(0,ios::beg);
    if(m>0)
    while(!file.eof()){
            FCB *fcb1;
            fcb1 = new FCB;
            file.read((char*)fcb1, sizeof(FCB));
//     		cout<<"读--1:"<<fcb1->right<<endl;
//    		cout<<"读--2:"<<fcb1->size<<endl;
//			cout<<"读--3:"<<fcb1->type<<endl;
//	 		cout<<"读--4:"<<fcb1->time<<endl;
//			cout<<"读--5:"<<fcb1->name<<endl;
            fcbInfo.push_back(fcb1);
            char s[2];
            file.read(s,sizeof(char));
            if(file.eof())
                break;
            else{
                file.seekg(-1,ios::cur);
            }
    }
    file.close();
}
void DiskManager::LoadIntoFCB()
{
    cout<<"写入前FCB大小"<<sizeof(FCB);
    std:ofstream f("FCB.dat",ios::binary);
     if (f.is_open()) {
        vector<FCB *>::iterator iter;
        for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++) {
            f.write((char*)(*iter),sizeof(FCB));
//			cout<<"写--1:"<<(*iter)->right<<endl;
//    		cout<<"写--2:"<<(*iter)->size<<endl;
//			cout<<"写--3:"<<(*iter)->type<<endl;
//	 		cout<<"写--4:"<<(*iter)->time<<endl;
//			cout<<"写--5:"<<(*iter)->name<<endl;
        }
    }
    f.close();
    readFCB();
}



bool DiskManager:: add_fCB(string a_name,int a_type,int a_right)
{
    FCB *ne_fcb ;
    ne_fcb = new FCB;
    ne_fcb->type=a_type;//文件类型为0,或 1
    ne_fcb->size=0;//文件初始大小为0KB
    ne_fcb->right=a_right;
    Strcpy(ne_fcb->name,a_name);
    Strcpy(ne_fcb->time,getTime());
    fcbInfo.push_back(ne_fcb);
    LoadIntoFCB();
}

bool DiskManager::update_fCB(string u_name,int u_size){
    FCB * update_fcb;
    update_fcb = FindFCBinfo(u_name);
    cout<<"u_size"<<u_size;
    if(!(update_fcb==NULL)){
        update_fcb->size=u_size*4;
        Strcpy(update_fcb->time,getTime());
        directoryList node=getNodeByInput(u_name);
        LoadIntoFCB();
        node = node->father;
        while (node->father!=NULL){
            cout<<"更新其父文件夹大小"<<endl;
            cout<<node->name;
            update_fcb = FindFCBinfo(getFullPathByNode(node));
            if(!(update_fcb==NULL)){
                update_fcb->size+=u_size*4;
                Strcpy(update_fcb->time,getTime());
                LoadIntoFCB();
                //directoryList node=getNodeByInput(u_name);
            }
            node = node->father;
        }
        LoadIntoFCB();
        vector<FCB *>::iterator iter;
        for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++)
            cout<<"更新后FCB中存储的fcb信息："<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;
        return true;
    }
    else{
        cout<<"找不到更新时找不到FCB"<<endl;
        return false;
    }
}

FCB* DiskManager::FindFCBinfo(string file_name){
    readFCB();

    //vector<FCB *>::iterator iter;
    //for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++)
    //	cout<<"看下vector中存储的fcb信息："<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;

    vector<FCB *>::iterator iter2;
    for (iter2 = fcbInfo.begin(); iter2!= fcbInfo.end(); iter2++) {
        //cout<<"要遍历的文件名字："<<file_name<<endl;
        //cout<<"迭代器名字："<<(*iter2)->name<<endl;
        if((*iter2)->name==file_name)
            return *(iter2);
    }
    cout<<"找不到相应文件的属性！"<<endl;
    return NULL;
}
bool DiskManager::dele_fCB(string d_name){

    cout<<"删除FCB"<<endl;
    vector<FCB *>::iterator iter2;
    int pos=0;
    for (iter2 = fcbInfo.begin(); iter2!= fcbInfo.end(); iter2++,pos++) {
    //	cout<<"删除--要遍历的文件名字："<<d_name<<endl;
    //    cout<<"删除--迭代器名字："<<(*iter2)->name<<endl;
        if((*iter2)->name==d_name){
            fcbInfo.erase(iter2);
            LoadIntoFCB();
            return true;
        }
    }
    cout<<"无法找到对应FCB块，删除FCB块失败！"<<endl;
    return false;
}
//符串获取目录节点的绝对路径，若该目录节点为空，则返回长度为0的字
string DiskManager::getFullPathByNode(directoryList node)
{
    //若该目录节点为空，则返回长度为0的字符串
    if (NULL == node) {
        cout<<"this node is null"<<endl;
        return "";
    }
    string ans = "";
    //循环在加入其父的名字，直到根目录
    while (NULL != node) {
        ans = node->name + ans;
        node = node->father;
       // cout<<"ans:"<<ans<<endl;
    }

  //  cout<<"返回的DiskManager::getFullPathByNode值为"<<ans<<endl;

    return ans;
}

//获取相对路径对应的目录节点，若该目录节点不存在，则返回NULL
directoryList DiskManager::getNodeByInput(string input_path)
{
    directoryList ans = NULL;//文件节点
    //若输入路径为绝对路径，则从根目录开始计算
    if (0 < input_path.length() && '/' == input_path[0]) {
        input_path = input_path.substr(1, (int)input_path.size() - 1);
  //      cout<<"input_path______here！！："<<input_path<<endl;
        ans = directory_list;
    }
    //否则该路径为相对路径，则从当前路径开始计算
    else {
        ans = current_path;
    }
    //pos指向子路径开始处，next_pos指向下一子路径开始处
    int pos = 0, next_pos = 0;
    //temp_path表示当前子路径
    string temp_path;
    //标识是否读取了文件名
    bool over = true;
    for (pos = 0; pos <= (int)input_path.size() - 1; pos = next_pos) {
        //读取到文件名
        if (string::npos == input_path.find('/', pos)) {
            temp_path = input_path.substr(pos, (int)input_path.size() - pos);
            over = false;
        }
        //读取到目录名
        else {
            next_pos = (int)input_path.find('/', pos) + 1;
            temp_path = input_path.substr(pos, next_pos - pos);
        }
        //读取到../则指向父路径
        if ("../" == temp_path) {
            if (NULL != ans->father)
                ans = ans->father;
        }
        //根据temp_path获取目录节点
        else {
            if (NULL == ans) {
                return NULL;
            }
            else {
                ans = ans->child;
                while (NULL != ans && temp_path != ans->name)
                    ans = ans->brother;
            }
            if (NULL == ans) {
                return NULL;
            }
        }
        //读取到文件名则跳出
        if (false == over) {
            break;
        }
    }
    return ans;
}

//获取一个相对路径的绝对路径，若该路径不存在，则返回长度为0的字符串
string DiskManager::getFullPathByInput(string input_path)
{
    //获取与相对路径对应的目录节点，若该目录节点不存在，则返回NULL
    directoryList node = getNodeByInput(input_path);
    //获取目录节点的绝对路径，若该目录节点为NULL，则返回长度为0的字符串
    string ans = getFullPathByNode(node);

    //cout<<"getFullPathByInput(string input_path)"<<ans<<endl;
    return ans;
}

//判断一个路径是否为文件
bool DiskManager::isFile(string name)
{
    if (0 < name.length() && '/' == name[(int)name.size() - 1]) {
        return false;
    }
    else {
        return true;
    }
}

//将文件夹的最后一个/去掉
string DiskManager::getDirectorySelfName(string name)
{
    //若确实为目录则去掉最后一个斜杠
    if (false == isFile(name)) {
        name = name.substr(0, (int)name.size() - 1);
    }
    return name;
}

//从磁盘中读出所有信息，以vector形式返回
vector<string> DiskManager::readStream()
{
    vector<string> string_buffer;
    string buffer;
    ifstream fin("Disk.txt");
    //if (fin.is_open()) {
        for (int i = 0; i <= DISK_SIZE / BLOCK_SIZE + 3; i++) {
            getline(fin, buffer);
            string_buffer.push_back(buffer);
        }
        fin.close();
    //}
    return string_buffer;
}

//将全部信息写入磁盘
void DiskManager::writeStream(vector<string> buffer)
{
    ofstream out("Disk.txt");
    if (out.is_open()) {
        vector<string>::iterator iter;
        for (iter = buffer.begin(); iter != buffer.end(); iter++) {
            out << *iter << endl;
        }
    }
}

//清空目录链表，只留根节点/
void DiskManager::clearDirectory()
{
    //中序遍历目录链表，逐个删除
    directoryList current = directory_list;
    directoryList stack[STACK_SIZE];
    int top = 0;
    while (0 != top || NULL != current) {
        if (NULL == current) {
            current = stack[--top];
        }
        else {
            directoryList temp = current;
            stack[top++] = current->brother;
            current = current->child;
            if ("/" != temp->name) {
                delete(temp);
            }
            else {
                temp->child = NULL;
                temp->brother = NULL;
            }
        }
    }
}

//将磁盘读入目录链表
void DiskManager::loadDirectoryToList()
{
    //清空内存目录链表
    clearDirectory();
    //读入磁盘中的目录信息
    vector<string> buffer = readStream();
    //vector<string> buffer2 = readFCBStream();




//    buffer.front();
   // vector<string>::iterator i = buffer.begin();
    //DIRECTORY_INFO

    cout<<"buffersize"<<buffer.size()<<endl;
//   cout<<buffer.at(0);
    string directory=buffer[0];


   // string directory = buffer.front();


    //以下四个变量分别记录父目录与子目录的启终位置
    int father_first = 0, father_last = 0;
    int child_first = 0, child_last = 0;
    //定位每一个父子目录对，创建子目录的节点并链接到目录链表中
    for (int current = 0; current <= (int)directory.size() - 1; current++) {
        //记录父目录的开始位置
        if ('<' == directory[current]) {
            father_first = current + 1;
        }
        //记录父目录的结束位置与子目录的开始位置
        else if (',' == directory[current]) {
            father_last = current;
            child_first = current + 1;
        }
        //记录子目录的结束位置
        else if ('>' == directory[current]) {
            child_last = current;
            if (father_first < father_last && child_first < child_last) {
                directoryList current_directory = directory_list;
                //提取出父目录与子目录
                string father_path = directory.substr(father_first, father_last - father_first);
                string child_name = directory.substr(child_first, child_last - child_first);
                //根据父路径将指针指向正确位置
                for (int pos = 0; pos <= (int)father_path.size() - 1; pos = (int)father_path.find('/', pos) + 1) {
                    string temp_path = father_path.substr(pos, (int)father_path.find('/', pos) - pos + 1);
                    if (0 != pos) {
                        current_directory = current_directory->child;
                    }
                    while (NULL != current_directory && temp_path != current_directory->name)
                        current_directory = current_directory->brother;
                }
                //父路径节点下无孩子节点时，创建该子节点并赋值
                if (NULL == current_directory->child) {
                    current_directory->child = new directoryNode;
                    current_directory->child->father = current_directory;
                    current_directory = current_directory->child;
                }
                //父路径下有孩子节点时，找到该孩子节点的最后一个兄弟节点，创建该节点的兄弟节点的并赋值
                else {
                    directoryList father_directory = current_directory;
                    current_directory = current_directory->child;
                    while (NULL != current_directory->brother) {
                        current_directory = current_directory->brother;
                    }
                    current_directory->brother = new directoryNode;
                    current_directory->brother->father = father_directory;
                    current_directory = current_directory->brother;
                }
                current_directory->name = child_name;
                current_directory->brother = NULL;
                current_directory->child = NULL;
            }
        }
    }
    return;
}

//将目录文件读入磁盘
void DiskManager::loadDirectoryToDisk() {
    //中序遍历目录文件
    string ans = "";
    directoryList stack[STACK_SIZE];
    int top = 0;
    directoryList current = directory_list;
    while (0 != top || NULL != current) {
        if (NULL == current) {
            current = stack[--top];
        }
        else {
            if (NULL != current->child) {
                //将目录对并入目录对串字符串中
                directoryList temp = current->child;
                while (NULL != temp) {
                    directoryList father_current = current;
                    string father_path = "";
                    while (NULL != father_current) {
                        father_path = father_current->name + father_path;
                        father_current = father_current->father;
                    }
                    ans += ("<" + father_path + "," + temp->name + ">");
                    temp = temp->brother;
                }
            }
            stack[top++] = current->brother;
            current = current->child;
        }
    }
    //将目录对串写入磁盘
    vector<string> buffer = readStream();
    buffer[DIRECTORY_INFO] = ans;
    writeStream(buffer);
}

//string型转换到int型
int DiskManager::stringToInt(string temp)
{
    stringstream ss;
    ss << temp;
    int ans;
    ss >> ans;
    return ans;
}

//int型转换到string型
string DiskManager::intToString(int temp)
{
    string ans;
    stringstream ss;
    ss << temp;
    ss >> ans;
    return ans;
}
//获取当前路径的绝对路径
string DiskManager::getCurrentPath()
{
    string ans = getFullPathByNode(current_path);
    return ans;
}

//更改当前路径，若输入的路径不存在则返回false
bool DiskManager::changeCurrentPath(string input_path)
{
    directoryList ans = getNodeByInput(input_path);
    if (NULL == ans) {
        return false;
    }
    else {
        current_path = ans;
        return true;
    }
}

//获取一个文件夹下的所有文件与文件夹
vector<string> DiskManager::getAllFileOrDirectoryOfPath(string input_path)
{
    vector<string> ans;
    //获取该文件夹的目录节点
    directoryList father_node = getNodeByInput(input_path);
    //输出其所有的子节点名称
    if (NULL == father_node) {
        return ans;
    }
    else if (NULL == father_node->child) {
        return ans;
    }
    else {
        directoryList current = father_node->child;
        while (NULL != current) {
            ans.push_back(current->name);
            //cout<<"show里面的遍历current->brother->name"<<current->brother->name<<endl;
            current = current->brother;

        }
    }
    return ans;
}

//判断输入的路径是否存在
bool DiskManager::isInputPathExisting(string input_path)
{
    directoryList ans = getNodeByInput(input_path);
    if (NULL == ans) {
        return false;
    }
    else {
        return true;
    }
}

//判断两个文件或文件夹是否重名
bool DiskManager::isSameName(string name1, string name2)
{
    if (getDirectorySelfName(name1) == getDirectorySelfName(name2)) {
        return true;
    }
    else {
        return false;
    }
}

int DiskManager::deleteFile(string input_path)
{
    if (NULL == getNodeByInput(input_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    string full_name = getFullPathByInput(input_path);
    //以下是将该文件的内容、索引删除
    vector<string> buffer = readStream();
    //temp记录文件索引
    string temp = buffer[FILE_INDEX];
    int pos = (int)temp.find(full_name, 0);
    //pos指向该文件索引
    pos += (int)full_name.size() + 1;
    //index_int记录文件索引
    vector<int> index_int;
    int i;
    for (i = pos; '>' != temp[i]; i++) {
        string index_temp = "";
        while (',' != temp[i] && '>' != temp[i]) {
            index_temp += temp[i++];
        }
        index_int.push_back(stringToInt(index_temp));
        if ('>' == temp[i]) {
            break;
        }
    }
    //ipos记录该文件索引的结尾
    int ipos = i + 1;
    pos = (int)temp.find(full_name, 0) - 1;
    string modified_file = temp.substr(0, pos) + temp.substr(ipos, (int)temp.size() - ipos);
    buffer[1] = modified_file;
    string bit_string = buffer[BIT_MAP];
    for (vector<int>::iterator iter = index_int.begin(); iter != index_int.end(); iter++) {
        int posi = *iter + FILE_CONTENT;
        buffer[posi] = "";
        bit_string[*iter] = '0';
    }
    buffer[BIT_MAP] = bit_string;
    writeStream(buffer);
    //以下是将该文件从目录中删除
    directoryList current_directory = getNodeByInput(full_name)->father;
    if (NULL == current_directory) {
        return RIGHT_PROBLEM;
    }
    if (full_name == getFullPathByNode(current_directory->child)) {
        directoryList temp = current_directory->child;
        current_directory->child = temp->brother;
        delete(temp);
    }
    else {
        current_directory = current_directory->child;
        while (NULL != current_directory->brother && full_name != getFullPathByNode(current_directory->brother)) {
            current_directory = current_directory->brother;
        }
        directoryList temp = current_directory->brother;
        current_directory->brother = temp->brother;
        delete(temp);
    }
    loadDirectoryToDisk();
    return SUCCESS;
}

int DiskManager::deleteDirectory(string input_path)
{
    if (NULL == getNodeByInput(input_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    string full_name = getFullPathByInput(input_path);
    vector<string> all_file = getAllFileOrDirectoryOfPath(full_name);
    if (0 != (int)all_file.size()) {
        all_file.clear();
        return FILES_EXIST_PROBLEM;
    }
    else {
        all_file.clear();
        //以下是将该文件从目录中删除
        directoryList current_directory = getNodeByInput(full_name)->father;
        if (NULL == current_directory) {
            return RIGHT_PROBLEM;
        }
        if (full_name == getFullPathByNode(current_directory->child)) {
            directoryList temp = current_directory->child;
            current_directory->child = temp->brother;
            delete(temp);
        }
        else {
            current_directory = current_directory->child;
            while (NULL != current_directory->brother && full_name != getFullPathByNode(current_directory->brother)) {
                current_directory = current_directory->brother;
            }
            directoryList temp = current_directory->brother;
            current_directory->brother = temp->brother;
            delete(temp);
        }
        loadDirectoryToDisk();
        return SUCCESS;
    }
}

string DiskManager::readFileContent(string input_path)
{
    string full_name = getFullPathByInput(input_path);
    if ("" == full_name) {
        return "";
    }
    vector<string> buffer = readStream();
    string temp = buffer[FILE_INDEX];
    if (string::npos == temp.find(full_name, 0)) {
        return "";
    }
    int pos = (int)temp.find(full_name, 0);
    pos += (int)full_name.size() + 1;
    vector<int> index_int;
    for (int i = pos; '>' != temp[i]; i++) {
        string index_temp = "";
        while (',' != temp[i] && '>' != temp[i]) {
            index_temp += temp[i++];
        }
        index_int.push_back(stringToInt(index_temp));
        if ('>' == temp[i]) {
            break;
        }
    }
    string ans = "";
    for (vector<int>::iterator iter = index_int.begin(); iter != index_int.end(); iter++) {
        int posi = *iter + FILE_CONTENT;
        ans += buffer[posi];//按每页读取文件内容
    }
    return ans;
}

int DiskManager::insertIntoDisk(string father_path, string name)
{
    father_path = getFullPathByInput(father_path);

 //   cout<<"father_path---"<<father_path<<endl;

    directoryList father_node = getNodeByInput(father_path);
    if (NULL == father_node) {
        return TARGET_NOT_EXIST_PROBLEM;
  //      cout<<"father_node是否为空"<<endl;
    }
    directoryList current = father_node;


    vector<string> all_file = getAllFileOrDirectoryOfPath(father_path);


    for (vector<string>::iterator iter = all_file.begin(); iter != all_file.end(); iter++) {
        if (isSameName(*iter, name)) {
            all_file.clear();
            return SAME_NAME_EXISTS_PROBLEM;
        }
    }
    if (NULL == current->child) {
        current->child = new directoryNode;
        current->child->father = father_node;
        current = current->child;
    }
    else {
        current = current->child;
        while (NULL != current->brother) {
            current = current->brother;
        }
        current->brother = new directoryNode;
        current->brother->father = father_node;
        current = current->brother;
    }
    current->name = name;
    current->child = NULL;
    current->brother = NULL;
    loadDirectoryToDisk();
    if (string::npos == name.find('/', 0)) {


        createFileInDisk(father_path + name);


        //father_path="/lizi/";
      //  cout<<"father_path---"<<father_path<<"---name---"<<name<<endl;
        if (SUCCESS == updateFile(father_path + name, "")) {

            //cout<<"father_path---"<<father_path<<"---name---"<<name<<endl;
   //         cout<<SUCCESS<<"SUCCESS"<<endl;
            return SUCCESS;

        }
        else {

            deleteFileOrDirectory(father_path + name);

   //         cout<<"出错了"<<endl;
            return OUT_OF_DISK_CPACITY_PROBLEM;



        }
    }
    else {
//        cout<<SUCCESS<<"SUCCESS！"<<endl;
        return SUCCESS;

    }
}

void DiskManager::createFileInDisk(string full_name)
{
    string ans = "<" + full_name + ":>";
    vector<string> buffer = readStream();
    string temp = buffer[FILE_INDEX];
    temp += ans;
    buffer[FILE_INDEX] = temp;
    writeStream(buffer);
}

int DiskManager::updateFile(string input_path, string content)
{
    string full_name = getFullPathByInput(input_path);

    if ("" == full_name) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    //modified_size记录文件需要的块数
    int content_size = (int)content.size();
    int modified_size = (content_size - 1) / BLOCK_SIZE + 1;
    vector<string> buffer = readStream();
    //temp记录文件索引
    string temp = buffer[FILE_INDEX];
    int pos = (int)temp.find("<" + full_name + ":", 0) + 1;
    //pos指向该文件索引
    pos += (int)full_name.size() + 1;
    //index_int记录文件索引
    vector<int> index_int;
    int i;
    for (i = pos; '>' != temp[i]; i++) {
        string index_temp = "";
        while (',' != temp[i] && '>' != temp[i]) {
            index_temp += temp[i++];
        }
        index_int.push_back(stringToInt(index_temp));
        if ('>' == temp[i]) {
            break;
        }
    }
    //ipos记录该文件索引的结尾
    int ipos = i + 1;
    if ((int)index_int.size() == modified_size) {
        string ans = "";
        int temp_pos = 0;
        for (vector<int>::iterator iter = index_int.begin(); iter != index_int.end(); iter++) {
            ans = content.substr(temp_pos, BLOCK_SIZE);
            int posi = *iter + FILE_CONTENT;
            buffer[posi] = ans;
            temp_pos += BLOCK_SIZE;
        }
    }
    else if ((int)index_int.size() < modified_size) {
        string bit_string = buffer[BIT_MAP];
        for (int i = 0; i <= BLOCK_NUM - 1 && (int)index_int.size() < modified_size; i++) {
            if ('0' == bit_string[i]) {
                index_int.push_back(i);
            }
        }
        if ((int)index_int.size() < modified_size) {
            cout<<(int)index_int.size()<<modified_size<<endl;
            cout<<"there is the size fault"<<endl;
            return OUT_OF_DISK_CPACITY_PROBLEM;
        }
        else {
            string modified_file = temp.substr(0, pos);
            for (vector<int>::iterator iter = index_int.begin(); iter != index_int.end(); iter++) {
                string index_temp = intToString(*iter);//文件加页
                modified_file += index_temp + ",";
            }
            modified_file[(int)modified_file.size() - 1] = '>';
            modified_file += temp.substr(ipos, (int)temp.size() - ipos);
            buffer[FILE_INDEX] = modified_file;
            string ans = "";
            int temp_pos = 0;
            for (vector<int>::iterator iter = index_int.begin(); iter != index_int.end(); iter++) {
                ans = content.substr(temp_pos, BLOCK_SIZE);
                bit_string[*iter] = '1';
                int posi = *iter + FILE_CONTENT;
                buffer[posi] = ans;
                temp_pos += BLOCK_SIZE;
            }
            buffer[BIT_MAP] = bit_string;
        }
    }
    else {
        string bit_string = buffer[BIT_MAP];
        int times = (int)index_int.size() - modified_size;
        for (int i = 0; i <= times - 1; i++) {
            bit_string[*(index_int.begin())] = '0';
            buffer[*(index_int.begin()) + FILE_CONTENT] = "";
            index_int.erase(index_int.begin());
        }
        string modified_file = temp.substr(0, pos);
        for (vector<int>::iterator iter = index_int.begin(); iter != index_int.end(); iter++) {
            string index_temp = intToString(*iter);
            modified_file += index_temp + ",";
        }
        modified_file[(int)modified_file.size() - 1] = '>';
        modified_file += temp.substr(ipos, (int)temp.size() - ipos);
        buffer[1] = modified_file;
        string ans = "";
        int temp_pos = 0;
        for (vector<int>::iterator iter = index_int.begin(); iter != index_int.end(); iter++) {
            ans = content.substr(temp_pos, BLOCK_SIZE);
            bit_string[*iter] = '1';
            int posi = *iter + FILE_CONTENT;
            buffer[posi] = ans;
            temp_pos += BLOCK_SIZE;
        }
        buffer[BIT_MAP] = bit_string;
    }

    update_fCB(full_name,modified_size);
    writeStream(buffer);
    return SUCCESS;
    /*
    if(modified_size==0||update_fCB(full_name,modified_size)){
         writeStream(buffer);
         return SUCCESS;
    }
    else
        return FCB_ERROR;
    */
}

int DiskManager::deleteFileOrDirectory(string input_path)
{
    if (NULL == getNodeByInput(input_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    string full_path = getFullPathByInput(input_path);
    if (true == isFile(full_path)) {
        return deleteFile(full_path);
    }
    else {
        return deleteDirectory(full_path);
    }
}

int DiskManager::copyFileOrDirectory(string source_path, string target_path)
{
    //确定输入的源文件/文件夹与目标文件夹合法
    if (NULL == getNodeByInput(source_path) || NULL == getNodeByInput(target_path)
        || true == isFile(target_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    //将源文件/文件夹与目的文件夹均变换为绝对路径
    source_path = getFullPathByInput(source_path);
    target_path = getFullPathByInput(target_path);
    //若目的文件夹是源文件夹的子文件夹，则该复制操作无法进行，返回权限错误
    if (string::npos != target_path.find(source_path)) {
        return NO_DATA_IN_BUFFER;
    }
    //对待复制项为文件的情况进行处理
    if (true == isFile(getFullPathByInput(source_path))) {
        directoryList source_node = getNodeByInput(source_path);
        string target_full_path = getFullPathByInput(target_path);
        int ans = insertIntoDisk(target_full_path, source_node->name);
        if (SUCCESS == ans) {
            ans = updateFile(target_full_path + source_node->name, readFileContent(getFullPathByNode(source_node)));
            if (SUCCESS == ans) {
                return ans;
            }
            else {
                deleteFileOrDirectory(target_full_path + source_node->name);
                return ans;
            }
        }
        else {
            deleteFileOrDirectory(target_full_path + source_node->name);
            return ans;
        }
    }
    //对待复制项为文件夹的情况进行处理
    else {
        //获取源文件夹本身与其父亲的节点
        directoryList source_node = getNodeByInput(source_path);
        directoryList source_father = source_node->father;
        //父亲节点的绝对路径长度
        int source_father_path_size = getFullPathByNode(source_father).size();
        //目的文件夹的绝对路径
        string target_full_path = getFullPathByInput(target_path);
        //首先将空的源文件夹写入目的文件夹中
        int ans = insertIntoDisk(target_full_path, source_node->name);
        if (SUCCESS != ans) {
            return ans;
        }
        //中序遍历源文件夹节点的子树进行复制
        directoryList current = source_node->child;
        directoryList stack[STACK_SIZE];
        directoryList created_stack[STACK_SIZE];
        int top = 0;
        int created_top = 0;
        while (0 != top || NULL != current) {
            if (NULL == current) {
                current = stack[--top];
            }
            else {
                string t_path = target_full_path + getFullPathByNode(current->father).substr(source_father_path_size);
                ans = insertIntoDisk(t_path, current->name);
                if (SUCCESS == ans) {
                    if (isFile(t_path + current->name)) {
                        ans = updateFile(t_path + current->name, readFileContent(getFullPathByNode(current)));
                        if (SUCCESS == ans) {
                            created_stack[created_top++] = current;
                            stack[top++] = current->brother;
                            current = current->child;
                        }
                        else {
                            deleteFileOrDirectory(t_path + current->name);
                            break;
                        }
                    }
                }
                else {
                    break;
                }
            }
        }
        if (0 == top) {
            return SUCCESS;
        }
        else {
            for (int i = created_top - 1; i >= 0; i--) {
                current = created_stack[i];
                string t_path = target_full_path + getFullPathByNode(current->father).substr(source_father_path_size);
                deleteFileOrDirectory(t_path + current->name);
            }
            return ans;
        }
    }
}

int DiskManager::cutFileOrDirectory(string source_path, string target_path)
{
    //确定输入的源文件/文件夹与目标文件夹合法
    if (NULL == getNodeByInput(source_path) || NULL == getNodeByInput(target_path)
        || true == isFile(target_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    //将源文件/文件夹与目的文件夹均变换为绝对路径
    source_path = getFullPathByInput(source_path);
    target_path = getFullPathByInput(target_path);
    //若目的文件夹是源文件夹的子文件夹，则该剪切操作无法进行，返回权限错误
    if (string::npos != target_path.find(source_path)) {
        return NO_DATA_IN_BUFFER;
    }
    //将源文件/文件夹复制到目的文件夹中
    int ans = copyFileOrDirectory(source_path, target_path);
    //若复制不成功，则返回错误信息
    if (SUCCESS != ans) {
        return ans;
    }
    //若复制成功，则需要将源文件/文件夹删除
    else {
        //若第一形参为文件，则直接将该文件删除
        if (true == isFile(getFullPathByInput(source_path))) {
            deleteFileOrDirectory(source_path);
            return SUCCESS;
        }
        //若第一形参为文件夹，则将该文件夹内的所有文件与文件夹以及该文件夹本身均删除
        //获取源文件夹的目录节点
        directoryList source_node = getNodeByInput(source_path);
        //中序遍历该文件夹的子树，删除每个文件与文件夹
        directoryList current = source_node->child;
        directoryList stack[STACK_SIZE];
        directoryList cutting_stack[STACK_SIZE];
        int top = 0;
        int cutting_top = 0;
        while (0 != top || NULL != current) {
            if (NULL == current) {
                current = stack[--top];
            }
            else {
                cutting_stack[cutting_top++] = current;
                stack[top++] = current->brother;
                current = current->child;
            }
        }
        for (int i = cutting_top - 1; i >= 0; i--) {
            current = cutting_stack[i];
            deleteFileOrDirectory(getFullPathByNode(current));
        }
        //删除源文件夹本身
        deleteFileOrDirectory(source_path);
        return SUCCESS;
    }
}

int DiskManager::renameFileOrDirectory(string input_path, string target_name)
{
    if (NULL == getNodeByInput(input_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    else {
        vector<string> files = getAllFileOrDirectoryOfPath(getFullPathByNode(getNodeByInput(input_path)->father));
        for (vector<string>::iterator iter = files.begin(); iter != files.end(); iter++) {
            cout << *iter  << "------------------------------------------------ " << target_name << endl;

            if (true == isSameName(*iter, target_name)) {
                files.clear();
                return SAME_NAME_EXISTS_PROBLEM;
            }
        }
        files.clear();
        string full_path = getFullPathByInput(input_path);
        if (true == isFile(full_path)) {
            directoryList current = getNodeByInput(input_path);
            current->name = target_name;
            loadDirectoryToDisk();
            vector<string> buffer = readStream();
            string index_string = buffer[FILE_INDEX];
            int f_pos = (int)index_string.find("<" + full_path + ":", 0) + 1;
            int l_pos = f_pos + (int)full_path.size();
            string ans = index_string.substr(0, f_pos) + getFullPathByNode(current) + index_string.substr(l_pos);
            buffer[FILE_INDEX] = ans;
            writeStream(buffer);
            return SUCCESS;
        }
        else {
            directoryList current = getNodeByInput(input_path);
            current->name = target_name + "/";
            loadDirectoryToDisk();
            vector<string> buffer = readStream();
            string index_string = buffer[FILE_INDEX];
            int f_pos = 0;
            int l_pos = 0;
            while (string::npos != index_string.find("<" + full_path, l_pos)) {
                f_pos = (int)index_string.find("<" + full_path, l_pos) + 1;
                l_pos = f_pos + (int)full_path.size();
                index_string = index_string.substr(0, f_pos) + getFullPathByNode(current) + index_string.substr(l_pos);
            }
            buffer[FILE_INDEX] = index_string;
            writeStream(buffer);
            FCB * rename_fcb;
            rename_fcb = FindFCBinfo(full_path);
            if(!(rename_fcb==NULL)){
                Strcpy(rename_fcb->name,target_name);	//重命名不改变文件其他信息，修改时间无需改变
                vector<FCB *>::iterator iter;
                LoadIntoFCB();
                for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++)
                cout<<"重命名后FCB中存储的fcb信息："<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;

            }

            return SUCCESS;
        }
    }
    return 0;
}

DiskManager::~DiskManager()
{
    loadDirectoryToDisk();
    clearDirectory();
    delete(directory_list);
    directory_list = NULL;
    current_path = NULL;
}
