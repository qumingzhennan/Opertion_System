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
    void sendtomem(int type,int id,string text);//������½������Ͳ��÷�text
    void sendid(int id);
public:
    process *Pro;
    int choose_pro;//��ѡ����
    int choose_pro_index;//��ѡ�����±�
    int choose_column;
    PPCB temp;
    int timeslip;//��ǰʱ��Ƭ
    int type;//0Ϊѡȡ�������ж��еĽ��̣�1Ϊѡȡ�ȴ����н���
    int scheduler;//��ǰ��ѡ�����㷨 1����ת�����㷨 2.�����ȷ�������ȼ� 3.����ҵ���� 4.�����ȷ���
    int opreation;//��ǰ��ѡ����  1.ģ����̲��� 2.�жϲ���
    int interruption;//�жϲ��� 1.���� 2.���� 3.���� 4.ɾ��
    QTimer *timer;
    //��������

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
     //�ڴ沿�ֺ���
     void getfromfile(int type,string text,int id);//���������ļ�ģ����½��͹رս������� type 0�½� 1�ر� 2��

     void on_diaodu_currentIndexChanged(int index);

private:
    Ui::Process_Dialog *ui;
};

#endif // PRO_DIALOG_H
