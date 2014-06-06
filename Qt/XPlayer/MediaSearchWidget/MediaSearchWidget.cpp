/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaSearchWidget.cpp
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

#include "MediaSearchWidget.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>

MediaSearchWidget::MediaSearchWidget(QWidget *parent) :
    QWidget(parent)
{
    m_animate = new StateMachineServer;

    this->setMinimumSize(160, 20);
    this->setFixedSize(160, 20);

    m_searchKeyEdit = new QLineEdit;
    m_searchKeyEdit->setText( tr("Searching...") );

    QToolButton* searchBtn = new QToolButton;

    QSize size = QSize( 20, this->size().height() );
    searchBtn->setMinimumSize(size);
    searchBtn->setMaximumSize(size);
    searchBtn->setIconSize(size);
    searchBtn->setIcon( QIcon(":/images/Button-search.png"));
    searchBtn->setFocusPolicy(Qt::NoFocus);// 得到焦点时，不显示虚线框
    searchBtn->setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout *editLayout = new QHBoxLayout();
    editLayout->setContentsMargins(0, 0, 0, 0);
    editLayout->addStretch();
    editLayout->addWidget(searchBtn);
    m_searchKeyEdit->setLayout(editLayout);
    // 设置输入框中文件输入区，不让输入的文字在被隐藏在按钮下
    m_searchKeyEdit->setTextMargins(0, 1, size.width(), 1);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(4);
    hLayout->addWidget(m_searchKeyEdit);
    hLayout->setAlignment(Qt::AlignLeft);
    this->setLayout(hLayout);

    connect(searchBtn, SIGNAL(clicked()), this, SLOT(searchMusic()) );
    connect(this, SIGNAL(animateHide()), m_animate, SIGNAL(transHide()) );
    connect(this, SIGNAL(animateShow()), m_animate, SIGNAL(transShow()) );

}

void MediaSearchWidget::searchMusic()
{
    QString key = m_searchKeyEdit->text();
    emit searchMusic(key);
}

void MediaSearchWidget::resizeEvent(QResizeEvent *event)
{
//    m_animate->setAnimate(this, StateMachineServer::Top);
    QWidget::resizeEvent(event);
}
