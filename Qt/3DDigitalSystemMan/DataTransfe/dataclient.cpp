#include "dataclient.h"
#include "qtfilecopier.h"

#include <QObject>
#include <QFile>
#include <QFileInfo>

#include <QDebug>

static QString formatSize(qint64 size)
{
    QString str = DataClient::tr("%1 B").arg(size);
    int base = 1024;
    qint64 mult = 1;
    for (int i = 0; i < 4; i++)
    {
        qint64 div = size / mult;
        if (div < base)
        {
            double val = (double)size / mult;
            switch (i)
            {
                case 1: str = DataClient::tr("%1Kb").arg(val, 0, 'f', 1); break;
                case 2: str = DataClient::tr("%1Mb").arg(val, 0, 'f', 1); break;
                case 3: str = DataClient::tr("%1Gb").arg(val, 0, 'f', 1); break;
                default: break;
            }
            break;
        }
        mult *= base;
    }
    return str;
}

void DataClientPrivate::init()
{
    Q_Q(DataClient);
    qRegisterMetaType<qint64>("qint64");

    this->fileCopier = 0;
    autoClose = false;

    currentFile = 0;
    totalSize = 0;
    currentProgress = 0;
    currentDone = 0;
    currentProgressTime = 0;
    currentDoneTime = 0;
    dirCount = 0;
    currentDir = 0;
    lastCurrentId = -1;
    completedPercent = 0;

    showTimer = new QTimer(q);
    showTimer->setInterval(2000);
    showTimer->setSingleShot(true);
    //q->connect(showTimer, SIGNAL(timeout()), q, SLOT(showDialog()) );
}

//错误信息显示在每个列表中"状态"列中
void DataClientPrivate::error(int id, QtFileCopier::Error error, bool stopped)
{
    Request r = requests[id];
    if (!stopped)
        return;
    showProgress();
//    showDialog();

    QString title;
    QString text;
    bool handled = false;
    switch (error)
    {
        case QtFileCopier::SourceNotExists:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Source doesn't exist.");
            break;
        }
        case QtFileCopier::DestinationExists:
        {
//            QtOverwriteDialog mb(q_ptr);
//            switch (mb.execute(r.source, r.dest))
//            {
//                case QtOverwriteDialog::Cancel       : fileCopier->cancelAll();
//                                                       fileCopier->retry();        break;
//                case QtOverwriteDialog::Skip         : fileCopier->skip();         break;
//                case QtOverwriteDialog::SkipAll      : fileCopier->skipAll();      break;
//                case QtOverwriteDialog::Overwrite    : fileCopier->overwrite();    break;
            fileCopier->overwriteAll(); break;
//                default                              : fileCopier->retry();        break;
//            }
            handled = true;
            break;
        }
        case QtFileCopier::SourceDirectoryOmitted:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Source file is a directory. Omitting source directory.");
            break;
        }
        case QtFileCopier::SourceFileOmitted:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Source directory is a file. Omitting source file.");
            break;
        }
        case QtFileCopier::PathToDestinationNotExists:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Path to destination does not exist.");
            break;
        }
        case QtFileCopier::CannotCreateDestinationDirectory:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Cannot create destination directory.");
            break;
        }
        case QtFileCopier::CannotOpenSourceFile:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Cannot open source file. Please check permissions.");
            break;
        }
        case QtFileCopier::CannotOpenDestinationFile:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Cannot open destination file. Please check permissions.");
            break;
        }
        case QtFileCopier::CannotRemoveDestinationFile:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Force: Cannot remove destination file.");
            break;
        }
        case QtFileCopier::CannotCreateSymLink:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Cannot create symbolic link targeting to source.");
            break;
        }
        case QtFileCopier::CannotReadSourceFile:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Cannot read source file.");
            break;
        }
        case QtFileCopier::CannotWriteDestinationFile:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Cannot write destination file.");
            break;
        }
        case QtFileCopier::CannotRemoveSource:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Cannot remove source.");
            break;
        }
        default:
        {
            title = q_ptr->tr("Copy Warning");
            text = q_ptr->tr("Error code: %1").arg(error);
            break;
        }
    }

    if (!handled)
    {
//        QtOtherDialog mb(q_ptr);
//        switch (mb.execute(r.source, r.dest, title, text))
//        {
//            case QtOtherDialog::Cancel  : fileCopier->cancelAll();
//                                          fileCopier->retry();   break;
//            case QtOtherDialog::Skip    : fileCopier->skip();    break;
//            case QtOtherDialog::SkipAll : fileCopier->skipAll(); break;
//            case QtOtherDialog::Retry   : fileCopier->retry();   break;
//            default                     : fileCopier->retry();   break;
//        }
    }
}

