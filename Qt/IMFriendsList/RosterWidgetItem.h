#ifndef ROSTERWIDGETITEM_H
#define ROSTERWIDGETITEM_H

#include <QWidget>
#include <QXmpp/QXmppPresence.h>
#include <QXmpp/QXmppClient.h>

class UserChatDialog;
class QTreeWidgetItem;

namespace Ui {
class RosterWidgetItem;
}

/**
 *  Ϊÿһ���û����ƶ�һ�����촰��
 *  ��ÿ����������һ�����촰��
 *  ͨ���û�ָ��,�����ҵ��Լ������촰��
 */
class RosterWidgetItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit RosterWidgetItem(QTreeWidgetItem *group, QTreeWidgetItem *item, QWidget *parent = 0);
    ~RosterWidgetItem();
    
    /*���úͻ�ȡ�û�����*/
    void    setName( const QString &username);
    QString name() const { return m_username; }

    void    setBareJid(const QString &jid);
    QString bareJid()const { return m_bareJid; }

    /*���úͻ�ȡ�û���ǩ��*/
    void    setSignature( const QString &signature );
    QString signature() const { return m_signature; }

    /*���úͻ�ȡ�û���ͷ��,����ͷ�����ڵ�ͼƬλ��*/
    void    setAvatar( const QString &avatar );
    void    setAvatar(const QImage& image);
    QImage  avatar() const { return m_avatar; }
    void    grayAvatar();

    void    setStatusIcon(const QString &statusIcon, const QString &statusTip);

    // �Ƿ�����
    void setPresence(const QXmppPresence &presence);

    RosterWidgetItem*   rosterItem(const QString &barejid);
    UserChatDialog*     chatDialog(const QString &barejid);
    void                setQXmppClient(QXmppClient* client);
public slots:
    void doubleClicked(QTreeWidgetItem*, int);

private:
    Ui::RosterWidgetItem*ui;
    QXmppClient*         m_client;
    UserChatDialog      *m_chatDlg;
    QTreeWidgetItem     *m_onItem;
    QTreeWidgetItem     *m_groupItem;

    QString             m_username;
    QString             m_bareJid;
    QString             m_signature;
    QImage              m_avatar;
};

#endif // ROSTERWIDGETITEM_H
