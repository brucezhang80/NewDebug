/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:LeftTopWidget.h
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

#ifndef LEFTTOPWIDGET_H
#define LEFTTOPWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class LeftTopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftTopWidget(QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit*      m_searchKeyEdit;

};

#endif // LEFTTOPWIDGET_H
