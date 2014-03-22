/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaItemWidget.cpp
**   Author: Young Yori
**   Data: Administrator 2014/2/8 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#include "MediaPlayListWidget.h"
#include "MediaItemView.h"

#include <QTableView>
#include <QGridLayout>
#include <QToolButton>
#include <QDebug>

MediaPlayListWidget::MediaPlayListWidget(QWidget *parent) :
    QWidget(parent)
{
    m_mediaItemView = new MediaItemView();
    m_animate = new StateMachineServer;
    QGridLayout *gridLayout = new QGridLayout(this);

    m_mediaItemView->setMouseTracking(true);
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    QToolButton* arrowBtn = new QToolButton();
    arrowBtn->setCursor(QCursor(Qt::PointingHandCursor));
    arrowBtn->setFixedSize(20, 20);
    arrowBtn->setIconSize( QSize(20, 20) );
    arrowBtn->setIcon(QIcon("://images/Arrow-right.png"));
//    arrowBtn->setGeometry(tableView->pos().x()+arrowBtn->width(),
//                          tableView->pos().y()+tableView->height()/2,
//                          arrowBtn->width(), arrowBtn->height());
    gridLayout->addWidget(arrowBtn, 0, 1, 1, 1);
    gridLayout->addWidget(m_mediaItemView, 0, 2, 1, 1);

//    connect(arrowBtn, SIGNAL(clicked(bool)), m_animate, SIGNAL(transShow(bool)) );
    connect(this, SIGNAL(animateHide()), m_animate, SIGNAL(transHide()) );
    connect(this, SIGNAL(animateShow()), m_animate, SIGNAL(transShow()) );

}

void MediaPlayListWidget::resizeEvent(QResizeEvent *event)
{
    m_animate->setAnimate(this, StateMachineServer::Right);
    QWidget::resizeEvent(event);
}
