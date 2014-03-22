/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaItemDelegate.h
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

#ifndef MEDIAITEMDELEGATE_H
#define MEDIAITEMDELEGATE_H

#include <QItemDelegate>

class MediaItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MediaItemDelegate(QObject *parent = 0);

signals:

public slots:

};

#endif // MEDIAITEMDELEGATE_H
