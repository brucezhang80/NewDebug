#include "client.h"
#include "constance.h"

#include <QHostAddress>
#include <QNetworkInterface>

#include <QDebug>

Client::Client(QObject *parent) :
    QObject(parent)
{
    m_address = localHost();

    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                 this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Client::connectToServer()
{
    qDebug() << "Connect to server.";

    m_socket->abort();
    m_socket->connectToHost(QHostAddress::LocalHost, ServerPort);
}

void Client::connectToServer(const QString &host, const int port)
{
    qDebug() << "Connect to server: " << host << port;
    Q_ASSERT(!host.isEmpty());

    QHostAddress serverAddr = QHostAddress::QHostAddress(host);
    m_socket->abort();
    m_socket->connectToHost(serverAddr, port);
}

void Client::close()
{
    m_socket->abort();
}

void Client::onConnected()
{
    qDebug() << "Connection established.";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << (quint16)0;
    out << QString("New connection:") + m_address;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_socket->write(block);
    m_socket->flush();
}

void Client::onReadyRead()
{
    qDebug() << "Connection ready read.";
    quint16 blockSize = 0;
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_4_8);
    if (m_socket->bytesAvailable() < (int)sizeof(quint16))
        return;
    in >> blockSize;

    if (m_socket->bytesAvailable() < blockSize)
        return;

    QString socketData;
    in >> socketData;

    qDebug() << socketData;
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
         case QAbstractSocket::RemoteHostClosedError:
             break;
         case QAbstractSocket::HostNotFoundError:
             m_errorString = tr("The host was not found. Please check the "
                               "host name and port settings.");
             break;
         case QAbstractSocket::ConnectionRefusedError:
             m_errorString = tr("The connection was refused by the peer. "
                                "Make sure the fortune server is running, "
                                "and check that the host name and port "
                                "settings are correct.");
             break;
         default:
             m_errorString = tr("The following error occurred: %1.")
                                .arg(m_socket->errorString());
         }
}

QString Client::localHost()
{
    QString ipAddress = "0:0:0:0";
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    if (ipAddress == "0:0:0:0")
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    return ipAddress;
}
