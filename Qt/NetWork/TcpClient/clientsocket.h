#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QTcpSocket>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    ClientSocket(QString host, int port);

private slots:
    void slotConneted();
    void slotReadyRead();
    void slotDisconnected();
    void connectError();

private:
    QTcpSocket*     m_tcpsocket;
    QString         m_host;
    int             m_port;
};

#endif // CLIENTSOCKET_H
