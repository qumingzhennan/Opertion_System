#include "text_dialog.h"
#include "ui_text_dialog.h"
#include "QPushButton"
text_Dialog::text_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::text_Dialog)
{
    ui->setupUi(this);
    //this->setWindowTitle("Text editor");
   //this->setWindowFlags(Qt::SubWindow);
}

text_Dialog::~text_Dialog()
{
    delete ui;
}

void text_Dialog::on_pushButton_clicked()
{
    emit sendtopro_id(this->id,this->name,this->full_path,this->order);
    this->hide();
}