void DataClientPrivate::stateChanged(QtFileCopier::State state)
{
    Q_Q(DataClient);
    if (state == QtFileCopier::Busy)
    {
        if (fileCopier->state() == QtFileCopier::Idle)
        {
            reset();
            showProgress();
            showTimer->start();
        }
        startTime.start();
        //ui.cancelButton->setEnabled(true);
        //ui.closeButton->setEnabled(false);
    }
    else if (state == QtFileCopier::Completed)
    {
        showTimer->stop();
        showProgress();
        //QString str = DataClient::tr("100% of %1 (Done)").arg(formatSize(totalSize));
        //q->setWindowTitle(str);
        //ui.cancelButton->setEnabled(false);
        //ui.closeButton->setEnabled(true);
    }
    else
    {
        showTimer->stop();
        //ui.cancelButton->setEnabled(true);
        //ui.closeButton->setEnabled(false);
    }
}

void DataClientPrivate::done(bool /*error*/)
{
    Q_Q(DataClient);

    qDebug() << "done";
    showTimer->stop();
//    q->reject();
//    q->copier->deleteLater();
//    q->copier = NULL;
    emit q_ptr->transferDoneSignal(q);
//    if (autoClose)
//        q->accept();
}

void DataClientPrivate::started(int id)
{
    startTime.start();
    lastCurrentId = id;
    currentProgress = 0;
    currentProgressTime = 0;
    QFileInfo fi(requests[id].source);
    qint64 size = fi.isDir() ? 0 : fi.size();
    if (requests[id].size != size)
    {
        totalSize -= requests[id].size;
        totalSize += size;
        requests[id].size = size;
    }
    QTimer::singleShot(0, q_ptr, SLOT(showProgress()));
}

void DataClientPrivate::dataTransferProgress(int id, qint64 progress)
{
    Q_ASSERT(lastCurrentId == id);
    Q_UNUSED(id);
    currentProgressTime = startTime.elapsed();
    currentProgress = progress;
    QTimer::singleShot(0, q_ptr, SLOT(showProgress()));
}

void DataClientPrivate::finished(int id, bool error)
{
    qDebug() << "finished";
    currentFile++;
    if (fileCopier->isDir(id))
        currentDir++;
    totalSize -= requests[id].size;
    if (!error)
    {
        currentDoneTime += currentProgressTime;
        currentDone += currentProgress;
        totalSize += currentProgress;
    }
    else
    {
        childrenCanceled(id);
    }
    lastCurrentId = fileCopier->currentId();
    if (lastCurrentId < 0)
        lastCurrentId = id;
    currentProgressTime = 0;
    currentProgress = 0;
    QTimer::singleShot(0, q_ptr, SLOT(showProgress()));
}

void DataClientPrivate::canceled()
{
    totalSize = currentDone;
    currentDir = dirCount;
    currentFile = requests.size() - dirCount;
    showProgress();
}

void DataClientPrivate::childrenCanceled(int id)
{
    if (!fileCopier->isDir(id))
        return;
    QList<int> children = fileCopier->entryList(id);
    QListIterator<int> itChild(children);
    while (itChild.hasNext())
    {
        int childId = itChild.next();
        currentFile++;
        totalSize -= requests[childId].size;
        if (fileCopier->isDir(childId))
        {
            currentDir++;
            childrenCanceled(childId);
        }
    }
}

