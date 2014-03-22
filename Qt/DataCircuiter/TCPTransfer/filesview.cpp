#include <QHeaderView>
#include <QFontMetrics>
#include "filesview.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QItemDelegate>
#include <QHeaderView>
FilesView::FilesView(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(true);

    QStringList headers;
    headers << tr("#") << tr("Name") << tr("Progress") << tr("Rate") <<tr("Size") << tr("Remain") << tr("Total") <<tr("Completed") << tr("State");
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, qMax(fm.width(headers.at(1)+ "####"), fm.width(" name-for-a-file.ext ")) );
    header->resizeSection(2, qMax(fm.width(headers.at(2)+"####"), fm.width("Progress")));
    header->resizeSection(3, qMax(fm.width(headers.at(3)+"####"), fm.width(" 999Kb/s ")));
    header->resizeSection(4, qMax(fm.width(headers.at(4)+"####"), fm.width(" 999Mb ")) );
    header->resizeSection(5, qMax(fm.width(headers.at(5)+"####"), fm.width(" hh:mm:ss ")) );
    header->resizeSection(6, qMax(fm.width(headers.at(6)+"####"), fm.width(" hh:mm:ss ")) );
    header->resizeSection(7, qMax(fm.width(headers.at(6)+"####"), fm.width(" 999Mb/999Mb ")) );
    header->resizeSection(8, qMax(fm.width(headers.at(7)+"####"), fm.width(" copying ")) );

    this->setItemDelegate(new FilesViewDelegate(this));

    curRuning = 0;
    completedCounts = 0;
}

FilesView::~FilesView()
{
    curRuning = 0;
    completedCounts = 0;
    jobs.clear();
}

const DataClient *FilesView::clientForRow(int row) const
{
    return jobs.at(row).dataClient;
}


void FilesView::dragMoveEvent(QDragMoveEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
    else
        event->ignore();
}

void FilesView::dropEvent(QDropEvent *event)
{
    if( event->mimeData()->hasFormat("text/uri-list") )
    {
        QString localFile;
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            event->acceptProposedAction();
            event->setDropAction(Qt::CopyAction);
            addClient(localFile, serverPath);
        }
    }
    else
        event->ignore();
}

void FilesView::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
}

int FilesView::rowOfClient(DataClient *client) const
{
    int row = 0;
    foreach (DataJob job, jobs)
    {
        if (job.dataClient == client)
            return row;
        ++row;
    }
    return -1;
}


