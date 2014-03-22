#include "jobhistoryview.h"
#include <QHeaderView>
#include <QTime>

#include <QDebug>

JobHistoryView::JobHistoryView(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(false);

    QStringList headers;
    headers << tr("#") << tr("Name") << tr("Size")  << tr("To") << tr("From")<<tr("time");
    this->setHeaderLabels(headers);

    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, qMax(fm.width(headers.at(1)+ "####"), fm.width(" name-for-a-file.ext ")) );
    header->resizeSection(2, qMax(fm.width(headers.at(2)+"####"), fm.width(" 999Mb ")));
    header->resizeSection(3, qMax(fm.width(headers.at(3)+"####"), fm.width("   ")));
    header->resizeSection(4, qMax(fm.width(headers.at(4)+"####"), fm.width("   ")) );
    header->resizeSection(5, qMax(fm.width(headers.at(5)+"####"), fm.width(" hh:mm:ss ")) );
}

void JobHistoryView::saveSenderData(const QString &resource, const QString &destination)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(this);

    QTime time;
    QString strTime = time.currentTime().toString("HH:mm:ss");
    int counts = this->model()->rowCount();
    item->setText(0, QString("%1").arg(counts));
    item->setText(1, resource);
    item->setText(2, "0");
    item->setText(3, destination);
    item->setText(4, resource);
    item->setText(5, strTime);
}
