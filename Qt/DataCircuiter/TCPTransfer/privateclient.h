#ifndef PRIVATECLIENT_H
#define PRIVATECLIENT_H

#include <QWidget>
#include <QTextTableFormat>
#include <QtNetwork>

namespace Ui {
class PrivateClient;
}

class PrivateClient : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrivateClient(QWidget *parent = 0);
    ~PrivateClient();

    void setRecieverNick( const QString &recverNick);
    void setRecieverAddress( const QHostAddress &recverAddress);
    void setRecieverPort( const quint16 &recverPort);
    void setMyNickName(const QString &myName){ myNickName = myName;}
    void setRecieverMsg(const QString &from, const QMap<QString, QString> &msgMap);

signals:
    void sendMessage(const QString &, const QStringList &);
    void sendMessage(const QString &, const QString &);
    void sendMessage(QString msg);

public slots:
    void sendPress();
    void appendMessage(const QString &from, const QString &message, const QString &time);

protected:

private slots:

private:
    Ui::PrivateClient *ui;
    QTextTableFormat tableFormat;
    QString myNickName;
    QString nick;
    QHostAddress addr;
    quint16 port;
    QString sendMsg;
};

#endif // PRIVATECLIENT_H
