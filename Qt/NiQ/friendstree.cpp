#include "friendstree.h"
#include "userwidget.h"
#include "groupwidget.h"
#include "mainwidget.h"
#include "sendfiledlg.h"
#include <QtGui>
#include <QTreeWidgetItemIterator>

#include <QDebug>

FriendsTree::FriendsTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setExpandsOnDoubleClick(true);
    setColumnCount(1);

    addSelf("Myself");

    //addGroup( QString("Dev Friends") );
}

void FriendsTree::addSelf(const QString &name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setExpanded(true);
    item->setText(0, name);
    item->setTextColor(0, QColor(255, 255, 255, 255));

    m_selfGroupItem = item;
}

QTreeWidgetItem *FriendsTree::self() const
{
    return m_selfGroupItem;
}

QTreeWidgetItem *FriendsTree::groupItem(const QString& groupname)
{
    QTreeWidgetItemIterator it(this);
    while( *it )
    {
        QWidget *widget = itemWidget(*it, 0);
        if( widget )
        {
            if( widget->objectName() == QString("GroupWidget") )
            {
                GroupWidget* group = static_cast<GroupWidget*>(widget);
                if( group )
                {
                    if(group->groupName() == groupname)
                    break;
                }
            }
        }
        it++;
    }

    return (*it);
}

QTreeWidgetItem* FriendsTree::addGroup(const QString& groupname)
{
    QTreeWidgetItem *itemgroup = new QTreeWidgetItem(this);
//    itemgroup->setText(0, QString( groupname ));
//    itemgroup->setTextColor(0, QColor(255,255,255,255));
    itemgroup->setExpanded(true);
    GroupWidget *group = new GroupWidget(this);
    group->setGroupName(groupname);
    group->setGroupItem(itemgroup);
    this->setItemWidget(itemgroup, 0, group);
    m_groupItem = itemgroup;

    connect(group, SIGNAL(showSendFileWidget()), this, SLOT(showSendFileWidget()) );

    return itemgroup;
}

QList<UserWidget *> FriendsTree::getGroupUsers(QTreeWidgetItem *groupitem)
{
    QList<UserWidget*> userList;

    if(groupitem->childCount() != 0)
    {
        for(int i = 0; i < groupitem->childCount(); ++i)
        {
            QTreeWidgetItem* chileItem = groupitem->child(i);
            if(!chileItem) continue;
            QWidget *widget = this->itemWidget(chileItem, 0);
            if(!widget) continue;
            UserWidget* user = static_cast<UserWidget*>(widget);
            if(user)
                userList.append(user);
        }
    }

    return userList;
}

void FriendsTree::showSendFileWidget()
{
    if (GroupWidget *group = qobject_cast<GroupWidget *>(sender()))
    {
        QTreeWidgetItem* groupitem = group->groupItem();
        SendFileDlg *sendDlg = new SendFileDlg();
        QList<UserWidget*> userList = this->getGroupUsers(groupitem);
        sendDlg->setGroupUsers(userList);
        sendDlg->show();
    }
}

//void FriendsTree::newParticipant(const QString &nick, const QHostAddress &ip, const quint16 &port)
//{
//    UserWidget *user = new UserWidget();
//    user->setUserName(nick);
//    user->setUserIp(ip);
//    user->setUserPort(port);
//    user->setOfficeTitle("Dev Group");

//    QTreeWidgetItem *item = new QTreeWidgetItem(m_gernerGroupItem);
//    m_gernerGroupItem->addChild(item);
//    setItemWidget(item, 0, user);
//}

//void FriendsTree::newParticipant(Connection *connection)
//{
//    UserWidget *user = new UserWidget(connection);
//    user->setOfficeTitle("Dev Group");

//    QTreeWidgetItem *item = new QTreeWidgetItem(m_gernerGroupItem);
//    m_gernerGroupItem->addChild(item);
//    setItemWidget(item, 0, user);

//    MainWidget *main = this->parentWidget();
//    main->reporNewParticipantt(user->userName());
//    connect( user, SIGNAL(reportNewMessage(QString)), main, SLOT(reportNewMessage(QString)) );
//}


//void FriendsTree::participantLeft(const QString &nick, const QHostAddress &ip, const quint16 &port)
//{
//    QList<QTreeWidgetItem*> items = this->findItems(nick, Qt::MatchCaseSensitive, 0);
//    if(items.isEmpty())
//        return;
//    delete items.at(0);
//}

//void FriendsTree::participantLeft(Connection *connection)
//{
//    QTreeWidgetItemIterator it(this);
//    while (*it)
//    {
//        QWidget *widget = itemWidget(*it, 0);
//        if( widget )
//        {
//            UserWidget* user = static_cast<UserWidget*>(widget);
//            if( user->userName() == connection->name() )
//            {
//                delete *it;
//                break;
//            }
//        }
//        ++it;
//    }
//}

