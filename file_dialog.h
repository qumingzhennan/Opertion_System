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

    void get_re_name();//��ȡ�����������

    //void on_file_close_clicked();

public:
    //�ļ����ֱ���
   bool findmatchid(string name);
    int id_count;//�����Ľ�����
    int id_order;//Ҫɾ���Ľ����� Id�����е����
    int Id[20];//�� ID_name ����һһ��Ӧ
    string ID_name[20];//���֧��20�����̲���
    int name;// �ڼ����� �����½����ļ�
    int lastpath_flag=0;//Ϊ0��ʾ�ǳ�ʼ������
    int is_copy=0;//���� ճ��  ����ָʾ����
    int is_paste=0;
    int is_cut=0;
    QTreeWidgetItem *cur_treeItem;//��ǰѡ������ڵ�
    QString cur_file;//��ǰ�������ļ�  ��treewidget˫����ʱ��ı�
    QString temp_cur_file;//����  setpressentpath ����
    void show_cur_add();
     QStandardItemModel* model;
     void appendfile(QString  name,int row,int flag,QStandardItem* parent_item);
         void getcur(const QModelIndex &index);//��ȡ��ǰѡ�е�Ŀ¼/�ļ���Ϣ
         QModelIndex getcurRoot(QModelIndex itemIndex);//��ȡ��ǰ�ļ������ļ������ڸ�Ŀ¼

    file_control *file_controler;
    string get_fullpath(QTreeWidgetItem *item);
    int click_flag=2;//2 ��ʾδѡ���ļ�  1��ʾѡ���ļ� �Դ���ʾ��ͬ�Ĳ˵���
    int temp_right=1;//�ļ�/�ļ�������
    string filename;//��ǰ��treewidget�б�������item������
    int file_type=0;//0���ļ�  1���ļ���
    string last_path;//��һ��·��
    int rename_flag=0;

    void get_lastpath();
    void setTree();
signals:
    void sendtopro(int type,string text,int id);//�����½����̻��ǹرս���ѡ�� 0���½�  1�ǹر�  text ��Ҫ���͸��ڴ�ģ�������
};

#endif // FILE_DIALOG_H
