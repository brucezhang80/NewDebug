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

    void sendMessage(const QString &message);       // ������Ϣ
    QString nickName() const;                       // �û���
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;    // ��������

signals:
     void newMessage(const QString &from, const QString &message);
     void newParticipant(const QString &nick);      // �û�����
     void participantLeft(const QString &nick);     // �û�����

private slots:
    void newConnection(QByteArray, QHostAddress, int);    // ������
//     void connectionError(QAbstractSocket::SocketError socketError);    // ���ӳ���
////     void disconnected();                           // �Ͽ�����
    void readyForUse();                             // ����

 private:
//     void removeConnection(Connection *connection); // �Ƴ�����

     QTcpServer                                 tcpServer;
     BroadcastServer                            *broadcastServer;
     QMultiHash<QHostAddress, int>     peers;
};

#endif // BROADCASTCLIENT_H
