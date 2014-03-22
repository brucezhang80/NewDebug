#include "userwidget.h"
#include "friendstablewnd.h"
#include <QHeaderView>
#include "privatechatwidget.h"

FriendsTableWnd::FriendsTableWnd(QWidget *parent) :
    QTableWidget(parent)
{
    initTable();

}

void FriendsTableWnd::initTable()
{
    int row = 30;
    int col = 1;
    setRowCount( row );
    setColumnCount( col );
}

void FriendsTableWnd::newParticipant(const QString &nick, const QString &ip, const QString &officetitle)
{
//    UserWidget *user = new UserWidget(this);
//    user->setUserName(nick);
//    user->setUserIp(ip);
//    user->setOfficeTitle(officetitle);
//    for(int i = 0; i != rowCount(); ++i)
//    {
//        if(!cellWidget(i, 0))
//        {
//            this->setCellWidget(i, 0, user);
//            break;
//        }
//    }
}

void FriendsTableWnd::newParticipant(const QString &nick)
{
//    UserWidget *user = new UserWidget(this);
//    user->setUserName(nick);

//    for(int i = 0; i != rowCount(); ++i)
//    {
//        if(!cellWidget(i, 0))
//        {
//            this->setCellWidget(i, 0, user);
//            break;
//        }
//    }
}

void FriendsTableWnd::showPrivateChat(int row, int col)
{
//    UserWidget* widget = (UserWidget*)cellWidget(row, col);
//    if( !widget )
//    {
//        return;
//    }
//    QString titleName = widget->userName();
//    QString titleIp = widget->userIp();
//    PrivateChatWidget* privateChat = new PrivateChatWidget();
//    privateChat->setAttribute(Qt::WA_DeleteOnClose);
//    privateChat->setWindowTitle(titleName+" " + titleIp);
//    privateChat->show();
}
