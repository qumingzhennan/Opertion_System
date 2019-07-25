/********************************************************************************
** Form generated from reading UI file 'text_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXT_DIALOG_H
#define UI_TEXT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_text_Dialog
{
public:
    QTextEdit *textEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *text_Dialog)
    {
        if (text_Dialog->objectName().isEmpty())
            text_Dialog->setObjectName(QStringLiteral("text_Dialog"));
        text_Dialog->resize(460, 414);
        textEdit = new QTextEdit(text_Dialog);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(0, 0, 461, 391));
        pushButton = new QPushButton(text_Dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(380, 390, 75, 23));

        retranslateUi(text_Dialog);

        QMetaObject::connectSlotsByName(text_Dialog);
    } // setupUi

    void retranslateUi(QDialog *text_Dialog)
    {
        text_Dialog->setWindowTitle(QApplication::translate("text_Dialog", "Dialog", 0));
        pushButton->setText(QApplication::translate("text_Dialog", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class text_Dialog: public Ui_text_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXT_DIALOG_H
