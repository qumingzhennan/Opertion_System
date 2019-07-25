#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_pro_dialog.h"
#include "ui_file_dialog.h"
#include "process.h"
#include<QDebug>
#include<QTableWidget>
#include<QTimer>
#include<QMessageBox>
#include<QTime>
#include<QTextCodec>
#include<QPainter>
#include<QIcon>
#include<QtMath>
#include<QFile>
#include<QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::SubWindow);
    pro=new Pro_Dialog(this);
    file_dia=new file_Dialog(this);
    mem_choose=new mem_Dialog;

    memory_control=new mem();

    connect(file_dia,SIGNAL(sendtopro(int,string,int)),pro,SLOT(getfromfile(int,string,int)));
    connect(pro,SIGNAL(sendtomem(int,int,string)),this,SLOT(getfrompro(int,int,string)));
    connect(pro,SIGNAL(sendid(int)),file_dia,SLOT(getid(int)));
    this->setWindowFlags(Qt::Window);
    this->showFullScreen();
    QIcon my_computer(":/icon/timg.jpg");
    QIcon my_file(":/icon/file.jpg");
    QIcon off(":/icon/off.jpg");
    ui->file_controler->setIcon(my_file);
    ui->task->setIcon(my_computer);
    ui->pushButton->setIcon(off);
    ui->file_controler->setIconSize(QSize(75,60));
    ui->task->setIconSize(QSize(75,60));
    ui->pushButton->setIconSize(QSize(35,40));
    ui->file_controler->setFlat(true);
    ui->task->setFlat(true);
    ui->pushButton->setFlat(true);



    this->hide();
    mem_choose->exec();


    this->memory_control->memorytype=this->mem_choose->mem_mode+1;


    this->pro->diaodu=this->mem_choose->pro_mode+1;

    qDebug()<<"this->memory_control->memorytype="<<this->memory_control->memorytype;
     this->memory_control->memoryC.MemoryInit(this->memory_control->memorytype);
    //qDebug()<<"this->memory_control->memorytype="<<this->memory_control->memorytype;

    this->hide();
        ui->lcdNumber->setDigitCount(10); //设置晶体管控件QLCDNumber能显示的位数
        ui->lcdNumber->setMode(QLCDNumber::Dec); //设置显示的模式为十进制
        ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat); //设置显示方式
        QTimer *cur_timer=new QTimer(this);
        cur_timer->setInterval(1000);
        connect(cur_timer,SIGNAL(timeout()),this,SLOT(cur_time()));
        QTime time=QTime::currentTime();
        ui->lcdNumber->display(time.toString("hh:mm:ss"));
        cur_timer->start();
}

void MainWindow::cur_time()//时间和内存使用信息 1s刷新一次
{
    QTime time=QTime::currentTime();
    ui->lcdNumber->display(time.toString("hh:mm:ss"));
    int uti=memory_control->mem_uti*100;
    //cout<<"memory_control->mem_uti:"<<memory_control->mem_uti<<endl;
    this->memory_control->mem_utilization();
   //qDebug()<<"BUG"<<uti;
    ui->progressBar->setValue(uti);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_task_clicked(bool checked)
{
    pro->show();
}

void MainWindow::on_file_controler_clicked(bool checked)
{
    file_dia->show();
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}

void MainWindow::paintEvent(QPaintEvent *event)//重写主窗口的绘图函数。如果不重写产生的界面会很不好看
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
   int  n=qrand()%4;//根据n的数值来确定要显示哪个背景图片
    QPixmap map0,map1,map2,map3,map4;
    map0.load(":/new/prefix1/ChMkJ1jt1RCIHUVLAAJadLznWa0AAbmTgN7HygAAlqM280.jpg");//将图片载入
    map1.load(":/new/prefix1/ChMkJljt1P-IPK6EAAOYdB8Fj1EAAbmTgG5F7kAA5iM481.jpg");
    map2.load(":/new/prefix1/ChMkJ1jt1RCIHUVLAAJadLznWa0AAbmTgN7HygAAlqM280.jpg");
    map3.load(":/new/prefix1/ChMkJljt1P-IPK6EAAOYdB8Fj1EAAbmTgG5F7kAA5iM481.jpg");
    map4.load(":/new/prefix1/ChMkJ1jt1RCIHUVLAAJadLznWa0AAbmTgN7HygAAlqM280.jpg");
    QPainter painter(this);//设置画笔 参数表示该画笔作用的对象是主窗口 就是c++的类中的this的用法


        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10, 10, this->width()+20, this->height()+20);

        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(path, QBrush(Qt::white));

        QColor color(0, 0, 0, 50);
        for(int i=0; i<10; i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
            color.setAlpha(150 - qSqrt(i)*50);
            painter.setPen(color);
            painter.drawPath(path);
        }
        painter.setOpacity(0.6);
        switch(n)
         {
         case 0:
             painter.drawPixmap(0,0,this->width(),this->height(),map0);
             break;
         case 1:
             painter.drawPixmap(0,0,this->width(),this->height(),map1);
             break;
         case 2:
             painter.drawPixmap(0,0,this->width(),this->height(),map2);
             break;
         case 3:
             painter.drawPixmap(0,0,this->width(),this->height(),map3);
             break;
         case 4:
             painter.drawPixmap(0,0,this->width(),this->height(),map4);
             break;
          default:break;
         }
}

void MainWindow::getfrompro(int type, int id, string text)
{
    //QMessageBox::about(NULL,"tip","opt from pro");
    if(type==0)//新建进程
    {
        //qDebug()<<"ID1:"<<id;
        //QMessageBox::about(NULL,"tip","open opt from pro");
        //qDebug()<<"Text"<<QString::fromStdString(text);
        this->memory_control->memoryC.Initize(id,text);
    }
    else if(type==1)//关闭进程
    {
        //QMessageBox::about(NULL,"tip","close opt from pro");
        //qDebug()<<"ID:"<<id;
        this->memory_control->memoryC.Delete(id);
    }
    else if(type==2)
    {
        //QMessageBox::about(NULL,"tip","change opt from pro");
       // qDebug()<<"Text"<<QString::fromStdString(text);
        this->memory_control->memoryC.change(id,text);
    }
}
