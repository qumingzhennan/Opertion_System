#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"process.h"
#include"mem.h"
#include"file_control.h"
#include "file_dialog.h"
#include "pro_dialog.h"  
#include<QStandardItemModel>
#include<QModelIndex>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QTreeWidgetItemIterator>
#include<QTableWidgetItem>
#include<QMenu>
#include"mem_dialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*//文件部分变量
    int is_copy=0;//复制 粘贴  剪切指示变量
    int is_paste=0;
    int is_cut=0;
    QTreeWidgetItem *cur_treeItem;//当前选择的树节点
    QString cur_file;//当前操作的文件
    QString temp_cur_file;
    void show_cur_add();
     QStandardItemModel* model;
     void appendfile(QString  name,int row,int flag,QStandardItem* parent_item);
         void getcur(const QModelIndex &index);//获取当前选中的目录/文件信息
         QModelIndex getcurRoot(QModelIndex itemIndex);//获取当前文件或者文件夹所在根目录

    file_control *file_controler;
    string get_fullpath(QTreeWidgetItem *item);
    int click_flag=0;//0 表示未选中文件  1表示选中文件 以此显示不同的菜单栏
    int temp_right=1;//文件/文件夹属性
    string filename;//名字
    int file_type=0;//0是文件  1是文件夹
    string last_path;
    int rename_flag=0;

    void get_lastpath();
    void setTree();

*/
    //
    mem_Dialog *mem_choose;
    mem *memory_control;
    Pro_Dialog *pro;
    file_Dialog *file_dia;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //
  /*  //内存部分变量
    process *Pro;
    int choose_pro;//所选进程
    int choose_pro_index;//所选进程下标
    int choose_column;
     PPCB temp;
     int timeslip;//当前时间片
     int type;//0为选取的是运行队列的进程，1为选取等待队列进程
     int diaodu;//当前所选调度算法 1：轮转调度算法 2.先来先服务加优先级 3.短作业优先 4.先来先服务
     int opreation;//当前所选操作  1.模拟进程操作 2.中断操作
     int interrupt;//中断操作 1.插入 2.阻塞 3.唤醒 4.删除
     QTimer *timer;
    //
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     void fresh();
     void showpro(int type);*/
     void paintEvent(QPaintEvent *);
private slots:
     //内存部分函数
   /* void on_diaodu_currentTextChanged(const QString &arg1);

    void on_timeslips_currentTextChanged(const QString &arg1);

    void on_task_clicked(bool checked);

    void on_task_sure_clicked(bool checked);

    void pro_opt_sure();

    //void on_chooseopr_highlighted(const QString &arg1);

    void pro_opt(int op);

    void fresh_timer();

    void insert();

    void back();

    void wake();

    void dele();

    void on_tableWidget_2_customContextMenuRequested(const QPoint &pos);
    //内存部分函数

    //void on_treeView_clicked(const QModelIndex &index);

    void on_file_controler_clicked(bool checked);

    void add_file(int row,int colume,QString name);

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_close_file_clicked(bool checked);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);


    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void new_file();

    void get_filename();

    void get_fil_folderename();

    void on_filename_returnPressed();

    void on_go_clicked(bool checked);

    void on_pushButton_clicked(bool checked);

    void copy_file();

    void paste_file();

    void rename_file();

    void delete_file();

    void cut_file();


    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    //void on_tabWidget_customContextMenuRequested(const QPoint &pos);

    void on_back_clicked();

    void get_re_name();//获取更名后的名字

    void on_file_close_clicked();*/

    void cur_time();


    void on_task_clicked(bool checked);

    void on_file_controler_clicked(bool checked);

    void on_pushButton_clicked();

    void getfrompro(int type,int id,string text);//0新建  1关闭


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
