#ifndef USER_MANAGER_H
#define USER_MANAGER_H

//#include "owner.h"
#include "message.h"
#include "main_window.h"
#include "usertreewidget.h"
#include <QObject>

class Owner;
class UserTreeWidget;
class UserItemWidget;
class GroupManager;

class UserManager : public QObject
{
    Q_OBJECT
public:
    friend class MainWindow;
    explicit UserManager(QObject *parent = 0);
    ~UserManager();

    void broadcastExit() const;
    void broadcastEntry() const;

    bool contains(QString ip);

    QString ip(QTreeWidgetItem *item);
    QString name(QTreeWidgetItem* item);
    QString group(QTreeWidgetItem *item);
    QString host(QTreeWidgetItem *item);
    QString loginName(QTreeWidgetItem *item);

    Owner ownerSelf() const { return m_ownerSelf; }
    void updateOwnerSelf();

    QString entryMessage() const;
    QString exitMessage() const;


signals:
    void userCountUpdated(int userCount);

private slots:
    void newUserMsg(Message msg);
    void newExitMsg(Message msg);

private:
    void updateUser(const Owner &owner, QTreeWidgetItem* groupItem);
    void addUser(const Owner &owner, QTreeWidgetItem* groupItem);
    UserItemWidget* itemWidget( QTreeWidgetItem *item );

    static Owner        m_ownerSelf;
    GroupManager        *m_group;
};

#endif // USER_MANAGER_H
