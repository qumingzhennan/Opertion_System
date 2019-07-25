#ifndef MEM_DIALOG_H
#define MEM_DIALOG_H
#include "text_dialog.h"
#include "ui_text_dialog.h"
#include <QDialog>
namespace Ui {
class mem_Dialog;
}

class mem_Dialog : public QDialog
{
    Q_OBJECT

private:
    //mem *mem_control;

public:
    int mem_mode=0;//д╛хон╙ R_R
    int pro_mode=0;//
    explicit mem_Dialog(QWidget *parent = 0);
    ~mem_Dialog();

private slots:
    void on_pushButton_clicked();


private:
    Ui::mem_Dialog *ui;
};

#endif // MEM_DIALOG_H
