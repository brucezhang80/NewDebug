#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QHostAddress>
#include "common/connection.h"

namespace Ui {
class UserWidget;
}

class UserWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UserWidget(QWidget *parent = 0);
    explicit UserWidget(Connection *connection, QWidget *parent = 0);
    ~UserWidget();
    Connection* userConnection() const {return m_connection;}

    void    setNickName(const QString& nickname);
    QString nickName() const{return m_nickName;}

    void    setUserAvatar(const QString& pixmapfilename);
    QString userAvatar() const {return m_userAvatar;}

    void    setOfficeTitle(const QString& officetitle);
    QString officeTitle() const {return m_officeTitle;}

    QString userName() const{return m_userName;}
    quint16 userPort() const{return m_userPort;}
    QHostAddress userIp() const {return m_userIp;}

    void    setConnection(Connection *connection);
    void    setUserName(const QString& username);
    void    setUserIp(const QHostAddress& userip);
    void    setUserPort(const quint16& userport);

    QMap<QString, QString> recieveCacheMsg() const;//���촰��δ��ʱ�������Ϣ
    void    emptyCacheMsg();    //��ջ������Ϣ
    void    openChat();        //���촰�ڴ�

signals:
    void appendMessage(const QString&, const QString&, const QString &);
    void reportNewMessage(const QString &from);
    void removedChat();     //�����촰�ڹر�ʱ,������Ϣ֪ͨ�����ͷŸô��ڵĴ洢

private slots:
    void flickeUser();      //����ͷ��
    void closeChat();       //���촰�ڹر�
    void recieveMessage(const QString&, const QString&, const QString &);
//    void newMessage(const QString &from, const QString &message);

private:
    Ui::UserWidget *ui;
    bool                m_isChat;
    QMap<QString, QString> m_cacheMsg;
    Connection          *m_connection;
    QString             m_nickName;
    QString             m_userName;
    QHostAddress        m_userIp;
    quint16             m_userPort;
    QString             m_userAvatar;
    QString             m_officeTitle;

};

#endif // USERWIDGET_H
