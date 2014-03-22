#include "recv_file_treewidget.h"
#include "send_file_treewidget.h"
#include "constants.h"
#include "macai.h"
#include "system.h"
#include "preferences.h"
#include "message_window.h"

#include <QtGui>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QDateTime>
#include <QDir>
#include <QDesktopServices>

RecvFileTreeWidget::RecvFileTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    initialize();
    m_fsWatcher.addPath(watchPath());
    setConnect();

    m_isCollected = false;
}

void RecvFileTreeWidget::retranslateUi()
{
    QStringList headers;
    headers << "#" << tr("Name") <<tr("Size")  << tr("Time") << tr("From") << tr("State");
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, qMax(fm.width(headers.at(1)+ "########"), fm.width(" name-for-a-file.ext ")) );
    header->resizeSection(2, qMax(fm.width(headers.at(2)+"####"), fm.width("999Mb")));
    header->resizeSection(3, qMax(fm.width(headers.at(3)+"########"), fm.width(" yyyy/hh/dd hh:mm:ss ")) );
    header->resizeSection(4, qMax(fm.width(headers.at(4)+"####"), fm.width(" From:00.00.00.00 ")) );
    header->resizeSection(5, qMax(fm.width(headers.at(5)+"####"), fm.width(" State ")) );
}

void RecvFileTreeWidget::initialize()
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
//    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setFocusPolicy(Qt::NoFocus);
    retranslateUi();
}

void RecvFileTreeWidget::setConnect()
{
    connect(Macai::sendFileTreeWidget, SIGNAL(sendFileDone(QList<QString>)),
            this, SLOT(recvFileDone(QList<QString>)) );

    connect(&m_fsWatcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(directoryChanged(QString)) );
    connect(&m_fsWatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(fileChanged(QString)) );
}

void RecvFileTreeWidget::contextMenuEvent(QContextMenuEvent */*event*/)
{
    QMenu *popMenu = new QMenu(this);
    QAction *openAct = popMenu->addAction(QIcon(":/images/file-open.png"),tr("&Open"));
    popMenu->addSeparator();
    QAction *deleteAct = popMenu->addAction(QIcon(":/images/file-delete.png"),tr("&Delete"));
    popMenu->addSeparator();
    QAction* refreshAct = popMenu->addAction(QIcon(":/images/file-refresh.png"),tr("&Refresh"));

    int _x = mapFromGlobal(QCursor::pos()).x();
    int _y = mapFromGlobal(QCursor::pos()).y();
    QTreeWidgetItem *item = this->itemAt( _x, _y - this->header()->height() );

    if( !item )
    {
        this->clearSelection();;
        openAct->setEnabled(false);
        deleteAct->setEnabled(false);
    }
    else
    {
        openAct->setEnabled(true);
        deleteAct->setEnabled(true);
    }

    connect(openAct, SIGNAL( triggered() ), this, SLOT( openFile() ));
    connect(deleteAct, SIGNAL( triggered() ), this, SLOT( deleteFile() ));
    connect(refreshAct, SIGNAL( triggered() ), this, SLOT( refresh() ));
    popMenu->exec(cursor().pos());
}

void RecvFileTreeWidget::collectFiles()
{
    if( m_isCollected )
        return;
    m_isCollected = true;

    int row = count();
    QDir dir(watchPath());
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        QString filename = fileInfo.fileName();
        QDateTime datetime = fileInfo.created();
        QString strTime = datetime.toString("yyyy/MM/dd hh:mm:ss");
        QFileIconProvider fileIcon;
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(0, QString("%1").arg(row++));
        item->setText(1, filename);
        item->setText(2, System::bytesToUnitString((fileInfo.size())) );
        item->setText(3, strTime);
//        item->setText(4, sender);
//        item->setText(5, state);

        item->setIcon(1, fileIcon.icon(fileInfo));
        item->setToolTip(1, tr("Path: %1").arg(fileInfo.filePath()));
//        item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}

///////////////////////////////////////////////////
void RecvFileTreeWidget::addRow(const QString &path)
{
    int row = count();

    QFileInfo fileInfo(path);
    QDateTime datetime = fileInfo.created();
    QString strTime = datetime.toString("yyyy/MM/dd hh:mm:ss");
    QFileIconProvider fileIcon;
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setText(0, QString("%1").arg(row++));
    item->setText(1, path);
    item->setText(2, System::bytesToUnitString((fileInfo.size())) );
    item->setText(3, strTime);
//        item->setText(4, sender);
//        item->setText(5, state);

    item->setIcon(1, fileIcon.icon(fileInfo));
    item->setToolTip(1, tr("Path: %1").arg(path));
//    item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
}

void RecvFileTreeWidget::removeRow(int id)
{

}

QString RecvFileTreeWidget::watchPath() const
{
    return System::sharedPath();
}

void RecvFileTreeWidget::recvFileDone(const QList<QString> &fileLogList)
{

}

// 路径有动静,重新加载一次
void RecvFileTreeWidget::directoryChanged(const QString &path)
{
    this->clear();
    int row = count();
    QDir dir(path);
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries))
    {
        QString filename = fileInfo.fileName();
        QDateTime datetime = fileInfo.created();
        QString strTime = datetime.toString("yyyy/MM/dd hh:mm:ss");
        QFileIconProvider fileIcon;
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(0, QString("%1").arg(row++));
        item->setText(1, filename);
        item->setText(2, System::bytesToUnitString((fileInfo.size())) );
        item->setText(3, strTime);
//        item->setText(4, sender);
//        item->setText(5, state);

        item->setIcon(1, fileIcon.icon(fileInfo));
        item->setToolTip(1, tr("Path: %1").arg(fileInfo.filePath()));
//        item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}

void RecvFileTreeWidget::fileChanged(const QString &path)
{
    addRow(path);
}

void RecvFileTreeWidget::openFile()
{
    QTreeWidgetItem* curItem = this->currentItem();
    if( curItem )
    {
        QString filename = System::sharedPath() + "/" + curItem->text(1);
        QFileInfo fileInfo(filename);
        QDesktopServices::openUrl( QUrl::fromLocalFile(fileInfo.absolutePath()) );
    }
}

void RecvFileTreeWidget::deleteFile()
{
    QTreeWidgetItem* curItem = this->currentItem();
    if( curItem )
    {
        QString filename = System::sharedPath() + "/" + curItem->text(1);
        QFile file(filename);
        if( file.exists() )
        {
            file.remove();
            delete curItem;
        }
    }
}

void RecvFileTreeWidget::refresh()
{
    this->clear();
    int row = count();
    QDir dir(System::sharedPath());
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        QString filename = fileInfo.fileName();
        QDateTime datetime = fileInfo.created();
        QString strTime = datetime.toString("yyyy/MM/dd hh:mm:ss");
        QFileIconProvider fileIcon;
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(0, QString("%1").arg(row++));
        item->setText(1, filename);
        item->setText(2, System::bytesToUnitString((fileInfo.size())) );
        item->setText(3, strTime);
//        item->setText(4, sender);
//        item->setText(5, state);

        item->setIcon(1, fileIcon.icon(fileInfo));
        item->setToolTip(1, tr("Path: %1").arg(fileInfo.filePath()));
//        item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}

