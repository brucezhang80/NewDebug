#ifndef FILE_SERVER_H
#define FILE_SERVER_H

#include <QTcpServer>

class FileTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit FileTcpServer(QObject *parent = 0);
    
signals:
    
private slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    void startListening();
};

#endif // FILE_SERVER_H
