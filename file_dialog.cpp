#include "file_dialog.h"
#include "ui_file_dialog.h"
#include<QDebug>
#include<QMessageBox>
#include<QMenu>
#include<QIcon>
#include<QtMath>

file_Dialog::file_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_Dialog)
{
    ui->setupUi(this);
    this->open_text_count=0;
for(int i=0;i<20;i++)
{
    this->text[i]=new text_Dialog(this);
    connect(this->text[i],SIGNAL(sendtopro_id(int,string,string,int)),this,SLOT(getmesfromtext(int,string,string,int)));
    this->text[i]->hide();
}
    cur_treeItem=new QTreeWidgetItem;
    QIcon title(":/icon/file.jpg");
    this->setWindowIcon(title);
    this->setWindowTitle("File Manager");
    QIcon back(":/icon/back.png");
    QIcon go(":/icon/refresh.png");
    ui->back->setIcon(back);
    ui->back->setIconSize(QSize(26,20));
    ui->go->setIcon(go);
    ui->go->setIconSize(QSize(26,20));
    this->file_controler=new file_control;
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    this->Open_filecon();
    ui->filename->hide();
    ui->file_close->hide();
    ui->pushButton->hide();
    ui->textEdit->hide();
    ui->label->hide();
    this->id_count=0;
}

file_Dialog::~file_Dialog()
{
    delete ui;
}

void file_Dialog::appendfile(QString  name,int row,int flag,QStandardItem* parent_item)//flag=0��ʾ���ڸ�Ŀ¼����չ =1��ʾ������Ŀ¼��չ����Ҫ�и��׽ڵ�
{
    QStandardItem* itemProject = new QStandardItem(name);
    //QStandardItem* itemProjectsize = new QStandardItem("2048K");
    //QStandardItem* itemProjectdata = new QStandardItem("2017/4/26 18:15");
    //QStandardItem* itemProjectChild = new QStandardItem(name+"ios");
    //QStandardItem* temp=new QStandardItem(name);

    //model->setItem(row,1,itemProjectsize);
    //model->setItem(row,2,itemProjectdata);
    //itemProjectChild->setChild(0,temp);
    //itemProject->setChild(0,itemProjectChild);
    if(flag==0)//��չ����Ŀ¼  ��������ͼ��
    {
        model->setItem(row,0,itemProject);
    }
     else
      {
          parent_item->setChild(0,itemProject);
      }
    model->setItem(row,itemProject);

}

void file_Dialog::getcur(const QModelIndex &index)
{

}


QModelIndex file_Dialog::getcurRoot(QModelIndex itemIndex)
{
    QModelIndex secondItem = itemIndex;
        while(itemIndex.parent().isValid())
        {
            secondItem = itemIndex.parent();
            itemIndex = secondItem;
        }
        if(secondItem.column() != 0)
        {
             secondItem = secondItem.sibling(secondItem.row(),0);
        }
        return secondItem;
}

void file_Dialog::Open_filecon()
{
    this->file_controler->show_cur_folder();//���е�С����
    setTree();
    //this->lastpath_flag=1;
    show_cur_add();
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setFrameShape(QFrame::StyledPanel); //�����ޱ߿�
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //
    //
    QStringList header;
        header<<"FileName           "<<"Type                "<<"Size                "<<"Attribute          "<<"Create Data           ";
        ui->tableWidget->setHorizontalHeaderLabels(header);
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        ui->tableWidget->verticalHeader()->setVisible(false);//������ߴ�ֱ
}

void file_Dialog::add_file(int row,int colume,QString name)
{
    ui->tableWidget->setItem(row,colume,new QTableWidgetItem(name));
}


void file_Dialog::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    //ѡ���ļ���
    //if(item->text(1)=="�ļ�")
    //{
    /*QString s1=QString::fromStdString(this->file_controler->open_file(filename));
    qDebug()<<"Flag"+s1;
    this->file_controler->change_cur_address();
    show_cur_add();
    //}
    else
    {
        if(this->file_controler->fm->setPresentPath(get_fullpath(item->text()))){
           qDebug()<<"OK";
             this->file_controler->curpath=this->file_controler->fm->getPresentPath();
        }
        else
            qDebug()<<"bug";
        show_cur_add();
    }*/
}

