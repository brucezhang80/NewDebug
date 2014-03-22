#ifndef BROADCASTSERVER_H
#define BROADCASTSERVER_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QTimer>
#include <QtNetwork>

class BroadcastClient;
class Connection;

class BroadcastServer : public QObject
{
    Q_OBJECT
public:
    explicit BroadcastServer(BroadcastClient *parent = 0);
    void setServerPort(int port);
    QByteArray userName() const;
    void startBroadcasting();
    bool isLocalHostAddress(const QHostAddress &address);
    QUdpSocket             *broadcastSocket;        //广播套接字

signals:
    void newConnection(QByteArray, QHostAddress, int);
    
public slots:

private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();

private:
     void updateAddresses();

     BroadcastClient        *broadcastClient;

     QList<QHostAddress>    broadcastAddresses;     //广播地址
     QList<QHostAddress>    ipAddresses;            //ip地址
     QTimer                 broadcastTimer;         //每时间段进行一次广播
     QByteArray             username;               //用户
     int                    serverport;             //服务器端口
};

#endif // BROADCASTSERVER_H
