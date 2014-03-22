#ifndef RECV_FILE_THREAD_H
#define RECV_FILE_THREAD_H


#include <QThread>

class RecvFileThread : public QThread
{
    Q_OBJECT
public:
    explicit RecvFileThread(QObject *parent = 0);
    ~RecvFileThread();

    void run();

    void resumeTransfer();

signals:
    void recvFileError(QString errorString);
    void recvFileFinished();
    void stopTransfer();
    void abortTransfer();

private slots:

private:

};

#endif // RECV_FILE_THREAD_H
