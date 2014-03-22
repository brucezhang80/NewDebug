#ifndef FRIENDSTREE_H
#define FRIENDSTREE_H

#include <QTreeWidget>
#include <QHostAddress>
#include "common/connection.h"
#include "userwidget.h"

class FriendsTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FriendsTree(QWidget *parent = 0);

    void addSelf(const QString &name);
    QTreeWidgetItem* self() const;

    QTreeWidgetItem* addGroup(const QString &groupname);
    QList<UserWidget*> getGroupUsers(QTreeWidgetItem *groupitem);

    QTreeWidgetItem* groupItem(const QString &groupname);

signals:

public slots:
//    void newParticipant(const QString &nick, const QHostAddress &ip, const quint16 &port);
//    void newParticipant(Connection *connection);

//    void participantLeft(const QString &nick, const QHostAddress &ip, const quint16 &port);
//    void participantLeft(Connection *connection);
    void showSendFileWidget();

private:
    QTreeWidgetItem* m_groupItem;
    QTreeWidgetItem* m_selfGroupItem;
};

#endif // FRIENDSTREE_H
