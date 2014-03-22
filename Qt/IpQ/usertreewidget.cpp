#include "usertreewidget.h"
#include "useritemwidget.h"
#include "message_window.h"
#include "macai.h"
#include "constants.h"
#include "message.h"
#include "send_message.h"

#include <QDebug>

UserTreeWidget::UserTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    initTreeWidget();
    setConnect();
}

void UserTreeWidget::initTreeWidget()
{
    this->setExpandsOnDoubleClick(true);
    setColumnCount(1);
    this->setIconSize(QSize(16,16));
    this->setFocusPolicy(Qt::NoFocus);
}

void UserTreeWidget::setConnect()
{
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(showMessageWindow(QTreeWidgetItem*, int)));
}

void UserTreeWidget::showMessageWindow(QTreeWidgetItem *item, int col)
{
    QWidget* widget = this->itemWidget(item, col);
    if( widget && widget->objectName() == QString("UserItemWidget") )
    {
        UserItemWidget* userwidget = static_cast<UserItemWidget*>(widget);
        if( userwidget )
        {
            emit showMsgWindow(userwidget);
        }
    }
}


