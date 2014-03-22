#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QLabel>

#include "common/client.h"

class PrivateChatWidget;
class SystemTrayIcon;
class UserWidget;
//class TrayNotifyDlg;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    
private slots:

    void newParticipant(const QString &nick, const QHostAddress &ip, const quint16 &port);
    void newParticipant(Connection *connection);

    void participantLeft(const QString &nick, const QHostAddress &ip, const quint16 &port);
    void participantLeft(Connection *connection);
    void reporNewParticipantt(const QString &from);//新用户上线通知窗口

    void showPrivateChat(QTreeWidgetItem*, int);//显示聊天窗口
    void reportNewMessage(const QString &from); //新消息通知窗口
    void showMessage(const QString&);

    // icon
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void showTrayIconMessage();
    void removedUserChat();             //当一个聊天窗口关闭时,清除窗口缓存

protected:
    void closeEvent(QCloseEvent *e);
    void changeEvent(QEvent *e);        //最小化到托盘,不显示在任务栏

private:
    Ui::MainWidget *ui;

private:
    Client          m_client;
    QString         m_myNickName;
    QHostAddress    m_myIp;
    quint16         m_myPort;

    QMap<UserWidget*, PrivateChatWidget*> m_userChating;
    SystemTrayIcon  *m_trayIcon;
};

#endif // MAINWIDGET_H
