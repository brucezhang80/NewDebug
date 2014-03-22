#include "datatransfehistory.h"
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QHeaderView>

#include <QDebug>


DataTransfeHistory::DataTransfeHistory(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);

    QStringList headers;
    headers << tr("#") << tr("文件名") << tr("大小") << tr("提交用户")<< tr("提交时间") << tr("历史状态");
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, fm.width(headers.at(1)+"name-for-a-file.ext"));
    header->resizeSection(2, fm.width(headers.at(2)+"####"));
    header->resizeSection(3, fm.width(headers.at(3)+"####"));
    header->resizeSection(4, fm.width(headers.at(4)+"####"));
    header->resizeSection(5, fm.width(headers.at(5)+"####"));
}

void DataTransfeHistory::addItem(const int &num, const QString &filename)
{

    QFileInfo info = QFileInfo(filename);
    QString strSize;
    if( info.isFile() )
    {
        bytesToSize(info.size(), strSize);
    }
    else
    {
        qint64 _size=0;
        this->getFolderSize(filename, _size );
        bytesToSize(_size, strSize);
    }
    QString baseFileName = info.fileName();
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setText(0, QString("%1").arg(num));
    item->setText(1, baseFileName);
    item->setText(2, strSize);
    item->setText(3, "anonymous");
    item->setText(4, tr("2013/2/1"));
    item->setText(5, tr("成功"));
}

void DataTransfeHistory::getFolderSize(const QString & path, qint64 &size)
{
    QDir *dir=new QDir(path);
    QStringList filter;
    //filter<<"*.dat";
    //dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>( dir->entryInfoList(filter) );
    qint64 _size = 0;
    for ( int i = 0; i < fileInfo->size(); ++i )
    {
        _size +=  fileInfo->at(i).size();
    }

    size = _size;
}

void DataTransfeHistory::bytesToSize(const qint64 &bytes, QString &size)
{
    qint32 B = 1; //byte
    qint32 KB = 1024 * B; //kilobyte
    qint32 MB = 1024 * KB; //megabyte
    qint64 GB = 1024 * MB; //gigabyte
    qint64 TB = 1024 * GB;//terabyte

    if ((bytes >= KB) && ( bytes < MB))
    {
        size = QString("%1 KB").arg( (float)bytes / KB );
    }
    else if ((bytes >= MB) && (bytes < GB))
    {
        size = QString("%1 MB").arg( (float)bytes / MB );
    }
    else if ((bytes >= GB) && (bytes < TB))
    {
       size = QString("%1 GB").arg( (float)bytes / GB );
    }
    else if (bytes >= TB)
    {
        size = QString("%1 TB").arg( (float)bytes / TB );
    }
    else
    {
        size = QString("%1 B").arg(bytes);
    }
}