void file_Dialog::setTree()
{
    ui->treeWidget->clear();
    ui->treeWidget->setWindowTitle("QTreeWidget");
    ui->treeWidget->setHeaderLabels(QStringList()<<"Current Catalg");
    //FileManager *fm = FileManager::getIncetence();
    string father_path = "/";
    string current_path = "";
    vector<string> temp_string;
    //�趨������
    QTreeWidgetItem *head =new QTreeWidgetItem(QStringList()<<"/");
    head->setData(0,12,"/");
    ui->treeWidget->addTopLevelItem(head);

    QTreeWidgetItemIterator  it (ui->treeWidget);
    QIcon file(":/icon/choose.png");
    QIcon folder(":/icon/dir.png");
    while((*it))
    {
        QTreeWidgetItem *currentChild = (*it);
        father_path = currentChild->data(0,12).toString().toStdString();
        temp_string =this->file_controler->fm->getDm()->getAllFileOrDirectoryOfPath(father_path);
        for (int i = 0; i < temp_string.size(); i++)
        {
            //

            int flag=0;//0  �����ļ�  1�����ļ���
            //
            current_path = father_path+temp_string[i];
            QStringList columItemList;
            QTreeWidgetItem *child;
            QString key,value;
            if(temp_string[i].substr(temp_string[i].length()-1,1) == "/")//���һ���ַ������/˵�����ļ���
            {
                key = QString::fromStdString(temp_string[i].substr(0,temp_string[i].length()-1));
               // qDebug()<<"setTree(folder)<<"<<key;
                flag=1;
            }
            else//���ļ�
            {
                 key = QString::fromStdString(temp_string[i]);
                 //qDebug()<<"setTree(file)<<"<<key;
                 flag=0;
            }
            value = QString::fromStdString(current_path);

            columItemList << key;
            child = new QTreeWidgetItem(columItemList);
            child->setData(0,12,value);
            if(flag==0)
            child->setIcon(0,file);
            else
            child->setIcon(0,folder);
            currentChild->addChild(child);
        }
        it++;
    }

}

void file_Dialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString s=item->text(0);
    this->cur_file=s;
    //qDebug()<<"current choose file/folder:"<<this->cur_file;
    string filename=s.toStdString();
    ui->tableWidget->clearContents();
    //
    FCB * fcb;
    fcb= this->file_controler->fm->FindFCB((get_fullpath(item)));
    if(fcb==NULL)//�ļ���
    {
        this->file_controler->curpath=get_fullpath(item)+"/";
        this->file_controler->show_cur_folder();
        //
        show_cur_add();
        this->temp_cur_file=this->cur_file+"/";
    }
    else//�ļ�
       {        
       //cout<<"BUBUBU "<<this->last_path<<endl;
        //cout<<"UNUBUB "<<this->cur_file.toStdString()<<endl;
        if(!is_file_opened(filename))//����ļ�δ��**********************************************************************/
        {
            this->file_controler->curpath=get_fullpath(item);
            qDebug()<<QString::fromStdString(this->file_controler->curpath);
            QString s1=QString::fromStdString(this->file_controler->open_file(filename));
            //this->ID_name[id_count]=this->cur_file.toStdString();
             //cout<<"this->open_text_count="<<this->open_text_count<<endl;
        this->text[this->open_text_count]->ui->textEdit->setText(s1);
            this->text[this->open_text_count]->name=filename;
    this->text[this->open_text_count]->order=this->open_text_count;
            this->text[this->open_text_count]->full_path=this->file_controler->curpath;

                  emit sendtopro(2,s1.toStdString(),1);//�򿪲���
  int last=this->open_text_count-1;//��Ϊgetid��ʱ���1��
  QString title="FileName:"+QString::fromStdString(this->text[last]->name)+"  Pro_Id:"+QString::number(this->text[last]->id,10);
this->text[last]->setWindowTitle(title);
 this->text[last]->show();
                //cout<<"UNUBUB "<<endl;

                       /*else
                       {
                         QMessageBox::about(NULL,"Tip","You have opened too many file");
                       }*/
            //close ��ʱ��Ҫ�����źŹ����ر�
        this->file_controler->change_cur_address();
        show_cur_add();
        this->temp_cur_file=this->cur_file;
        }//�ж��ļ��Ƿ��������
        else
            QMessageBox::about(NULL,"Can't open","OPen failed.The file is already opened");
       }
    string temp=this->temp_cur_file.toStdString();

    //cout<<"curpath does not fixed"<<temp<<endl;
   //cout<<"*****66"<<this->file_controler->fm->setPresentPath(temp);//��֪���õĶԲ���


      //
    if(fcb==NULL)
    {
    int t_row=1,t_column=0;
    for(int i=0;i<this->file_controler->count;i++)
    {
        s=QString::fromStdString(this->file_controler->file_fcb[i]->name);//����
        add_file(t_row,t_column,s);
        t_column++;

        if(this->file_controler->file_fcb[i]->type==0)
        {
         s="Folder";
         add_file(t_row,t_column,s);
         t_column++;
        }
        else
        {
         s="File";
         add_file(t_row,t_column,s);
         t_column++;
        }
         s=QString::number(this->file_controler->file_fcb[i]->size,10);//��С
        add_file(t_row,t_column,s);
        t_column++;

        if(this->file_controler->file_fcb[i]->right==0)
        s="Read Only";//Ȩ��
        else if(this->file_controler->file_fcb[i]->right==1)
        s="Read && Write";
        else
        s="Read && Delete";
       add_file(t_row,t_column,s);
        t_column++;

        s=QString::fromLocal8Bit(this->file_controler->file_fcb[i]->time);//����
        add_file(t_row,t_column,s);
        t_column++;
        t_row++;
    }
    }
    else
    {
        int t_row=1,t_column=0;
        s=QString::fromStdString(fcb->name);//����
        add_file(t_row,t_column,s);
        t_column++;

        s="File";
        add_file(t_row,t_column,s);
        t_column++;

        s=QString::number(fcb->size,10);//��С
       add_file(t_row,t_column,s);
       t_column++;

       if(fcb->right==0)
       s="Read Only";//Ȩ��
       else if(fcb->right==1)
       s="Read && Write";
       else
       s="Read && Delete";
      add_file(t_row,t_column,s);
       t_column++;

       s=QString::fromLocal8Bit(fcb->time);//����
       add_file(t_row,t_column,s);
    }

}

