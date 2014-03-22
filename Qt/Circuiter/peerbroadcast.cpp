#include "personclient.h"
#include "connection.h"
#include "peerbroadcast.h"

#include <QDebug>

static const qint32 BroadcastInterval = 2000;
static const unsigned broadcastPort = 52000;

PeerBroadcast::PeerBroadcast(PersonClient *perClient) :
    QObject(perClient)
{
    this->m_perClient = perClient;
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";

    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                m_userName = stringList.at(1).toUtf8();
                break;
            }
        }
    }

    if (m_userName.isEmpty())
        m_userName = "Unknown";

    updateAddresses();
    m_serverPort = 0;

    m_broadcastSocket.bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    connect(&m_broadcastSocket, SIGNAL(readyRead()), this, SLOT(readBroadcastDatagram()));

    m_broadcastTimer.setInterval(BroadcastInterval);
    connect(&m_broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcastDatagram()));
}

void PeerBroadcast::setServerPort(int port)
{
    m_serverPort = port;
}

QByteArray PeerBroadcast::getUserName() const
{
    return m_userName;
}

QString PeerBroadcast::getLocalHostAddress() const
{
    return m_localHostAddr;
}

int PeerBroadcast::getServerPort() const
{
    return m_serverPort;
}

void PeerBroadcast::startBroadcasting()
{
    // timer start
    m_broadcastTimer.start();
}

bool PeerBroadcast::isLocalHostAddress(const QHostAddress &address)
{
    foreach (QHostAddress localAddress, m_ipAddresses)
    {
        if (address == localAddress)
            return true;
    }
    return false;
}

void PeerBroadcast::sendBroadcastDatagram()
{
    QByteArray datagram(m_userName);
    datagram.append('@');
    datagram.append(QByteArray::number(m_serverPort));

    bool validBroadcastAddresses = true;
    foreach (QHostAddress address, m_broadcastAddresses)
    {
        if (m_broadcastSocket.writeDatagram(datagram, address, broadcastPort) == -1)
            validBroadcastAddresses = false;
    }

    if (!validBroadcastAddresses)
        updateAddresses();
}

void PeerBroadcast::readBroadcastDatagram()
{
    while (m_broadcastSocket.hasPendingDatagrams())
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(m_broadcastSocket.pendingDatagramSize());
//        m_broadcastSocket.readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort);
        if (m_broadcastSocket.readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort) == -1)
            continue;

        QList<QByteArray> list = datagram.split('@');
        if (list.size() != 2)
            continue;

        int senderServerPort = list.at(1).toInt();
        if (isLocalHostAddress(senderIp) && senderServerPort == m_serverPort)
        {

            m_localHostAddr = senderIp.toString();
            continue;
        }

        if (!m_perClient->hasConnection(senderIp))
        {
            // 广播服务器地址和端口
            // socket接收并连接该广播地址和端口
            Connection *connection = new Connection(this);
            emit newConnection(connection);
            connection->connectToHost(senderIp, senderServerPort);
        }
    }
}

void PeerBroadcast::updateAddresses()
{
    m_broadcastAddresses.clear();
    m_ipAddresses.clear();
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, interface.addressEntries())
        {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost)
            {
                m_broadcastAddresses << broadcastAddress;
                m_ipAddresses << entry.ip();
            }
        }
    }
}
