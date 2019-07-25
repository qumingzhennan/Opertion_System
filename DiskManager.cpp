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
    initialFCB();//��ʼ��
    loadDirectoryToList();//��ʼ��
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
        cout<<"��ʼ��FCB�д洢��fcb��Ϣ��"<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;
}


string DiskManager::getTime(void)
{
    struct tm *local,*ptr; //����tm�ṹָ��洢ʱ����Ϣ
    time_t t; //ʱ��ṹ���߶���
    t=time(NULL); //��ȡ��ǰϵͳ������ʱ��
    local=localtime(&t);//localtime()�����ǽ�����ʱ��ת��Ϊ����ʱ��
    ptr=gmtime(&t);//������ʱ��ת��Ϊ�����׼ʱ��
    //printf("The UTC time is %s/n",asctime(ptr)); //��ʽ����������׼ʱ��
    //printf("The local time is %s/n",ctime(&t));//�������ʱ��
    /*asctime()����(����Ϊtm�ṹָ��)��ctime()����(����Ϊtime_t�ṹ)��ʱ���Թ̶��ĸ�ʽ��ʾ���������ߵķ���ֵ����char*�͵��ַ��������ص�ʱ���ʽΪ�����ڼ� �·� ���� ʱ:��:�� ��/n/0 */
    return ctime(&t);
}
void DiskManager::Strcpy(char *p2,string p1){
    int i=0;
    for(;i<p1.length();i++){
        p2[i]=p1[i];
    }
    p2[i]='\0';
}
//�Ӵ����ж�������FCB��Ϣ����vector��ʽ����
void DiskManager::readFCB()
{
    fcbInfo.clear();
//  std::ifstream file1("FCB.dat",std::ios::binary);
//    long l,m;
//	  file1.seekg(0, ios::beg);
//    l = file1.tellg();
//    cout<<"�ļ�ͷ"<<l<<endl;
//    file1.seekg(0, ios::end);
//    cout<<"�ļ�β"<<m<<endl;
//	  cout <<"size of ";
//    cout <<" is "<< (m-l)<<" bytes.\n";
//    file1.close();
    std::ifstream file("FCB.dat",std::ios::binary);
    file.seekg(0,ios::end);
    long m = file.tellg();
//	cout<<"m��ֵ"<<m<<endl;
    file.seekg(0,ios::beg);
    if(m>0)
    while(!file.eof()){
            FCB *fcb1;
            fcb1 = new FCB;
            file.read((char*)fcb1, sizeof(FCB));
//     		cout<<"��--1:"<<fcb1->right<<endl;
//    		cout<<"��--2:"<<fcb1->size<<endl;
//			cout<<"��--3:"<<fcb1->type<<endl;
//	 		cout<<"��--4:"<<fcb1->time<<endl;
//			cout<<"��--5:"<<fcb1->name<<endl;
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
    cout<<"д��ǰFCB��С"<<sizeof(FCB);
    std:ofstream f("FCB.dat",ios::binary);
     if (f.is_open()) {
        vector<FCB *>::iterator iter;
        for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++) {
            f.write((char*)(*iter),sizeof(FCB));
//			cout<<"д--1:"<<(*iter)->right<<endl;
//    		cout<<"д--2:"<<(*iter)->size<<endl;
//			cout<<"д--3:"<<(*iter)->type<<endl;
//	 		cout<<"д--4:"<<(*iter)->time<<endl;
//			cout<<"д--5:"<<(*iter)->name<<endl;
        }
    }
    f.close();
    readFCB();
}



bool DiskManager:: add_fCB(string a_name,int a_type,int a_right)
{
    FCB *ne_fcb ;
    ne_fcb = new FCB;
    ne_fcb->type=a_type;//�ļ�����Ϊ0,�� 1
    ne_fcb->size=0;//�ļ���ʼ��СΪ0KB
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
            cout<<"�����丸�ļ��д�С"<<endl;
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
            cout<<"���º�FCB�д洢��fcb��Ϣ��"<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;
        return true;
    }
    else{
        cout<<"�Ҳ�������ʱ�Ҳ���FCB"<<endl;
        return false;
    }
}

