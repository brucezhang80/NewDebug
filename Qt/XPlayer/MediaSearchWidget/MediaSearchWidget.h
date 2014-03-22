/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaSearchWidget.h
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

#ifndef MEDIASEARCHWIDGET_H
#define MEDIASEARCHWIDGET_H

#include <QWidget>
#include "StateMachineServer/StateMachineServer.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class MediaSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaSearchWidget(QWidget *parent = 0);

signals:
    void searchMusic(const QString &key);
    void animateHide();
    void animateShow();

public slots:
    void searchMusic();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QLineEdit*      m_searchKeyEdit;
    StateMachineServer*      m_animate;
};

#endif // MEDIASEARCHWIDGET_H