void DataClientPrivate::showProgress()
{
    //qDebug() << "requests[lastCurrentId].size: " << requests[lastCurrentId].size;
    qint64 completed = currentDone + currentProgress;
    //qDebug() << "completed: " << completed;
    //setFileLabel(currentFile - currentDir, requests.size() - dirCount);
    //setDirLabel(currentDir, dirCount);
    setCompleted(completed, totalSize, currentDoneTime + currentProgressTime);
    if (lastCurrentId == -1)
    {
        setCurrentProgress(0, 1);
        //setFileNames(QString(), QString());
    }
    else
    {
        if (fileCopier->currentId() == -1)
            setCurrentProgress(1, 1);
        else
            setCurrentProgress(currentProgress, requests[lastCurrentId].size);
        //setFileNames(requests[lastCurrentId].source, requests[lastCurrentId].dest);
    }
}

void DataClientPrivate::reset()
{
    currentFile = 0;
    totalSize = 0;
    currentProgress = 0;
    currentDone = 0;
    currentProgressTime = 0;
    currentDoneTime = 0;
    dirCount = 0;
    currentDir = 0;
    lastCurrentId = -1;
    completedPercent = 0;
    requests.clear();
    if (fileCopier)
    {
        QList<int> idList = fileCopier->pendingRequests();
        QListIterator<int> itId(idList);
        while (itId.hasNext())
        {
            addRequest(itId.next());
        }
    }

    showProgress();
}


void DataClientPrivate::setCurrentProgress(qint64 completed, qint64 totalSize)
{
    int percent = totalSize > 0 ? (int)((double)completed * 100 / totalSize + 0.5) : 0;
    //qDebug() << percent << completed << totalSize;
//    completedPercent = percent;
//    emit q_ptr->progressUpdatedSignal(percent);
    //ui.currentProgressBar->setValue(percent);
}

void DataClientPrivate::setCompleted(qint64 completed, qint64 totalSize, int msecs)
{
    //完成数
    QString strCompleted = DataClient::tr("%1/%2").arg(formatSize(completed)).arg(formatSize(totalSize));
    emit q_ptr->completedTrasferedSignal(strCompleted);

    //进度
    int percent = totalSize > 0 ? (int)( (double)completed * 100 / totalSize + 0.5) : 100;
    completedPercent = percent;
    emit q_ptr->progressUpdatedSignal(percent);

    //ui.totalProgressBar->setValue(percent);

    //速度
    QString transferStr = DataClient::tr("0 B");
    if (msecs > 0)
    {
        int transfer = (int)((double)completed * 1000 / msecs);
        transferStr = formatSize(transfer);
    }
    else
    {
        transferStr = formatSize(completed);
    }
    QString strTransferRemain = DataClient::tr("%1/s").arg(transferStr);
    emit q_ptr->transferRateUpdateSignal(strTransferRemain);
    // ui.remainingLabel->setText(str);

    QString estStr("00:00:00");
    if (completed > 0)
    {
        QTime est;
        int estMSecs = (int)((double)totalSize * msecs / completed - msecs + 0.5);
        est = est.addMSecs(estMSecs);
        estStr = est.toString("hh:mm:ss");
    }
    emit q_ptr->transferRemainTimeSignal(estStr);

    QString totalTime("00:00:00");
    QTime totTime;
    totTime = totTime.addMSecs(msecs);
    totalTime =totTime.toString("hh:mm:ss");
    emit q_ptr->transferTotalTimeSiganl(totalTime);



    //状态
    QString state;
    if (fileCopier && fileCopier->state() != QtFileCopier::Completed)
        state = DataClient::tr("正在传输...");
    else
        state = DataClient::tr("已完成");
    QString strState = DataClient::tr("%3").arg(state);
    emit q_ptr->transferStateSignal(strState);
    //q->setWindowTitle(strCompletedPercent);

}

void DataClientPrivate::addRequest(int id)
{
    Request r;
    r.source = fileCopier->sourceFilePath(id);
    r.dest = fileCopier->destinationFilePath(id);
    QFileInfo fis(r.source);
    r.size = fis.size();
    if (fileCopier->isDir(id))
    {
        r.size = 0;
        dirCount++;
    }
    requests[id] = r;

    totalSize += r.size;
}

