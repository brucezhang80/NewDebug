#ifndef PRIVATECHATDLG_H
#define PRIVATECHATDLG_H

#include <QDialog>
#include <QTextTableFormat>
#include <QtNetwork>
#include <QMap>
#include "client.h"
#include "connection.h"

namespace Ui {
class PrivateChatDlg;
}

class PrivateChatDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit PrivateChatDlg(Client *client, Connection *connection, QWidget *parent = 0);
    ~PrivateChatDlg();
    
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

private:
    Ui::PrivateChatDlg *ui;
    QTextTableFormat tableFormat;
    QString myNickName;
    QString nick;
    QHostAddress addr;
    quint16 port;
    QMap<QString, QString> receiveMsgMap;
};

#endif // PRIVATECHATDLG_H