bool FilesView::addClient(const QString &fileName, const QString &destinationFolder, const QByteArray &resumeState)
{
    foreach(DataJob job, jobs)
    {
        if (job.fileName == fileName && job.destinationDirectory == destinationFolder)
        {
            QMessageBox::warning(this, tr("Already uploading"),tr("The data file %1 is already being uploaded.").arg(fileName));
            return false;
        }
    }

    // Create a new file client and attempt to parse the file data.

    //QtFileCopier *copier = new QtFileCopier(this);
    DataClient *client = new DataClient(this);
    QtFileCopier *copier = new QtFileCopier(this);
    client->setFileCopier(copier);

    // ���������ļ���״̬���µ��źŲ�����
    connect(copier, SIGNAL(stateChanged(QtFileCopier::State)), this, SLOT(stateChanged(QtFileCopier::State)));
    connect(client, SIGNAL(progressUpdatedSignal(int)), this, SLOT(updateProgressSlot(int)));
    connect(client, SIGNAL(transferRateUpdateSignal(QString)), this, SLOT(updateUploadRateSlot(QString)));
    connect(client, SIGNAL(transferStateSignal(QString)), this, SLOT(updateTransferStateSlot(QString)));
    connect(client, SIGNAL(completedTrasferedSignal(QString)), this, SLOT(updateCompletedSlot(QString)));
    connect(client, SIGNAL(transferRemainTimeSignal(QString)), this, SLOT(updateRemainTimeSlot(QString)));
    connect(client, SIGNAL(transferDoneSignal(DataClient*)), this, SLOT(transferDoneSlot(DataClient*)));
    connect(client, SIGNAL(transferTotalTimeSiganl(QString)), this, SLOT(updateTotalTimeSlot(QString)));

    // Add the client to the list of downloading jobs.
    DataJob job;
    job.copier = copier;
    job.dataClient = client;
    job.fileName = fileName;
    //job.destinationDirectory = destinationFolder;
    jobs << job;


    //�������ӵ��ļ�������ʾ��treeview��,
    //...
    QFileInfo info = QFileInfo(fileName);

    QString strSize;
    if( info.isFile() )
    {
        bytesToSize(info.size(), strSize);
    }
    else
    {
        qint64 _size=0;
        this->getFolderSize(fileName, _size );
        bytesToSize(_size, strSize);
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(this);

    QString baseFileName = QFileInfo(fileName).fileName();
//    if (baseFileName.toLower().endsWith(".*"))
//        baseFileName.remove(baseFileName.size() - 8);

    item->setText(0, QString("%1").arg( this->model()->rowCount()) );
    item->setText(1, baseFileName);
    //item->setToolTip(1, tr("File: %1<br>Destination: %2").arg(baseFileName).arg(serverPath));
    item->setText(2, "0.0 KB/s");
    item->setText(3, "0.0 KB/s");
    item->setText(4, strSize);
    item->setText(8, tr("Idle"));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//    item->setTextAlignment(1, Qt::AlignHCenter);
    //���ﱣ����ӵ����ݵ�settings,Ϊ�´�������ȡ����,�Լ���ӵ���ʷ�б���

    return true;

}
// start upload...
void FilesView::startUploadDataClientSlot()
{
    if(jobs.count() == 0 ) return;
    curRuning = 0;
    //completedCounts = 0;
    timer = new QTimer(this);
    timer->setSingleShot(false);
//    timer->setInterval(1000);
    connect( timer, SIGNAL(timeout()), this, SLOT(transferNextSlot()) );
    timer->start(1000);

<<<<<<< .mine
    updateControlSignal(false);
    //    QTimer::singleShot(2000, this, SLOT(updataCtrlTime()));
}
=======
void FilesView::setServerPath(const QString &path)
{
    QDir dir(path);
    if(!dir.exists())
    {
        //dir.mkpath(path);
        dir.mkdir(path);
    }
    serverPath = path;
    foreach(DataJob job, jobs)
    {
        if(job.copier && job.copier->state() == QtFileCopier::Idle)
        {
            job.destinationDirectory =  serverPath;
        }
    }
//copiedDir = serverPath;
}
>>>>>>> .r58

<<<<<<< .mine
void FilesView::stopUploadDataClientSlot()
{
//    if( curRuning > 0)
    {
        foreach(DataJob job, jobs)
        {
            if(job.copier && job.copier->state() == QtFileCopier::Busy)
            {
                job.copier->cancelAll();
                timer->stop();
                emit updateControlSignal(true);
            }
        }
    }
}

void FilesView::transferDoneSlot(DataClient *)
{
    curRuning--;
    completedCounts++;
    if( completedCounts == jobs.count() )
    {
//        timer->stop();
//        emit updateControlSignal(true);
        //qDebug() << "���������ϴ����.�ȴ����������,10�������ȴ�...";
        //�ȴ�5���Ƿ������������,���û����ֹͣ,���¿ؼ�
        QTimer::singleShot(2000, this, SLOT(transferAllDone()));
    }
}

void FilesView::transferAllDone()
{
    timer->stop();
    emit updateControlSignal(true);
    qDebug() << "û�����������,���������ϴ����.";
}

void FilesView::clearDataJobs()
{
    jobs.clear();
}

void FilesView::clearAllClient()
{
    if(jobs.count() > 0)
    {
        foreach(DataJob job, jobs)
        {
            if(job.copier)
                job.copier->deleteLater();
            if(job.dataClient)
                job.dataClient->deleteLater();
        }
        QTimer::singleShot(3000, this, SLOT(clearDataJobs()));
    }
    this->clear();
    curRuning = 0;
    completedCounts = 0;
    createdDir = "";
    copiedDir = "";
    timer->stop();
}

void FilesView::transferNextSlot()
{
    // �����ϴ�������С��2��,������µ�һ������
    if( curRuning < 2)
    {
        foreach(DataJob job, jobs)
        {
            if(job.copier && job.copier->state() == QtFileCopier::Idle)
            {
                QtFileCopier::CopyFlags f = currentFlags();
                createdDir = job.fileName;
                copiedDir = job.destinationDirectory;
                job.copier->copy(createdDir, copiedDir, f);
                curRuning++;
                break;
            }
        }
    }
}

=======
// start upload...
void FilesView::startUploadDataClientSlot()
{
    if( jobs.count() == 0 || this->model()->rowCount() == 0 ) return;
    curRuning = 0;
    //completedCounts = 0;
    timer = new QTimer(this);
    timer->setSingleShot(false);
//    timer->setInterval(1000);
    connect( timer, SIGNAL(timeout()), this, SLOT(transferNextSlot()) );
    timer->start(1000);

    updateControlSignal(false);
    //    QTimer::singleShot(2000, this, SLOT(updataCtrlTime()));
}

void FilesView::stopUploadDataClientSlot()
{
//    if( curRuning > 0)
    {
        foreach(DataJob job, jobs)
        {
            //if(job.copier && job.copier->state() == QtFileCopier::Busy)
            {
                job.copier->cancelAll();
                timer->stop();
                emit updateControlSignal(true);
            }
        }
    }
}

void FilesView::transferDoneSlot(DataClient *)
{
    curRuning--;
    completedCounts++;
    if( completedCounts == jobs.count() )
    {
//        timer->stop();
//        emit updateControlSignal(true);
        //qDebug() << "���������ϴ����.�ȴ����������,10�������ȴ�...";
        //�ȴ�5���Ƿ������������,���û����ֹͣ,���¿ؼ�
        QTimer::singleShot(2000, this, SLOT(transferAllDone()));
    }
}

void FilesView::transferAllDone()
{
    timer->stop();
    emit updateControlSignal(true);
    qDebug() << "û�����������,���������ϴ����.";
}

void FilesView::clearDataJobs()
{
    jobs.clear();
}

void FilesView::clearAllClient()
{
    if(jobs.count() > 0)
    {
        foreach(DataJob job, jobs)
        {
            if(job.copier)
                job.copier->deleteLater();
            if(job.dataClient)
                job.dataClient->deleteLater();
        }
        QTimer::singleShot(3000, this, SLOT(clearDataJobs()));
        timer->stop();
    }
    if(this->model()->rowCount() > 0)
        this->clear();
    curRuning = 0;
    completedCounts = 0;
    createdDir = "";
    copiedDir = "";
}

void FilesView::transferNextSlot()
{
    // �����ϴ�������С��2��,������µ�һ������
    if( curRuning < 2)
    {
        foreach(DataJob job, jobs)
        {
            if(job.copier && job.copier->state() == QtFileCopier::Idle)
            {
                QtFileCopier::CopyFlags f = currentFlags();
                createdDir = job.fileName;
                copiedDir = serverPath;//job.destinationDirectory;
                job.copier->copy(createdDir, copiedDir, f);
                curRuning++;
                break;
            }
        }
    }
}

>>>>>>> .r58
void FilesView::bytesToSize(const qint64 &bytes, QString &size)
{
    qint32 B = 1; //byte
    qint32 KB = 1024 * B; //kilobyte
    qint32 MB = 1024 * KB; //megabyte
    qint64 GB = 1024 * MB; //gigabyte
    qint64 TB = 1024 * GB;//terabyte

    if ((bytes >= KB) && ( bytes < MB))
    {
        size = QString("%1 KB").arg( (double)bytes / KB, 0, 'f', 1 );
    }
    else if ((bytes >= MB) && (bytes < GB))
    {
        size = QString("%1 MB").arg( (double)bytes / MB, 0, 'f', 1 );
    }
    else if ((bytes >= GB) && (bytes < TB))
    {
       size = QString("%1 GB").arg( (double)bytes / GB, 0, 'f', 1 );
    }
    else if (bytes >= TB)
    {
        size = QString("%1 TB").arg( (double)bytes / TB, 0, 'f', 1 );
    }
    else
    {
        size = QString("%1 B").arg((double)bytes, 0, 'f', 1);
    }
}

void FilesView::getFolderSize(const QString &path, qint64 &size)
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

// ����
void FilesView::updateProgressSlot(int percent)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);
    // Update the progressbar.
    QTreeWidgetItem *item = this->topLevelItem(row);
    if (item)
        item->setText(2, QString::number(percent));
}

// ����
void FilesView::updateUploadRateSlot(QString rate)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(3, rate);
}

// ״̬
void FilesView::updateTransferStateSlot(QString state)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(8, state);
}

// �����
void FilesView::updateCompletedSlot(QString completed)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(7, completed);
}

// ʣ��ʱ��
void FilesView::updateRemainTimeSlot(QString remianTime)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(5, remianTime);
}

// ������ʱ��
void FilesView::updateTotalTimeSlot(QString totalTime)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(6, totalTime);
}

void FilesView::stateChanged(QtFileCopier::State state)
{
<<<<<<< .mine
    state = QtFileCopier::Idle;
=======
    state = QtFileCopier::Idle;
}

>>>>>>> .r58
QtFileCopier::CopyFlags FilesView::currentFlags() const
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

<<<<<<< .mine
QtFileCopier::CopyFlags FilesView::currentFlags() const
=======
void FilesView::currentProjectPath(int)
>>>>>>> .r58
{
<<<<<<< .mine
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
=======

>>>>>>> .r58
}
