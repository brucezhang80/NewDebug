#include "owner.h"
#include "macai.h"
#include "system.h"
#include "message_thread.h"
#include "send_message.h"
#include "constants.h"
#include "useritemwidget.h"
//#include "usertreewidget.h"
#include "user_manager.h"
#include "preferences.h"
#include "group_manager.h"

#include <QStringList>
#include <QStandardItemModel>
#include <QMessageBox>

Owner UserManager::m_ownerSelf;

UserManager::UserManager(QObject *parent) :
    QObject(parent)
{
    updateOwnerSelf();

    m_group = new GroupManager( "Default", Macai::userTreeWidget);
}

UserManager::~UserManager()
{
}

void UserManager::broadcastExit() const
{
    qDebug("UserManager::broadcastExit");

    quint32 flags = 0;
    flags |= IPQ_BROADCAST_EXIT ;//| QIPMSG_CAPACITY;

    SendMessage sendMsg(QHostAddress::Null, 0/* port */,
                    exitMessage(), ""/* extendedInfo */, flags);

    Macai::messageThread->addSendMsg(Message(sendMsg));

}

void UserManager::broadcastEntry() const
{
    qDebug("UserManager::broadcastEntry");

    quint32 flags = 0;
    flags |= IPQ_BROADCAST_ENTRY;// | QIPMSG_CAPACITY;

    SendMessage sendMsg(QHostAddress::Null, 0/* port */, entryMessage(), ""/* extendedInfo */, flags);

    Macai::messageThread->addSendMsg(Message(sendMsg));
}

bool UserManager::contains(QString ip)
{
    QTreeWidgetItemIterator it(Macai::userTreeWidget);
    while (*it)
    {
        QTreeWidgetItem* item = *it;
        UserItemWidget* user_widget = itemWidget( item );
        if( user_widget )
        {
            if( user_widget->userIp() == ip )
            {
                return true;
            }
        }
        ++it;
    }

    return false;
}

QString UserManager::ip(QTreeWidgetItem *item)
{
    QString ip = "00:00:00:00";
    UserItemWidget *user_widget = itemWidget(item);
    if( user_widget )
    {
        ip = user_widget->userIp();
    }
    return ip;
}

QString UserManager::name(QTreeWidgetItem *item)
{
    QString name = tr("Unknow");
    UserItemWidget *user_widget = itemWidget(item);
    if( user_widget )
    {
        name = user_widget->nickName();
    }
    return name;
}

QString UserManager::group(QTreeWidgetItem *item)
{
    QString group = "GerneralGroup";
    UserItemWidget *user_widget = itemWidget(item);
    if( user_widget )
    {
        group = user_widget->userGroup();
    }
    return group;
}

QString UserManager::host(QTreeWidgetItem *item)
{
    QString hostname = "Unknow Hostname";
    UserItemWidget *user_widget = itemWidget(item);
    if( user_widget )
    {
        hostname = user_widget->hostName();
    }
    return hostname;
}

QString UserManager::loginName(QTreeWidgetItem *item)
{
    QString logname = "Unknow Logname";
    UserItemWidget *user_widget = itemWidget(item);
    if( user_widget )
    {
        logname = user_widget->loginName();
    }
    return logname;
}

void UserManager::updateOwnerSelf()
{
    if (Macai::preferences->userName.isEmpty())
    {
        m_ownerSelf.setName(System::loginName());
    }
    else
    {
        m_ownerSelf.setName(Macai::preferences->userName);
    }

    m_ownerSelf.setGroup(Macai::preferences->groupName);
    m_ownerSelf.setLoginName(System::loginName());
    m_ownerSelf.setHost(System::hostName());
//    m_ownerSelf.setDisplayLevel(Macai::preferences->displayLevel);
}

QString UserManager::entryMessage() const
{
    return QString("%1%2%3%4").arg(m_ownerSelf.name())
        .arg(QString("\n"))
        .arg(m_ownerSelf.group())
        .arg(QString("\n"));
}

QString UserManager::exitMessage() const
{
    return entryMessage();
}

// �����û�����
void UserManager::newUserMsg(Message msg)
{
    qDebug() << "[UserManager]new user online:message from message server "
             << "Name:" << msg->owner().name()
             << "IP:" <<  msg->ipAddress()
             << "Port:" << msg->port()
             << "Group:" << msg->owner().group();
    // ���һ��ƫ�����ù�����ѡ��.
    // ���ҵ�ǰ���Ƿ��и��û�
    // �и��û�����ǰ���������
    if (!msg->owner().group().isEmpty() && !Macai::preferences->groupNameList.contains(msg->owner().group()))
    {
        Macai::preferences->groupNameList.prepend(msg->owner().group());
    }
    if (contains(msg->ip()))
    {
        qDebug() << "exsits user:" << msg->ip();
        //updateUser(msg->owner(), ipToRow(msg->ip()));
    }
    else
    {
        addUser(msg->owner(), m_group->groupItem());
//        emit userCountUpdated(m_model->rowCount());
    }
}

void UserManager::newExitMsg(Message msg)
{
    qDebug() << "UserManager::newExitMsg(Message msg)";
    QTreeWidgetItemIterator it(Macai::userTreeWidget);
    while (*it)
    {
        UserItemWidget *user_widget = itemWidget(*it);
        if( user_widget )
        {
//            QString str;
//            str.append("user_widget->loginName:" + user_widget->loginName());
//            str.append("\nmsg->owner().loginName:" + msg->owner().loginName());
//            str.append("\nuser_widget->userIp:" + user_widget->userIp());
//            str.append("\nmsg->ip:" + msg->ip());
//            str.append("\nmsg->owner().ip():" + msg->owner().ip());

//            QMessageBox::information(Macai::userTreeWidget->window(), "debug", str );
            if( user_widget->userIp() == msg->owner().ip() )
            {
                delete *it;
                break;
            }
        }
        ++it;
    }
}

void UserManager::updateUser(const Owner &owner, QTreeWidgetItem *groupItem)
{
    // qtreewidget ��������model,ֻ�ù���һ��userTreeWidget���������û�
    QTreeWidgetItem* item = new QTreeWidgetItem(groupItem);
    groupItem->addChild(item);
    UserItemWidget *userItemWidget = new UserItemWidget(owner.name(), owner.ip(), item, Macai::userTreeWidget);
    userItemWidget->setLoginName(owner.loginName());
    userItemWidget->setHostName(owner.host());
    userItemWidget->setUserGroup(groupItem->text(0));

    Macai::userTreeWidget->setItemWidget(item, 0, userItemWidget);
}

void UserManager::addUser(const Owner &owner, QTreeWidgetItem* groupItem)
{
    updateUser(owner, groupItem);
}

UserItemWidget *UserManager::itemWidget(QTreeWidgetItem *item)
{
    QWidget* widget = Macai::userTreeWidget->itemWidget(item, 0);
    if( widget && widget->objectName() == QString("UserItemWidget") )
    {
        UserItemWidget* userwidget = static_cast<UserItemWidget*>(widget);
        if( userwidget )
        {
            return userwidget;
        }
    }

     // �����ǵ�ǰItem��group
     return NULL;
}

