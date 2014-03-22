#include "connection.h"
#include "broadcastclient.h"
#include "broadcastserver.h"
#include <QStringList>
#include <QDebug>

static const qint32 BroadcastInterval = 2000;   //�㲥�׽���ʱ����ʱΪ2000����
static const unsigned broadcastPort = 45000;    //�㲥�˿�

BroadcastServer::BroadcastServer(BroadcastClient *parent) :
    QObject(parent)
{
    this->broadcastClient = parent;
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*" << "HOSTNAME.*" << "DOMAINNAME.*";

    // ȡ��ϵͳ�û���
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                username = stringList.at(1).toUtf8();
                break;
            }
        }
    }

    if ( username.isEmpty() )
    {
        username = "Unknown";
    }

    updateAddresses();
    serverport = 0;

    broadcastSocket = new QUdpSocket(this);
    // ���׽���,���ð�����������ַ,�˿�45000
    broadcastSocket->bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    // ���������Ĺ㲥�׽���
    connect(broadcastSocket, SIGNAL(readyRead()), this, SLOT(readBroadcastDatagram()));

    // ���ù㲥��ʱ
    broadcastTimer.setInterval( BroadcastInterval );

    // ÿ2���ӷ���һ���׽ӹ㲥
    connect( &broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcastDatagram()) );
}

/*
 * @���÷������˿�
 * @brief BroadcastServer::setServerPort
 * @param port
 */
void BroadcastServer::setServerPort(int port)
{
    serverport = port;
}

/*
 * @�û���
 * @brief BroadcastServer::userName
 * @return
 */
QByteArray BroadcastServer::userName() const
{
    return username;
}

/*
 * @��ʼ�㲥,ÿ��2�뼤��һ�ι㲥
 * @brief BroadcastServer::startBroadcasting
 */
void BroadcastServer::startBroadcasting()
{
    broadcastTimer.start();
}

/*
 * @ �Ƿ�Ϊ����������ַ
 * @brief BroadcastServer::isLocalHostAddress
 * @param address
 * @return
 */
bool BroadcastServer::isLocalHostAddress(const QHostAddress &address)
{
    foreach (QHostAddress localAddress, ipAddresses)
    {
        if (address == localAddress)
            return true;
    }
    return false;
}

/*
 * @ ���͹㲥�׽���,ʵ�����ݰ�����
 * @brief BroadcastServer::sendBroadcastDatagram
 */
void BroadcastServer::sendBroadcastDatagram()
{
    QByteArray datagram(username);
    datagram.append('@');
    datagram.append(QByteArray::number(serverport));

    bool validBroadcastAddresses = true;
    foreach (QHostAddress address, broadcastAddresses)
    {
        qDebug() << address.toString();
        if (broadcastSocket->writeDatagram(datagram.data(),datagram.size(), address, broadcastPort) == -1) // ��������Ϊ:���ݰ�����,���͵��ĵ�ַ�Ͷ˿�
        {
            qDebug() << "bad datagram...";
            validBroadcastAddresses = false;
        }
        //qDebug() << address.toString() << broadcastPort;
    }

    if (!validBroadcastAddresses)
        updateAddresses();
}

/*
 * @��ȡ�������͵Ĺ㲥�׽���,ʵ�����ݰ�����
 * @brief BroadcastServer::readBroadcastDatagram
 */
void BroadcastServer::readBroadcastDatagram()
{
    while (broadcastSocket->hasPendingDatagrams()) // �������ݰ�����ʱ
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(broadcastSocket->pendingDatagramSize());
        if (broadcastSocket->readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort) == -1) // ��������Ϊ:���ݰ�����,���ݰ���С,���͵��ĵ�ַ�Ͷ˿�
        {

            continue;
        }

        qDebug() << datagram.data() << senderIp.toString() << senderPort;

        QList<QByteArray> list = datagram.split('@');
        if (list.size() != 2)
            continue;

        int senderServerPort = list.at(1).toInt();
        if (isLocalHostAddress(senderIp) && senderServerPort == serverport)
            continue;

        //if (broadcastClient->hasConnection(senderIp, senderPort) )
        {
            //Connection *connection = new Connection(this);
            //connection->connectToHost(senderIp, senderServerPort);
            emit newConnection(username, senderIp, senderServerPort);
            broadcastSocket->connectToHost(senderIp, senderServerPort);
            //qDebug() << senderIp.toString() << senderServerPort;
        }
    }
}

/*
 * @���¹㲥��ַ
 * @brief BroadcastServer::updateAddresses
 */
void BroadcastServer::updateAddresses()
{
    broadcastAddresses.clear();
    ipAddresses.clear();
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, interface.addressEntries())
        {
            QHostAddress broadcastAddr = entry.broadcast();
            if (broadcastAddr != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost)
            {
                broadcastAddresses << broadcastAddr;
                ipAddresses << entry.ip();
                qDebug() << "------------" << entry.ip().toString();
            }
        }
    }
}
