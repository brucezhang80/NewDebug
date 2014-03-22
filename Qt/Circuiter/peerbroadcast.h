#ifndef PEERBROADCAST_H
#define PEERBROADCAST_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QTimer>
#include <QUdpSocket>

class PersonClient;
class Connection;

class PeerBroadcast : public QObject
{
    Q_OBJECT
public:
    explicit PeerBroadcast(PersonClient *perClient);
    void            setServerPort(int port);
    QByteArray      getUserName() const;
    QString    getLocalHostAddress() const;
    int             getServerPort() const;
    void            startBroadcasting();
    bool            isLocalHostAddress(const QHostAddress &address);

signals:
    void newConnection(Connection *connection);

private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();

private:
    void updateAddresses();
//    void setLocalHostAddress(const QHostAddress &address){m_localHostAddr = address;}

    PersonClient        *m_perClient;
    QList<QHostAddress> m_broadcastAddresses;
    QList<QHostAddress> m_ipAddresses;
    QUdpSocket          m_broadcastSocket;
    QTimer              m_broadcastTimer;
    QByteArray          m_userName;
    QString             m_localHostAddr;
    int                 m_serverPort;
};

#endif // PEERBROADCAST_H
