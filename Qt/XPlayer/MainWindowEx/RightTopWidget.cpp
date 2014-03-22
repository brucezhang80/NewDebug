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

#include "RightTopWidget.h"
#include <QPushButton>
#include <QHBoxLayout>

RightTopWidget::RightTopWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(4);

    QPushButton* closeBtn = new QPushButton("X");
    closeBtn->setFixedSize(20,20);
    QPushButton* maximumBtn = new QPushButton("O");
    maximumBtn->setFixedSize(20,20);
    QPushButton* minimumBtn = new QPushButton("-");
    minimumBtn->setFixedSize(20,20);

    hLayout->addWidget(minimumBtn);
    hLayout->addWidget(maximumBtn);
    hLayout->addWidget(closeBtn);
    hLayout->setAlignment(Qt::AlignRight);
    this->setLayout(hLayout);
}
