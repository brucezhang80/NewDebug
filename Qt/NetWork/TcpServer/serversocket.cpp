#include "serversocket.h"
#include <QTcpSocket>

ServerSocket::ServerSocket(QObject *parent) :
    QTcpServer(parent)
{
}

ServerSocket::ServerSocket(int port, QObject *parent):
    m_serverport(port), QTcpServer(parent)
{
}

void ServerSocket::incomingConnection(int socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket( parent());
    socket->setSocketDescriptor(socketDescriptor);
//    emit newClient(socket);
}
