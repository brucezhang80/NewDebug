#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "privatechatwidget.h"
#include "userwidget.h"
#include "systemtrayicon.h"
#include "traynotifydlg.h"
#include "sendfiledlg.h"
#include "groupwidget.h"

#include <QMessageBox>

#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint /*& ~Qt::WindowMinimizeButtonHint*/);
//    setWindowFlags(Qt::Tool);
    m_trayIcon = new SystemTrayIcon(QIcon(":/images/user_chat.png"), this);
    m_trayIcon->setToolTips( tr("NiQ\n右键更多功能") );

//    connect(&m_client, SIGNAL(newMessage(QString,QString)),
//            ui->friendsTree, SLOT(reciveMessage(QString,QString)));

    connect(&m_client, SIGNAL(newParticipant(Connection *)), this, SLOT(newParticipant(Connection *)));
    connect(&m_client, SIGNAL(participantLeft(Connection *)), this, SLOT(participantLeft(Connection *)));

    m_myNickName = m_client.nickName();
    m_myIp = m_client.address();
    m_myPort = m_client.port();
    ui->lb_name->setText( m_myNickName );
    ui->lb_ip->setText( m_myIp.toString() );
    newParticipant(m_myNickName, m_myIp, m_myPort);

    this->setWindowTitle(m_myNickName);

    connect(ui->friendsTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(showPrivateChat(QTreeWidgetItem*, int)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}

void MainWidget::changeEvent(QEvent *e)
{
    if((e->type() == QEvent::WindowStateChange)&&this->isMinimized())
    {
        QTimer::singleShot(100, this, SLOT(hide()));
    }
}

void MainWidget::newParticipant(const QString &nick, const QHostAddress &ip, const quint16 &port)
{
    UserWidget *user = new UserWidget();
    user->setUserName(nick);
    user->setUserIp(ip);
    user->setUserPort(port);
    user->setOfficeTitle("Dev Group");

    QTreeWidgetItem *self = ui->friendsTree->self();
    QTreeWidgetItem *item = new QTreeWidgetItem(self);
    self->addChild(item);
    ui->friendsTree->setItemWidget(item, 0, user);
}

void MainWidget::newParticipant(Connection *connection)
{
    UserWidget *user = new UserWidget(connection);
    user->setOfficeTitle("Dev Group");

    QTreeWidgetItem *group = ui->friendsTree->groupItem(user->officeTitle());
    if(!group)
            group = ui->friendsTree->addGroup(user->officeTitle());
    QTreeWidgetItem *item = new QTreeWidgetItem(group);
    group->addChild(item);
    ui->friendsTree->setItemWidget(item, 0, user);

    reporNewParticipantt(user->userName());

    connect( user, SIGNAL(reportNewMessage(QString)), this, SLOT(reportNewMessage(QString)) );
}

void MainWidget::participantLeft(const QString &/*nick*/, const QHostAddress &/*ip*/, const quint16 &/*port*/)
{
}

void MainWidget::participantLeft(Connection *connection)
{
    QTreeWidgetItemIterator it(ui->friendsTree);
    while (*it)
    {
        QWidget *widget = ui->friendsTree->itemWidget(*it, 0);
        if( widget && widget->objectName() == QString("UserWidget") )
        {
            UserWidget* user = static_cast<UserWidget*>(widget);
            if( user->userName() == connection->name() )
            {
                delete *it;
                break;
            }
        }
        ++it;
    }
}

void MainWidget::reporNewParticipantt(const QString &from)
{
    TrayNotifyDlg* trayNotify = new TrayNotifyDlg();
    trayNotify->nameFrom = from;
    trayNotify->setNotifyMessage( from + tr(" 上线了!") );
    trayNotify->setWindowTitle( tr("消息通知") );
    trayNotify->show();
    QRect rect = m_trayIcon->SystemTrayIcon::geometry();
    trayNotify->moveTo(rect.x(), rect.y(), trayNotify->taskbarDirect());
    trayNotify->raise();
    trayNotify->activateWindow();

    connect( trayNotify, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)) );
}

