#ifndef PRO_DIALOG_H
#define PRO_DIALOG_H

#include <QDialog>
#include<QTimer>
namespace Ui {
class Process_Dialog;
}

class Process_Dialog : public QDialog
{
    Q_OBJECT
signals:
    void sendtomem(int type,int id,string text);//如果是新建操作就不用发text
    void sendid(int id);
public:
    process *Pro;
    int choose_pro;//所选进程
    int choose_pro_index;//所选进程下标
    int choose_column;
    PPCB temp;
    int timeslip;//当前时间片
    int type;//0为选取的是运行队列的进程，1为选取等待队列进程
    int scheduler;//当前所选调度算法 1：轮转调度算法 2.先来先服务加优先级 3.短作业优先 4.先来先服务
    int opreation;//当前所选操作  1.模拟进程操作 2.中断操作
    int interruption;//中断操作 1.插入 2.阻塞 3.唤醒 4.删除
    QTimer *timer;
    //函数部分

    explicit Process_Dialog(QWidget *parent = 0);
    ~Process_Dialog();
private slots:
     void on_diaodu_currentTextChanged(const QString &arg1);

     void on_timeslips_currentTextChanged(const QString &arg1);
     void open_pro();
     void pro_opt_sure();
     void pro_opt(int op);
     void fresh_timer();
     void insert();
     void back();
     void wake();
     void dele();
     void fresh();

     void on_tableWidget_2_customContextMenuRequested(const QPoint &pos);
     //内存部分函数
     void getfromfile(int type,string text,int id);//接收来自文件模块的新建和关闭进程请求 type 0新建 1关闭 2打开

     void on_diaodu_currentIndexChanged(int index);

private:
    Ui::Process_Dialog *ui;
};

#endif // PRO_DIALOG_H
