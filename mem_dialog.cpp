#include "mem_dialog.h"
#include "ui_mem_dialog.h"
#include<QDebug>
mem_Dialog::mem_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mem_Dialog)
{
    ui->setupUi(this);
    //mem_control=new mem(this);
    this->setWindowTitle("Initial my system");
    ui->mem_model->insertItem(0,"Page storage");
    ui->mem_model->insertItem(1,"Segment storage");
    ui->pro_model->insertItem(0,"R_R");
    ui->pro_model->insertItem(1,"FCFS && Priority");
    ui->pro_model->insertItem(2,"STFS");
    ui->pro_model->insertItem(3,"FCFS");
}

mem_Dialog::~mem_Dialog()
{
    delete ui;
}

void mem_Dialog::on_pushButton_clicked()
{
    this->mem_mode=ui->mem_model->currentIndex();
    this->pro_mode=ui->pro_model->currentIndex();
    //qDebug()<<"this->mem_mode="<<this->mem_mode;
    this->hide();
}

