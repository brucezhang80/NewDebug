#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include "dataclient.h"

#include <QThread>
#include <QQueue>

class DataRequest
{
public:
    DataRequest()
    {
        move = false;
        dir = false;
    }
    bool move;
    bool dir;
    QQueue<int> childrenQueue;
    QString source;
    QString dest;
};

class CopyThread : public QThread
{
    Q_OBJECT
public:
    explicit CopyThread(DataClient *parent = 0);
    
    void emitProgress(qint64 progress);

signals:
    void dataTransferProgress(qint64 progress);
    void started();
    void finished(bool error);
    
public slots:
    void  restart();
    void copy(const DataRequest &request);
    void copy(const QMap<int, DataRequest> &request);

    void progress();
    
protected:
    void run();
};

#endif // COPYTHREAD_H