string file_Dialog::get_fullpath(QTreeWidgetItem *item)
{
    int count=0;
    string full;
    QString filename[20],father_path;
    while(item->parent()!=NULL)
    {
        filename[count]=item->text(0);
        //qDebug()<<filename[count];
        item=item->parent();
        count++;
    }
    for(int i=count;i>=0;i--)
    {
        if(i!=0)
        father_path+=filename[i]+"/";
        else
        father_path+=filename[i];

    }
    //qDebug()<<"(get_fullpath):"<<father_path;
    full=father_path.toStdString();

    return full;
}

void file_Dialog::show_cur_add()
{
    QString curpath=QString::fromStdString(this->file_controler->curpath);
    ui->cur_address->setText(curpath);
    this->get_lastpath();//ÿһ��·���ı䶼Ҫ�ı��ϼ�·��
    this->file_controler->fm->present_path=this->last_path;
    //cout<<"12456   "<<this->last_path;
}

void file_Dialog::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *mene=new QMenu(ui->tableWidget);
    QMenu *submene=new QMenu(ui->tableWidget);
    QMenu *mene2=new QMenu(ui->tableWidget);
    QAction *action=new QAction("Copy",ui->tableWidget);
    QAction *action1=new QAction("Paste",ui->tableWidget);
    QAction *action2=new QAction("Delete",ui->tableWidget);
    QAction *action3=new QAction("Rename",ui->tableWidget);
    QAction *action4=new QAction("New",ui->tableWidget);

    QAction *subaction=new QAction("File",ui->tableWidget);
    QAction *subaction2=new QAction("Folder",ui->tableWidget);
    mene->addAction(action);
    mene->addAction(action1);
    mene->addAction(action2);
    mene->addAction(action3);
    mene->addAction(action4);

    submene->addAction(subaction);
    submene->addAction(subaction2);
    action4->setMenu(submene);

    mene2->addAction(action4);
    connect(subaction,SIGNAL(triggered(bool)),this,SLOT(get_filename()));
    connect(subaction2,SIGNAL(triggered(bool)),this,SLOT(get_fil_folderename()));
    connect(action2,SIGNAL(triggered(bool)),this,SLOT(delete_file()));
    //connect(action,SIGNAL(triggered(bool),this,SLOT(copy())));

    if(click_flag==1)//ѡ���ļ�
    mene->exec(QCursor::pos());
    else if(click_flag==2)//δѡ���ļ�
    mene2->exec(QCursor::pos());
    else
        ;
    click_flag=2;
}

void file_Dialog::get_filename()
{
    ui->label->show();
    this->file_type=0;
       ui->filename->show();
}

void file_Dialog::get_fil_folderename()
{
    ui->label->show();
    this->file_type=1;
       ui->filename->show();
}

