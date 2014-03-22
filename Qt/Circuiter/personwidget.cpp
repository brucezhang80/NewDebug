#include "personwidget.h"
#include "ui_personwidget.h"
#include <QtNetwork>
#include <QString>
#include <QByteArray>

#include <QDebug>

PersonWidget::PersonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonWidget)
{
    ui->setupUi(this);
}

PersonWidget::~PersonWidget()
{
    delete ui;
}

void PersonWidget::setUserName(const QString &username)
{
    ui->lb_personName->setText( username );
    m_userName = username;
}

void PersonWidget::setlocalHostName(const QString &localhostname)
{
    m_localHostName = localhostname;
}

QString PersonWidget::localHostName() const
{
    QString username = QString( tr("Unknow HostName") );

    return username;
}

void PersonWidget::setUserIp(const QString &userip)
{
    ui->lb_personIp->setText( userip );
    m_userIp = userip;
}

void PersonWidget::setUserPort(const QString &userport)
{
    m_userPort = userport;
}

void PersonWidget::setUserAvatar(const QString &pixmapfilename)
{
    QPixmap pixmap(pixmapfilename);
    ui->lb_personAvatar->setPixmap(pixmap);
    m_userAvatar = pixmapfilename;
}
