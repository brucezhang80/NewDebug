/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaHintButtonWidget.cpp
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

#include "MediaHintButtonWidget.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QDebug>

MediaHintButtonWidget::MediaHintButtonWidget(QWidget *parent) :
    QWidget(parent)
{
//    m_animate = new StateMachineServer;

    this->setMinimumSize(80, 20);
    this->setFixedSize(80, 20);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(4);

    QToolButton* closeBtn = new QToolButton();
    closeBtn->setFixedSize(20,20);
    closeBtn->setIconSize( QSize(12, 12) );
    closeBtn->setIcon( QIcon(":/images/Button-close.png") );

    QToolButton* maximizedBtn = new QToolButton();
    maximizedBtn->setFixedSize(20,20);
    maximizedBtn->setIconSize( QSize(12, 12) );
    maximizedBtn->setIcon( QIcon(":/images/Button-max.png") );

    QToolButton* minimizedBtn = new QToolButton();
    minimizedBtn->setFixedSize(20,20);
    minimizedBtn->setIconSize( QSize(12, 12) );
    minimizedBtn->setIcon( QIcon(":/images/Button-min.png") );

    QToolButton* settingBtn = new QToolButton();
    settingBtn->setFixedSize(20,20);
    settingBtn->setIconSize( QSize(12, 12) );
    settingBtn->setIcon( QIcon(":/images/Button-setting.png") );

    hLayout->addWidget(settingBtn);
    hLayout->addWidget(minimizedBtn);
    hLayout->addWidget(maximizedBtn);
    hLayout->addWidget(closeBtn);
    hLayout->setAlignment(Qt::AlignRight);
    this->setLayout(hLayout);

    connect( closeBtn, SIGNAL(clicked()), this, SIGNAL(closed()) );
    connect( maximizedBtn, SIGNAL(clicked()), this, SIGNAL(maximized()) );
    connect( minimizedBtn, SIGNAL(clicked()), this, SIGNAL(minimized()) );
//    connect(this, SIGNAL(animateHide()), m_animate, SIGNAL(transHide()) );
//    connect(this, SIGNAL(animateShow()), m_animate, SIGNAL(transShow()) );

}

void MediaHintButtonWidget::resetAnimate()
{
//    m_animate->setAnimate(this, StateMachineServer::Top);
    qDebug() << this->geometry();
}

void MediaHintButtonWidget::resizeEvent(QResizeEvent *event)
{
//    m_animate->setAnimate(this, StateMachineServer::Top);
    QWidget::resizeEvent(event);
}
