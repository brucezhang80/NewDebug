#include "send_file_treewidget.h"
#include "send_users_treewidget.h"
#include "transfer_file_window.h"
#include "send_file_propertise.h"
#include "system.h"
#include "constants.h"
#include "macai.h"
#include "preferences.h"
#include "send_message.h"
#include "message_thread.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QTimer>
#include <QMap>

SendFileTreeWidget::SendFileTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    initialize();

    m_bOneClientDone = true;
    m_completedCounts = 0;
}

SendFileTreeWidget::~SendFileTreeWidget()
{
    sendJobs.clear();
}

void SendFileTreeWidget::retranslateUi()
{
    QStringList headers;
    headers << "#" << tr("Name") << tr("Progress") << tr("Rate") <<tr("Size") << tr("Remain") << tr("Total") <<tr("Completed") << tr("State");
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, qMax(fm.width(headers.at(0)+" ## "), fm.width(" ## ")));
    header->resizeSection(1, qMax(fm.width(headers.at(1)+ "####"), fm.width(" name-for-a-file.ext ")) );
    header->resizeSection(2, qMax(fm.width(headers.at(2)+"####"), fm.width("Progress")));
    header->resizeSection(3, qMax(fm.width(headers.at(3)+"####"), fm.width(" 999Kb/s ")));
    header->resizeSection(4, qMax(fm.width(headers.at(4)+"####"), fm.width(" 999Mb ")) );
    header->resizeSection(5, qMax(fm.width(headers.at(5)+"####"), fm.width(" hh:mm:ss ")) );
    header->resizeSection(6, qMax(fm.width(headers.at(6)+"####"), fm.width(" hh:mm:ss ")) );
    header->resizeSection(7, qMax(fm.width(headers.at(6)+"####"), fm.width(" 999Mb/999Mb ")) );
    header->resizeSection(8, qMax(fm.width(headers.at(7)+"####"), fm.width(" transfing... ")) );
    header->setHighlightSections(true);
}

void SendFileTreeWidget::initialize()
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
//    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(true);
    this->setFocusPolicy(Qt::NoFocus);

    retranslateUi();
    this->setItemDelegate(new SendFileItemDelegate(this));
}

void SendFileTreeWidget::setConnect()
{
}

//////////////////////////////////////////////////////////////
// handle members
const SendFileClient *SendFileTreeWidget::clientForRow(int row) const
{
    return sendJobs.at(row).sendClient;
}

void SendFileTreeWidget::addFileClient()
{
    // Show the file dialog, let the user select what torrent to start downloading.
     QStringList pathList = QFileDialog::getOpenFileNames(this, tr("Add Files"),
                                                    Macai::preferences->lastSendFilePath,
                                                    tr("All files (*)"));
    if (pathList.isEmpty())
    {
        return;
    }

    QStringList dupPathList;
    bool bAlready = false;
    foreach (SendJob job, sendJobs)
    {
        if ( pathList.contains(job.fileName) )
        {
            dupPathList << job.fileName;
            pathList.removeOne(job.fileName);
            bAlready = true;
        }
    }

    if( !pathList.isEmpty() )
    {
        addFiles(pathList);
        Macai::preferences->lastSendFilePath = pathList.at(0);
    }
    if(bAlready)
    {
        QString list;
        foreach( QString str, dupPathList)
        {
            list.append(str);
            list.append("\n");
        }
        QMessageBox::warning(this, tr("Already added"),
                             tr("The data file(s):\n%1is "
                                "already being added.").arg(list));
    }
}

void SendFileTreeWidget::addFiles(const QStringList &pathList)
{
    foreach (QString path, pathList)
    {
        addFile(path);
    }
}

void SendFileTreeWidget::addFile(const QString &path)
{
    if (path.isEmpty())
    {
        return;
    }

    SendFileClient *fileClient = new SendFileClient(this);
    QtFileCopier *copier = new QtFileCopier(this);
    fileClient->setFileCopier(copier);

    // 设置数据文件端状态更新的信号槽连接
    connect(copier, SIGNAL(stateChanged(QtFileCopier::State)), this, SLOT(stateChanged(QtFileCopier::State)));
    connect(fileClient, SIGNAL(transferProgressUpdatedSignal(int)), this, SLOT(updateProgressSlot(int)));
    connect(fileClient, SIGNAL(transferRateUpdateSignal(QString)), this, SLOT(updateSendRateSlot(QString)));
    connect(fileClient, SIGNAL(transferStateSignal(QString)), this, SLOT(updateTransferStateSlot(QString)));
    connect(fileClient, SIGNAL(trasfereCompletedSignal(QString)), this, SLOT(updateCompletedSlot(QString)));
    connect(fileClient, SIGNAL(transferRemainTimeSignal(QString)), this, SLOT(updateRemainTimeSlot(QString)));
    connect(fileClient, SIGNAL(transferDoneSignal()), this, SLOT(transferDoneSlot()));
    connect(fileClient, SIGNAL(transferTotalTimeSiganl(QString)), this, SLOT(updateTotalTimeSlot(QString)));


    // Add the client to the list of downloading jobs.
    QFileInfo fileInfo(path);
    QString strSize = sizeString( fileInfo );
    SendJob job;
    job.copier = copier;
    job.sendClient = fileClient;
    job.fileName = path;
    job.id = sendJobs.count()+1;
//    job.destination = destinationFolder;// 有多个目标目录
    job.size = strSize;
    job.from = System::loginName() + ":" + System::hostAddress();
    job.project = Macai::sendFilePropertise->currentProjectName();
    sendJobs << job;


     // id name progress rate size remain totaltime completed state , fullfilename
    QStringList columns;
    int row = rowCount()+1;
    columns << QString("%1").arg(row)
       << fileInfo.fileName()
       << "0 %"
       << "0.0 Kb/s"
       << sizeString(fileInfo)
       << "00:00:00"
       << "00:00:00"
       << "0 Kb"
       << "Idle"  // 8
       << fileInfo.absoluteFilePath();//9

    addRow(columns);
    m_pathList << fileInfo.absoluteFilePath();
}

