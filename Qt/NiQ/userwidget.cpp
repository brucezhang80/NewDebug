#include "userwidget.h"
#include "ui_userwidget.h"
#include <QPropertyAnimation>
#include <QMessageBox>

UserWidget::UserWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::UserWidget)
{
    ui->setupUi(this);
    m_isChat = false;
    m_cacheMsg.clear();
}

UserWidget::UserWidget(Connection *connection, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserWidget)
{
    ui->setupUi(this);

    Q_ASSERT(connection != 0);
    if(!connection)
    {
        return;
    }
    m_connection = NULL;
    setConnection(connection);

    QString hostName = m_connection->name();
    QHostAddress hostAddr = m_connection->peerAddress();
    quint16 hostPort = m_connection->peerPort();
    setUserName(hostName);
    setUserIp(hostAddr);
    setUserPort(hostPort);
    setUserAvatar(":/images/buddy-green.png");
    m_cacheMsg.clear();
    m_isChat = false;
    connect(m_connection, SIGNAL(newMessage(const QString&, const QString&, const QString&)),
            this, SLOT(recieveMessage(const QString&, const QString&, const QString&)));
}

UserWidget::~UserWidget()
{
    delete ui;
    m_cacheMsg.clear();
}

void UserWidget::setNickName(const QString &nickname)
{
    m_nickName = nickname;
}

void UserWidget::setConnection(Connection *connection)
{
    m_connection = connection;
}

void UserWidget::setUserName(const QString &username)
{
    ui->lb_userName->setText( username );
    m_userName = ui->lb_userName->text();
}

void UserWidget::setUserIp(const QHostAddress &userip)
{
    //ui->lb_userIp->setText( userip.toString() );
    m_userIp = userip;
}

void UserWidget::setUserPort(const quint16 &userport)
{
    m_userPort = userport;
}

QMap<QString, QString> UserWidget::recieveCacheMsg() const
{
    return m_cacheMsg;
}

void UserWidget::emptyCacheMsg()
{
    m_cacheMsg.clear();
}

void UserWidget::setUserAvatar(const QString &pixmapfilename)
{
    QPixmap pixmap(pixmapfilename);
    ui->lb_userAvatar->setPixmap(pixmap);
    m_userAvatar = pixmapfilename;
}

void UserWidget::setOfficeTitle(const QString &officetitle)
{
    ui->lb_officeTitle->setText(officetitle);
    m_officeTitle = officetitle;
}

// slot
void UserWidget::flickeUser()
{
    setUserAvatar(":/images/buddy-blue.png");
//    QRect rect = ui->lb_userAvatar->geometry();
//    int x0 = rect.x();
//    int y0 = rect.y();
//    QPropertyAnimation *anim1=new QPropertyAnimation(ui->lb_userAvatar, "pos");
//    anim1->setDuration(200);
//    anim1->setEasingCurve(QEasingCurve::OutBounce);
//    anim1->setStartValue(QPoint(x0, y0));
//    anim1->setEndValue(QPoint(x0-5, x0-3));
//    anim1->start();
    //    ui->lb_userAvatar->move(x0+5, y0+3);
}

void UserWidget::closeChat()
{
    emptyCacheMsg();
    m_isChat = false;
    emit removedChat();
    qDebug() << "closed";
}

void UserWidget::openChat()
{
    m_isChat = true;
    qDebug() << "opened";
}

void UserWidget::recieveMessage(const QString &from, const QString &message, const QString &time)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    if(!m_isChat)
    {
        flickeUser();
        m_cacheMsg.insert(time, message);
        emit reportNewMessage(from);
    }
    else
    {
        setUserAvatar(":/images/buddy-green.png");
        emit appendMessage(from, message, time);
    }

}
