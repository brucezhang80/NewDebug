#ifndef SERVER_H
#define SERVER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QTcpSocket;
class QHostAddress;

QT_BEGIN_NAMESPACE

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

    bool listen();
    QString errorString() const;

    inline QString host() const { return m_serverHost;}

    void setPort( int port ){m_serverPort = port; }
    inline int port() const { return m_serverPort;}

    void close();

signals:
    
private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QString         serverHost();

    QTcpServer*         m_server;
    QTcpSocket*         m_socket;

    QString             m_errorString;
    QString             m_serverHost;
    int                 m_serverPort;
};

#endif // SERVER_H
