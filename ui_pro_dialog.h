/********************************************************************************
** Form generated from reading UI file 'pro_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRO_DIALOG_H
#define UI_PRO_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_Pro_Dialog
{
public:
    QTableWidget *tableWidget_2;
    QComboBox *timeslips;
    QLabel *label_2;
    QComboBox *diaodu;
    QLabel *label;

    void setupUi(QDialog *Pro_Dialog)
    {
        if (Pro_Dialog->objectName().isEmpty())
            Pro_Dialog->setObjectName(QStringLiteral("Pro_Dialog"));
        Pro_Dialog->resize(452, 489);
        tableWidget_2 = new QTableWidget(Pro_Dialog);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(0, 0, 451, 451));
        timeslips = new QComboBox(Pro_Dialog);
        timeslips->setObjectName(QStringLiteral("timeslips"));
        timeslips->setGeometry(QRect(410, 461, 41, 31));
        label_2 = new QLabel(Pro_Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(350, 460, 51, 31));
        diaodu = new QComboBox(Pro_Dialog);
        diaodu->setObjectName(QStringLiteral("diaodu"));
        diaodu->setGeometry(QRect(0, 460, 231, 31));
        label = new QLabel(Pro_Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(240, 460, 111, 31));

        retranslateUi(Pro_Dialog);

        QMetaObject::connectSlotsByName(Pro_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Pro_Dialog)
    {
        Pro_Dialog->setWindowTitle(QApplication::translate("Pro_Dialog", "Dialog", 0));
        label_2->setText(QApplication::translate("Pro_Dialog", "TimeSlot", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Pro_Dialog: public Ui_Pro_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRO_DIALOG_H
