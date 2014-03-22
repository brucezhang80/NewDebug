#include "connection.h"
#include "broadcastclient.h"
#include "broadcastserver.h"
#include <QStringList>
#include <QDebug>

static const qint32 BroadcastInterval = 2000;   //广播套接字时间延时为2000毫秒
static const unsigned broadcastPort = 45000;    //广播端口

BroadcastServer::BroadcastServer(BroadcastClient *parent) :
    QObject(parent)
{
    this->broadcastClient = parent;
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*" << "HOSTNAME.*" << "DOMAINNAME.*";

    // 取得系统用户名
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
    // 绑定套接字,设置绑定任意主机地址,端口45000
    broadcastSocket->bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    // 接收其他的广播套接字
    connect(broadcastSocket, SIGNAL(readyRead()), this, SLOT(readBroadcastDatagram()));

    // 设置广播延时
    broadcastTimer.setInterval( BroadcastInterval );

    // 每2秒钟发送一次套接广播
    connect( &broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcastDatagram()) );
}

/*
 * @设置服务器端口
 * @brief BroadcastServer::setServerPort
 * @param port
 */
void BroadcastServer::setServerPort(int port)
{
    serverport = port;
}

/*
 * @用户名
 * @brief BroadcastServer::userName
 * @return
 */
QByteArray BroadcastServer::userName() const
{
    return username;
}

/*
 * @开始广播,每隔2秒激活一次广播
 * @brief BroadcastServer::startBroadcasting
 */
void BroadcastServer::startBroadcasting()
{
    broadcastTimer.start();
}

/*
 * @ 是否为本地主机地址
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
 * @ 发送广播套接字,实现数据包发送
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
        if (broadcastSocket->writeDatagram(datagram.data(),datagram.size(), address, broadcastPort) == -1) // 参数依次为:数据包自身,发送到的地址和端口
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
 * @读取其他发送的广播套接字,实现数据包接收
 * @brief BroadcastServer::readBroadcastDatagram
 */
void BroadcastServer::readBroadcastDatagram()
{
    while (broadcastSocket->hasPendingDatagrams()) // 当有数据包读入时
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(broadcastSocket->pendingDatagramSize());
        if (broadcastSocket->readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort) == -1) // 参数依次为:数据包自身,数据包大小,发送到的地址和端口
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
 * @更新广播地址
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
