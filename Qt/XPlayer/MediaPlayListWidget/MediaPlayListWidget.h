/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaItemWidget.h
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

#ifndef MEDIAITEMWIDGET_H
#define MEDIAITEMWIDGET_H

#include <QWidget>
#include "StateMachineServer/StateMachineServer.h"

class MediaPlayListView;

class MediaPlayListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaPlayListWidget(QWidget *parent = 0);

signals:
    void animateHide();
    void animateShow();

public slots:

protected:
    void resizeEvent(QResizeEvent *);

private:
    MediaPlayListView*           m_mediaPlaylistView;
    StateMachineServer*      m_animate;
};

#endif // MEDIAITEMWIDGET_H
