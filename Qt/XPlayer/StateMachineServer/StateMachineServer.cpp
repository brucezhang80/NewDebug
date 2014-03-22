/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:StateMachineServer.cpp
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

#include "StateMachineServer.h"
#include <QFinalState>
#include <QApplication>
#include <QSignalTransition>
#include <QDebug>

StateMachineServer::StateMachineServer(QObject *parent) :
    QObject(parent)
{
}

StateMachineServer::~StateMachineServer()
{
}

void StateMachineServer::setAnimate(QWidget *widget,
                                   StateMachineServer::Direction direct)
{
    QRect rect = widget->geometry();

    QStateMachine *machine = new QStateMachine();
    QState* state1 = new QState(machine);
    state1->assignProperty(widget, "geometry", rect);
    machine->setInitialState(state1);

    QState* state2 = new QState(machine);
    setAnimateDirection(widget, state2, direct);

//    QFinalState* final = new QFinalState(state2);

    QPropertyAnimation *ani=new QPropertyAnimation(widget, "geometry");
    ani->setDuration(1000);
    ani->setEasingCurve(QEasingCurve::OutExpo);   //动画效果

    QSignalTransition* tran1 = state1->addTransition(this, SIGNAL(transHide()), state2);
    tran1->addAnimation(ani);
    QSignalTransition* tran2 = state2->addTransition(this, SIGNAL(transShow()), state1);
    tran2->addAnimation(ani);

    machine->start();
}

void StateMachineServer::setAnimateDirection(QWidget *widget, QState *state,
                                             const StateMachineServer::Direction direct)
{
    QRect rect = widget->geometry();
    switch(direct)
    {
    case Left:
        state->assignProperty(widget, "geometry", QRect(rect.x()- rect.width(),
                                                        rect.y(),
                                                        rect.width(), rect.height()));
        break;
    case Top:
        state->assignProperty(widget, "geometry", QRect(widget->x(),
                                                        widget->y()-widget->height()-9,
                                                        rect.width(), rect.height()));
        break;
    case Right:
        state->assignProperty(widget, "geometry", QRect(widget->x()+widget->width()-20,
                                                        widget->y(),
                                                        rect.width(), rect.height()));
        break;
    case Bottom:
        state->assignProperty(widget, "geometry", QRect(widget->x(),
                                                        widget->y()+widget->height(),
                                                        rect.width(), rect.height()));
        break;
    default:
        break;
    }
}

