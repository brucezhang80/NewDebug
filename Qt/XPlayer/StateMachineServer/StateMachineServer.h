/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:StateMachineServer.h
**   Author: Young Yori
**   Data: Administrator 2014/2/12 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#ifndef STATEMACHINESERVER_H
#define STATEMACHINESERVER_H

#include <QObject>
#include <QWidget>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QMap>

/**
 * @窗口部件状态机/动画状态类
 */

class StateMachineServer : public QObject
{
    Q_OBJECT
public:

    enum Direction
    {
        None,
        Left,
        Top,
        Right,
        Bottom
    };

    explicit StateMachineServer(QObject *parent = 0);
    ~StateMachineServer();

    void setAnimate(QWidget *widget, Direction direct = Left);

private:
    void setAnimateDirection(QWidget *widget,
                             QState *state, const Direction direct);
signals:
    void transHide();
    void transShow();

public slots:

};

#endif // STATEMACHINESERVER_H
