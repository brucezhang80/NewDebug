#ifndef MESSAGE_SERVER_H
#define MESSAGE_SERVER_H

#include "message.h"

#include <QObject>
#include <QUdpSocket>
#include <QAbstractSocket>

//int id = qRegisterMetaType(QAbstractSocket::SocketError);

class MessageServer : public QObject
{
    Q_OBJECT
public:
    explicit MessageServer(QObject *parent = 0);
    void start();

signals:
    void newUserMsg(Message msg);
    void newExitMsg(Message msg);
    void newMessage(Message msg);
    void error(QAbstractSocket::SocketError, QString errorString);
    void releaseFile(QString additionalInfo);
    void msgReaded(QString name);

public slots:
    void processSendMsg();
    void socketEerror(QAbstractSocket::SocketError errorCode);
private slots:
    void readPacket();
    
private:
    void handleMsg(Message &msg);
    bool isSupportedCommand(QByteArray &) const;
    void broadcastMsg(Message &msg);
    void broadcastUserMsg(Message &msg);
    bool isResendNeeded(Message &msg) const;
    void updateAddresses();

    void processRecvMsg(Message msg);
    void processRecvReleaseFilesMsg(Message msg);
    void processRecvReadMsg(Message msg);
    void processRecvSendMsg(Message msg);
    void processEntryMsg(Message msg);

    QList<QHostAddress> m_broadcastAddresses;
    QList<QHostAddress> m_ipAddresses;
    QUdpSocket          m_udpSocket;
};

#endif // MESSAGE_SERVER_H
