#include "send_file_client.h"
#include "system.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

void SendFileClientPrivate::initialize()
{
    Q_Q(SendFileClient);
    qRegisterMetaType<qint64>("qint64");

    this->fileCopier = 0;

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
}

void SendFileClientPrivate::error(int id, QtFileCopier::Error error, bool stopped)
{
    Q_Q(SendFileClient);
    if(!stopped) return;

    Request r = requests[id];
    showProgress();

    switch (error)
    {
    case QtFileCopier::SourceNotExists:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Source doesn't exist"));
        break;
    case QtFileCopier::DestinationExists:
    {
//                    QtOverwriteDialog mb(q_ptr);
//                    switch (mb.execute(r.source, r.dest))
//                    {
//                        case QtOverwriteDialog::Cancel       : fileCopier->cancelAll();
//                                                               fileCopier->retry();        break;
//                        case QtOverwriteDialog::Skip         : fileCopier->skip();         break;
//                        case QtOverwriteDialog::SkipAll      : fileCopier->skipAll();      break;
//                        case QtOverwriteDialog::Overwrite    : fileCopier->overwrite();    break;
                    fileCopier->overwriteAll(); break;
                        default                                 : fileCopier->retry();        break;
//                    }
//                    handled = true;
    };
        break;
    case QtFileCopier::SourceDirectoryOmitted:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Source file is a directory. Omitting source directory"));
        break;
    case QtFileCopier::SourceFileOmitted:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Source directory is a file. Omitting source file."));
        break;
    case QtFileCopier::PathToDestinationNotExists:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Cannot create destination directory."));
        break;
    case QtFileCopier::CannotCreateDestinationDirectory:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Cannot open source file. Please check permissions."));
        break;
    case QtFileCopier::CannotOpenSourceFile:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Cannot open destination file. Please check permissions."));
        break;
    case QtFileCopier::CannotOpenDestinationFile:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Force: Cannot remove destination file."));
        break;
    case QtFileCopier::CannotRemoveDestinationFile:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Cannot create symbolic link targeting to source."));
        break;
    case QtFileCopier::CannotCreateSymLink:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Source file is a directory. Omitting source directory"));
        break;
    case QtFileCopier::CannotReadSourceFile:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Cannot read source file."));
        break;
    case QtFileCopier::CannotWriteDestinationFile:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Cannot write destination file."));
        break;
    case QtFileCopier::CannotRemoveSource:
        m_errorString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Cannot remove source"));
        break;
    }
}

void SendFileClientPrivate::stateChanged(QtFileCopier::State state)
{
    Q_Q(SendFileClient);

    if (state == QtFileCopier::Busy)
    {
        if (fileCopier->state() == QtFileCopier::Idle)
        {
            reset();
            showProgress();
            showTimer->start();
        }
        startTime.start();
    }
    else if (state == QtFileCopier::Completed)
    {
        showTimer->stop();
        showProgress();
    }
    else
    {
        showTimer->stop();
    }

    switch (state)
    {
    case QtFileCopier::Idle:
        m_stateString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Idle"));
        break;
    case QtFileCopier::Busy:
        m_stateString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Busy"));
        break;
    case QtFileCopier::Completed:
        m_stateString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("Completed"));
        break;
    case QtFileCopier::WaitingForInteraction:
        m_stateString = QT_TRANSLATE_NOOP(SendFileClient, QObject::tr("WaitingForInteraction"));
        break;
    }
}

void SendFileClientPrivate::started(int id)
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

void SendFileClientPrivate::done(bool /*error*/)
{
    Q_Q(SendFileClient);

    qDebug() << "done";
    showTimer->stop();
    emit q_ptr->transferDoneSignal();
}

void SendFileClientPrivate::dataTransferProgress(int id, qint64 progress)
{
    Q_ASSERT(lastCurrentId == id);
    Q_UNUSED(id);
    currentProgressTime = startTime.elapsed();
    currentProgress = progress;
    QTimer::singleShot(0, q_ptr, SLOT(showProgress()));
}

void SendFileClientPrivate::finished(int id, bool error)
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

void SendFileClientPrivate::canceled()
{
    totalSize = currentDone;
    currentDir = dirCount;
    currentFile = requests.size() - dirCount;
    showProgress();
}

void SendFileClientPrivate::childrenCanceled(int id)
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

void SendFileClientPrivate::showProgress()
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

void SendFileClientPrivate::showDialog()
{
}

void SendFileClientPrivate::reset()
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

void SendFileClientPrivate::setCurrentProgress(qint64 completed, qint64 totalSize)
{
    int percent = totalSize > 0 ? (int)((double)completed * 100 / totalSize + 0.5) : 0;

}

void SendFileClientPrivate::setCompleted(qint64 completed, qint64 totalSize, int msecs)
{
    //完成数
    QString strCompleted = QString("%1/%2").arg(System::formatSize(completed)).arg(System::formatSize(totalSize));
    emit q_ptr->trasfereCompletedSignal(strCompleted);

    //进度
    int percent = totalSize > 0 ? (int)( (double)completed * 100 / totalSize + 0.5) : 100;
    completedPercent = percent;
    emit q_ptr->transferProgressUpdatedSignal(percent);

    //ui.totalProgressBar->setValue(percent);

    //速度
    QString transferStr = QString("0 B");
    if (msecs > 0)
    {
        int transfer = (int)((double)completed * 1000 / msecs);
        transferStr = System::formatSize(transfer);
    }
    else
    {
        transferStr = System::formatSize(completed);
    }
    QString strTransferRemain = QString("%1/s").arg(transferStr);
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
    if (fileCopier && fileCopier->state() == QtFileCopier::Busy)
        state = SendFileClient::tr("transfering...");
    else if(fileCopier && fileCopier->state() == QtFileCopier::Completed)
        state = SendFileClient::tr("done");

    if (fileCopier && fileCopier->state() == QtFileCopier::Canceled)
        state = SendFileClient::tr("Interrupted");
    QString strState = QString("%3").arg(state);
    emit q_ptr->transferStateSignal(strState);
    //q->setWindowTitle(strCompletedPercent);
}

void SendFileClientPrivate::addRequest(int id)
{
    Request r;
    r.source = fileCopier->sourceFilePath(id);
    r.destination = fileCopier->destinationFilePath(id);
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



///////////////////////////////////////////////////////////////////////////
/*
 * @brief SendFileClient::SendFileClient
 * @param parent
 */
SendFileClient::SendFileClient(QObject *parent, Qt::WFlags flag)
    :/*QObject(parent), QObject(flag),*/ d_ptr(new SendFileClientPrivate)
{
    Q_D(SendFileClient);
    d->q_ptr = this;
    d->initialize();
}

SendFileClient::SendFileClient(QtFileCopier *copier, QObject *parent, Qt::WFlags flag)
    :/*QObject(parent), QObject(flag),*/d_ptr(new SendFileClientPrivate)
{
    Q_D(SendFileClient);
    d->q_ptr = this;
    d->initialize();
    setFileCopier(copier);
}

SendFileClient::~SendFileClient()
{
    Q_D(SendFileClient);
    delete d;
}

void SendFileClient::setFileCopier(QtFileCopier *copier)
{
    Q_D(SendFileClient);
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

QtFileCopier *SendFileClient::fileCopier() const
{
    Q_D(const SendFileClient);
    return d->fileCopier;
}

int SendFileClient::minimumDuration() const
{
    Q_D(const SendFileClient);
    return d->showTimer->interval();
}

void SendFileClient::setMinimumDuration(int ms)
{
    Q_D(const SendFileClient);
    d->showTimer->setInterval(ms);
}