/***********************************************************************************
 ***********************************************************************************
 ***********************************************************************************
 ***********************************************************************************
 ***********************************************************************************
 */
/*
 * @brief DataClient::DataClient
 * @param parent
 */
DataClient::DataClient(QObject *parent, Qt::WFlags f)
//    :QObject(parent, f)
{
    d_ptr = new DataClientPrivate;
    Q_D(DataClient);
    d->q_ptr = this;
    d->init();
    //copier = new QtFileCopier(this);
//    setFileCopier(copier);
}

DataClient::DataClient(QtFileCopier *copier, QObject *parent, Qt::WFlags f)
//    :QObject(parent, f)
{
    d_ptr = new DataClientPrivate;
    Q_D(DataClient);
    d->q_ptr = this;
    d->init();

    setFileCopier(copier);
}

DataClient::~DataClient()
{
    Q_D(DataClient);
    delete d;
    //delete copier;
}

void DataClient::setFileCopier(QtFileCopier *copier)
{
    Q_D(DataClient);
    if (d->fileCopier == copier)
        return;

    if (copier && copier->state() != QtFileCopier::Idle)
    {
        // can't start track copier while not idle
        return;
    }

    if (d_ptr->fileCopier)
    {
        disconnect(d->fileCopier, SIGNAL(error(int, QtFileCopier::Error, bool)),
                this, SLOT(error(int, QtFileCopier::Error, bool)));
        disconnect(d->fileCopier, SIGNAL(stateChanged(QtFileCopier::State)),
                this, SLOT(stateChanged(QtFileCopier::State)));
        disconnect(d->fileCopier, SIGNAL(started(int)),
                this, SLOT(started(int)));
        disconnect(d->fileCopier, SIGNAL(done(bool)),
                this, SLOT(done(bool)));
        disconnect(d->fileCopier, SIGNAL(dataTransferProgress(int, qint64)),
                this, SLOT(dataTransferProgress(int, qint64)));
        disconnect(d->fileCopier, SIGNAL(finished(int, bool)),
                this, SLOT(finished(int, bool)));
        disconnect(d->fileCopier, SIGNAL(canceled()),
                this, SLOT(canceled()));
    }

    d->fileCopier = copier;

    if (d->fileCopier)
    {
        connect(d->fileCopier, SIGNAL(error(int, QtFileCopier::Error, bool)),
                this, SLOT(error(int, QtFileCopier::Error, bool)));
        connect(d->fileCopier, SIGNAL(stateChanged(QtFileCopier::State)),
                this, SLOT(stateChanged(QtFileCopier::State)));
        connect(d->fileCopier, SIGNAL(started(int)),
                this, SLOT(started(int)));
        connect(d->fileCopier, SIGNAL(done(bool)),
                this, SLOT(done(bool)));
        connect(d->fileCopier, SIGNAL(dataTransferProgress(int, qint64)),
                this, SLOT(dataTransferProgress(int, qint64)));
        connect(d->fileCopier, SIGNAL(finished(int, bool)),
                this, SLOT(finished(int, bool)));
        connect(d->fileCopier, SIGNAL(canceled()),
                this, SLOT(canceled()));
    }
}

QtFileCopier *DataClient::fileCopier() const
{
    Q_D(const DataClient);
    return d->fileCopier;
}

int DataClient::minimumDuration() const
{
    Q_D(const DataClient);
    return d->showTimer->interval();
}

void DataClient::setMinimumDuration(int ms)
{
    Q_D(DataClient);
    d->showTimer->setInterval(ms);
}

bool DataClient::autoClose() const
{
    Q_D(const DataClient);
    return d->autoClose;
}

void DataClient::setAutoClose(bool b)
{
    Q_D(DataClient);
    d->autoClose = b;
}

void DataClient::reject()
{
    Q_D(DataClient);
    if (d->fileCopier && d->fileCopier->state() != QtFileCopier::Idle)
    {
        qDebug() << "state:" << d->fileCopier->state();
        d->fileCopier->cancelAll();
    }
    else
    {
        qDebug() << "not completed";
    }
    //QDialog::reject();
}

//#include "qtfilecopier.moc"
//#include "moc_qtfilecopier.cpp"
