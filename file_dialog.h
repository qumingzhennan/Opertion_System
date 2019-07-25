#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <QDialog>
#include"file_control.h"
#include<QStandardItemModel>
#include<QModelIndex>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QTreeWidgetItemIterator>
#include<QTableWidgetItem>
#include<QMenu>
#include "text_dialog.h"
#include "ui_text_dialog.h"
namespace Ui {
class file_Dialog;
}
class file_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit file_Dialog(QWidget *parent = 0);
    ~file_Dialog();
    int open_text_count=0;
    text_Dialog *text[20];
    Ui::file_Dialog *ui;
    bool is_file_opened(string name);
private slots:
    void getid(int id);
    void getmesfromtext(int id,string name,string full_path,int order);
    void Open_filecon();

    void add_file(int row,int colume,QString name);

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    //void on_close_file_clicked(bool checked);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);


    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void new_file();

    void get_filename();

    void get_fil_folderename();

    void on_filename_returnPressed();

    void on_go_clicked(bool checked);

    //void on_pushButton_clicked(bool checked);

    void copy_file();

    void paste_file();

    void rename_file();

    void delete_file();

    void cut_file();


    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_back_clicked();

    void get_re_name();//获取更名后的名字

    //void on_file_close_clicked();

public:
    //文件部分变量
   bool findmatchid(string name);
    int id_count;//创建的进程数
    int id_order;//要删除的进程在 Id数组中的序号
    int Id[20];//跟 ID_name 数组一一对应
    string ID_name[20];//最多支持20个进程操作
    int name;// 第几个打开 或者新建的文件
    int lastpath_flag=0;//为0表示是初始化操作
    int is_copy=0;//复制 粘贴  剪切指示变量
    int is_paste=0;
    int is_cut=0;
    QTreeWidgetItem *cur_treeItem;//当前选择的树节点
    QString cur_file;//当前操作的文件  对treewidget双击的时候改变
    QString temp_cur_file;//用于  setpressentpath 函数
    void show_cur_add();
     QStandardItemModel* model;
     void appendfile(QString  name,int row,int flag,QStandardItem* parent_item);
         void getcur(const QModelIndex &index);//获取当前选中的目录/文件信息
         QModelIndex getcurRoot(QModelIndex itemIndex);//获取当前文件或者文件夹所在根目录

    file_control *file_controler;
    string get_fullpath(QTreeWidgetItem *item);
    int click_flag=2;//2 表示未选中文件  1表示选中文件 以此显示不同的菜单栏
    int temp_right=1;//文件/文件夹属性
    string filename;//当前在treewidget中被单击的item的名字
    int file_type=0;//0是文件  1是文件夹
    string last_path;//上一级路径
    int rename_flag=0;

    void get_lastpath();
    void setTree();
signals:
    void sendtopro(int type,string text,int id);//发送新建进程还是关闭进程选项 0是新建  1是关闭  text 是要发送给内存模块的内容
};

#endif // FILE_DIALOG_H
