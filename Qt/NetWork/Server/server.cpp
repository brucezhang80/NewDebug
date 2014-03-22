#include "server.h"
#include "constance.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>

#include <QDebug>

Server::Server(QObject *parent) :
    QObject(parent)
{
    m_serverHost = serverHost();
    m_serverPort = DefautPort;

    m_server = new QTcpServer(this);
    m_socket = new QTcpSocket(this);

    connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

Server::~Server()
{
    m_socket->deleteLater();
}

bool Server::listen()
{
    if( !m_server->listen(QHostAddress::Any, m_serverPort) )
    {
        m_errorString = m_server->errorString();
        return false;
    }

    qDebug() << "Server started.";

    return true;
}

QString Server::errorString() const
{
    return m_errorString;
}

void Server::close()
{
    qDebug() << "Server stopped.";

    m_server->close();
}

void Server::onNewConnection()
{
    qDebug() << "New connection.";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << (quint16)0;
    out << QString("Has connected server.");
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_socket = m_server->nextPendingConnection();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    m_socket->write(block);
    m_socket->flush();
//    m_socket->disconnectFromHost();
}

void Server::onReadyRead()
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

void Server::onDisconnected()
{
    qDebug() << "Connection disconnected.";

    disconnect(m_socket, SIGNAL(disconnected()));
    disconnect(m_socket, SIGNAL(readyRead()));
}

QString Server::serverHost()
{
    QString ipAddress = "0:0:0:0";
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress == "0:0:0:0")
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    return ipAddress;
}

