#include "pro_dialog.h"
#include "ui_pro_dialog.h"
#include"process.h"
#include<QDebug>
#include<QMenu>
#include<QPoint>
#include<QTextCodec>
#include<QMessageBox>
Process_Dialog::Process_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Process_Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Task Manager");
    this->Pro=new process;
    this->fresh();
    //this->diaodu=1;//默认
    timer=new QTimer(this);
    timer->setInterval(1000);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(fresh_timer()));
    ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    this->open_pro();
}

Process_Dialog::~Process_Dialog()
{
    delete ui;
}

void Process_Dialog::fresh_timer()
{
    switch (this->scheduler) {
    case 1:
       this->Pro->R_R(1,1,1,1,1);
        this->setWindowTitle("Current: Round_Robin");
        //ui->label->setText("Current: Round_Robin");
        break;
    case 2:
       this->Pro->FCFS_pro(1,1,1,1,1);
        this->setWindowTitle("Current: FCFS(Pri)");
        //ui->label->setText("Current: FCFS(Pri)");
        break;
    case 3:
       this->Pro->SJF(1,1,1,1,1);
        this->setWindowTitle("Current: STFS");
       // ui->label->setText("Current: STFS");
        break;
    case 4:
       this->Pro->FCFS(1,1,1,1,1);
        this->setWindowTitle("Current: FCFS");
        //ui->label->setText("Current: FCFS");
        break;
    default:
        break;
    }    
    this->open_pro();
}

void Process_Dialog::fresh()
{
       ui->diaodu->clear();
       ui->timeslips->clear();
       ui->diaodu->insertItem(0,tr("default dispatch algorithm is R-R"));
       ui->diaodu->insertItem(1,tr("Round_robin"));
       ui->diaodu->insertItem(2,tr("First Come First Serve && Priority"));
       ui->diaodu->insertItem(3,tr("Shortest Task First Serve"));
       ui->diaodu->insertItem(4,tr("First Come First Serve"));
       ui->timeslips->insertItem(0,tr("1s"));
       ui->timeslips->insertItem(1,tr("2s"));
       ui->timeslips->insertItem(2,tr("4s"));


}

void Process_Dialog::on_diaodu_currentTextChanged(const QString &arg1)
{
}

void Process_Dialog::on_timeslips_currentTextChanged(const QString &arg1)
{
    QString time;
    time=ui->timeslips->currentText();
    if(time=="1s")
    {
        this->Pro->timeslip=1;
        //this->timer->setInterval(1000);
    }
    else if(time=="2s")
    {
        this->Pro->timeslip=2;
        //this->timer->setInterval(2000);
    }
    else if(time=="4s")
    {
        this->Pro->timeslip=4;
        //this->timer->setInterval(4000);
    }
}

void Process_Dialog::pro_opt_sure()
{
    //PPCB *  和PPCB  变量之间不可相互赋值
    int temp_id;
    //qDebug()<<"i:"<<this->Pro->i;
    if(this->type==0)//选取运行队列的
    {
		temp_id=this->choose_pro;
		temp.cputime=this->Pro->run_queue[this->choose_pro_index].cputime;
		temp.id=this->Pro->run_queue[this->choose_pro_index].id;
		temp.lefttime=this->Pro->run_queue[this->choose_pro_index].lefttime;
		temp.priority=this->Pro->run_queue[this->choose_pro_index].priority;
		temp.state=this->Pro->run_queue[this->choose_pro_index].state;
    }
    else
    {
        temp_id=this->choose_pro;
        //qDebug()<<"Next"<<this->choose_pro;
        temp.cputime=this->Pro->wait_queue[this->choose_pro_index].cputime;
        temp.id=this->Pro->wait_queue[this->choose_pro_index].id;
        temp.lefttime=this->Pro->wait_queue[this->choose_pro_index].lefttime;
        temp.priority=this->Pro->wait_queue[this->choose_pro_index].priority;
        temp.state=this->Pro->wait_queue[this->choose_pro_index].state;
    }
    if(this->Pro->i==1)//轮转调度算法
    {
         this->Pro->R_R(this->Pro->k,this->Pro->j,temp.cputime,temp.priority,this->choose_pro);
    }
    else if(this->Pro->i==2)//先来先服务加优先级
    {
         this->Pro->FCFS_pro(this->Pro->k,this->Pro->j,temp.cputime,temp.priority,temp.id);
    }
    else if(this->Pro->i==3)//短作业优先
    {
         this->Pro->SJF(this->Pro->k,this->Pro->j,temp.cputime,temp.priority,temp.id);
    }
    else if(this->Pro->i==4)//先来先服务
    {
         this->Pro->FCFS(this->Pro->k,this->Pro->j,temp.cputime,temp.priority,temp.id);
    }
    fresh();//刷新
    //ui->chooseid->setEnabled(0);
}

