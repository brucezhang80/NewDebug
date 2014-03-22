#ifndef SERVER_H
#define SERVER_H

#include "Connect_global.h"
#include <QTcpServer>
#include <QtNetwork>

class Connection;

class CONNECTSHARED_EXPORT Server : public QTcpServer
{
    Q_OBJECT
public:
    Server( QObject *parent = 0 );
signals:
    void newConnection(Connection *connection);

protected:
    void incomingConnection(int socketDescriptor);
};

#endif // SERVER_H
