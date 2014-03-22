#ifndef SEND_FILE_THREAD_H
#define SEND_FILE_THREAD_H

#include <QThread>
#include <QTcpSocket>

class SendFileThread : public QThread
{
    Q_OBJECT
public:
    explicit SendFileThread(QObject *parent = 0);
    explicit SendFileThread(int socketDescriptor, QObject *parent = 0);
    
    void run();

#if 0
signals:
    void error(QString errorString, QString packageId);
    void sendFileFinished(QString packageId, int fileId);

private slots:
    void handleSendFileError(QString errorString, QString packageId);
#endif

signals:
    
public slots:
    
private:
    int m_socketDescriptor;
};

#endif // SEND_FILE_THREAD_H
