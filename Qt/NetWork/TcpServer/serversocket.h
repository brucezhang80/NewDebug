#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QTcpServer>

class ServerSocket : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServerSocket(QObject *parent = 0);
    explicit ServerSocket(int port, QObject *parent = 0);
    
    void setServerPort(int port){ m_serverport = port;}
    inline int serverPort() const{ return m_serverport;}

signals:
    void newConnection();

public slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    int         m_serverport;
};

#endif // SERVERSOCKET_H
