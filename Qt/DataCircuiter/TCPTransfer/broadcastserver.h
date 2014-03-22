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
    QUdpSocket             *broadcastSocket;        //�㲥�׽���

signals:
    void newConnection(QByteArray, QHostAddress, int);
    
public slots:

private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();

private:
     void updateAddresses();

     BroadcastClient        *broadcastClient;

     QList<QHostAddress>    broadcastAddresses;     //�㲥��ַ
     QList<QHostAddress>    ipAddresses;            //ip��ַ
     QTimer                 broadcastTimer;         //ÿʱ��ν���һ�ι㲥
     QByteArray             username;               //�û�
     int                    serverport;             //�������˿�
};

#endif // BROADCASTSERVER_H