void MainWidget::showPrivateChat(QTreeWidgetItem *item, int col)
{
    UserWidget* user = (UserWidget*)ui->friendsTree->itemWidget(item, col);
    if( !user )
    {
        return;
    }
    if(m_userChating.contains(user))
    {
        QList<PrivateChatWidget*> chatWnds = m_userChating.values(user);
        foreach(PrivateChatWidget *chat, chatWnds)
        {
            if(chat->getConnection() == user->userConnection())
            {
                if(chat->isMinimized())
                    chat->showNormal();
                chat->raise();
                chat->activateWindow();
            }
        }
        return;
    }

    QString revievrName = user->userName();
    QHostAddress revievrIp = user->userIp();
    quint16 revievrPort = user->userPort();
    Connection *connection = user->userConnection();

    PrivateChatWidget* privateChat = new PrivateChatWidget(connection);
    privateChat->setRevieverName(revievrName);
    privateChat->setRevieverAddress(revievrIp);
    privateChat->setRevieverPort(revievrPort);
    QString winTitle = "Chat with [" + revievrName+"] - ip:" + revievrIp.toString() + " - port:"+ QString::number(revievrPort);
    privateChat->setWindowTitle( winTitle );
    privateChat->setSenderName(m_myNickName);
    privateChat->show();
    user->openChat();
    QMap<QString, QString> cacheMsg = user->recieveCacheMsg();
    if(cacheMsg.size() != 0)
    {
        QMapIterator<QString, QString> i(cacheMsg);
        while( i.hasNext() )
        {
            i.next();
            privateChat->appendMessage(revievrName,  i.value(), i.key());
        }
        user->setUserAvatar(":/images/buddy-green.png");
        cacheMsg.clear();
        user->emptyCacheMsg();
    }
    connect( user, SIGNAL(appendMessage(QString,QString,QString)),
            privateChat, SLOT(appendMessage(QString,QString,QString)) );
    connect( privateChat, SIGNAL(closed()), user, SLOT(closeChat()) );
    connect( user, SIGNAL(removedChat()), this, SLOT(removedUserChat()) );


    m_userChating.insert(user, privateChat);
}

void MainWidget::reportNewMessage(const QString &from)
{
    TrayNotifyDlg* trayNotify = new TrayNotifyDlg();
    trayNotify->nameFrom = from;
    trayNotify->setNotifyMessage( from + tr(" 发来消息!") );
    trayNotify->setWindowTitle( tr("消息通知") );
    QRect rect = m_trayIcon->SystemTrayIcon::geometry();
    trayNotify->show();
    trayNotify->moveTo(rect.x(), rect.y(), trayNotify->taskbarDirect());
    trayNotify->raise();
    trayNotify->activateWindow();

    connect( trayNotify, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)) );
}

void MainWidget::showMessage(const QString& from)
{
    QTreeWidgetItemIterator it(ui->friendsTree);
    while (*it)
    {
        QWidget *widget = ui->friendsTree->itemWidget(*it, 0);
        if( widget && widget->objectName() == QString("UserWidget") )
        {
            UserWidget* user = static_cast<UserWidget*>(widget);
            if( user->userName() == from )
            {
                showPrivateChat(*it, 0);
                break;
            }
        }
        ++it;
    }
}

// system trayicon handler
void MainWidget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        this->showNormal();
        this->raise();
        this->activateWindow();
        break;
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        this->raise();
        this->activateWindow();
        break;
    case QSystemTrayIcon::MiddleClick:
        showTrayIconMessage();
        break;
    default:
        return;
    }
}

void MainWidget::showTrayIconMessage()
{
    m_trayIcon->showTrayIconMessage(tr("NiQ"), QString("右键更多功能... "),
                                    QSystemTrayIcon::Information, 1000);
}

void MainWidget::removedUserChat()
{
    if (UserWidget *user = qobject_cast<UserWidget *>(sender()))
        if(m_userChating.contains(user))
            m_userChating.remove(user);
}

