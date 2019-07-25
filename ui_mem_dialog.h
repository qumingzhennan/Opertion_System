/********************************************************************************
** Form generated from reading UI file 'mem_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEM_DIALOG_H
#define UI_MEM_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_mem_Dialog
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QComboBox *mem_model;
    QComboBox *pro_model;
    QLabel *label_3;

    void setupUi(QDialog *mem_Dialog)
    {
        if (mem_Dialog->objectName().isEmpty())
            mem_Dialog->setObjectName(QStringLiteral("mem_Dialog"));
        mem_Dialog->resize(322, 295);
        mem_Dialog->setStyleSheet(QStringLiteral("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
        pushButton = new QPushButton(mem_Dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(230, 260, 75, 23));
        pushButton->setStyleSheet(QString::fromUtf8("font: 12pt \"\346\245\267\344\275\223\";\n"
"color: rgb(85, 255, 0);"));
        label = new QLabel(mem_Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 100, 51, 20));
        label->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 255);"));
        label_2 = new QLabel(mem_Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 180, 54, 21));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 255);"));
        mem_model = new QComboBox(mem_Dialog);
        mem_model->setObjectName(QStringLiteral("mem_model"));
        mem_model->setGeometry(QRect(140, 100, 121, 22));
        mem_model->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        pro_model = new QComboBox(mem_Dialog);
        pro_model->setObjectName(QStringLiteral("pro_model"));
        pro_model->setGeometry(QRect(140, 180, 121, 22));
        pro_model->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        label_3 = new QLabel(mem_Dialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(70, 30, 141, 31));
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 75 20pt \"\345\271\274\345\234\206\";"));

        retranslateUi(mem_Dialog);

        QMetaObject::connectSlotsByName(mem_Dialog);
    } // setupUi

    void retranslateUi(QDialog *mem_Dialog)
    {
        mem_Dialog->setWindowTitle(QApplication::translate("mem_Dialog", "Dialog", 0));
        pushButton->setText(QApplication::translate("mem_Dialog", "OK", 0));
        label->setText(QApplication::translate("mem_Dialog", "\345\255\230\345\202\250\346\250\241\345\274\217", 0));
        label_2->setText(QApplication::translate("mem_Dialog", "\350\277\233\347\250\213\350\260\203\345\272\246", 0));
        label_3->setText(QApplication::translate("mem_Dialog", "\347\263\273\347\273\237\345\210\235\345\247\213\345\214\226", 0));
    } // retranslateUi

};

namespace Ui {
    class mem_Dialog: public Ui_mem_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEM_DIALOG_H
