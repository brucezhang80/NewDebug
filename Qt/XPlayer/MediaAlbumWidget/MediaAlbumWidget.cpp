/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaAlbumWidget.cpp
**   Author: Young Yori
**   Data: Administrator 2014/2/18 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#include "MediaAlbumWidget.h"
#include <QListView>
#include <QGridLayout>
#include <QToolButton>

#include <QDebug>

MediaAlbumWidget::MediaAlbumWidget(QWidget *parent) :
    QWidget(parent)
{
    m_animate = new StateMachineServer;

//    this->setMinimumSize(QSize(200, 320));
    QListView* listView = new QListView();
    listView->setMouseTracking(true);
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    QToolButton* arrowBtn = new QToolButton();
    arrowBtn->setCursor(QCursor(Qt::PointingHandCursor));
    arrowBtn->setFixedSize(20, 20);
    arrowBtn->setIconSize( QSize(20, 20) );
    arrowBtn->setIcon(QIcon("://images/Arrow-left.png"));

    gridLayout->addWidget(listView, 0, 1, 1, 1);
//    gridLayout->addWidget(arrowBtn, 0, 2, 1, 1);

    connect(this, SIGNAL(animateHide()), m_animate, SIGNAL(transHide()) );
    connect(this, SIGNAL(animateShow()), m_animate, SIGNAL(transShow()) );
}

void MediaAlbumWidget::resetAnimate()
{
    m_animate->setAnimate(this, StateMachineServer::Left);
}

void MediaAlbumWidget::resizeEvent(QResizeEvent *event)
{
    m_animate->setAnimate(this, StateMachineServer::Left);
    QWidget::resizeEvent(event);
}
