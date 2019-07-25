/********************************************************************************
** Form generated from reading UI file 'file_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_DIALOG_H
#define UI_FILE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_file_Dialog
{
public:
    QTreeWidget *treeWidget;
    QTableWidget *tableWidget;
    QPushButton *back;
    QLineEdit *cur_address;
    QPushButton *go;
    QLineEdit *filename;
    QTextEdit *textEdit;
    QPushButton *pushButton;
    QPushButton *file_close;
    QLabel *label;

    void setupUi(QDialog *file_Dialog)
    {
        if (file_Dialog->objectName().isEmpty())
            file_Dialog->setObjectName(QStringLiteral("file_Dialog"));
        file_Dialog->resize(686, 527);
        treeWidget = new QTreeWidget(file_Dialog);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(0, 31, 151, 471));
        tableWidget = new QTableWidget(file_Dialog);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(150, 31, 541, 471));
        back = new QPushButton(file_Dialog);
        back->setObjectName(QStringLiteral("back"));
        back->setGeometry(QRect(0, 10, 75, 21));
        back->setMinimumSize(QSize(75, 21));
        back->setFlat(true);
        cur_address = new QLineEdit(file_Dialog);
        cur_address->setObjectName(QStringLiteral("cur_address"));
        cur_address->setGeometry(QRect(80, 9, 551, 21));
        go = new QPushButton(file_Dialog);
        go->setObjectName(QStringLiteral("go"));
        go->setGeometry(QRect(640, 10, 41, 21));
        go->setMinimumSize(QSize(41, 21));
        go->setAutoDefault(false);
        go->setFlat(true);
        filename = new QLineEdit(file_Dialog);
        filename->setObjectName(QStringLiteral("filename"));
        filename->setGeometry(QRect(370, 510, 301, 21));
        textEdit = new QTextEdit(file_Dialog);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(190, 120, 321, 371));
        pushButton = new QPushButton(file_Dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(430, 460, 75, 23));
        file_close = new QPushButton(file_Dialog);
        file_close->setObjectName(QStringLiteral("file_close"));
        file_close->setGeometry(QRect(340, 460, 75, 23));
        label = new QLabel(file_Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 500, 251, 31));
        label->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));
        treeWidget->raise();
        tableWidget->raise();
        back->raise();
        cur_address->raise();
        go->raise();
        textEdit->raise();
        pushButton->raise();
        file_close->raise();
        filename->raise();
        label->raise();

        retranslateUi(file_Dialog);

        QMetaObject::connectSlotsByName(file_Dialog);
    } // setupUi

    void retranslateUi(QDialog *file_Dialog)
    {
        file_Dialog->setWindowTitle(QApplication::translate("file_Dialog", "Dialog", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("file_Dialog", "/", 0));
        back->setText(QString());
        go->setText(QString());
        pushButton->setText(QApplication::translate("file_Dialog", "\344\277\235\345\255\230", 0));
        file_close->setText(QApplication::translate("file_Dialog", "\345\205\263\351\227\255", 0));
        label->setText(QApplication::translate("file_Dialog", "\350\257\267\350\276\223\345\205\245\346\226\207\344\273\266/\346\226\207\344\273\266\345\244\271\347\232\204\345\220\215\345\255\227\345\220\216\346\214\211\345\233\236\350\275\246", 0));
    } // retranslateUi

};

namespace Ui {
    class file_Dialog: public Ui_file_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_DIALOG_H