void SendFileTreeWidget::addRow(const QStringList &columns )
{
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    int col = 0;
    foreach (QString str, columns)
    {
        item->setText(col, str);
        ++col;
    }

    addDelBtn(item);

    QFileInfo fileInfo(columns.at(9));
    QFileIconProvider fileIcon;
    item->setIcon(1, fileIcon.icon(fileInfo));
    item->setToolTip(1, tr("Path: %1").arg(columns.at(9)));
    item->setTextAlignment(0, Qt::AlignRight | Qt::AlignVCenter);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
}

void SendFileTreeWidget::addDelBtn(QTreeWidgetItem *item)
{
    QPushButton *delBtn = new QPushButton;
    delBtn->setFlat(true);
    delBtn->setFixedSize(18,18);
    delBtn->setIcon(QIcon(":/images/file-delete.png"));
    setItemWidget(item, 0, delBtn);
    connect(delBtn, SIGNAL(clicked()),this, SLOT(deleteRow()));
}

QStringList SendFileTreeWidget::pathList() const
{
    return m_pathList;
}

int SendFileTreeWidget::rowCount() const
{
    return this->model()->rowCount();
}

void SendFileTreeWidget::removeRow()
{
    int row = this->indexOfTopLevelItem(this->currentItem());
    SendFileClient *client = sendJobs.at(row).sendClient;

    // Stop the client.
    client->disconnect();
//    connect(client->d_ptr, SIGNAL(canceled()), this, SLOT(torrentStopped()));
//    client->stop();

    // Remove the row from the view.
    delete this->takeTopLevelItem(row);
    sendJobs.removeAt(row);
}

int SendFileTreeWidget::rowOfClient(SendFileClient *client) const
{
    int row = 0;
    foreach (SendJob job, sendJobs)
    {
        if (job.sendClient == client)
            return row;
        ++row;
    }
    return -1;
}

QString SendFileTreeWidget::absolutePath(int row) const
{
    return this->model()->data( this->model()->index(row, 9) ).toString();
}

QString SendFileTreeWidget::fileName(int row) const
{
    return this->model()->data( this->model()->index(row, 1) ).toString();
}

QString SendFileTreeWidget::sizeString(QFileInfo &fileInfo) const
{
    QString sizeString;
    if (fileInfo.isDir())
    {
        QString folder = fileInfo.absoluteFilePath();
        qint64 size = System::folderSize(folder);
        sizeString =  System::bytesToUnitString(size);
    }
    else
    {
        sizeString =  System::bytesToUnitString( fileInfo.size() );
    }

    return sizeString;
}


/////////////////////////////////////////////////////////

// 停止发送
void SendFileTreeWidget::stopSendSlot()
{
}

// 进度
void SendFileTreeWidget::updateProgressSlot(const int &percent)
{
    SendFileClient *client = qobject_cast<SendFileClient *>(sender());
    int row = rowOfClient(client);
    // Update the progressbar.
    QTreeWidgetItem *item = this->topLevelItem(row);
    if (item)
        item->setText(2, QString::number(percent));
}

// 速率
void SendFileTreeWidget::updateSendRateSlot(const QString &rate)
{
    SendFileClient *client = qobject_cast<SendFileClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(3, rate);
}

// 剩余时间
void SendFileTreeWidget::updateRemainTimeSlot(const QString &remianTime)
{
    SendFileClient *client = qobject_cast<SendFileClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(5, remianTime);
}

// 已用总时间
void SendFileTreeWidget::updateTotalTimeSlot(const QString &totalTime)
{
    SendFileClient *client = qobject_cast<SendFileClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(6, totalTime);
}

// 完成量
void SendFileTreeWidget::updateCompletedSlot(const QString &completed)
{
    SendFileClient *client = qobject_cast<SendFileClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(7, completed);
}

// 状态
void SendFileTreeWidget::updateTransferStateSlot(const QString &state)
{
    SendFileClient *client = qobject_cast<SendFileClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(8, state);
}

