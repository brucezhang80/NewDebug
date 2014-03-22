/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:LeftTopWidget.cpp
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

#include "LeftTopWidget.h"

#include <QLineEdit>
#include <QHBoxLayout>

LeftTopWidget::LeftTopWidget(QWidget *parent) :
    QWidget(parent)
{
    m_searchKeyEdit = new QLineEdit;
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(4);
    hLayout->addWidget(m_searchKeyEdit);
    hLayout->setAlignment(Qt::AlignLeft);
    this->setLayout(hLayout);
}
