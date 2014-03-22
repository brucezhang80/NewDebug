#include "privatechatdlg.h"
#include "privateclient.h"
#include "friendsview.h"
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QDebug>

FriendsView::FriendsView(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    QString header;
    header = tr("Name") ;
    this->setHeaderLabel(header);
    this->setIconSize(QSize(32,32));

    m_privateClient = NULL;
    m_privateChatDlg = NULL;

    connect(&client, SIGNAL(newMessage(QString,QString,QString)), this, SLOT(appendMessage(QString,QString,QString)));
    connect(&client, SIGNAL(newParticipant(QString)), this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)), this, SLOT(participantLeft(QString)));

    myNickName = client.nickName();
    this->newParticipant(myNickName);
    tableFormat.setBorder(0);
//  QTimer::singleShot(2 * 1000, this, SLOT(showInformation()));
}

void FriendsView::appendMessage(const QString &from, const QString &message, const QString &time)
{
    if (from.isEmpty() || message.isEmpty())
        return;

//    QMessageBox::information(this, from, ("收到 "+from + "发来的消息:" + message));

//    QDialog *newMsg = new QDialog(this->window());
//    QLabel *msgLb = new QLabel(newMsg);
//    msgLb->setText(from + " 发来新消息!");
//    //newMsg->setModal(true);
//    newMsg->resize(300, 100);
//    newMsg->showNormal();

    //receiverMsgList.append(message);
//    receiverMsgMap.insert(time, message);
//    receiverName = from;
//    qDebug() << "发送者:" << client.nickName() << client.port() << client.address();
//    qDebug() << "接收者:" << from <<  receiverIp << receiverPort;
//    newPrivateChat(from, client.address(), client.port());
//    appendMessage(from, message);
//    flashParticipant(from);
}

void FriendsView::returnPressed(QString msg)
{
    client.sendPrivateMessage(receiverIp, receiverPort, msg);
}

void FriendsView::newParticipant(const QString &nick)
{
    if( nick.isEmpty() )
        return;

    //qDebug() << nick;
    QTreeWidgetItem *item = new QTreeWidgetItem( this);
    item->setText(0, nick);
    QIcon icon(":/images/buddy-blue.png");
    item->setIcon(0, icon);
    this->addTopLevelItem( item );
}

void FriendsView::participantLeft(const QString &nick)
{
    QList<QTreeWidgetItem *> items = this->findItems(nick, Qt::MatchExactly);
    qDebug() << "nick: " << nick;
    if (items.isEmpty())
        return;
    delete items.at(0);
}

void FriendsView::showInformation()
{
    if (this->model()->rowCount() == 1)
    {
        QMessageBox::information(this, tr("Chat"),
                                tr("Launch several instances of this "
                                    "program on your local network and "
                                    "start chatting!"));
    }
}

void FriendsView::newPrivateChat(Client *client, Connection *connection)
{
    receiverIp = connection->peerAddress();
    receiverPort = connection->peerPort();
    receiverName = connection->name();
    if(m_privateChatDlg)
    {
        if(m_privateChatDlg->isEnabled() && m_privateChatDlg->windowTitle() == receiverName)
            return;
        else
            m_privateChatDlg->show();
        return;
    }
    m_privateChatDlg = new PrivateChatDlg(client, connection);
    m_privateChatDlg->setWindowTitle("与 "+ receiverName + " 通信中...");
    m_privateChatDlg->setRecieverNick(receiverName);
    m_privateChatDlg->setRecieverAddress(receiverIp);
    m_privateChatDlg->setRecieverPort(receiverPort);
    m_privateChatDlg->setMyNickName(myNickName);
    m_privateChatDlg->setRecieverMsg(receiverName, receiverMsgMap);
    m_privateChatDlg->show();

    connect(client, SIGNAL(newMessage(QString,QString,QString)), m_privateChatDlg, SLOT(appendMessage(QString,QString,QString)));
    connect( m_privateChatDlg, SIGNAL(sendMessage(QString)), this, SLOT(returnPressed(QString)) );

    receiverMsgMap.clear();
}

void FriendsView::newPrivateChat(QString name, QHostAddress addr, quint16 port)
{
    if(m_privateClient)
    {
        if(m_privateClient->isEnabled() && m_privateClient->windowTitle() == name)
            return;
        else
            m_privateClient->show();
        return;
    }

    m_privateClient = new PrivateClient();
    m_privateClient->setWindowTitle("与 "+name + " 通信中...");
    m_privateClient->setRecieverNick(name);
    m_privateClient->setRecieverAddress(addr);
    m_privateClient->setRecieverPort(port);
    m_privateClient->setMyNickName(myNickName);
    m_privateClient->setRecieverMsg(receiverName, receiverMsgMap);
    m_privateClient->show();

    connect(&client, SIGNAL(newMessage(QString,QString,QString)), m_privateClient, SLOT(appendMessage(QString,QString,QString)));
    connect( m_privateClient, SIGNAL(sendMessage(QString)), this, SLOT(returnPressed(QString)) );

    receiverMsgMap.clear();
}

void FriendsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton)
    {
        QModelIndex index = static_cast<QModelIndex>(this->indexAt(event->pos()));
        if(index.isValid())
        {
            this->setCurrentIndex(index);
            QString curItem = index.data().toString();

            foreach(Connection *connection, client.peers.values())
            {
                this->currentItem()->setTextColor(0, QColor(1, 1, 1));

                //qDebug() << connection->name() << curItem;
                if(connection->name() == curItem)
                {
                    newPrivateChat(&client, connection);

//                    receiverIp = connection->peerAddress();
//                    receiverPort = connection->peerPort();
//                    receiverName = connection->name();
//                    newPrivateChat(curItem, connection->peerAddress(), connection->peerPort());
                }
            }
        }
    }
}