// 开始发送
void SendFileTreeWidget::startSendSlot()
{
    if( sendJobs.count() == 0 || this->rowCount() == 0
            && Macai::sendUsersTreeWidget->userCount() == 0 )
        return;

    m_recverInfo = Macai::transferFileWindow->collectRecieverInfos();

    m_sendTimer = new QTimer(this);
    m_sendTimer->setSingleShot(false);
    m_sendTimer->setInterval(1000);
    connect( m_sendTimer, SIGNAL(timeout()), this, SLOT(transferNextSlot()) );
    m_sendTimer->start();
    m_bOneClientDone = true;
}

// 一个任务完成发送
// 记录当前完成时间
void SendFileTreeWidget::transferDoneSlot()
{
    QTime time;
    QDateTime date;
    QString strDatetime = date.currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    m_bOneClientDone = true;
    m_completedCounts++;
    if( m_completedCounts == sendJobs.count() )
    {
        QTimer::singleShot(2000, this, SLOT(transferAllDone()));
    }

    SendFileClient *client = qobject_cast<SendFileClient *>(sender());
    int row = rowOfClient(client);
    SendJob job = sendJobs.at(row);
    job.time = strDatetime;

    QList<QString> jobLogs;
    QString filename = job.fileName;
    QString filesize = job.size;
//    QString sender = job.from;
    QString project = job.project;
    QString state = QString("%1").arg(job.copier->state());
    QMap<QString, QString> recivers = Macai::transferFileWindow->reciversMap();
    QString recverStr;

    QMap<QString, QString>::const_iterator i = recivers.constBegin();
    while (i != recivers.constEnd() )
    {
        recverStr.append(i.value() );
        recverStr.append(":");
        recverStr.append(i.key());
        recverStr.append("\n");
        ++i;
    }
    jobLogs.append(filename);
    jobLogs.append(filesize);
    jobLogs.append(strDatetime);
    jobLogs.append(recverStr);
    jobLogs.append(project);
    jobLogs.append(state);

    emit updateHistoryLog(jobLogs);
}

// 开始下一个
void SendFileTreeWidget::transferNextSlot()
{
    if( m_bOneClientDone )
    {
        m_bOneClientDone = false;
        foreach(SendJob job, sendJobs)
        {
            if(job.copier && job.copier->state() == QtFileCopier::Idle)
            {
                QtFileCopier::CopyFlags f = currentFlags();
                QString src = job.fileName;
                for(int i = 0; i < m_recverInfo.count(); ++i)
                {
                    QString des = m_recverInfo.at(i) + "/" + job.project;
                    QDir dir(des);
                    if( !dir.exists() )
                        dir.mkdir(des);
                    job.copier->copy(src, des, f);
                }
                break;
            }
        }
    }
}

// 全部完成
// 保留至历史记录
void SendFileTreeWidget::transferAllDone()
{
    qDebug() << "all send done.";

    quint32 flags = 0;
    flags |= IPQ_SEND_FILE;

    QList<QString> ipList = Macai::transferFileWindow->reciversIp();
    QString logs;
    foreach(SendJob job, sendJobs)
    {
        QString doneTime = job.time;
        QString filename = job.fileName;
        QString filesize = job.size;
        QString sender = job.from;
        QString project = job.project;
        QString state = job.copier->state();
        QString log = filename + "_" + filesize + "_" + doneTime + "_" + sender + "_" + project + "_" + state+"\n";

        logs += log;
    }

    foreach( QString ip, ipList)
    {
        SendMessage sendMsg = SendMessage( QHostAddress(ip), IPQ_DEFAULTPORT, logs, "", flags);
        Macai::messageThread->addSendMsg( Message(sendMsg) );
    }
}

void SendFileTreeWidget::stateChanged(QtFileCopier::State /*state*/)
{
//    state = QtFileCopier::Idle;
}

void SendFileTreeWidget::clearDataJobs()
{
    sendJobs.clear();
}

void SendFileTreeWidget::clearAllClient()
{
}

void SendFileTreeWidget::deleteRow()
{
    QPushButton *delBtn = qobject_cast<QPushButton *>(sender());

    for(int i = 0; i < rowCount(); ++i)
    {
        QWidget *widget = itemWidget( topLevelItem(i), 0 );
        if( widget )
        {
            QPushButton *btn = qobject_cast<QPushButton *>(widget);
            if( delBtn == btn)
            {
                delBtn->disconnect();
                // Remove the row from the view.
                delete this->takeTopLevelItem(i);
                sendJobs.takeAt(i);
            }
        }
    }
}

QtFileCopier::CopyFlags SendFileTreeWidget::currentFlags() const
{
    QtFileCopier::CopyFlags f = 0;
//    bool makeLinksFlag = ui.makeLinksCheckBox->isChecked();
//    bool followLinksFlag = ui.followLinksCheckBox->isChecked();
//    bool forceFlag = ui.forceCheckBox->isChecked();
//    if (makeLinksFlag)
//        f = f | QtFileCopier::MakeLinks;
//    if (followLinksFlag)
//        f = f | QtFileCopier::FollowLinks;
//    if (forceFlag)
//        f = f | QtFileCopier::Force;
    return f;
}



