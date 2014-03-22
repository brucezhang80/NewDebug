#include "useritemwidget.h"


#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QHostAddress>
#include <QTreeWidgetItem>
#include <QHoverEvent>

UserItemWidget::UserItemWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi();
}

UserItemWidget::UserItemWidget(QString name, QString ip, QTreeWidgetItem *item, QWidget *parent )
    : m_nickName(name), m_userIp(ip), m_currentItem(item), QWidget(parent)
{
    setupUi();
    setLoginName(name);
    setUserIp(ip);
}

void UserItemWidget::setNickName(const QString &nickname)
{
    lb_nickName->setText(nickname);
    m_nickName = nickname;
}

void UserItemWidget::setLoginName(const QString &loginname)
{
    lb_nickName->setText(loginname);
     m_loginName = loginname;
}

void UserItemWidget::setHostName(const QString &hostname)
{
     m_hostName = hostname;
}

void UserItemWidget::setUserIp(const QString &userip)
{
    lb_userIp->setText(userip);
    m_userIp = userip;
}

void UserItemWidget::setUserPort(const QString &userport)
{
    m_userPort = userport;
}

void UserItemWidget::setUserAvatar(const QString &pixmapfilename)
{
    QPixmap pixmap(pixmapfilename);
    lb_userAvatar->setPixmap(pixmap);
    m_userAvatar = pixmapfilename;
}

void UserItemWidget::setOfficeTitle(const QString &officetitle)
{
    m_officeTitle = officetitle;
}

void UserItemWidget::setUserGroup(const QString &usergroup)
{
    m_userGroup = usergroup;
}

void UserItemWidget::setCurrentItem(QTreeWidgetItem* item)
{
    m_currentItem = item;
}

void UserItemWidget::enterEvent(QEvent *event)
{
    btn_userManager->show();
    btn_userChat->show();
}

void UserItemWidget::leaveEvent(QEvent *)
{
    btn_userManager->hide();
    btn_userChat->hide();
}

///////////////////////UI/////////////////////////////////
void UserItemWidget::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("UserItemWidget"));
    this->resize(140, 42);
    this->setMaximumSize(QSize(16777215, 40));
    this->setFocusPolicy(Qt::NoFocus);

    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 4, 0, 4);
    gridLayout->setSpacing(0);
    lb_userAvatar = new QLabel(this);
    lb_userAvatar->setMaximumSize(QSize(40, 40));
    lb_userAvatar->setPixmap(QPixmap(QString::fromUtf8(":/images/buddy-green.png")));
    lb_userAvatar->setScaledContents(true);

    gridLayout->addWidget(lb_userAvatar, 0, 0, 2, 1);

//////////////////////////
    horizontalLayout_1 = new QHBoxLayout();

    lb_nickName = new QLabel(this);
//    lb_nickName->setMaximumHeight(25);
    lb_nickName->setAlignment(Qt::AlignTop|Qt::AlignLeading|Qt::AlignLeft);

    btn_userManager = new QToolButton(this);
    btn_userManager->setMaximumSize(QSize(16, 16));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/images/user_settings.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_userManager->setIcon(icon1);
    btn_userManager->setIconSize(QSize(16, 16));

    horizontalLayout_1->addWidget(lb_nickName);
    horizontalLayout_1->addWidget(btn_userManager);

     gridLayout->addLayout(horizontalLayout_1, 0, 1, 1, 1);
///////////////////////////////////////
    horizontalLayout_2 = new QHBoxLayout();
    lb_userIp = new QLabel(this);
    lb_userIp->setMinimumSize(QSize(140, 0));

    horizontalSpacer = new QSpacerItem(4, 4, QSizePolicy::Expanding, QSizePolicy::Minimum);

    btn_userChat = new QToolButton();
    btn_userChat->setMaximumSize(QSize(16, 16));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/user_chat.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_userChat->setIcon(icon);
    btn_userManager->setIconSize(QSize(16, 16));

    horizontalLayout_2->addWidget(lb_userIp);
    horizontalLayout_2->addItem(horizontalSpacer);
    horizontalLayout_2->addWidget(btn_userChat);
//////////////

    gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);

    btn_userManager->hide();
    btn_userChat->hide();
    this->setMouseTracking(true);

    retranslateUi();
}

void UserItemWidget::retranslateUi()
{
    setWindowTitle(QApplication::translate("UserItemWidget", "Form", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    btn_userChat->setToolTip(QApplication::translate("UserItemWidget", "Chat...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    btn_userChat->setText(QString());
#ifndef QT_NO_TOOLTIP
    lb_nickName->setToolTip(QApplication::translate("UserItemWidget", "User name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    lb_nickName->setText(QApplication::translate("UserItemWidget", "Name", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    lb_userIp->setToolTip(QApplication::translate("UserItemWidget", "User ip", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    lb_userIp->setText(QApplication::translate("UserItemWidget", "User ip", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    btn_userManager->setToolTip(QApplication::translate("UserItemWidget", "User settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    btn_userManager->setText(QString());
#ifndef QT_NO_TOOLTIP
    lb_userAvatar->setToolTip(QApplication::translate("UserItemWidget", "User avatar", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    lb_userAvatar->setText(QString());
}
