#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QTimerEvent>

static const int MaxBufferSize = 1024000;

class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);

    /*连接状态*/
    enum ConnectionState
    {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };
    /*数据类型*/
    enum DataType
    {
        PlainText,
        File,
        Ping,
        Pong,
        Greeting,
        Undefined
    };

    /*消息发送*/
    QString name() const;
    void setGreetingMessage(const QString &message);
    bool sendMessage(const QString &message);

signals:
    void readyForUse();
    void newMessage(const QString &from, const QString &message, const QString &time);

public slots:
    void processReadyRead();
    void sendPing();
    void sendGreetingMessage();

protected:
    void timerEvent(QTimerEvent *timerEvent);

private:
     int readDataIntoBuffer(int maxSize = MaxBufferSize);
     int dataLengthForCurrentDataType();
     bool readProtocolHeader();
     bool hasEnoughData();
     void processData();

     QString sendtime;
     QString greetingMessage;
     QString username;
     QTimer pingTimer;
     QTime pongTime;

     QByteArray buffer;
     ConnectionState state;
     DataType currentDataType;
     int numBytesForCurrentDataType;
     int transferTimerId;
     bool isGreetingMessageSent;
};

#endif // CONNECTION_H
