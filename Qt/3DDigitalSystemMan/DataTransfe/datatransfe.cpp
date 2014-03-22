#include "datatransfe.h"
//#include "transfetask.h"
#include "dataclient.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QItemDelegate>
#include <QHeaderView>

DataTransfe::DataTransfe(QWidget *parent) :
    QTreeWidget(parent),taskIndex(0)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(true);

    QStringList headers;
    headers << tr("#") << tr("文件") << tr("进度") << tr("速度") << tr("大小") << tr("剩余时间") << tr("已用时间") << tr("已提交") << tr("状态");
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, qMax(fm.width(headers.at(1)+ "######"), fm.width(" name-for-a-file.ext ")) );
    header->resizeSection(2, qMax(fm.width(headers.at(2)+"######"), fm.width("      ")));
    header->resizeSection(3, qMax(fm.width(headers.at(3)+"######"), fm.width(" 999Kb/s  ")));
    header->resizeSection(4, qMax(fm.width(headers.at(4)+"########"), fm.width(" 999.999Mb  ")) );
    header->resizeSection(5, qMax(fm.width(headers.at(5)+"######"), fm.width(" hh:mm:ss ")) );
    header->resizeSection(6, qMax(fm.width(headers.at(6)+"######"), fm.width(" hh:mm:ss ")) );
    header->resizeSection(7, qMax(fm.width(headers.at(6)+"########"), fm.width(" 999Mb/999Mb ")) );
    header->resizeSection(8, qMax(fm.width(headers.at(7)+"####"), fm.width(" copying ")) );

    this->setItemDelegate(new DataViewDelegate(this));
//    this->setStyleSheet( "QTreeWidget::item:hover{background-color:rgb(0,255,0,50)}"
//    "QTreeWidget::item:selected{background-color:rgb(255,0,0,100)}");
}

DataTransfe::~DataTransfe()
{
    jobs.clear();
}

void DataTransfe::getFolderSize(const QString & path, qint64 &size)
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

void DataTransfe::bytesToSize(const qint64 &bytes, QString &size)
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

const DataClient *DataTransfe::clientForRow(int row) const
{
    // Return the client at the given row.
    return jobs.at(row).dataClient;
}

void DataTransfe::dragMoveEvent(QDragMoveEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
    else
        event->ignore();
}

void DataTransfe::dropEvent(QDropEvent *event)
{
    if( event->mimeData()->hasFormat("text/uri-list") )
    {
        QString localFile;
        //QRegExp rx("*$", Qt::CaseInsensitive);
        //int num = this->model()->rowCount();
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            //if( rx.indexIn(localFile) >= 0 )
            {
                event->acceptProposedAction();
                event->setDropAction(Qt::CopyAction);
                //add item
                //addItem(num++, localFile);
                addClient(localFile, "//YVI-PC/public/test/");
            }
            //else
                //event->ignore();
        }
    }
    else
        event->ignore();
}

void DataTransfe::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
}

int DataTransfe::rowOfClient(DataClient *client) const
{
    // Return the row that displays this client's status, or -1 if the
    // client is not known.
    int row = 0;
    foreach (DataJob job, jobs)
    {
        if (job.dataClient == client)
            return row;
        ++row;
    }
    return -1;
}


bool DataTransfe::addClient(const QString &fileName, const QString &destinationFolder, const QByteArray &resumeState)
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

    // 设置数据文件端状态更新的信号槽连接
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
    job.destinationDirectory = destinationFolder;
    jobs << job;


    //这里把添加的文件数据显示到treeview中,
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
    item->setToolTip(1, tr("File: %1<br>Destination: %2").arg(baseFileName).arg(destinationFolder));
    item->setText(2, "0.0 KB/s");
    item->setText(3, "0.0 KB/s");
    item->setText(4, strSize);
    item->setText(8, tr("Idle"));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//    item->setTextAlignment(1, Qt::AlignHCenter);
    //这里保存添加的数据到settings,为下次启动读取进度,以及添加到历史列表中

    return true;
}

void DataTransfe::startUploadDataClientSlot()
{
//    foreach(DataJob job, jobs)
//    {
//        if( job.copier && job.copier->state() != QtFileCopier::Busy)
//        {
//            createdDir = job.fileName;
//            copiedDir = job.destinationDirectory;
//            movedDir = job.destinationDirectory;
//            copiedFiles = job.destinationDirectory;
//            movedFiles = job.destinationDirectory;

//            createDestDir(job.destinationDirectory);

//            //job.copier->copy(createdDir, copiedDir, f);
//            //copy a file
//            //copier->copy(job.fileName, copiedDir, f);
//            //this->copy();
//            //copy folder
//            //QStringList files = sourceFiles();
//            //copier->copyFiles(files, copiedDir, f);
//            //job.dataClient->start();
//        }
//        else
//        {
//            qDebug() << job.fileName << "has been done.";
//        }
//    }

    timer = new QTimer(this);
    //timer->setInterval(1000);
    timer->setSingleShot(true);
    connect( timer, SIGNAL(timeout()), this, SLOT(transferNextSlot()) );
    timer->start(2000);
}

void DataTransfe::transferDoneSlot(DataClient* client)
{
    if( client->fileCopier()->state() == QtFileCopier::Completed)
    {
        qDebug() << "file copier state is Completed(" <<client->fileCopier()->state() << ")";
    }
    else
    {
        qDebug() << "file copier state is Idle or Busy.(" <<client->fileCopier()->state() << ")";

    }

    int idle = 0;
    for( int i = 0; i < jobs.size(); ++i)
    {
        if( jobs[i].copier && jobs[i].copier->state() == QtFileCopier::Completed)
        {
            qDebug() << "copy completed .";
            jobs[i].copier->deleteLater();
            jobs[i].copier = NULL;
            break;
        }
        if( jobs[i].copier && jobs[i].copier->state() == QtFileCopier::Idle)
        {
            idle++;
        }
    }
    if( idle == 0 )
    {
        timer->stop();
        emit updateControlSignal(true);
    }
}

