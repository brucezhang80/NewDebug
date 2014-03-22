#ifndef PERSONCLIENT_H
#define PERSONCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QAbstractSocket>
#include <QHash>

#include "tcpserver.h"

class PeerBroadcast;

class PersonClient : public QObject
{
    Q_OBJECT
public:
    explicit PersonClient(QObject *parent = 0);
    QString getUserName() const;
    QString getHostName() const;
    QString getHostAddress() const;
    QString getHostPort() const;
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;

signals:
    void newParticipantSignal(const QString&, const QString&, const QString&);
    void participantLeftSignal(const QString&, const QString&, const QString&);

public slots:
    void readyForUse();
    void newConnection(Connection *connection);
    void disconnected();
    void connectionError(QAbstractSocket::SocketError socketError);
    void removeConnection(Connection*);

private:
    PeerBroadcast   *m_peerBroadcast;
    QMultiHash<QHostAddress, Connection *> m_peers;
    TcpServer       m_server;
    QString         m_hostName;
    QString         m_hostAddress;
    QString         m_hostPort;
};

#endif // PERSONCLIENT_H
