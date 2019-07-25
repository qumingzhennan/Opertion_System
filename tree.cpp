#include "tree.h"

tree::tree()
{
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("名称")<<QStringLiteral("大小")<<QStringLiteral("创建日期"));
    this->setModel(model);
}

