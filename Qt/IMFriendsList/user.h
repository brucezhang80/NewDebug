#ifndef USER_H
#define USER_H

#include <QWidget>

class UserChatDialog;
class QTreeWidgetItem;

namespace Ui {
class User;
}

/**
 *  Ϊÿһ���û����ƶ�һ�����촰��
 *  ��ÿ����������һ�����촰��
 *  ͨ���û�ָ��,�����ҵ��Լ������촰��
 */
class User : public QWidget
{
    Q_OBJECT
    
public:
    explicit User(QTreeWidgetItem *group, QTreeWidgetItem *item, QWidget *parent = 0);
    ~User();
    
    /*���úͻ�ȡ�û�����*/
    void    setUserName( const QString &username);
    QString userName() const { return m_username; }

    /*���úͻ�ȡ�û���ǩ��*/
    void    setSignature( const QString &signature );
    QString signature() const { return m_signature; }

    /*���úͻ�ȡ�û���ͷ��,����ͷ�����ڵ�ͼƬλ��*/
    void    setAvatar( const QString &avatar );
    QString avatar() const { return m_avatar; }

public slots:
    void doubleClicked(QTreeWidgetItem*, int);

private:
    Ui::User            *ui;
    UserChatDialog      *m_chatDlg;
    QTreeWidgetItem     *m_onItem;
    QTreeWidgetItem     *m_groupItem;

    QString             m_username;
    QString             m_signature;
    QString             m_avatar;       // user QIcon?
};

#endif // USER_H
