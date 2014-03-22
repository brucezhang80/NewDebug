#ifndef SEND_FILE_CLIENT_H
#define SEND_FILE_CLIENT_H

#include "send_file_thread.h"
#include "qtfilecopier.h"

#include <QObject>
#include <QTimerEvent>
#include <QByteArray>
#include <QBitArray>
#include <QList>
#include <QTimer>
#include <QTime>
#include <QMap>

//class SendFileThread;
class SendFileClientPrivate;

class SendFileClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int minimumDuration READ minimumDuration WRITE setMinimumDuration)
public:
    friend class SendFileClientPrivate;
    SendFileClientPrivate       *d_ptr;

    explicit SendFileClient(QObject *parent = 0, Qt::WFlags flag = 0);
    SendFileClient(QtFileCopier *copier, QObject *parent = 0, Qt::WFlags flag = 0);
    ~SendFileClient();

    void setFileCopier(QtFileCopier *copier);
    QtFileCopier *fileCopier() const;

    int minimumDuration() const;
    void setMinimumDuration(int ms);

signals:
    void transferProgressUpdatedSignal(int);
    void transferRateUpdateSignal(QString);
    void trasfereCompletedSignal(QString);
    void transferStateSignal(QString);
    void transferRemainTimeSignal(QString);
    void transferTotalTimeSiganl(QString);
    void transferDoneSignal();

private:
    void setConnect();

    friend class SendFileClientPrivate;

    Q_DECLARE_PRIVATE(SendFileClient)
    Q_DISABLE_COPY(SendFileClient)

    Q_PRIVATE_SLOT(d_func(), void error(int, QtFileCopier::Error, bool))
    Q_PRIVATE_SLOT(d_func(), void stateChanged(QtFileCopier::State))
    Q_PRIVATE_SLOT(d_func(), void done(bool))
    Q_PRIVATE_SLOT(d_func(), void started(int))
    Q_PRIVATE_SLOT(d_func(), void dataTransferProgress(int, qint64))
    Q_PRIVATE_SLOT(d_func(), void finished(int, bool))
    Q_PRIVATE_SLOT(d_func(), void canceled())
    Q_PRIVATE_SLOT(d_func(), void showProgress())
};

//////////////////////////////////////////////////////////
class SendFileClientPrivate
{
    Q_DECLARE_PUBLIC(SendFileClient)
public:
    SendFileClient      *q_ptr;
    SendFileClientPrivate(){}
    void initialize();

    // State / error
    void error(int id, QtFileCopier::Error error, bool stopped);
    void stateChanged(QtFileCopier::State state);
    void started(int id);
    void done(bool error);
    void dataTransferProgress(int id, qint64 progress);
    void finished(int id, bool error);
    void canceled();
    void childrenCanceled(int id);

    void showProgress();
    void showDialog();

    void reset();
//    void setFileLabel(int currentFile, int totalFiles);
//    void setDirLabel(int currentDir, int totalDirs);
//    void setFileName(const QString &source, const QString *destination);
    void setCurrentProgress(qint64 completed, qint64 totalSize);
    void setCompleted(qint64 completed, qint64 totalSize, int msecs);

    void addRequest(int id);

    struct Request
    {
        QString source;
        QString destination;
        qint64  size;
    };

    QtFileCopier        *fileCopier;
    QTimer              *showTimer;
    QTime               startTime;

    QMap<int, Request>  requests;
    int                 currentFile;
    qint64              totalSize;
    qint64              currentProgress;
    qint64              currentDone;
    qint64              currentProgressTime;
    qint64              currentDoneTime;
    int                 dirCount;
    int                 currentDir;
    int                 completedPercent;
    int                 lastCurrentId;

    QString             m_errorString;
    QString             m_stateString;
};

#endif // SEND_FILE_CLIENT_H