void file_Dialog::new_file()
{
    //�޸�!!
    this->file_controler->fm->present_path=this->file_controler->curpath;
    this->file_controler->fm->setPresentPath(this->file_controler->curpath);

    //cout<<"create new file ----"<<this->file_controler->fm->present_path;
    string text="";
    //this->ID_name[id_count]=this->cur_file.toStdString();
    emit sendtopro(0,text,1);//�½�����
    this->file_controler->new_file(this->temp_right,this->filename);
}

void file_Dialog::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    qDebug()<<item->text();
    if(item->text()!="NULL")
    {
    if(item->text().right(1)=="/")
    {
        click_flag=0;
        qDebug()<<"Choose folder";
    }
    else
    {
        click_flag=1;
        qDebug()<<"choose file";
    }
    }
    else
        click_flag=3;//����Ϊ
}

void file_Dialog::on_filename_returnPressed()
{
    ui->label->hide();
    ui->filename->hide();
    if(ui->filename->text()!="")//Ϊ���Ǵ������
    {
    if(rename_flag!=1)
    {
    QString s=ui->filename->text();
    this->filename=s.toStdString();

    if(this->file_type==0)//�ļ�
    this->new_file();
    else//�ļ���
    {
        this->filename+="/";

        //�޸�!!
        this->file_controler->fm->present_path=this->file_controler->curpath;
        this->file_controler->fm->setPresentPath(this->file_controler->curpath);



        this->file_controler->new_file_folder(this->temp_right,this->filename);
        //this->file_controler->curpath=this->file_controler->fm->getPresentPath();
    }
    setTree();
    show_cur_add();
    }
    else//Ϊ1˵��������������
    {
     this->rename_flag=0;
    this->rename_file();
    }
    }
}

void file_Dialog::on_go_clicked(bool checked)//ת����·���͸Ĺ�ȥ
{
    QString name=ui->cur_address->text();
    string s=name.toStdString();
    text_Dialog *mes=new text_Dialog;
     //qDebug()<<"this->file_controler->curpath"<<name;
    string text=this->file_controler->open_file(s);
    cout<<"text="<<text<<endl<<endl;    //mes->ui
    this->file_controler->change_cur_address();
    this->file_controler->fm->present_path=this->file_controler->curpath;
    show_cur_add();
}

/*void file_Dialog::on_pushButton_clicked(bool checked)
{
    //ui->textEdit->hide();
    //ui->pushButton->hide();
    //ui->file_close->hide();
    string text=ui->textEdit->toPlainText().toStdString();
    qDebug()<<"close_file:"<<QString::fromStdString(this->file_controler->curpath);
    //isopen Ҫ������һ����·��
    QString s;
    if(this->file_controler->fm->isFileOpened(this->file_controler->curpath,this->temp_cur_file.toStdString()))
    {

        if(this->file_controler->close_file(this->cur_file.toStdString(),text))
        {
            s="File["+this->cur_file+"]close success";
            QMessageBox::about(NULL,"Tip",s);
        }
        else
        {
            s="File["+this->cur_file+"]close fail";
            QMessageBox::about(NULL,"Error",s);
        }
    }
    else
    {
        s="File["+this->cur_file+"]was not opened";
        QMessageBox::about(NULL,"Error",s);
    }
}*/

void file_Dialog::delete_file()
{

    this->file_controler->fm->present_path=this->file_controler->curpath;
    this->file_controler->fm->setPresentPath(this->file_controler->curpath);
    //ui->treeWidget->currentItem()->parent()->takeChild(ui->treeWidget->currentIndex().row());
    this->file_controler->del_file(this->filename);
    //qDebug()<<"OK filename"<<QString::fromStdString(this->filename);
    setTree();
}

void file_Dialog::get_re_name()
{
   this->rename_flag=1;
       ui->filename->show();
       ui->label->show();
}

void file_Dialog::rename_file()
{
    string t_name=ui->filename->text().toStdString();
    this->file_controler->rename(this->filename,t_name);
    setTree();
}

void file_Dialog::copy_file()
{
    is_copy=1;
    this->file_controler->lastpath=this->last_path;
    this->file_controler->copy_file(this->filename);
}

void file_Dialog::paste_file()
{
    is_copy=0;

    this->file_controler->fm->present_path=this->file_controler->curpath;
    this->file_controler->paste_file(this->filename);
}

void file_Dialog::cut_file()
{
    is_copy=1;
    this->file_controler->lastpath=this->last_path;
    this->file_controler->cut_file(this->filename);
}

