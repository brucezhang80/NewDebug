/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:WindowSytle.h
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

#ifndef WINDOWSYTLE_H
#define WINDOWSYTLE_H

#include <QWidget>

namespace Ui {
class WindowSytle;
}

class WindowSytle : public QWidget
{
    Q_OBJECT

public:
    explicit WindowSytle(QWidget *parent = 0);
    ~WindowSytle();

protected:
    bool winEvent(MSG *message, long *result);
    void paintEvent(QPaintEvent *);

private:
    Ui::WindowSytle *ui;
};

#endif // WINDOWSYTLE_H
