#ifndef BROADCASTCLIENT_H
#define BROADCASTCLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QtNetwork>
#include "connection.h"

class BroadcastServer;

class BroadcastClient : public QObject
{
    Q_OBJECT
public:
    BroadcastClient();

    void sendMessage(const QString &message);       // 发送消息
    QString nickName() const;                       // 用户名
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;    // 已有连接

signals:
     void newMessage(const QString &from, const QString &message);
     void newParticipant(const QString &nick);      // 用户上线
     void participantLeft(const QString &nick);     // 用户离线

private slots:
    void newConnection(QByteArray, QHostAddress, int);    // 新连接
//     void connectionError(QAbstractSocket::SocketError socketError);    // 连接出错
////     void disconnected();                           // 断开连接
    void readyForUse();                             // 就绪

 private:
//     void removeConnection(Connection *connection); // 移除连接

     QTcpServer                                 tcpServer;
     BroadcastServer                            *broadcastServer;
     QMultiHash<QHostAddress, int>     peers;
};

#endif // BROADCASTCLIENT_H