void file_Dialog::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *mene=new QMenu(ui->treeWidget);

    QAction *action=new QAction("Copy",ui->treeWidget);
    QAction *action1=new QAction("Paste",ui->treeWidget);
    QAction *action2=new QAction("Delete",ui->treeWidget);
    QAction *action3=new QAction("Rename",ui->treeWidget);
    QAction *action4=new QAction("Cut",ui->treeWidget);
    mene->addAction(action);
    mene->addAction(action1);
    mene->addAction(action2);
    mene->addAction(action3);
    mene->addAction(action4);

    if(is_copy==0)
    {
        action1->setEnabled(false);
    }
    connect(action2,SIGNAL(triggered(bool)),this,SLOT(delete_file()));
    connect(action,SIGNAL(triggered(bool)),this,SLOT(copy_file()));
    connect(action1,SIGNAL(triggered(bool)),this,SLOT(paste_file()));
    connect(action3,SIGNAL(triggered(bool)),this,SLOT(get_re_name()));
    connect(action4,SIGNAL(triggered(bool)),this,SLOT(cut_file()));
    if(this->filename!="")
    mene->exec(QCursor::pos());
    else
        QMessageBox::about(NULL,"Operation Error","You are not choosing file/folder");

}

void file_Dialog::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)//��ȡ��һ��·������û����
{
    this->filename=item->text(column).toStdString();
    this->cur_treeItem=item;
    this->get_lastpath();
    //qDebug()<<item->text(column)<<"   "<<QString::fromStdString(this->last_path);
}

void file_Dialog::on_back_clicked()
{
    this->Open_filecon();
    this->file_controler->curpath=this->last_path;
    this->file_controler->fm->present_path=this->file_controler->curpath;
    //this->file_controler->fm->setPresentPath()

}

void file_Dialog::get_lastpath()
{
    QTreeWidgetItem *item;//һ��ʼ������ʱ��û��ָ�� cur_treeItem
    item=this->cur_treeItem;
    if(this->cur_treeItem!=NULL)
    {
    if(this->cur_treeItem->parent()!=NULL)//���ײ�Ϊ��˵���������ļ���
    {
    item=this->cur_treeItem->parent();
    this->last_path=get_fullpath(item)+"/";
    }
    else//˵�����ļ�
        this->last_path=get_fullpath(item);
    }
    else
       this->last_path="/";

}


/*void file_Dialog::on_file_close_clicked()
{
    string text=ui->textEdit->toPlainText().toStdString();
    findmatchid(this->cur_file.toStdString());

    emit sendtopro(1,text,this->Id[this->id_order]);//�رս���

    this->id_count--;
     this->on_pushButton_clicked(1);
    //
    this->get_lastpath();
    this->file_controler->curpath=this->last_path;
    this->file_controler->fm->present_path=this->last_path;
    show_cur_add();

}*/

bool file_Dialog::findmatchid(string name)
{

    for(int i=0;i<this->id_count;i++)
    {
        if(this->ID_name[i]==name)
        {
            this->id_order=i;
            return true;
        }
    }
    return false;
}

void file_Dialog::getid(int id)//id �ǽ��̺�
{
    this->text[this->open_text_count]->id=id;
    this->open_text_count++;

}

void file_Dialog::getmesfromtext(int id, string name, string full_path,int order)
{
    string my_text=this->text[order]->ui->textEdit->toPlainText().toStdString();

    emit sendtopro(1,my_text,id);//�رս���

    //�����һ����ܻ�������
    this->get_lastpath();
    this->file_controler->curpath=this->last_path;
    this->file_controler->fm->present_path=this->last_path;
    show_cur_add();

    qDebug()<<"close_file:"<<QString::fromStdString(full_path);
    //isopen Ҫ������һ����·��
    QString s;
    if(this->file_controler->fm->isFileOpened(full_path,name))
    {

        if(this->file_controler->close_file(name,my_text))
        {
            s="File["+QString::fromStdString(name)+"]close success";
            QMessageBox::about(NULL,"Tip",s);
        }
        else
        {
            s="File["+QString::fromStdString(name)+"]close fail";
            QMessageBox::about(NULL,"Error",s);
        }
    }
    else
    {
        s="File["+QString::fromStdString(name)+"]was not opened";
        QMessageBox::about(NULL,"Error",s);
    }
}

bool file_Dialog::is_file_opened(string name)
{
    int i=this->open_text_count-1;
    for(int j=0;j<i;j++)
    {
        if(this->text[j]->name==name)
            return true;
    }
    return false;
}