void Process_Dialog::open_pro()  
{
    //任务管理器
    ui->tableWidget_2->clear();
    QStringList header;
        header<<"  Process_ID "<<"  priority "<<"  cur_state "<<"  running_time  "<<"  rest_time  ";
                //
            ui->tableWidget_2->setRowCount(20);
            ui->tableWidget_2->setColumnCount(5);
                ui->tableWidget_2->setFrameShape(QFrame::NoFrame); //设置无边框
                ui->tableWidget_2->setShowGrid(false);
                ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
                ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
                    ui->tableWidget_2->setHorizontalHeaderLabels(header);
                    ui->tableWidget_2->resizeColumnsToContents();
                    ui->tableWidget_2->resizeRowsToContents();
                    ui->tableWidget_2->verticalHeader()->setVisible(false);//隐藏左边垂直
                //
        if(this->Pro->run_count!=0)
       {
        for(int i=1;i<=this->Pro->run_count;i++)
        {
            int k=i-1;
            QString s;
        for(int j=0;j<5;j++)
        {
            if(j==0)
            {
            s=QString::number(this->Pro->run_queue[k].id,10);

            }
            else if(j==1)
            {
             s=QString::number(this->Pro->run_queue[k].priority,10);
            }
            else if(j==2)
            {
                    s="Running";
            }
            else if(j==3)
            {
             s=QString::number(this->Pro->run_queue[k].cputime,10);
            }
            else if(j==4)
            {
             s=QString::number(this->Pro->run_queue[k].lefttime,10);
            }
            ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(s));
        }
        }
        //******************//
        }
        if(this->Pro->wait_count!=0)
        {
        for(int i=1;i<=this->Pro->wait_count;i++)
        {
            int k=i-1;
            int t=i+this->Pro->run_count;
            QString s;
        for(int j=0;j<5;j++)
        {
            if(j==0)
            {
            s=QString::number(this->Pro->wait_queue[k].id,10);

            }
            else if(j==1)
            {
             s=QString::number(this->Pro->wait_queue[k].priority,10);
            }
            else if(j==2)
            {
                    s="waiting";
            }
            else if(j==3)
            {
             s=QString::number(this->Pro->wait_queue[k].cputime,10);
            }
            else if(j==4)
            {
             s=QString::number(this->Pro->wait_queue[k].lefttime,10);
            }
            ui->tableWidget_2->setItem(t,j,new QTableWidgetItem(s));
        }
        }
        //******************//
        }
        this->timer->start();
}


void Process_Dialog::pro_opt(int op)
{
   /* if(op==1)//插入
    {
       // qDebug()<<"插入";
        this->Pro->k=2;
        this->Pro->j=1;
        this->type=1;
    }*/
     if(op==2)//阻塞
    {
        //qDebug()<<"阻塞";
        this->Pro->k=2;
         this->Pro->j=3;
        this->type=0;
    }
    else if(op==3)//唤醒
    {
        //qDebug()<<"wake"<<this->choose_pro;
        this->Pro->k=2;
        this->Pro->j=4;
        this->type=1;
    }
    else if(op==4)//删除
    {
       // qDebug()<<"删除";
        this->Pro->k=2;
        this->Pro->j=5;
        this->type=0;
    }
}

void Process_Dialog::insert()
{
    this->pro_opt(1);
    this->pro_opt_sure();
    this->open_pro();
    this->timer->start();
}

