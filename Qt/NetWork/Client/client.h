#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
QT_BEGIN_NAMESPACE

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    void connectToServer();
    void connectToServer(const QString &host, const int port);

    inline QString errorString() const { return m_errorString; }
    inline QString address()const { return m_address; }

    void close();

signals:
    
public slots:
    void onConnected();
    void onReadyRead();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QString         localHost();

    QTcpSocket*     m_socket;
    QString         m_errorString;
    QString         m_address;
};

#endif // CLIENT_H
