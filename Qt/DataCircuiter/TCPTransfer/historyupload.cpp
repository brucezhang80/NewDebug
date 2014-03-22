#include "historyupload.h"

HistoryUpload::HistoryUpload(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(true);

    QStringList headers;
    headers << tr("#") << tr("Name") << tr("Size")  << tr("To") << tr("From")<<tr("time");
    this->setHeaderLabels(headers);
}
