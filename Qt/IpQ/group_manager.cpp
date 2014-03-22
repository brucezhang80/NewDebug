#include "group_manager.h"
#include "macai.h"
#include "transfer_file_window.h"
#include "usertreewidget.h"
#include "useritemwidget.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QSpacerItem>
#include <QHBoxLayout>

#include <QDebug>

GroupManager::GroupManager(QObject *parent) :
    QObject(parent)
{
    connect(m_sendFileBtn, SIGNAL(toggled(bool)), this, SLOT(createTransferFileWin(bool)) );
}

GroupManager::~GroupManager()
{
}

GroupManager::GroupManager(QString title, QTreeWidget *treeWidget)
    :m_title(title), QObject(treeWidget)
{
    m_treeWidget = treeWidget;
    setGroupItem(treeWidget, m_title);
    retranslateUi();
    connect(m_sendFileBtn, SIGNAL(toggled(bool)), this, SLOT(createTransferFileWin(bool)) );

}

void GroupManager::retranslateUi()
{
    m_sendFileBtn->setToolTip(QString(tr("Send file(s) to current group user(s).")));

}

QTreeWidgetItem* GroupManager::groupItem() const
{
    return m_group;
}


void GroupManager::setGroupItem(QTreeWidget* tree, const QString &title)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(tree);
    item->setText(0, title);
    tree->setItemWidget(item, 0 , createWidget());
    item->setExpanded(true);

    m_group = item;
}

QWidget *GroupManager::createWidget()
{
    QWidget* widget = new QWidget();
    widget->setContentsMargins(0,0,0,0);
    m_sendFileBtn = new QPushButton(widget);
    m_sendFileBtn->setCheckable(1);
    m_sendFileBtn->setChecked(0);
    QSpacerItem* horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout* hlayout = new QHBoxLayout(widget);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->addItem(horizontalSpacer);
    hlayout->addWidget(m_sendFileBtn);
//    m_sendFileBtn->setMaximumHeight(14);
    m_sendFileBtn->setFixedSize(14, 14);
    m_sendFileBtn->setIcon(QIcon(":/images/file-send.png"));
    m_sendFileBtn->setIconSize(QSize(14,14));

    return widget;
}

QList<QTreeWidgetItem *> GroupManager::groupChildren()
{
    QList<QTreeWidgetItem*> list;
    for( int i = 0; i < m_group->childCount(); ++i)
    {
        QTreeWidgetItem* ch = m_group->child(i);
        if( ch )
        {
            list.append(ch);
        }
    }

    return list;
}

void GroupManager::createTransferFileWin(bool bShow)
{
    if(bShow)
    {
        Macai::transferFileWindow->clearUserList();
        Macai::transferFileWindow->show();

        QList<QTreeWidgetItem*> list = groupChildren();
        foreach( QTreeWidgetItem* item, list)
        {
            QWidget* widget = Macai::userTreeWidget->itemWidget(item, 0);
            if( widget && widget->objectName() == QString("UserItemWidget") )
            {
                UserItemWidget* user = static_cast<UserItemWidget*>(widget);
                if( user )
                {
                    Macai::transferFileWindow->insertUserToList(user->loginName(), user->userIp());
//                    Macai::transferFileWindow->collectHostDestinaton(user->userIp(), user->loginName());
                }
            }
        }
        connect(Macai::transferFileWindow, SIGNAL(setChecked(bool)), m_sendFileBtn, SLOT(setChecked(bool)) );
    }
}
