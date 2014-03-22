#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>

static const int MaxBufferSize = 1024000;

class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);
    QString userName() const;
    QString localHostName() const;
    QString localHostAddress() const;
    QString localHostPort() const;

signals:
    void readyForUse();

public slots:
    void connected();
    void processReadyRead();
    void stop();

private:
    int readDataIntoBuffer(int maxSize = MaxBufferSize);
    int dataLengthForCurrentDataType();
    bool readProtocolHeader();
    bool hasEnoughData();
    void processData();

    QString m_userName;
    QString m_localHostName;
    QString m_localHostAddress;
    QString m_localHostPort;
    QString m_sendtime;


    QString m_connectedMessage;        // 连接消息
    bool    m_isOnline;
    QByteArray m_buffer;
    int numBytesForCurrentDataType;
    int transferTimerId;
};

#endif // CONNECTION_H
