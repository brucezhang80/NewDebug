#include "connection.h"
#include "server.h"
#include <QDebug>

Server::Server( QObject *parent )
    : QTcpServer( parent )
{
    listen(QHostAddress::Any);

}

void Server::incomingConnection(int socketDescriptor)
{
    qDebug() << "incoming connection.";
    Connection *connection = new Connection(this);
    connection->setSocketDescriptor(socketDescriptor);
    emit newConnection(connection);
}
