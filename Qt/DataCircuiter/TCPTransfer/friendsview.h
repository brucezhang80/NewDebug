#ifndef FRIENDSVIEW_H
#define FRIENDSVIEW_H

#include <QtNetwork>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTextTableFormat>
#include <QMouseEvent>

#include "client.h"
#include "connection.h"

class PrivateChatDlg;
class PrivateClient;

class FriendsView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FriendsView(QWidget *parent = 0);

signals:
    
public slots:
    void appendMessage(const QString &from, const QString &message, const QString &time);
    //void sendMessage(QString, QString);

    void returnPressed(QString msg);
    void newParticipant(const QString &nick);//a new friend or user online
    void participantLeft(const QString &nick);// user offline
    void showInformation();
//    void flashParticipant(const QString &name);

    void newPrivateChat(Client *client, Connection *connection);
    void newPrivateChat(QString name, QHostAddress addr, quint16 port);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
//    void changeEvent(QEvent *);
//    void closeEvent(QCloseEvent *);

public:
    PrivateClient       *m_privateClient;
    PrivateChatDlg      *m_privateChatDlg;
    Client              client;
    QString             myNickName;
    QTextTableFormat    tableFormat;
    QList<Connection*>  connectionList;
    QHostAddress        receiverIp;
    quint16             receiverPort;
    QString             receiverName;
    //QStringList         receiverMsgList;
    QMap<QString, QString> receiverMsgMap;

    QTimer              flashTimer;
};

#endif // FRIENDSVIEW_H
