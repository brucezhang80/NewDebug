#include "personclient.h"
#include "connection.h"
#include "peerbroadcast.h"

#include <QDebug>

PersonClient::PersonClient(QObject *parent) :
    QObject(parent)
{
    m_peerBroadcast = new PeerBroadcast(this);
    m_peerBroadcast->setServerPort(m_server.serverPort());
    m_peerBroadcast->startBroadcasting();

    // 广播地址和端口,有新的客户端连接
    QObject::connect(m_peerBroadcast, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
    QObject::connect(&m_server, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
}

QString PersonClient::getUserName() const
{
    return m_peerBroadcast->getUserName();
}

QString PersonClient::getHostName() const
{
    return QString( QHostInfo::localHostName() );
}

QString PersonClient::getHostAddress() const
{

    return (m_peerBroadcast->getLocalHostAddress());
}

QString PersonClient::getHostPort() const
{
    return QString::number( m_peerBroadcast->getServerPort() );
}

/*
 * @当前的新用户连接是否已经存在,仅Ip地址和端口号为条件
 */
bool PersonClient::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
        return m_peers.contains(senderIp);

    if (!m_peers.contains(senderIp))
        return false;

    QList<Connection *> connections = m_peers.values(senderIp);
    foreach (Connection *connection, connections)
    {
        if (connection->peerPort() == senderPort)
            return true;
    }

    return false;
}

// connection handler

// 连接就绪,通知有新用户连接
void PersonClient::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(), connection->peerPort()))
    {
        qDebug() << "connection is not exists.";
        return;
    }
    m_peers.insert(connection->peerAddress(), connection);

    QString nick = connection->localHostName();
    m_hostName = connection->peerName();
    m_hostAddress = connection->peerAddress().toString();
    m_hostPort = QString::number(connection->peerPort());
    if (!nick.isEmpty())
        emit newParticipantSignal(nick, m_hostAddress, m_hostPort);
}

// 有新用户连接, 设置信号和槽
void PersonClient::newConnection(Connection *connection)
{
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));

}

// 有用户断开连接
void PersonClient::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

// 新用户连接出错处理
void PersonClient::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

// 有用户下线时移除一个连接
void PersonClient::removeConnection(Connection *connection)
{
    if (m_peers.contains(connection->peerAddress())) {
        m_peers.remove(connection->peerAddress());
        QString nick = connection->localHostName();
        QString name = connection->peerName();
        QString address = connection->peerAddress().toString();
        QString port = QString::number(connection->peerPort());
        if (!nick.isEmpty())
            emit participantLeftSignal(name, address, port);
    }
    connection->deleteLater();
}
