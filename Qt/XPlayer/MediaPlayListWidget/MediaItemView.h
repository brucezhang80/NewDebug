/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaItemView.h
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

#ifndef MEDIAITEMVIEW_H
#define MEDIAITEMVIEW_H

#include <QTableView>

class MediaItemView : public QTableView
{
    Q_OBJECT
public:
    explicit MediaItemView(QWidget *parent = 0);

signals:

public slots:

};

#endif // MEDIAITEMVIEW_H
