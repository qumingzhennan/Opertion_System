#ifndef TEXT_DIALOG_H
#define TEXT_DIALOG_H

#include <QDialog>

namespace Ui {
class text_Dialog;
}

class text_Dialog : public QDialog
{
    Q_OBJECT
signals:
    void sendtopro_id(int id,string name,string full_path,int order);
public:
    explicit text_Dialog(QWidget *parent = 0);
    ~text_Dialog();
    string name;
    string full_path;
    int id;
    int order;//第几个被打开的文件
     Ui::text_Dialog *ui;
    void close();

private slots:
    void on_pushButton_clicked();
};

#endif // TEXT_DIALOG_H
