/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/12 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "MusicListWidget.h"
#include <QDebug>

MusicListWidget::MusicListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setMouseTracking(true);
    m_musicItem = new MusicItemWidget;

    for(int i = 0; i < 100; ++i)
    {
        QString title = QString("%1-Title abcde - Author - Album<<xxx x>> 2013").arg(i);
        QListWidgetItem* item = new QListWidgetItem(title, this);
        m_itemHeight = item->sizeHint().height();
    }

    connect(this, SIGNAL(itemEntered(QListWidgetItem*)),
            this, SLOT(itemHoverEnter(QListWidgetItem*)) );
    connect(m_musicItem, SIGNAL(mouseLeave(QListWidgetItem*)),
            this, SLOT(itemHoverLeave(QListWidgetItem*)) );
}

void MusicListWidget::itemHoverEnter(QListWidgetItem *item)
{
    if(itemWidget(item)) return;
    this->setItemWidget(item, m_musicItem);
    m_musicItem->setItem(item);
    item->setSizeHint(QSize(item->sizeHint().width(), m_musicItem->height()));
}

void MusicListWidget::itemHoverLeave(QListWidgetItem* item)
{
    qDebug() << "leave";
    item->setSizeHint(QSize(item->sizeHint().width(), m_itemHeight));

//    removeItemWidget(item);
}
