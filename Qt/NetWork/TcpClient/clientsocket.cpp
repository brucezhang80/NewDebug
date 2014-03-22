#include "clientsocket.h"

ClientSocket::ClientSocket(QString host, int port)
{
    m_tcpsocket = new QTcpSocket(this);
    connect(m_tcpsocket, SIGNAL(connected()), this, SLOT(slotConneted()));
    connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_tcpsocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(m_tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectError(QAbstractSocket::SocketError)));

    m_host = host;
    m_port = port;

    m_tcpsocket->abort();
    m_tcpsocket->connectToHost(m_host, m_port);
}

void ClientSocket::slotConneted()
{
}

void ClientSocket::slotReadyRead()
{
}

void ClientSocket::slotDisconnected()
{
}

void ClientSocket::connectError()
{
}
