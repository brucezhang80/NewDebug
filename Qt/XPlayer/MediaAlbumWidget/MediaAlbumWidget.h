/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaAlbumWidget.h
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

#ifndef MEDIAALBUMWIDGET_H
#define MEDIAALBUMWIDGET_H

#include <QWidget>
#include "StateMachineServer/StateMachineServer.h"

class MediaAlbumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaAlbumWidget(QWidget *parent = 0);

signals:
    void animateHide();
    void animateShow();

public slots:
    void resetAnimate();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    StateMachineServer*      m_animate;
};

#endif // MEDIAALBUMWIDGET_H
