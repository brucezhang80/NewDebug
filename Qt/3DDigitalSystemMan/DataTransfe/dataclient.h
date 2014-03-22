#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <QtCore\qglobal.h>
#if defined(DATATRANSFE_LIBRARY)
#  define QT_DATACLIENT_EXPORT Q_DECL_EXPORT
#else
#  define QT_DATACLIENT_EXPORT Q_DECL_IMPORT
#endif

#include "qtfilecopier.h"
#include <QObject>
#include <QTimerEvent>
#include <QByteArray>
#include <QBitArray>
#include <QList>
#include <QTimer>
#include <QTime>
#include <QMap>


class DataClientPrivate;

class QT_DATACLIENT_EXPORT DataClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int minimumDuration READ minimumDuration WRITE setMinimumDuration)
    Q_PROPERTY(bool autoClose READ autoClose WRITE setAutoClose)
public:
    DataClient(QObject *parent = 0, Qt::WFlags f = 0);
    DataClient(QtFileCopier *copier, QObject *parent = 0, Qt::WFlags f = 0);
    DataClientPrivate *d_ptr;
    friend class DataClientPrivate;

    ~DataClient();
//    QtFileCopier *copier;

    void setFileCopier(QtFileCopier *copier);
    QtFileCopier *fileCopier() const;

    int minimumDuration() const;
    void setMinimumDuration(int ms);

    bool autoClose() const;
    void setAutoClose(bool b);

//    void setDestinationPath(const QString &directory);
//    QString destinationPath()const;
signals:
    void progressUpdatedSignal(int);
    void transferRateUpdateSignal(QString);
    void completedTrasferedSignal(QString);
    void transferStateSignal(QString);
    void transferRemainTimeSignal(QString);
    void transferTotalTimeSiganl(QString);
    void transferDoneSignal(DataClient*);

public Q_SLOTS:
    void reject();

private:

    //friend class DataClientPrivate;
    Q_DECLARE_PRIVATE(DataClient)
    Q_DISABLE_COPY(DataClient)

    Q_PRIVATE_SLOT(d_func(), void error(int, QtFileCopier::Error, bool))
    Q_PRIVATE_SLOT(d_func(), void stateChanged(QtFileCopier::State))
    Q_PRIVATE_SLOT(d_func(), void done(bool))
    Q_PRIVATE_SLOT(d_func(), void started(int))
    Q_PRIVATE_SLOT(d_func(), void dataTransferProgress(int, qint64))
    Q_PRIVATE_SLOT(d_func(), void finished(int, bool))
    Q_PRIVATE_SLOT(d_func(), void canceled())
    Q_PRIVATE_SLOT(d_func(), void showProgress())
    //Q_PRIVATE_SLOT(d_func(), void showDialog())
};


/*
 * @brief The DataClientPrivate class
 */
class DataClientPrivate
{
    DataClient* q_ptr;
    Q_DECLARE_PUBLIC(DataClient)
public:
    DataClientPrivate(){}

    void init();
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
    void setFileLabel(int currentFile, int totalFiles);
    void setDirLabel(int currentDir, int totalDirs);
    void setFileName(const QString &source, const QString *destination);
    void setCurrentProgress(qint64 completed, qint64 totalSize);
    void setCompleted(qint64 completed, qint64 totalSize, int msecs);

    void addRequest(int id);

    struct Request
    {
        QString source;
        QString dest;
        qint64 size;
    };

    QtFileCopier *fileCopier;
    bool autoClose;
    QTimer *showTimer;
    QTime startTime;

    QMap<int, Request> requests;
    int currentFile;
    qint64 totalSize;
    qint64 currentProgress;
    qint64 currentDone;
    qint64 currentProgressTime;
    qint64 currentDoneTime;
    int dirCount;
    int currentDir;
    int completedPercent;

    int lastCurrentId;
};

#endif // DATACLIENT_H
