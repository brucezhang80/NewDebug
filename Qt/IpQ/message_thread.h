#ifndef MESSAGE_THREAD_H
#define MESSAGE_THREAD_H

#include "message.h"
#include <QThread>
#include <QMutex>
#include <QMap>


class MessageServer;

class MessageThread : public QThread
{
    Q_OBJECT
public:
    explicit MessageThread(QObject *parent = 0);
    ~MessageThread();

    virtual void run();
//    void usleep(unsigned long secs);

    void addSendMsg(Message msg);
    void addSendMsgNotLock(Message msg);
    void removeSendMsg(QString packetNoString);
    void removeSendMsgNotLock(QString packetNoString);


signals:
    void newMsg(Message &msg);
    void newUserMsg(Message &msg);

private slots:
    void handleError(QAbstractSocket::SocketError errorCode, QString s);
    
public:
    QMutex                  m_lock;
    QMap<QString, Message>  m_sendMsgMap;
    MessageServer           *m_messageServer;
};

#endif // MESSAGE_THREAD_H