FCB* DiskManager::FindFCBinfo(string file_name){
    readFCB();

    //vector<FCB *>::iterator iter;
    //for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++)
    //	cout<<"����vector�д洢��fcb��Ϣ��"<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;

    vector<FCB *>::iterator iter2;
    for (iter2 = fcbInfo.begin(); iter2!= fcbInfo.end(); iter2++) {
        //cout<<"Ҫ�������ļ����֣�"<<file_name<<endl;
        //cout<<"���������֣�"<<(*iter2)->name<<endl;
        if((*iter2)->name==file_name)
            return *(iter2);
    }
    cout<<"�Ҳ�����Ӧ�ļ������ԣ�"<<endl;
    return NULL;
}
bool DiskManager::dele_fCB(string d_name){

    cout<<"ɾ��FCB"<<endl;
    vector<FCB *>::iterator iter2;
    int pos=0;
    for (iter2 = fcbInfo.begin(); iter2!= fcbInfo.end(); iter2++,pos++) {
    //	cout<<"ɾ��--Ҫ�������ļ����֣�"<<d_name<<endl;
    //    cout<<"ɾ��--���������֣�"<<(*iter2)->name<<endl;
        if((*iter2)->name==d_name){
            fcbInfo.erase(iter2);
            LoadIntoFCB();
            return true;
        }
    }
    cout<<"�޷��ҵ���ӦFCB�飬ɾ��FCB��ʧ�ܣ�"<<endl;
    return false;
}
//������ȡĿ¼�ڵ�ľ���·��������Ŀ¼�ڵ�Ϊ�գ��򷵻س���Ϊ0����
string DiskManager::getFullPathByNode(directoryList node)
{
    //����Ŀ¼�ڵ�Ϊ�գ��򷵻س���Ϊ0���ַ���
    if (NULL == node) {
        cout<<"this node is null"<<endl;
        return "";
    }
    string ans = "";
    //ѭ���ڼ����丸�����֣�ֱ����Ŀ¼
    while (NULL != node) {
        ans = node->name + ans;
        node = node->father;
       // cout<<"ans:"<<ans<<endl;
    }

  //  cout<<"���ص�DiskManager::getFullPathByNodeֵΪ"<<ans<<endl;

    return ans;
}

//��ȡ���·����Ӧ��Ŀ¼�ڵ㣬����Ŀ¼�ڵ㲻���ڣ��򷵻�NULL
directoryList DiskManager::getNodeByInput(string input_path)
{
    directoryList ans = NULL;//�ļ��ڵ�
    //������·��Ϊ����·������Ӹ�Ŀ¼��ʼ����
    if (0 < input_path.length() && '/' == input_path[0]) {
        input_path = input_path.substr(1, (int)input_path.size() - 1);
  //      cout<<"input_path______here������"<<input_path<<endl;
        ans = directory_list;
    }
    //�����·��Ϊ���·������ӵ�ǰ·����ʼ����
    else {
        ans = current_path;
    }
    //posָ����·����ʼ����next_posָ����һ��·����ʼ��
    int pos = 0, next_pos = 0;
    //temp_path��ʾ��ǰ��·��
    string temp_path;
    //��ʶ�Ƿ��ȡ���ļ���
    bool over = true;
    for (pos = 0; pos <= (int)input_path.size() - 1; pos = next_pos) {
        //��ȡ���ļ���
        if (string::npos == input_path.find('/', pos)) {
            temp_path = input_path.substr(pos, (int)input_path.size() - pos);
            over = false;
        }
        //��ȡ��Ŀ¼��
        else {
            next_pos = (int)input_path.find('/', pos) + 1;
            temp_path = input_path.substr(pos, next_pos - pos);
        }
        //��ȡ��../��ָ��·��
        if ("../" == temp_path) {
            if (NULL != ans->father)
                ans = ans->father;
        }
        //����temp_path��ȡĿ¼�ڵ�
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
        //��ȡ���ļ���������
        if (false == over) {
            break;
        }
    }
    return ans;
}

//��ȡһ�����·���ľ���·��������·�������ڣ��򷵻س���Ϊ0���ַ���
string DiskManager::getFullPathByInput(string input_path)
{
    //��ȡ�����·����Ӧ��Ŀ¼�ڵ㣬����Ŀ¼�ڵ㲻���ڣ��򷵻�NULL
    directoryList node = getNodeByInput(input_path);
    //��ȡĿ¼�ڵ�ľ���·��������Ŀ¼�ڵ�ΪNULL���򷵻س���Ϊ0���ַ���
    string ans = getFullPathByNode(node);

    //cout<<"getFullPathByInput(string input_path)"<<ans<<endl;
    return ans;
}

//�ж�һ��·���Ƿ�Ϊ�ļ�
bool DiskManager::isFile(string name)
{
    if (0 < name.length() && '/' == name[(int)name.size() - 1]) {
        return false;
    }
    else {
        return true;
    }
}

//���ļ��е����һ��/ȥ��
string DiskManager::getDirectorySelfName(string name)
{
    //��ȷʵΪĿ¼��ȥ�����һ��б��
    if (false == isFile(name)) {
        name = name.substr(0, (int)name.size() - 1);
    }
    return name;
}

//�Ӵ����ж���������Ϣ����vector��ʽ����
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

//��ȫ����Ϣд�����
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

