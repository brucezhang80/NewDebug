#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class Connection;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    
signals:
    void newConnection(Connection *connection);

public slots:

protected:
    void incomingConnection(int socketDescriptor);
};

#endif // TCPSERVER_H