void Process_Dialog::back()
{
    this->pro_opt(2);
    this->pro_opt_sure();
    this->open_pro();
    this->timer->start();
}

void Process_Dialog::wake()
{
    /*qDebug()<<"wake";
        qDebug()<<this->choose_pro;*/
    this->pro_opt(3);
    this->pro_opt_sure();
    this->open_pro();
    this->timer->start();
}

void Process_Dialog::dele()  
{
    /*this->pro_opt(4);
    this->pro_opt_sure();*/
    /*qDebug()<<"delete";//+
    qDebug()<<this->choose_pro;*/
    this->Pro->findmatpcb(this->choose_pro);
    this->open_pro();
    this->timer->start();
}

void Process_Dialog::on_tableWidget_2_customContextMenuRequested(const QPoint &pos)
{
    this->timer->stop();
    QMenu *mene=new QMenu(ui->tableWidget_2);//运行进程
    QMenu *mene2=new QMenu(ui->tableWidget_2);//等待进程
    //QAction *action=new QAction("insert",ui->tableWidget_2);
    QAction *action1=new QAction("block",ui->tableWidget_2);
    QAction *action2=new QAction("wake_up",ui->tableWidget_2);
    QAction *action3=new QAction("delete",ui->tableWidget_2);
    //mene2->addAction(action);
    mene->addAction(action1);
    mene2->addAction(action2);
    mene->addAction(action3);
    //mene2->addAction(action3);
   // connect(action,SIGNAL(triggered(bool)),this,SLOT(insert()));
    connect(action1,SIGNAL(triggered(bool)),this,SLOT(back()));
    connect(action2,SIGNAL(triggered(bool)),this,SLOT(wake()));
    connect(action3,SIGNAL(triggered(bool)),this,SLOT(dele()));
    if(ui->tableWidget_2->currentColumn()==0)
    {
        this->choose_pro_index=ui->tableWidget_2->currentItem()->row();
        this->choose_pro_index--;
        //qDebug()<<"状态："<<this->Pro->all_queue[this->choose_pro_index].state;
        if(this->Pro->all_queue[this->choose_pro_index].state==2)//运行队列
        {
        this->choose_pro=ui->tableWidget_2->currentItem()->text().toInt();
        mene->exec(QCursor::pos());
        }
        else
        {
         this->choose_pro=ui->tableWidget_2->currentItem()->text().toInt();
          mene2->exec(QCursor::pos());
        }
    }
}

void Process_Dialog::getfromfile(int type, string text,int id)//id 是要删除的id
{
    //QMessageBox::about(NULL,"ERROR","Get file"+QString::number(id,10));
    if(type==0)//新建操作
    {
        /*
		this->Pro->pri_flag=1;
        this->Pro->newprocess();
        QMessageBox::about(NULL,"ERROR","Get file");
        emit sendtomem(0,this->Pro->processid,text);//发送给内存
        QMessageBox::about(NULL,"ERROR","Get file1");
        emit sendid(this->Pro->processid);//发送id给file
        QMessageBox::about(NULL,"ERROR","Get file2");
		*/
    }
    else if(type==1)//删除
    {
        if(this->Pro->findmatpcb(id))
        {
            emit sendtomem(2,id,text);
            emit sendtomem(1,id,text);
        }
        else
            QMessageBox::about(NULL,"ERROR","This process doesn't exist");
    }
    else if(type==2)//打开文件操作
    {
        this->Pro->pri_flag=1;
        this->Pro->newprocess();
        emit sendtomem(0,this->Pro->processid,text);
        emit sendid(this->Pro->processid);
    }
    //QMessageBox::about(NULL,"Tip","get file operation");
}

void Process_Dialog::on_diaodu_currentIndexChanged(int index) //设置调度方法
{
    if(index==1)
    {
		this->scheduler=1;
		this->Pro->i=1;
    }
    else if(index==2)
    {
       this->Pro->i=2;
       this->scheduler=2;
    }
    else if(index==3)
    {
		this->Pro->i=3;
		this->scheduler=3;
    }
    else if(index==4)
    {
        this->Pro->i=4;
        this->scheduler=4;
    }
}
