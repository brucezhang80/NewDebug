/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:WindowSytle.cpp
**   Author: Young Yori
**   Data: Administrator 2014/1/17 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#include "WindowSytle.h"
#include "ui_WindowSytle.h"

#include <qt_windows.h>
#include <windowsx.h>
#include <QPainter>
#include <QtCore/qmath.h>
#include <QGraphicsDropShadowEffect>

WindowSytle::WindowSytle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowSytle)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    setStyleSheet("background-color:transparent");
//    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
//    effect->setBlurRadius(2.0);
//    effect->setColor(QColor(0, 0, 0, 120));
//    effect->setOffset(5.0);
//    setGraphicsEffect(effect);
}

WindowSytle::~WindowSytle()
{
    delete ui;
}

bool WindowSytle::winEvent(MSG *message, long *result)
{
    switch(message->message)
    {
    case WM_NCHITTEST:
        int xPos = GET_X_LPARAM(message->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(message->lParam) - this->frameGeometry().y();
        if(ui->frame->childAt(xPos,yPos) == 0)
        {
            *result = HTCAPTION;
        }else{
            return false;
        }
        int _max = 14;
        int _min = 10;
        if(xPos > _min && xPos < _max)
            *result = HTLEFT;
        if(xPos > (this->width() - _max) && xPos < (this->width()))
            *result = HTRIGHT;
        if(yPos > _min && yPos < _max)
            *result = HTTOP;
        if(yPos > (this->height() - _max) && yPos < (this->height()))
            *result = HTBOTTOM;
        if(xPos > _min && xPos < _max && yPos > _min && yPos < _max)
            *result = HTTOPLEFT;
        if(xPos > (this->width() - _max) && xPos < (this->width()) &&
                yPos > _min && yPos < _max)
            *result = HTTOPRIGHT;
        if(xPos > _min && xPos < _max && yPos > (this->height() - _max) &&
                yPos < (this->height()))
            *result = HTBOTTOMLEFT;
        if(xPos > (this->width() - _max) && xPos < (this->width()) &&
                yPos > (this->height() - _max) &&
                yPos < (this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    return false;
}

void WindowSytle::paintEvent(QPaintEvent *event)
{
    QPainterPath backPath;
    backPath.setFillRule(Qt::WindingFill);
    backPath.addRect(10, 10, this->width()-20, this->height()-20);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(backPath, QBrush(Qt::white));

    QColor color(0, 0, 0, 40);
    if(this->isActiveWindow())
    {
        for(int i=0; i < 10; i++)
        {
            QPainterPath shadowPath;
            shadowPath.setFillRule(Qt::WindingFill);
            shadowPath.addRect((10-i), 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
            color.setAlpha(150 - qSqrt(i)*50);
            painter.setPen(color);
            painter.drawPath(shadowPath);
        }
    }
    else
    {
        for(int i=0; i < 5; i++)
        {
            QPainterPath shadowPath;
            shadowPath.setFillRule(Qt::WindingFill);
            shadowPath.addRect((10-i), 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
            color.setAlpha(150 - qSqrt(i+4)*50);
            painter.setPen(color);
            painter.drawPath(shadowPath);
        }
    }

    QWidget::paintEvent(event);
}
