#ifndef CLIENT_H
#define CLIENT_H

#include "Connect_global.h"
#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include "server.h"

class PeerManager;
//class Connection;

class CONNECTSHARED_EXPORT Client : public QObject
{
    Q_OBJECT
public:
    Client();

    /*聊天消息发送*/
    void sendMessage(const QString &message);
    QString nickName() const;
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;

signals:
    /*聊天消息发送*/
    void newMessage(const QString &from, const QString &message);
    void newParticipant(const QString &nick);//用户上线
    void participantLeft(const QString &nick);//用户离线

private slots:
    void newConnection(Connection *connection);
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();

private:
     void removeConnection(Connection *connection);

     Server server;
     QMultiHash<QHostAddress, Connection *> peers;
};

#endif // CLIENT_H
