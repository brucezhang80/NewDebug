#include "send_history_treewidget.h"
#include "send_file_treewidget.h"
#include "macai.h"
#include "constants.h"
#include "system.h"

#include <QHeaderView>
#include <QFileInfo>
#include <QFileIconProvider>

SendHistoryTreeWidget::SendHistoryTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    initialize();
    setConnect();
}

void SendHistoryTreeWidget::initialize()
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
//    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(true);
    this->sortByColumn(1, Qt::AscendingOrder);

    QStringList headers;
    headers << "#" << tr("Name") <<tr("Size")  << tr("Time") << tr("To") << tr("State") ;
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, qMax(fm.width(headers.at(1)+"########"), fm.width(" name-for-a-file.ext ")) );
    header->resizeSection(2, qMax(fm.width(headers.at(2)+"####"), fm.width(" 999Mb ")));
    header->resizeSection(3, qMax(fm.width(headers.at(3)+"########"), fm.width(" yyyy/MM/DD hh:mm:ss")) );
    header->resizeSection(4, qMax(fm.width(headers.at(4)+"########"), fm.width(" To :00.00.00.00 ")) );
    header->resizeSection(5, qMax(fm.width(headers.at(5)+"##"), fm.width(" State ")) );
}

void SendHistoryTreeWidget::setConnect()
{
    connect(Macai::sendFileTreeWidget, SIGNAL(updateHistoryLog(QList<QString>)),
            this, SLOT(updateHistoryLogs(QList<QString>)) );
}

bool SendHistoryTreeWidget::connectDB()
{

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////
int SendHistoryTreeWidget::count()
{
    return this->model()->rowCount();
}

void SendHistoryTreeWidget::updateHistoryLogs(const QList<QString> &logs)
{
    QString no = QString("%1").arg(count()+1);
    QString filename = logs.at(0);
    QString filesize = logs.at(1);
    QString time = logs.at(2);
    QString recver = logs.at(3);
    QString state = logs.at(4);

    QFileInfo fileInfo(filename);
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setText(0, no);
    item->setText(1, fileInfo.fileName());
    item->setText(2, filesize);
    item->setText(3, time);
    item->setText(4, recver);
    item->setText(5, state);


    QFileIconProvider fileIcon;
    item->setIcon(1, fileIcon.icon(fileInfo));
    item->setToolTip(1, tr("Path: %1").arg(fileInfo.filePath()));
    item->setToolTip(4, tr("Recievers: %1").arg(recver));
//    item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
}
