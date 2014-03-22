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

    // �㲥��ַ�Ͷ˿�,���µĿͻ�������
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
 * @��ǰ�����û������Ƿ��Ѿ�����,��Ip��ַ�Ͷ˿ں�Ϊ����
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

// ���Ӿ���,֪ͨ�����û�����
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

// �����û�����, �����źźͲ�
void PersonClient::newConnection(Connection *connection)
{
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));

}

// ���û��Ͽ�����
void PersonClient::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

// ���û����ӳ�����
void PersonClient::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

// ���û�����ʱ�Ƴ�һ������
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
