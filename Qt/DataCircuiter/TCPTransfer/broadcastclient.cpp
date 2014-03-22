#include "broadcastserver.h"
#include "broadcastclient.h"
#include <QDebug>

// ���￪ʼ���й㲥
// ���չ㲥,ȡ�ù㲥�û���,������ַ�Լ��������ݰ�

BroadcastClient::BroadcastClient()
{
    tcpServer.listen(QHostAddress::Any);
    broadcastServer = new BroadcastServer(this);
    broadcastServer->setServerPort(tcpServer.serverPort());
    broadcastServer->startBroadcasting();

    QObject::connect(broadcastServer, SIGNAL(newConnection(QByteArray, QHostAddress, int)), this, SLOT(newConnection(QByteArray, QHostAddress, int)));
    QObject::connect( &tcpServer, SIGNAL(newConnection()), this, SLOT(readyForUse()) );
}

void BroadcastClient::sendMessage(const QString &message)
{
    if (message.isEmpty())
        return;
}

QString BroadcastClient::nickName() const
{
    // ϵͳ�û���+������ַ��+�˿ں�
    return QString(broadcastServer->userName()) + '@' + QHostInfo::localHostName() + ':' + QString::number(tcpServer.serverPort());
}

bool BroadcastClient::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
        return peers.contains(senderIp);

//    if (!peers.contains(senderIp))
//        return false;

//    qDebug() << "port << senderPort";

    QList<int> connections = peers.values(senderIp);
    qDebug() << "================";
    foreach (int port, connections)
    {

        qDebug() <<  port <<senderPort;
        if (port == senderPort)
            return true;
    }

    return false;
}

void BroadcastClient::readyForUse()
{
    qDebug() << "nick";

//    Connection *connection = qobject_cast<Connection *>(sender());
//    if (!connection /* || hasConnection(connection->peerAddress(), connection->peerPort())*/)
//        return;

//    connect(connection, SIGNAL(newMessage(QString,QString)), this, SIGNAL(newMessage(QString,QString)));

//    peers.insert(connection->peerAddress(), connection);
    QString nick = nickName();
    if (!nick.isEmpty())
        emit newParticipant(nick);

}

//void BroadcastClient::removeConnection(Connection *connection)
//{
//    if (peers.contains(connection->peerAddress()))
//    {
//        peers.remove(connection->peerAddress());
//        QString nick = connection->name();
//        if (!nick.isEmpty())
//        emit participantLeft(nick);
//    }
//    connection->deleteLater();
//}


void BroadcastClient::newConnection(QByteArray name, QHostAddress addr, int port)
{
//    Connection *connection = new Connection(this);
//    connection->setGreetingMessage(broadcastServer->userName());
//    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
//    //connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
//    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));

    peers.insert(addr, port);
    QString nick = (QString(name) + '@' + addr.toString() + ':' + QString("%1").arg(port));
    if (!nick.isEmpty())
        emit newParticipant(nick);
}