void DataTransfe::transferNextSlot()
{
    int curRun = 0;
    int idle = 0;
    for( int i = 0; i < jobs.size(); ++i)
    {
        if( jobs[i].copier && jobs[i].copier->state() == QtFileCopier::Busy)
        {
           curRun++;
           if( curRun >= 2 )
           {
               break;
           }
        }

        if( jobs[i].copier && jobs[i].copier->state() == QtFileCopier::Idle )
        {
            QtFileCopier::CopyFlags f = currentFlags();
            createdDir = jobs[i].fileName;
            copiedDir = jobs[i].destinationDirectory;
            jobs[i].copier->copy(createdDir, copiedDir, f);
        }
        else
        {
            idle++;
            continue;
        }
    }

    if(curRun == 0)
    {
        timer->stop();
        emit updateControlSignal(true);
    }

}

//传输速率
void DataTransfe::updateUploadRateSlot(QString rate)
{
    // Update the download rate.
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);


    this->topLevelItem(row)->setText(3, rate);

//    if (!saveChanges)
//    {
//        saveChanges = true;
//        QTimer::singleShot(5000, this, SLOT(saveSettings()));
    //    }
}
//传输状态
void DataTransfe::updateTransferStateSlot(QString state)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(8, state);
}

//已完成量
void DataTransfe::updateCompletedSlot(QString completed)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(7, completed);
}

//剩余时间
void DataTransfe::updateRemainTimeSlot(QString remianTime)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(5, remianTime);
}

//总时间
void DataTransfe::updateTotalTimeSlot(QString totalTime)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    this->topLevelItem(row)->setText(6, totalTime);
}

//进度条
void DataTransfe::updateProgressSlot(int percent)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);
    // Update the progressbar.
    QTreeWidgetItem *item = this->topLevelItem(row);
    if (item)
        item->setText(2, QString::number(percent));
}

/************************************************************/
void DataTransfe::createDestDir(const QString &dest)
{
    QDir destDir;
    if (!destDir.exists(dest))
        destDir.mkdir(dest);
}

QStringList DataTransfe::sourceFiles() const
{
    QDir sourceDir(createdDir);
    QStringList files;
    QFileInfoList entries = sourceDir.entryInfoList();
    QListIterator<QFileInfo> it(entries);
    while (it.hasNext())
    {
        QFileInfo fi = it.next();
        files.append(fi.absoluteFilePath());
    }
    return files;
}

void DataTransfe::removeDir(const QString &dir)
{
    QDir d(dir);
    QFileInfoList dirList = d.entryInfoList(QDir::Dirs);
    QListIterator<QFileInfo> itDir(dirList);
    while (itDir.hasNext())
    {
        QFileInfo fi = itDir.next();
        QString dirName = fi.fileName();
        if (fi.isDir() && dirName != QString(".") && dirName != QString(".."))
        {
            removeDir(fi.absoluteFilePath());
        }
    }
    QFileInfoList fileList = d.entryInfoList(QDir::Files |
                    QDir::Hidden | QDir::System);
    QListIterator<QFileInfo> itFile(fileList);
    while (itFile.hasNext())
    {
        QFileInfo fi = itFile.next();
        QString fileName = fi.fileName();
        if (!fi.isDir())
        {
            d.remove(fileName);
        }
    }
    QFileInfo fi(dir);
    QDir dirUp = fi.dir();
    dirUp.rmdir(fi.fileName());
}

void DataTransfe::create()
{
    QFileInfo fi(createdDir);
    if (fi.exists() && (!fi.isDir() || fi.isSymLink()))
        return;
    if (!fi.exists())
    {
        QDir dir = fi.dir();
        if (!dir.mkdir(createdDir))
            return;
    }
    //QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = 0; i < 10; i++)
    {
        QDir dir(createdDir);
        QString fileName = QString("file%1.tmp").arg(i);
        QFile f(dir.filePath(fileName));
        if (f.open(QFile::WriteOnly))
        {
            QByteArray a(1024, 'X');
            for (int j = 0; j < 4; j++)
                f.write(a, 1024);
            f.close();
        }
    }
}

void DataTransfe::copy()
{
    QtFileCopier::CopyFlags f = currentFlags();
//    copier->copy(createdDir, copiedDir, f);
}

void DataTransfe::copyDir()
{
    QtFileCopier::CopyFlags f = currentFlags();
//    copier->copyDirectory(createdDir, copiedDir, f);
}

void DataTransfe::moveDir()
{
    QtFileCopier::CopyFlags f = currentFlags();
//    copier->moveDirectory(createdDir, movedDir, f);
}

void DataTransfe::copyFiles()
{
    QtFileCopier::CopyFlags f = currentFlags();
    createDestDir(copiedFiles);
    QStringList files = sourceFiles();
//    copier->copyFiles(files, copiedFiles, f);
}

void DataTransfe::moveFiles()
{
    QtFileCopier::CopyFlags f = currentFlags();
    createDestDir(movedFiles);
    QStringList files = sourceFiles();
//    copier->moveFiles(files, movedFiles, f);
}

void DataTransfe::clearFiles()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    removeDir(createdDir);
    removeDir(copiedDir);
    removeDir(movedDir);
    removeDir(copiedFiles);
    removeDir(movedFiles);
    QApplication::restoreOverrideCursor();
}

void DataTransfe::stateChanged(QtFileCopier::State state)
{
    state = QtFileCopier::Idle;
}


QtFileCopier::CopyFlags DataTransfe::currentFlags() const
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
