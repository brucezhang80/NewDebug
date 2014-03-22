/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaHintButtonWidget.h
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

#ifndef MEDIAHINTBUTTONWIDGET_H
#define MEDIAHINTBUTTONWIDGET_H

#include <QWidget>
#include "StateMachineServer/StateMachineServer.h"

class MediaHintButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaHintButtonWidget(QWidget *parent = 0);

signals:
    void closed();
    void maximized();
    void minimized();
    void animateHide();
    void animateShow();

public slots:
    void resetAnimate();

private slots:

protected:
    void resizeEvent(QResizeEvent *event);

private:
    StateMachineServer*      m_animate;
};

#endif // MEDIAHINTBUTTONWIDGET_H
