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
    void reporNewParticipantt(const QString &from);//���û�����֪ͨ����

    void showPrivateChat(QTreeWidgetItem*, int);//��ʾ���촰��
    void reportNewMessage(const QString &from); //����Ϣ֪ͨ����
    void showMessage(const QString&);

    // icon
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void showTrayIconMessage();
    void removedUserChat();             //��һ�����촰�ڹر�ʱ,������ڻ���

protected:
    void closeEvent(QCloseEvent *e);
    void changeEvent(QEvent *e);        //��С��������,����ʾ��������

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