//���Ŀ¼����ֻ�����ڵ�/
void DiskManager::clearDirectory()
{
    //�������Ŀ¼�������ɾ��
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

//�����̶���Ŀ¼����
void DiskManager::loadDirectoryToList()
{
    //����ڴ�Ŀ¼����
    clearDirectory();
    //��������е�Ŀ¼��Ϣ
    vector<string> buffer = readStream();
    //vector<string> buffer2 = readFCBStream();




//    buffer.front();
   // vector<string>::iterator i = buffer.begin();
    //DIRECTORY_INFO

    cout<<"buffersize"<<buffer.size()<<endl;
//   cout<<buffer.at(0);
    string directory=buffer[0];


   // string directory = buffer.front();


    //�����ĸ������ֱ��¼��Ŀ¼����Ŀ¼������λ��
    int father_first = 0, father_last = 0;
    int child_first = 0, child_last = 0;
    //��λÿһ������Ŀ¼�ԣ�������Ŀ¼�Ľڵ㲢���ӵ�Ŀ¼������
    for (int current = 0; current <= (int)directory.size() - 1; current++) {
        //��¼��Ŀ¼�Ŀ�ʼλ��
        if ('<' == directory[current]) {
            father_first = current + 1;
        }
        //��¼��Ŀ¼�Ľ���λ������Ŀ¼�Ŀ�ʼλ��
        else if (',' == directory[current]) {
            father_last = current;
            child_first = current + 1;
        }
        //��¼��Ŀ¼�Ľ���λ��
        else if ('>' == directory[current]) {
            child_last = current;
            if (father_first < father_last && child_first < child_last) {
                directoryList current_directory = directory_list;
                //��ȡ����Ŀ¼����Ŀ¼
                string father_path = directory.substr(father_first, father_last - father_first);
                string child_name = directory.substr(child_first, child_last - child_first);
                //���ݸ�·����ָ��ָ����ȷλ��
                for (int pos = 0; pos <= (int)father_path.size() - 1; pos = (int)father_path.find('/', pos) + 1) {
                    string temp_path = father_path.substr(pos, (int)father_path.find('/', pos) - pos + 1);
                    if (0 != pos) {
                        current_directory = current_directory->child;
                    }
                    while (NULL != current_directory && temp_path != current_directory->name)
                        current_directory = current_directory->brother;
                }
                //��·���ڵ����޺��ӽڵ�ʱ���������ӽڵ㲢��ֵ
                if (NULL == current_directory->child) {
                    current_directory->child = new directoryNode;
                    current_directory->child->father = current_directory;
                    current_directory = current_directory->child;
                }
                //��·�����к��ӽڵ�ʱ���ҵ��ú��ӽڵ�����һ���ֵܽڵ㣬�����ýڵ���ֵܽڵ�Ĳ���ֵ
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

//��Ŀ¼�ļ��������
void DiskManager::loadDirectoryToDisk() {
    //�������Ŀ¼�ļ�
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
                //��Ŀ¼�Բ���Ŀ¼�Դ��ַ�����
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
    //��Ŀ¼�Դ�д�����
    vector<string> buffer = readStream();
    buffer[DIRECTORY_INFO] = ans;
    writeStream(buffer);
}

//string��ת����int��
int DiskManager::stringToInt(string temp)
{
    stringstream ss;
    ss << temp;
    int ans;
    ss >> ans;
    return ans;
}

//int��ת����string��
string DiskManager::intToString(int temp)
{
    string ans;
    stringstream ss;
    ss << temp;
    ss >> ans;
    return ans;
}
//��ȡ��ǰ·���ľ���·��
string DiskManager::getCurrentPath()
{
    string ans = getFullPathByNode(current_path);
    return ans;
}

//���ĵ�ǰ·�����������·���������򷵻�false
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

//��ȡһ���ļ����µ������ļ����ļ���
vector<string> DiskManager::getAllFileOrDirectoryOfPath(string input_path)
{
    vector<string> ans;
    //��ȡ���ļ��е�Ŀ¼�ڵ�
    directoryList father_node = getNodeByInput(input_path);
    //��������е��ӽڵ�����
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
            //cout<<"show����ı���current->brother->name"<<current->brother->name<<endl;
            current = current->brother;

        }
    }
    return ans;
}

//�ж������·���Ƿ����
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

//�ж������ļ����ļ����Ƿ�����
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
    //�����ǽ����ļ������ݡ�����ɾ��
    vector<string> buffer = readStream();
    //temp��¼�ļ�����
    string temp = buffer[FILE_INDEX];
    int pos = (int)temp.find(full_name, 0);
    //posָ����ļ�����
    pos += (int)full_name.size() + 1;
    //index_int��¼�ļ�����
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
    //ipos��¼���ļ������Ľ�β
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
    //�����ǽ����ļ���Ŀ¼��ɾ��
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
        //�����ǽ����ļ���Ŀ¼��ɾ��
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
        ans += buffer[posi];//��ÿҳ��ȡ�ļ�����
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
  //      cout<<"father_node�Ƿ�Ϊ��"<<endl;
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

   //         cout<<"������"<<endl;
            return OUT_OF_DISK_CPACITY_PROBLEM;



        }
    }
    else {
//        cout<<SUCCESS<<"SUCCESS��"<<endl;
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
    //modified_size��¼�ļ���Ҫ�Ŀ���
    int content_size = (int)content.size();
    int modified_size = (content_size - 1) / BLOCK_SIZE + 1;
    vector<string> buffer = readStream();
    //temp��¼�ļ�����
    string temp = buffer[FILE_INDEX];
    int pos = (int)temp.find("<" + full_name + ":", 0) + 1;
    //posָ����ļ�����
    pos += (int)full_name.size() + 1;
    //index_int��¼�ļ�����
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
    //ipos��¼���ļ������Ľ�β
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
                string index_temp = intToString(*iter);//�ļ���ҳ
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
    //ȷ�������Դ�ļ�/�ļ�����Ŀ���ļ��кϷ�
    if (NULL == getNodeByInput(source_path) || NULL == getNodeByInput(target_path)
        || true == isFile(target_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    //��Դ�ļ�/�ļ�����Ŀ���ļ��о��任Ϊ����·��
    source_path = getFullPathByInput(source_path);
    target_path = getFullPathByInput(target_path);
    //��Ŀ���ļ�����Դ�ļ��е����ļ��У���ø��Ʋ����޷����У�����Ȩ�޴���
    if (string::npos != target_path.find(source_path)) {
        return NO_DATA_IN_BUFFER;
    }
    //�Դ�������Ϊ�ļ���������д���
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
    //�Դ�������Ϊ�ļ��е�������д���
    else {
        //��ȡԴ�ļ��б������丸�׵Ľڵ�
        directoryList source_node = getNodeByInput(source_path);
        directoryList source_father = source_node->father;
        //���׽ڵ�ľ���·������
        int source_father_path_size = getFullPathByNode(source_father).size();
        //Ŀ���ļ��еľ���·��
        string target_full_path = getFullPathByInput(target_path);
        //���Ƚ��յ�Դ�ļ���д��Ŀ���ļ�����
        int ans = insertIntoDisk(target_full_path, source_node->name);
        if (SUCCESS != ans) {
            return ans;
        }
        //�������Դ�ļ��нڵ���������и���
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
    //ȷ�������Դ�ļ�/�ļ�����Ŀ���ļ��кϷ�
    if (NULL == getNodeByInput(source_path) || NULL == getNodeByInput(target_path)
        || true == isFile(target_path)) {
        return TARGET_NOT_EXIST_PROBLEM;
    }
    //��Դ�ļ�/�ļ�����Ŀ���ļ��о��任Ϊ����·��
    source_path = getFullPathByInput(source_path);
    target_path = getFullPathByInput(target_path);
    //��Ŀ���ļ�����Դ�ļ��е����ļ��У���ü��в����޷����У�����Ȩ�޴���
    if (string::npos != target_path.find(source_path)) {
        return NO_DATA_IN_BUFFER;
    }
    //��Դ�ļ�/�ļ��и��Ƶ�Ŀ���ļ�����
    int ans = copyFileOrDirectory(source_path, target_path);
    //�����Ʋ��ɹ����򷵻ش�����Ϣ
    if (SUCCESS != ans) {
        return ans;
    }
    //�����Ƴɹ�������Ҫ��Դ�ļ�/�ļ���ɾ��
    else {
        //����һ�β�Ϊ�ļ�����ֱ�ӽ����ļ�ɾ��
        if (true == isFile(getFullPathByInput(source_path))) {
            deleteFileOrDirectory(source_path);
            return SUCCESS;
        }
        //����һ�β�Ϊ�ļ��У��򽫸��ļ����ڵ������ļ����ļ����Լ����ļ��б����ɾ��
        //��ȡԴ�ļ��е�Ŀ¼�ڵ�
        directoryList source_node = getNodeByInput(source_path);
        //����������ļ��е�������ɾ��ÿ���ļ����ļ���
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
        //ɾ��Դ�ļ��б���
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
                Strcpy(rename_fcb->name,target_name);	//���������ı��ļ�������Ϣ���޸�ʱ������ı�
                vector<FCB *>::iterator iter;
                LoadIntoFCB();
                for (iter = fcbInfo.begin(); iter != fcbInfo.end(); iter++)
                cout<<"��������FCB�д洢��fcb��Ϣ��"<<(*iter)->name<<' '<<(*iter)->size<<' '<<(*iter)->right<<' '<<(*iter)->time<<' '<<(*iter)->type<<endl;

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
